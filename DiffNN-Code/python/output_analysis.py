import sys, gzip, pickle
from collections import defaultdict
from math import fsum

CHUNK_PREFIX="./"
TIME_PREFIX="time:"
TIMEOUT=object()
INCONCLUSIVE=object()

prop_target = {
    "1" : 0,
    "2" : 0,
    "3" : 0,
    "4" : 0,
    "5" : 4,
    "16": 0,
    "26": 0,
    "7" : 4,
    "8" : 1,
    "9" : 3,
    "10": 0,
    "11": 4,
    "12": 0,
    "13": 0,
    "14": 4,
    "15": 3
}

mnist_targets = [7,2,1,0,4,1,4,9,5,9,0,6,9,0,1,5,9,7,3,4,9,6,6,5,4,0,7,4,0,1,3,1,3,4,7,2,7,1,2,1,1,7,4,2,3,5,1,2,4,4,6,3,5,5,6,0,4,1,9,5,7,8,9,3,7,4,6,4,3,0,7,0,2,9,1,7,3,2,9,7,7,6,2,7,8,4,7,3,6,1,3,6,9,3,1,4,1,7,6,9]
for i in range(len(mnist_targets)):
    prop_target[str(400+i)] = mnist_targets[i]

def convert_defaultdicts_to_dicts(d):
    if isinstance(d, defaultdict) or isinstance(d, dict):
        d = {k: convert_defaultdicts_to_dicts(v) for k, v in d.items()}
    return d

"""
Returns the chunks of output for each experiment. The
beginning of an experiment is denoted by a line that
starts with "./" (or user-defined prefix).
@:return    [[lines of exp's output]] - list of lists
"""
def read_log(logFileName):
    if logFileName.endswith(".gz"):
        logFD = gzip.open(logFileName, mode="rt", encoding="utf-8")
    else:
        logFD = open(logFileName, "r", encoding="utf-8")
    chunk = []
    line = logFD.readline()
    if not line.startswith(CHUNK_PREFIX):
        print("Bad starting line")
        exit(1)
    chunk.append(line)
    line = logFD.readline()
    while line:
        if line.startswith(CHUNK_PREFIX):
            yield chunk
            chunk = [line]
        else:
            chunk.append(line)
        line = logFD.readline()
    logFD.close()
    yield chunk
    return None

class ExpResult(object):
    def __init__(self, chunk):
        self.args = list(map(lambda a: a.strip(""), chunk[0].split(" ")))
        self.cmd = chunk[0]
        self.chunk = chunk[1:]

    @property
    def crashed(self):
        return bool(list(filter(lambda s: "core" in s, self.chunk)))

    @property
    def time(self) -> str:
        if self.crashed:
            return INCONCLUSIVE
        time = list(filter(lambda s: s.startswith(TIME_PREFIX), self.chunk))
        if time:
            return float(time[-1].strip().split(" ")[-1])
        else:
            return TIMEOUT

    @property
    def isDiffNN(self):
        return self.args[0].startswith("./delta")
    @property
    def prop(self) -> str:
        return self.args[1]
    @property
    def epsilon(self) -> str:
        if self.isDiffNN:
            return self.args[4].strip()
        else:
            return self.args[3].strip()
    @property
    def nnet_name(self):
        if self.isDiffNN:
            path = self.args[3]
        else:
            path = self.args[2]
        return path.split("/")[-1]
    @property
    def isMNIST(self) -> bool:
        return "mnist" in self.nnet_name
    @property
    def isACAS(self) -> bool:
        return "ACAS" in self.nnet_name
    @property
    def sigfigs(self) -> str:
        return self.nnet_name.split(".")[0].split("_")[-1]
    @property
    def perturb(self) -> str:
        assert self.isMNIST
        if self.isDiffNN:
            return self.args[5]
        else:
            return self.args[4]

    @property
    def initial_out_eps(self):
        i = 0
        while i < len(self.chunk):
            if self.chunk[i].startswith("Initial output delta:"):
                break
            else:
                i+= 1
        if i >= len(self.chunk):
            print("Warning: Could not find initial delta")
            return None

        lowers = eval(self.chunk[i+1].strip().replace(" ", ","))
        uppers = eval(self.chunk[i + 2].strip().replace(" ", ","))
        return max(abs(lowers[prop_target[self.prop]]), abs(uppers[prop_target[self.prop]]))

    @property
    def depth_stats(self):
        total_depth = 0
        num_depths = 0
        for line in self.chunk:
            if line.startswith("depth:"):
                total_depth += int(line.split(" ")[-1].strip())
                num_depths += 1
        return float(total_depth)/float(num_depths), num_depths

