from common import *
import sys

def round_nnet_sigfigs(nnet, precision):
    weights = nnet["weights"]
    biases = nnet["biases"]
    new_weights = []
    new_biases = []
    for k, weightMatrix in enumerate(weights):
        matrix = []
        for row in weightMatrix:
            matrix.append(list(map(lambda f: round(float(f), precision), row)))
        new_weights.append(matrix)

        bias = list(map(lambda f: round(float(f), precision), biases[k]))
        new_biases.append(bias)
    nnet["weights"] = new_weights
    nnet["biases"] = new_biases
    return nnet

def round_nnet_16bit(nnet):
    import numpy as np

    weights = nnet["weights"]
    biases = nnet["biases"]
    new_weights = []
    new_biases = []
    for k, weightMatrix in enumerate(weights):
        matrix = []
        for row in weightMatrix:
            matrix.append(list(map(lambda f: np.float16(f), row)))
        new_weights.append(matrix)

        bias = list(map(lambda f: np.float16(f), biases[k]))
        new_biases.append(bias)
    nnet["weights"] = new_weights
    nnet["biases"] = new_biases
    return nnet

def main():
    if len(sys.argv) != 4:
        print("usage: python %s <nnet-path> <output-path> <round-precision> " % (sys.argv[0]))
        exit(1)

    nnet = read_network(sys.argv[1])

    if sys.argv[3] == "16bit":
        rounded_nnet = round_nnet_16bit(nnet)
    else:
        rounded_nnet = round_nnet_sigfigs(nnet, int(sys.argv[3]))

    write_network(rounded_nnet, sys.argv[2])

if __name__ == "__main__":
    main()
