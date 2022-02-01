#!/bin/bash

nnets="mnist_relu_4_1024.nnet
mnist_relu_3_100.nnet
mnist_relu_2_512.nnet"

sigfigs="16bit"

for nnet in $nnets; do
	for sigfig in $sigfigs; do
		python3 python/round_nnet.py nnet/$nnet \
			compressed_nnets/${nnet/\.nnet/_$sigfig.nnet} \
			$sigfig
		python3 python/subtract_nnets.py nnet/$nnet \
			compressed_nnets/${nnet/\.nnet/_$sigfig.nnet} \
			../ReluVal-for-comparison/subbed_nnets/${nnet/\.nnet/_$sigfig.nnet}
	done
done