def retrieve_property_times(logFilename):
    # times[property][sigfigs][epsilon] = {{nnet_name -> time}, total num timouts, timedout nnets}
    times = defaultdict(
        lambda: defaultdict(
            lambda: defaultdict(
                lambda: {"times":{}, "total_timeouts":0, "crashes": 0, "verified":0, "init_eps":{}}
            )
        )
    )
    for chunk in read_log(logFilename):
        # sanity check
        if not chunk[0].startswith(CHUNK_PREFIX):
            print("First line in chunk is not the command line")
            exit(1)

        # first arg and last arg are the property and epsilon, respectively
        result = ExpResult(chunk)
        prop = result.prop
        epsilon = result.epsilon

        # second to last arg is the compressed (or subbed) nnets name
        nnet_name = result.nnet_name
        sigfigs = result.sigfigs
        time = result.time
        if time is TIMEOUT:
            times[prop][sigfigs][epsilon]["total_timeouts"] += 1
            times[prop][sigfigs][epsilon]["times"][nnet_name] = time
        elif time is INCONCLUSIVE:
            times[prop][sigfigs][epsilon]["crashes"] += 1
            times[prop][sigfigs][epsilon]["times"][nnet_name] = time
        else:
            times[prop][sigfigs][epsilon]["verified"] += 1
            times[prop][sigfigs][epsilon]["times"][nnet_name] = float(time)
        times[prop][sigfigs][epsilon]["init_eps"][nnet_name] = result.initial_out_eps

    return convert_defaultdicts_to_dicts(times)

def compute_average_property_times(stats):
    ret = defaultdict(dict)
    for prop in stats.keys():
        for epsilon in stats[prop].keys():
            times = stats[prop][epsilon]["times"]
            ret[prop][epsilon] = fsum(times)
    return ret

def compute_diffs(avgs1, avgs2):
    diffs = defaultdict(lambda: defaultdict(dict))
    for prop in avgs1.keys():
        for epsilon in avgs1[prop].keys():
            diffs[prop][epsilon]["absolute"] = avgs1[prop][epsilon] - avgs2[prop][epsilon]
            diffs[prop][epsilon]["ratio"] = avgs1[prop][epsilon] / avgs2[prop][epsilon]
    return diffs

def print_diffs(diffs):
    rows = list(diffs.keys())
    cols = list(diffs[rows[0]].keys())
    print("," + ",".join(cols))
    for prop in rows:
        print(prop + "," + ",".join(map(lambda e: str(diffs[prop][e]["ratio"]) ,diffs[prop].keys())))

def print_stats_for_acas_exp(epsilon, sigfigs, stats_us, stats_them):
    props = sorted(stats_us.keys(), key=lambda p: int(p))
    print("\\begin{table}\n" +
        "\t\\renewcommand{\\arraystretch}{1.3}\n" +
            "\t\\begin{tabular}[h]{r | c | c | c | c}\n")
    print("\t\t$ \\varphi $ & "
          "\\makecell{\diffNN \\\\ Ver./Fail} & "
          "\\makecell{\ReluVal \\\\ Ver./Fail} & "
          "\makecell{\diffNN~Win/ \\\\ \ReluVal~Win} & "
          "\makecell{Avg. \\\\ Speedup} \\\\\\hline")
    for prop in props:
        total_time_us = total_time_them = 0.0
        they_won = we_won = 0
        times_us = stats_us[prop][sigfigs][epsilon]["times"]
        times_them = stats_them[prop][sigfigs][epsilon]["times"]
        for nnet in times_us.keys():
            time_them = times_them[nnet]
            time_us = times_us[nnet]
            if time_them in [TIMEOUT, INCONCLUSIVE] and type(time_us) is float:
                we_won+=1
            if time_us in [TIMEOUT, INCONCLUSIVE] and type(time_them) is float:
                they_won+=1

            if time_them in [TIMEOUT, INCONCLUSIVE]:
                total_time_them += 1800
            else:
                total_time_them += time_them
            if time_us in [TIMEOUT, INCONCLUSIVE]:
                total_time_us += 1800
            else:
                total_time_us += time_us

        total_timeouts_us = stats_us[prop][sigfigs][epsilon]["total_timeouts"]
        total_timeouts_them = stats_them[prop][sigfigs][epsilon]["total_timeouts"]
        total_crashes_us = stats_us[prop][sigfigs][epsilon]["crashes"]
        total_crashes_them = stats_them[prop][sigfigs][epsilon]["crashes"]
        total_verified_us = stats_us[prop][sigfigs][epsilon]["verified"]
        total_verified_them = stats_them[prop][sigfigs][epsilon]["verified"]

        avg_speed_up = total_time_them / total_time_us
        print("\t\t%s\t& %d/%d\t& %d/%d \t& %d/%d & $\\frac{%5.1f}{%5.1f} = %5.1f $ \t\\\\\\hline" %
              (prop, total_verified_us, total_timeouts_us + total_crashes_us,
               total_verified_them, total_timeouts_them + total_crashes_them, we_won, they_won, total_time_us, total_time_them, avg_speed_up))
    print("\t\\end{tabular}\n" +
        "\t\\caption{Verification results for ACAS networks rounded to %s sig. figs. with $\\epsilon=%s$. Timeout is set at 10 min}\n" % (sigfigs, epsilon) +
    "\\end{table}" )

