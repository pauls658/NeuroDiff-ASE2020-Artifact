import numpy as np
from types import MappingProxyType

"""
Neural net format:
    nnet["numLayers"] = number of layers in the nnet
    nnet["inputSize"] = number of input neurons
    nnet["outputSize"] = num output neurons
    nnet["maxLayerSize"] = max num neurons in any layer
    nnet["isSymmetric"] = not used
    nnet["minVals"] = list of min values for each input neuron
    nnet["maxVals"] = list of max values for each input neuron
    nnet["means"] = list of means for each input
    nnet["ranges"] = list of ranges for each input
    nnet["weights"] = 3D list
    nnet["biases"] = 2D list
    nnet["layertypes"] = 1D list
        0 = ReLU
"""

class NNet():
    def __init__(self, nnet=None):
        if nnet is not None:
            self.numLayers = nnet["numLayers"] # type: int
            self.inputSize = nnet["inputSize"] # type: int
            self.outputSize = nnet["outputSize"] # type: int
            self.maxLayerSize = nnet["maxLayerSize"] # type: int
            self.layerSizes = nnet["layerSizes"] # type: list[int]
            self.isSymmetric = nnet["isSymmetric"] # type: int
            self.minVals = nnet["minVals"] # type: list[str]
            self.maxVals = nnet["maxVals"] # type: list[str]
            self.means = nnet["means"] # type: list[str]
            self.ranges = nnet["ranges"] # type: list[str]
            # weights[k][m][n]: weight from n -> m, where m is in layer k
            self.weights = nnet["weights"] # type: list[list[list[str]]]
            self.biases = nnet["biases"] # type: list[list[str]]
            #self.layerTypes = nnet["layerTypes"]
        else:
            self.numLayers = 0 # type: int
            self.inputSize = 0 # type: int
            self.outputSize = 0  # type: int
            self.maxLayerSize = 0 # type: int
            self.layerSizes = [] # type: list[int]
            self.isSymmetric = 0 # type: int
            self.minVals = [] # type: list[str]
            self.maxVals = [] # type: list[str]
            self.means = [] # type: list[str]
            self.ranges = [] # type: list[str]
            # weights[k][m][n]: weight from n -> m, where m is in layer k
            self.weights = [] # type: list[list[list[str]]]
            self.biases = [] # type: list[list[str]]
            self.layerTypes = []

    def __getitem__(self, key):
        return getattr(self, key)

    def __setitem__(self, key, value):
        if not hasattr(self, key):
            raise Exception("Invalid NNet property {}".format(key))
        setattr(self, key, value)

def read_network(nnet_path):
    ret = {}
    strip_chars = ", \n"
    with open(nnet_path, "r", encoding="utf-8") as nnet_fd:
        tmp = nnet_fd.readline()
        while tmp.startswith("/"):
            tmp = nnet_fd.readline()

        params = list(map(int, tmp.strip(strip_chars).split(",")))
        ret["numLayers"] = params[0]
        ret["inputSize"] = params[1]
        ret["outputSize"] = params[2]
        ret["maxLayerSize"] = params[3]

        ret["layerSizes"] = list(map(int, nnet_fd.readline().strip(strip_chars).split(",")))

        ret["isSymmetric"] = list(map(int, nnet_fd.readline().strip(strip_chars).split(",")))[0]

        ret["minVals"] = nnet_fd.readline().strip(strip_chars).split(",")
        ret["maxVals"] = nnet_fd.readline().strip(strip_chars).split(",")
        ret["means"] = nnet_fd.readline().strip(strip_chars).split(",")
        ret["ranges"] = nnet_fd.readline().strip(strip_chars).split(",")


        weights = []
        biases = []
        for i in range(0, ret["numLayers"]):
            matrix = []
            bias = []
            for j in range(0, ret["layerSizes"][i + 1]):
                matrix.append(nnet_fd.readline().strip(strip_chars).split(","))
            for _ in range(0, ret["layerSizes"][i + 1]):
                bias.append(nnet_fd.readline().strip(strip_chars))
            weights.append(matrix)
            biases.append(bias)
        ret["weights"] = weights
        ret["biases"] = biases

    return NNet(ret)


def write_network(nnet, out_path):
    with open(out_path, "w+", encoding="utf-8") as out_fd:
        out_fd.write(
            ",".join(map(str,
                [nnet["numLayers"], nnet["inputSize"], nnet["outputSize"], nnet["maxLayerSize"]]
            )) + ",\n")
        out_fd.write(",".join(map(str, nnet["layerSizes"])) + ",\n")
        out_fd.write(str(nnet["isSymmetric"]) + ",\n")
        out_fd.write(",".join(map(str, nnet["minVals"])) + ",\n")
        out_fd.write(",".join(map(str, nnet["maxVals"])) + ",\n")
        out_fd.write(",".join(map(str, nnet["means"])) + ",\n")
        out_fd.write(",".join(map(str, nnet["ranges"])) + ",\n")

        for i in range(0, nnet["numLayers"]):
            for j in range(0, nnet["layerSizes"][i + 1]):
                # Add zero to ensure 0.0 is always positive
                out_fd.write(",".join(map(lambda x: repr(np.float32(x) + 0), nnet["weights"][i][j])) + ",\n")
            out_fd.write(",\n".join(map(lambda x: repr(np.float32(x) + 0), nnet["biases"][i])) + ",\n")
