import os
import sys
from common import write_network
import numpy as np

def read_tf_file(tfFile):
    weights = []
    biases = []
    with open(tfFile, "r", encoding="utf-8") as fd:
        line = fd.readline()
        while line:
            if line.startswith("ReLU") or line.startswith("Affine"):
                weight_line = fd.readline()
                weight = eval(weight_line)
                bias_line = fd.readline()
                bias = eval(bias_line)
                weights.append(weight)
                biases.append(bias)
            line = fd.readline()
    return weights, biases

def main(args):
    if len(args) < 2:
        print("Usage: python %s <tf-file> <out-file> [<min input> <max input> <mean> <range>]" % __file__)
        exit(1)

    tfFile = args[0]
    nnetFile = args[1]

    weights, biases = read_tf_file(tfFile)
    # Default values for input bounds and normalization constants

    nnet = {}
    nnet["numLayers"] = len(weights)
    nnet["inputSize"] = inputSize = len(weights[0][0])
    nnet["outputSize"] = len(weights[-1])
    nnet["maxLayerSize"] = max([len(layer) for layer in weights] + [nnet["inputSize"]])
    nnet["layerSizes"] = [nnet["inputSize"]] + [len(layer) for layer in weights]
    nnet["isSymmetric"] = 0

    if len(args) == 6:
        inputMins = nnet["inputSize"]*[args[2]]
        inputMaxes = nnet["inputSize"]*[args[3]]
        means = (nnet["inputSize"]+1)*[args[4]]
        ranges = (nnet["inputSize"]+1)*[args[5]]
    else:
        inputMins = inputSize*[np.finfo(np.float32).min]
        inputMaxes = inputSize*[np.finfo(np.float32).max]
        means = (nnet["inputSize"]+1)*[0.0]
        ranges = (nnet["inputSize"]+1)*[1.0]

    nnet["minVals"] = inputMins
    nnet["maxVals"] = inputMaxes
    nnet["means"] = means
    nnet["ranges"] = ranges
    nnet["weights"] = weights
    nnet["biases"] = biases

    # Write NNet file
    write_network(nnet, nnetFile)


if __name__=='__main__':
    main(sys.argv[1:])