def print_acas_comparison():
    epsilon = "0.01"
    sigfigs = "16bit"
    stats_all = [retrieve_property_times("logs_ASE/acas_0.01/reludiff_lineqall"),
                 retrieve_property_times("logs_ASE/acas_0.01/reludiff_lineq_extravars"),
                 retrieve_property_times("logs_ASE/acas_0.01/reludiff_lineqall_extravars")]
    stats_baseline = retrieve_property_times("logs_ASE/acas_0.01/reludiff_lineq")


    props = sorted(stats_baseline.keys(), key=lambda p: int(p))
    for prop in props:
        times_them = stats_baseline[prop][sigfigs][epsilon]["times"]

        total_timeouts_them = stats_baseline[prop][sigfigs][epsilon]["total_timeouts"]
        total_crashes_them = stats_baseline[prop][sigfigs][epsilon]["crashes"]
        total_verified_them = stats_baseline[prop][sigfigs][epsilon]["verified"]

        print("\t\t%s & %d/%d " %
              (prop, total_verified_them, total_timeouts_them + total_crashes_them), end="")
        for stats_us in stats_all:
            total_time_us = total_time_them = 0.0
            times_us = stats_us[prop][sigfigs][epsilon]["times"]
            for nnet in times_us.keys():
                time_them = times_them[nnet]
                time_us = times_us[nnet]

                if time_them in [TIMEOUT, INCONCLUSIVE]:
                    total_time_them += 1800
                else:
                    total_time_them += time_them
                if time_us in [TIMEOUT, INCONCLUSIVE]:
                    total_time_us += 1800
                else:
                    total_time_us += time_us

            total_timeouts_us = stats_us[prop][sigfigs][epsilon]["total_timeouts"]
            total_crashes_us = stats_us[prop][sigfigs][epsilon]["crashes"]
            total_verified_us = stats_us[prop][sigfigs][epsilon]["verified"]

            avg_speed_up = total_time_them / total_time_us

            print(" & %d/%d & $\\frac{%5.1f}{%5.1f} = %5.1f $ " %
                  (total_verified_us, total_timeouts_us + total_crashes_us,
                        total_time_us, total_time_them, avg_speed_up), end="")
        print("\\\\\\hline")


def summarize_stats_for_all_prop(stats, nnet, sigfig="3", eps="2.5"):
    verified = 0
    timeout = 0
    avg_time = 0.0

    for prop in stats.keys():
        if type(stats[prop][sigfig][eps]["times"][nnet]) is float:
            avg_time += stats[prop][sigfig][eps]["times"][nnet]
            verified += 1
        else:
            avg_time += 600.0
            timeout += 1

    #if verified > 0:
    #    avg_time /= float(verified)

    return (avg_time, verified, timeout)

def print_stats_for_mnist_exp(epsilon, sigfigs, stats_us, stats_them, stats_eth):
    # print("\\begin{table}\n" +
    #         "\t\\begin{tabular}[h]{r | c | c | c | c | c}\n")
    # print("\t\t Network & "
    #       "\\makecell{\ReluVal \\\\ Verif./Fail} & "
    #       "\\makecell{\DeepPoly \\\\ Verif./Fail} & "
    #       "\\makecell{\diffNN \\\\ Verif./Fail} & "
    #       "\\makecell{\DeepPoly \\\\ Avg. Time} & "
    #       "\\makecell{\diffNN \\\\ Avg. Time} & ")

    nnet = "mnist_relu_3_100_3.nnet"
    eth_res = summarize_stats_for_all_prop(stats_eth, nnet)
    us_res = summarize_stats_for_all_prop(stats_us, nnet)
    them_res = summarize_stats_for_all_prop(stats_them, nnet)
    print("Us", us_res)
    print("Them", them_res)
    print("ETH", eth_res)
    #print("\t3x100 & %d/%d & %d/%d & %d/%d & %4.2f & %4.2f \\\\\\hline" %
    #      (them_res[1], them_res[2], eth_res[1], eth_res[2], us_res[1], us_res[2], eth_res[0], us_res[0]))

    nnet = "mnist_relu_2_512_3.nnet"
    eth_res = summarize_stats_for_all_prop(stats_eth, nnet)
    us_res = summarize_stats_for_all_prop(stats_us, nnet)
    them_res = summarize_stats_for_all_prop(stats_them, nnet)
    print("Us", us_res)
    print("Them", them_res)
    print("ETH", eth_res)
    #print("\t2x512 & %d/%d & %d/%d & %d/%d & %4.2f & %4.2f \\\\\\hline" %
    #      (them_res[1], them_res[2], eth_res[1], eth_res[2], us_res[1], us_res[2], eth_res[0], us_res[0]))

    nnet = "mnist_relu_4_1024_3.nnet"
    eth_res = summarize_stats_for_all_prop(stats_eth, nnet)
    us_res = summarize_stats_for_all_prop(stats_us, nnet)
    them_res = summarize_stats_for_all_prop(stats_them, nnet)
    print("Us", us_res)
    print("Them", them_res)
    print("ETH", eth_res)
    #print("\t3x100 & %d/%d & %d/%d & %d/%d & %4.2f & %4.2f \\\\\\hline" %
    #      (them_res[1], them_res[2], eth_res[1], eth_res[2], us_res[1], us_res[2], eth_res[0], us_res[0]))

    #print("\t\\end{tabular}\n" +
    #      "\t\\caption{Verification results for the MNIST networks rounded to %s sig. figs. with $\\epsilon=%s$. Timeout is set at 10 min}\n" % (
    #      sigfigs, epsilon) +
    #      "\\end{table}")

