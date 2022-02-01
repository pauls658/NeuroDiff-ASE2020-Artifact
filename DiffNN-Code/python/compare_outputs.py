import sys
import math

def read_file(fileName):
    vals = []
    with open(fileName, "r", encoding="utf-8") as fd:
        for line in fd:
            if line.startswith("["):
                vals.append(eval(line.replace(" ",",")))
    return vals

def main(origFile, compFile, subbedFile):
    origVals = read_file(origFile)
    compVals = read_file(compFile)
    subbedVals = read_file(subbedFile)

    for i in range(len(origVals)):
        for j in range(len(origVals[i])):
            if math.fabs((origVals[i][j] - compVals[i][j]) - subbedVals[i][j]) > 0.01:
                print("Differences do not match.")
                exit(1)
    print("Good!")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("usage: python %s <orig-out> <comp-out> <subbed-out>" % __file__)
        exit(1)
    main(*sys.argv[1:])
