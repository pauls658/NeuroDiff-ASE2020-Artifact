from output_analysis import read_log
from itertools import zip_longest
from _collections import defaultdict

def main(log1, log2):
    bins = defaultdict(int)
    for chunk1, chunk2, in zip_longest(read_log(log1), read_log(log2)):
        assert chunk1[0] == chunk2[0]
        splits1 = next(filter(lambda c: c.startswith("numSplits"), chunk1))
        splits2 = next(filter(lambda c: c.startswith("numSplits"), chunk2))
        splits1 = int(splits1.split(":")[1].strip())
        splits2 = int(splits2.split(":")[1].strip())

        bins[int(int(splits2/splits1))] += 1
    for speedup, num in sorted(bins.items(), key=lambda t: t[0]):
        print(speedup, num)

if __name__ == "__main__":
    main("logs_ReluDiff2/exec-time_out_ACAS_eps0_05", "logs_ReluDiff2/exec-time_out_ACAS_eps0_05_ReluDiff1")