def print_stats_for_HAR_exp(stats_us, stats_them, epsilon, sigfigs):
    print("\\begin{table}\n" +
            "\t\\begin{tabular}[h]{r | c | c | c | c | c}\n")
    print("\t\t Network & "
          "\\makecell{\ReluVal \\\\ Verif./Fail} & "
          "\\makecell{\diffNN \\\\ Verif./Fail} & "
          "\\makecell{\ReluVal \\\\ Avg. Time} & "
          "\\makecell{\diffNN \\\\ Avg. Time} & ")

    our_res = summarize_stats_for_all_prop(stats_us, "HAR_2.nnet", "2", "0.5")
    their_res = summarize_stats_for_all_prop(stats_them, "HAR_2.nnet", "2", "0.5")

    print("\t\t %d/%d & %d/%d & %5.2f & %5.2f \\\\\hline" %
          (our_res[1], our_res[2], their_res[1], their_res[2], our_res[0], their_res[0]))
    print("\t\\end{tabular}\n" +
          "\t\\caption{Verification results for the HAR networks rounded to %s sig. figs. with $\\epsilon=%s$. Timeout is set at 10 min}\n" % (
              sigfigs, epsilon) +
          "\\end{table}")


def acas_times(args):
    if len(args) != 4:
        print("Usage: python %s raw_times [our-log] [their-log] [epsilon] [sigfigs]" % (__file__))
        exit(1)

    our_log, their_log, epsilon, sigfigs = args

    our_stats = retrieve_property_times(our_log)
    their_stats = retrieve_property_times(their_log)

    print_stats_for_acas_exp(epsilon, sigfigs, our_stats, their_stats)

def HAR_times(args):
    if len(args) != 4:
        print("Usage: python %s raw_times [our-log] [their-log] [epsilon] [sigfigs]" % (__file__))
        exit(1)

    our_log, their_log, epsilon, sigfigs = args

    our_stats = retrieve_property_times(our_log)
    their_stats = retrieve_property_times(their_log)

    print_stats_for_HAR_exp(our_stats, their_stats, epsilon, sigfigs)

def read_eth_stats():
    # times[property][sigfigs][epsilon] = {{nnet_name -> time}, total num timouts, timedout nnets}
    times = defaultdict(
        lambda: defaultdict(
            lambda: defaultdict(
                lambda: {"times": {}, "total_timeouts": 0, "crashes": 0, "verified": 0, "init_eps": {}}
            )
        )
    )

    sigfigs = "3"
    epsilon = "2.5"
    with open("results/result_DeepPoly_MNIST_3_100_perturb3", "r", encoding="utf-8") as fd:
        nnet = "mnist_relu_3_100_3.nnet"
        for line in fd:
            pieces = list(map(lambda s: s.strip(), line.split("&")))
            prop = str(int(pieces[0]) - 1 + 400)
            if pieces[1].lower() == "yes":
                times[prop][sigfigs][epsilon]["times"][nnet] = float(pieces[3])
                times[prop][sigfigs][epsilon]["verified"] += 1
            else:
                times[prop][sigfigs][epsilon]["times"][nnet] = INCONCLUSIVE
                times[prop][sigfigs][epsilon]["crashes"] += 1


    with open("results/result_DeepPoly_MNIST_2_512_perturb3", "r", encoding="utf-8") as fd:
        nnet = "mnist_relu_2_512_3.nnet"
        for line in fd:
            pieces = list(map(lambda s: s.strip(), line.split("&")))
            prop = str(int(pieces[0]) - 1 + 400)
            if pieces[1].lower() == "yes":
                times[prop][sigfigs][epsilon]["times"][nnet] = float(pieces[3])
                times[prop][sigfigs][epsilon]["verified"] += 1
            else:
                times[prop][sigfigs][epsilon]["times"][nnet] = INCONCLUSIVE
                times[prop][sigfigs][epsilon]["crashes"] += 1

    with open("results/result_DeepPoly_MNIST_4_1024_perturb3", "r", encoding="utf-8") as fd:
        nnet = "mnist_relu_4_1024_3.nnet"
        for line in fd:
            pieces = list(map(lambda s: s.strip(), line.split("&")))
            prop = str(int(pieces[0]) - 1 + 400)
            if pieces[1].lower() == "yes":
                times[prop][sigfigs][epsilon]["times"][nnet] = float(pieces[3])
                times[prop][sigfigs][epsilon]["verified"] += 1
            else:
                times[prop][sigfigs][epsilon]["times"][nnet] = INCONCLUSIVE
                times[prop][sigfigs][epsilon]["crashes"] += 1
            pieces = pieces[2].split(",")
            times[prop][sigfigs][epsilon]["init_eps"][nnet] = max(abs(float(pieces[0].strip("[] "))), abs(float(pieces[1].strip("[] "))))

    return times

