import re
from copy import deepcopy

class LayerData():
    def __init__(self):
        self.inputBounds = None  # type: List[(tuple(float, float))]
        self.absBounds = []  # type: List[tuple(float, float)]
        self.eqs = []  # type: List[tuple(List, List)]

    def getNumUnstableNeruons(self):
        ct = 0
        for absBound in self.absBounds:
            if absBound[0] < 0 and absBound[1] > 0:
                ct += 1
        return ct

def readFirstLayerBoundsAndEqs(logFile) -> LayerData:
    ret = LayerData()

    fd = open(logFile, "r", encoding="utf-8")
    bufInputRanges = fd.readline().strip()\
        .replace("# Forward pass: ", "")\
        .replace("[ ", "[")\
        .replace(" ", ",")
    inputLB, inputUB = eval(bufInputRanges)
    ret.inputBounds = list(map(lambda i: (inputLB[i], inputUB[i]), range(len(inputLB))))

    while True:
        buf = fd.readline()
        if buf.startswith("## neuron"):
            layer, _ = buf.split(" ")[1].split("_")[1].split(",")
            if layer != "1":
                break

            fd.readline() # PRE-RELU line
            absBoundsBuf = fd.readline().strip()\
                .replace("#### abs bounds: ", "")\
                .replace(" ", ",")
            n, nPrime, nDelta = eval(absBoundsBuf)
            ret.absBounds.append(n)

            eqBuf = fd.readline().strip()\
                .replace("#### eq: ", "")\
                .replace(" ", ",")
            eqBuf = re.sub("x[0-9]*", "", eqBuf)
            eqLow, eqUp = eval(eqBuf)
            eqLow = list(map(float, eqLow))
            eqUp = list(map(float, eqUp))
            ret.eqs.append((eqLow, eqUp))

    fd.close()
    return ret

def findFirstLayerGoodSplits(firstLayerData : LayerData):
    # (target intermediate neuron, split input)
    ret = []
    for j in range(len(firstLayerData.absBounds)):
        lb, ub = firstLayerData.absBounds[j]
        if not (lb < 0 and ub > 0):
            continue

        for i, inputWeight in enumerate(firstLayerData.eqs[j][0][:-1]):
            width = firstLayerData.inputBounds[i][1] - firstLayerData.inputBounds[i][0]
            if inputWeight < 0:
                width = -width

            if lb + width*inputWeight > 0:
                ret.append((j, i))
    return ret

def intWidth(interval):
    return interval[1] - interval[0]

def doSplit(layerData: LayerData, maxDepth : int, curDepth=0):
    intermediateNeuron, bounds = max(
            filter(lambda t: t[1][0] < 0 and t[1][1] > 0,
                   enumerate(layerData.absBounds)
                   ),
        key=lambda t: t[1][0])  # maximum based on LB

    splitInputIndex, splitInputWeight = max(
        enumerate(layerData.eqs[intermediateNeuron][0][:-1]),
        key=lambda t: abs(t[1]*intWidth(layerData.inputBounds[t[0]]))
    )

    print("Split info: " + str((intermediateNeuron, splitInputIndex)))

    layerData1, layerData2 = LayerData(), LayerData()

    origBound = layerData.inputBounds[splitInputIndex]

    mid = origBound[0] + intWidth(origBound)/2
    layerData1.inputBounds = deepcopy(layerData.inputBounds)
    layerData1.inputBounds[splitInputIndex] = (origBound[0], mid)
    layerData2.inputBounds = deepcopy(layerData.inputBounds)
    layerData2.inputBounds[splitInputIndex] = (mid, origBound[1])

    layerData1.absBounds = deepcopy(layerData.absBounds)
    layerData2.absBounds = deepcopy(layerData.absBounds)

    # these never change, no need for deepcopy
    layerData1.eqs = layerData.eqs
    layerData2.eqs = layerData.eqs

    for i in range(len(layerData.absBounds)):
        inputWeight = layerData.eqs[i][0][splitInputIndex]
        if inputWeight > 0:
            # upper bound decreases in first split, so decrease upper bound
            layerData1.absBounds[i][1] += (mid - origBound[1])*inputWeight
            # lower bound increases in second split, so increase lower bound
            layerData2.absBounds[i][0] += (mid - origBound[0])*inputWeight
        elif inputWeight < 0:
            # upper bound decreases in first split, and weight is negative,
            # so lower bound increases
            layerData1.absBounds[i][0] += (mid - origBound[1])*inputWeight
            # lower bound increases in second split, and weight is negative,
            # so upper bound decreases
            layerData2.absBounds[i][1] += (mid - origBound[0])*inputWeight
        # else: no change

    if curDepth < maxDepth:
        return min(doSplit(layerData1, maxDepth, curDepth + 1),
                   doSplit(layerData2, maxDepth, curDepth + 1))
    else:
        return min(layerData1.getNumUnstableNeruons(),
                   layerData2.getNumUnstableNeruons())


def main(logFile = "acas5_stats"):
    data = readFirstLayerBoundsAndEqs(logFile)
    ret = findFirstLayerGoodSplits(data)
    pass

if __name__ == "__main__":
    main()
