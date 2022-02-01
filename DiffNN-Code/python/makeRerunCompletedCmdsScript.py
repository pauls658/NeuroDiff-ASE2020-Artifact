from output_analysis import read_log
import sys


def main(logfile):
    with open("scripts/rerunCompletedCmds.sh", "w+", encoding="utf-8") as scriptfd:
        scriptfd.write("#!/bin/bash\n\n")
        scriptfd.write("out=\"rerun.out\"\n> $out\n")
        scriptfd.write("timeout=1800\n\n")
        for chunk in read_log(logfile):
            chunk = list(filter(bool, map(lambda l: l.strip(), chunk)))
            if any(map(lambda l: l.startswith("time: "), chunk)):
                scriptfd.write("echo \"" + chunk[0] + "\" >> $out\n")
                scriptfd.write("timeout -s 2 $timeout " + chunk[0] + " 2>> $out\n\n")

if __name__ == "__main__":
    main(sys.argv[1])