def mnist_scatter_plot():
    import matplotlib.pyplot as plt
    import numpy as np
    eth_stats = read_eth_stats()
    our_stats = retrieve_property_times("DiffNN-Code/logs/exec-time_out_MNIST_sigfigs3_eps2_5_perturb3_no_mono_4_1024")
    their_stats = retrieve_property_times("ReluVal-for-comparison/logs/mnist_init_output")
    us = []
    them = []
    thiers = []
    for prop in our_stats.keys():
        our_res = our_stats[prop]["3"]["2.5"]["init_eps"]["mnist_relu_4_1024_3.nnet"]
        their_res = eth_stats[prop]["3"]["2.5"]["init_eps"]["mnist_relu_4_1024_3.nnet"]
        theirs_res = their_stats[prop]["3"]["2.5"]["init_eps"]["mnist_relu_4_1024_3.nnet"]
        us.append(float(our_res))
        them.append(float(their_res))
        thiers.append(float(theirs_res))


    fontsize=20
    params = {
        'axes.labelsize': fontsize,
        'axes.titlesize': fontsize,
        'xtick.labelsize': fontsize,
        'ytick.labelsize': fontsize}
    plt.rcParams.update(params)

    yx = np.linspace(-5, 1000, 1000)
    # f, (ax1, ax2) = plt.subplots(2,1,sharex=True)  # type: (plt.Figure, (plt.Axes, plt.Axes))
    # ax1.plot(yx, yx, '-r')
    # ax1.scatter(us, thiers)
    # ax2.scatter(us, them)
    # ax1.set_xlim([0,40])
    # ax2.set_xlim([0,40])

    plt.plot(yx, yx, '-r')
    #plt.rc('text', usetex=True)
    #plt.rc('font', family='serif')
    plt.scatter(us, them, c='b', label="DeepPoly")
    plt.scatter(us, thiers, c='g', label="ReluVal")
    plt.xlabel("DiffNN", fontsize=fontsize)
    #plt.ylabel("DeepPoly", fontsize=fontsize)
    plt.legend(loc="upper right", framealpha=1.0, fontsize=fontsize-2)
    plt.ylim([1, 1200])
    plt.xlim([1, 1200])
    plt.yscale("log")
    plt.xscale("log")
    #plt.show()
    plt.tight_layout()
    plt.savefig("/home/brandon/school/my_papers/diff-paper-ICSE-tmp/figs/" +
               "deeppoly_mnist_4_1024_compare.png")


def prop4_dist():
    import matplotlib.pyplot as plt
    import seaborn as sns
    us = []
    them = []
    with open("DiffNN-Code/logs/property6_depths", "r", encoding="utf-8") as fd:
        for line in fd:
            if line.startswith("depth:"):
                us.append(int(line.split(" ")[-1].strip()))
    with open("ReluVal-for-comparison/logs/property6_depths", "r", encoding="utf-8") as fd:
        for line in fd:
            if line.startswith("depth:"):
                them.append(int(line.split(" ")[-1].strip()))

    fontsize=20
    params = {
        'axes.labelsize': fontsize,
        'axes.titlesize': fontsize,
        'xtick.labelsize': fontsize,
        'ytick.labelsize': fontsize}
    plt.rcParams.update(params)
    sns.kdeplot(us, shade=True, bw=1, label="ReluDiff")
    sns.kdeplot(them, shade=True, bw=1, label="ReluVal")
    plt.xlabel("Split Depth",fontsize=fontsize)
    plt.legend(prop={'size':fontsize})
    plt.tight_layout()
    #plt.show()
    plt.savefig("/home/brandon/school/my_papers/diff-paper-ICSE-tmp/figs/" +
                "prop6_split_depth.png")

def mnist_times(args):
    if len(args) != 4:
        print("Usage: python %s raw_times [our-log] [their-log] [epsilon] [sigfigs]" % (__file__))
        exit(1)

    our_log, their_log, epsilon, sigfigs = args

    our_stats = retrieve_property_times(our_log)
    their_stats = retrieve_property_times(their_log)
    eth_stats = read_eth_stats()


    print_stats_for_mnist_exp(epsilon, sigfigs, our_stats, their_stats, eth_stats)


