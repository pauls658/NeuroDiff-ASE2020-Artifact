import sys
from common import NNet, write_network 
from keras.models import load_model

def getInputSizeOfWeightMatrix(layername, weight):
    if layername.startswith("dense"):
        assert len(weight) == 1
        weight = weight[list(weight.keys())[0]]
        assert len(weight) == 2
        return len(weight["kernel:0"])
    else:
        raise Exception("Unsupported layer type: " + layername)

def convertH5ToNnet(keras_h5file, outfile):
    nnet = NNet()
    model = load_model(keras_h5file, compile=False)

    layers = model.layers

    # header meta data
    nnet["numLayers"] = len(layers)
    assert type(layers[0].input_shape[1]) is int
    nnet["inputSize"] = layers[0].input_shape[1]
    nnet["maxLayerSize"] = layers[0].input_shape[1]
    assert type(layers[-1].output_shape[1]) is int
    nnet["outputSize"] = layers[-1].output_shape[1]

    nnet["minVals"].extend([0]*nnet["inputSize"])
    nnet["maxVals"].extend([0]*nnet["inputSize"])
    nnet["means"].extend([0]*(nnet["inputSize"]+1))
    nnet["ranges"].extend([0]*(nnet["inputSize"]+1))

    nnet["layerSizes"].append(nnet["inputSize"])

    for layer in layers:
        activation = layer.get_config()['activation']
        nnet["layerTypes"].append(activation)
        if activation == "relu" or activation == "linear":
            weights, biases = layer.get_weights()
            nnet["maxLayerSize"] = max(nnet["maxLayerSize"],
                    layer.units)
            nnet["weights"].append(weights.transpose())
            nnet["biases"].append(biases)
            nnet["layerSizes"].append(layer.units)
        else:
            raise Exception("Unsupported activation: " + activation)

    write_network(nnet, outfile)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: %s <h5-file>" % sys.argv[0])
        sys.exit(1)

    convertH5ToNnet(sys.argv[1], sys.argv[1].replace(".h5", ".nnet"))
