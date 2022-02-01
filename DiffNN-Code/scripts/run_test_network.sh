#!/bin/bash

nnet="b2layer_unquantized.nnet"
sigfig="0"

python python/round_nnet.py nnet/$nnet compressed_nnets/${nnet/\.nnet/_$sigfig\.nnet} $sigfig
python python/subtract_nnets.py nnet/$nnet compressed_nnets/${nnet/\.nnet/_$sigfig\.nnet} \
	../ReluVal-for-comparison/subbed_nnets/${nnet/\.nnet/_$sigfig\.nnet}
./delta_network_test 300 nnet/$nnet compressed_nnets/${nnet/\.nnet/_$sigfig\.nnet} 0 0 0
cd ../ReluVal-for-comparison
./network_test 300 compressed_nnets/${nnet/\.nnet/_$sigfig\.nnet} 0 0 0