def retrieve_epsilon_verified(log):
    # ret["acas"][property][sigfig][nnet name] = {depth -> [epsilons verified]}
    # ret["mnist"][property][sigfig][perturb][nnet name] = {depth -> [epsilons verified]}
    ret = {}
    ret["acas"] = defaultdict( # prop
        lambda: defaultdict( # sigfig
            lambda: defaultdict ( # nnet name
                lambda: {} # {depth -> {"lowers":[], "uppers":[]}}
            )
        )
    )
    ret["mnist"] = defaultdict( # prop
        lambda: defaultdict( # sigfig
            lambda: defaultdict( # perturb
                lambda: defaultdict(
                    lambda: {} # {depth -> {"lowers":[], "uppers":[]}}
                )
            )
        )
    )

    for chunk in read_log(log):
        cmd = chunk[0]
        args = ExpResult(cmd)
        cur = 1
        # {depth -> [[lowers], [uppers]]}
        if args.isMNIST:
            eps_verif = ret["mnist"][args.prop][args.sigfigs][args.perturb][args.nnet_name]
        elif args.isACAS:
            eps_verif = ret["acas"][args.prop][args.sigfigs][args.nnet_name]
        else:
            raise Exception("These networks are not supported yet.")

        while cur < len(chunk):
            line = chunk[cur]
            if line.startswith("depth:"):
                depth = line.split(" ")[-1].strip()
                uppers = eval(chunk[cur + 1].replace(" ", ","))
                lowers = eval(chunk[cur + 2].replace(" ", ","))
                if depth not in eps_verif:
                    eps_verif[depth] = {"lowers":lowers, "uppers":uppers}
                else:
                    cur_lowers = eps_verif[depth]["lowers"]
                    cur_uppers = eps_verif[depth]["uppers"]
                    for i in range(len(eps_verif[depth]["lowers"])):
                        cur_lowers[i] = min(cur_lowers[i], lowers[i])
                        cur_uppers[i] = max(cur_uppers[i], uppers[i])
                cur += 3
            else:
                cur += 1
    return ret


def epsilon_verified(args):
    if len(args) != 2:
        print("usage: python %s <log-file> <out-file>" % __file__)
        exit(1)
    log, out = args
    stats = retrieve_epsilon_verified(log)
    with open(out, "wb+") as outFD:
        pickle.dump(convert_defaultdicts_to_dicts(stats), outFD)


def epsilon_verified_analysis(args):
    plot_types = {
        "max_range": plot_max_eps_range,
        "max_ratio": plot_max_diff_ratio,
        "mnist_compare": plot_mnist_depth_all,
        "acas_verif" : make_acas_tables,
        "acas_scatter" : make_acas_scatterplot
    }

    if len(args) < 5:
        print("usage: python %s eps_verif_anal <plot-type> <our-eps.pickle> <their-eps.pickle>" % (__file__) +
              "<property> <sigfig> [perturb]")
        print("Valid plot types are: " + ", ".join(plot_types.keys()))
        exit(1)

    plot_type, our_pickle, their_pickle, prop, sigfig = args[:5]
    if len(args) > 5:
        perturb = args[5]

    if plot_type not in plot_types:
        print("Invalid plot type.")
        print("Valid plot types are: " + ",".join(plot_types.keys()))
        exit(1)

    with open(our_pickle, "rb") as fd:
        our_stats = pickle.load(fd)
    with open(their_pickle, "rb") as fd:
        their_stats = pickle.load(fd)

    if plot_type in ["mnist_compare", "acas_verif", "acas_scatter"]:
        plot_types[plot_type](our_stats, their_stats, args[3:])
        return

    if 400 <= int(prop) <= 499:
        our_eps = our_stats["mnist"][prop][sigfig][perturb]
        their_eps = their_stats["mnist"][prop][sigfig][perturb]
    elif 1 <= int(prop) <= 100:
        our_eps = our_stats["acas"][prop][sigfig]
        their_eps = their_stats["acas"][prop][sigfig]
    else:
        raise Exception("Don't know how to handle property %s" % prop)

    plot_types[plot_type](our_eps, their_eps, prop, sigfig)

def make_acas_tables(our_eps, their_eps, args):
    prop, sigfig = args
    sep = "------------------------"
    our_acas = our_eps["acas"][prop][sigfig]
    their_acas = their_eps["acas"][prop][sigfig]

    for nnet in sorted(our_acas):
        print("\n\n")
        print("depth\t| ours\t\t| theirs\t")
        print(sep)
        for depth in sorted(our_acas[nnet].keys(), key=lambda d: int(d)):
            our_upper = our_acas[nnet][depth]["uppers"][prop_target[prop]]
            our_lower = our_acas[nnet][depth]["lowers"][prop_target[prop]]
            their_upper = their_acas[nnet][depth]["uppers"][prop_target[prop]]
            their_lower = their_acas[nnet][depth]["lowers"][prop_target[prop]]
            print("%s\t\t| %.2f\t| %.2f\t" %
                  (depth, max(abs(our_upper), abs(our_lower)), max(abs(their_upper), abs(their_lower))))
            print(sep)

def make_acas_scatterplot(our_eps, their_eps, args):
    import matplotlib.pyplot as plt
    import numpy as np
    prop, sigfig = args
    our_acas = our_eps["acas"][prop][sigfig]
    their_acas = their_eps["acas"][prop][sigfig]

    x = []; y = []
    color = "b"
    for nnet in sorted(our_acas):
        for depth in sorted(our_acas[nnet].keys(), key=lambda d: int(d)):
            if depth != "0": continue
            our_upper = our_acas[nnet][depth]["uppers"][prop_target[prop]]
            our_lower = our_acas[nnet][depth]["lowers"][prop_target[prop]]
            their_upper = their_acas[nnet][depth]["uppers"][prop_target[prop]]
            their_lower = their_acas[nnet][depth]["lowers"][prop_target[prop]]
            x.append(max(abs(our_upper), abs(our_lower)))
            y.append(max(abs(their_upper), abs(their_lower)))

    yx = np.linspace(-5, 1000, 1000)
    plt.plot(yx, yx, '-r')
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    plt.scatter(x, y, c=color)
    plt.xlabel("DiffNN", fontsize=20)
    plt.ylabel("ReluVal", fontsize="20")
    plt.ylim([min(x + y),max(x + y)])
    plt.xlim([min(x + y),max(x + y)])
    plt.yscale("log")
    plt.xscale("log")
    plt.show()

def plot_max_diff_ratio(our_eps, their_eps, prop, sigfig):
    import matplotlib.pyplot as plt
    x = []; y = []
    color = "b"
    for nnet in their_eps:
        if nnet not in our_eps:
            print("Warning: We do not have result for nnet %s" % nnet)
        for depth in our_eps[nnet]:
            our_max_range = 0.0
            their_max_range = 0.0
            our_upper = our_eps[nnet][depth]["uppers"]
            our_lowers = our_eps[nnet][depth]["lowers"]
            their_upper = their_eps[nnet][depth]["uppers"]
            their_lowers = their_eps[nnet][depth]["lowers"]

            for i in range(len(our_upper)):
                our_max_range = max(our_max_range, abs(our_upper[i] - our_lowers[i]))
                their_max_range = max(their_max_range, abs(their_upper[i] - their_lowers[i]))

            x.append(int(depth))
            y.append(float(their_max_range) / float(our_max_range))

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    plt.scatter(x,y,c=color)
    plt.xlabel("depth", fontsize=20)
    plt.ylabel(r'$\frac{DiffNN \; verified}{ReluVal \; verified}$',
               fontsize="20")

    plt.yscale("log")
    plt.show()
    #plt.savefig("/home/brandon/school/my_papers/diff-NN-analsys-paper/prelim_res_figs/" +
    #            "ACAS_p%s_sigfig%s.png" % (prop, sigfig))

def plot_mnist_depth_all(our_stats, their_stats, args):
    import matplotlib.pyplot as plt
    x = []; y = []
    color = "b"

    depth = "8"
    _, sigfig, perturb = args
    nnet = "mnist_relu_6_200_%s.nnet" % sigfig

    our_mnist = our_stats["mnist"]
    their_mnist = their_stats["mnist"]

    for prop in their_mnist.keys():
        if depth not in our_mnist[prop][sigfig][perturb][nnet]:
            print("Warning: no results for prop %s" % prop)
            continue
        our_res = our_mnist[prop][sigfig][perturb][nnet][depth]
        their_res = their_mnist[prop][sigfig][perturb][nnet][depth]
        our_max = their_max = 0.0
        for i in range(len(our_res["uppers"])):
            our_max = max(our_max, abs(our_res["uppers"][i] - our_res["lowers"][i]))
            their_max = max(their_max, abs(their_res["uppers"][i] - their_res["lowers"][i]))
        y.append(their_max)
        x.append(our_max)


    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    plt.scatter(x,y,c=color)
    plt.xlabel("DeltaVal Verified", fontsize=20)
    plt.ylabel("ReluVal Verified", fontsize=20)

    #plt.yscale("log")
    #plt.xlim([0, 900])
    #plt.ylim([0, 900])
    plt.show()
    #plt.savefig("/home/brandon/school/my_papers/diff-NN-analsys-paper/prelim_res_figs/" +
    #            "MNIST_4_1024_p%s_sigfig%s.png" % (prop, sigfig))


# plot max diff range (not epsilon) out of all outputs
def plot_max_eps_range(our_eps, their_eps):
    import matplotlib.pyplot as plt
    x = []; y = []
    colors = []
    our_color = "b" # blue
    their_color = "r" # red
    for nnet in their_eps:
        for depth in our_eps[nnet]:
            our_max_range = 0.0
            their_max_range = 0.0
            our_upper = our_eps[nnet][depth]["uppers"]
            our_lowers = our_eps[nnet][depth]["lowers"]
            their_upper = their_eps[nnet][depth]["uppers"]
            their_lowers = their_eps[nnet][depth]["lowers"]

            for i in range(len(our_upper)):
                our_max_range = max(our_max_range, abs(our_upper[i] - our_lowers[i]))
                their_max_range = max(their_max_range, abs(their_upper[i] - their_lowers[i]))

            x.append(int(depth))
            y.append(float(our_max_range))
            colors.append(our_color)
            x.append(int(depth))
            y.append(float(their_max_range))
            colors.append(their_color)

    plt.scatter(x,y,c=colors)
    plt.yscale("log")
    plt.show()


def numVerifedByInitDelta(logFile, eps):
    ret = 0
    for chunk in read_log(logFile):
        line = next(filter(lambda s: "Output delta interval:" in s, chunk))
        bounds = eval(line.split(":")[-1])
        epsVerif = max(abs(bounds[0]), abs(bounds[1]))
        if epsVerif < eps:
            ret += 1
    return ret


def plot_mnist_for_each_perturb():
    import os, re
    logDir = "logs_ASE"
    oldTool = {}
    newTool = {}
    for d in os.listdir(logDir):
        if not d.startswith("mnist_"): continue
        perturb = int(d.split("_")[-1][1])
        for f in os.listdir(logDir + "/" + d):
            mObj = re.search("mnist_relu_([0-9])_([0-9]{3,4})", f)
            arch = str(mObj.group(1)) + "x" + str(mObj.group(2))

            if arch not in newTool: newTool[arch] = {}
            if arch not in oldTool: oldTool[arch] = {}
            if "lineqall_extravarssym" in f:
                newTool[arch][perturb] = numVerifedByInitDelta(logDir + "/" + d + "/" + f, 1.0)
            elif "reludiff_lineq_mnist" in f:
                oldTool[arch][perturb] = numVerifedByInitDelta(logDir + "/" + d + "/" + f, 1.0)

    colorForNNet = {
        "3x100" : "y",
        "2x512" : "g",
        "4x1024" : "r"
    }

    import matplotlib.pyplot as plt

    for arch in newTool.keys():
        newToolLine = []
        oldToolLine = []
        perturbs = []
        if arch != "4x1024": continue
        for p in sorted(newTool[arch].keys()):
            perturbs.append(p)
            newToolLine.append(newTool[arch][p])
            oldToolLine.append(oldTool[arch][p])
        plt.plot(perturbs, newToolLine, '-' + colorForNNet[arch], label="New ReluDiff")
        plt.plot(perturbs, oldToolLine, ':' + colorForNNet[arch], label="ReluDiff+")
    print(newToolLine)
    print(oldToolLine)

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    # plt.scatter(x, y, c=color)
    plt.xlabel("Perturbation", fontsize=14)
    plt.ylabel("Percent Verified", fontsize=14)
    plt.legend()
    # plt.ylim([min(x + y), max(x + y)])
    # plt.xlim([min(x + y), max(x + y)])
    # plt.yscale("log")
    # plt.xscale("log")
    plt.show()

def getInitDeltas(logFile):
    ret = []
    for chunk in read_log(logFile):
        line = next(filter(lambda s: "Output delta interval:" in s, chunk))
        bounds = eval(line.split(":")[-1])
        epsVerif = max(abs(bounds[0]), abs(bounds[1]))
        ret.append(epsVerif)
    return ret

def printMNISTDetaAverages():
    logDir = "logs_ASE"
    orig = getInitDeltas(logDir + "/mnist_p7/reludiff_lineq_mnist_relu_4_1024.nnet")
    orig = sum(orig)/len(orig)
    lineqall = getInitDeltas(logDir + "/mnist_p7/reludiff_lineqall_mnist_relu_4_1024.nnet")
    lineqall = sum(lineqall)/len(lineqall)
    extraVars = getInitDeltas(logDir + "/mnist_p7/reludiff_lineq_extravars_mnist_relu_4_1024.nnet")
    extraVars = sum(extraVars)/len(extraVars)
    both = getInitDeltas(logDir + "/mnist_p7/reludiff_allopts_mnist_relu_4_1024.nnet")
    both = sum(both)/len(both)
    print("%1.2f & %1.2f & %1.2f & %1.2f" % (orig, lineqall, extraVars, both))

def mnist_scatterplot():
    import os, re
    logDir = "logs_ASE"
    newEps = getInitDeltas(logDir + "/mnist2_p8/reludiff_lineqall_extravarssym_mnist_relu_4_1024.nnet")
    oldEps = getInitDeltas(logDir + "/mnist2_p8/reludiff_lineq_mnist_relu_4_1024.nnet")

    import matplotlib.pyplot as plt
    import numpy as np

    yx = np.linspace(-5, 1000, 1000)

    plt.plot(yx, yx, '-b')
    plt.scatter(newEps, oldEps, c='g')
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    # plt.scatter(x, y, c=color)
    plt.xlabel("New ReluDiff", fontsize=14)
    plt.ylabel("ReluDiff+", fontsize=14)
    plt.ylim([0, 5.5])
    plt.xlim([0,5.5])
    # plt.yscale("log")
    # plt.xscale("log")
    plt.show()

def main():
    cmds = {
        "acas_times" : acas_times,
        "HAR_times" : HAR_times,
        "mnist_times" : mnist_times,
        "eps_verif" : epsilon_verified,
        "eps_verif_anal" : epsilon_verified_analysis
    }

    if len(sys.argv) < 2:
        print("usage: python %s [command] [args...] " % (sys.argv[0]))
        print("Valid commands are: " + ", ".join(cmds.keys()))
        exit(1)

    cmd = sys.argv[1]
    if cmd not in cmds:
        print("Valid commands are: " + ", ".join(cmds.keys()))
        exit(1)

    cmds[cmd](sys.argv[2:])

if __name__ == "__main__":
    plot_mnist_for_each_perturb()
