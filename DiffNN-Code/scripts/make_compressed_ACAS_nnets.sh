#!/bin/bash

sigfigs="16bit"


for nnet in nnet/ACAS*; do
	for sigfig in $sigfigs; do
		compressed_nnet=${nnet/nnet/compressed_nnets}
		compressed_nnet=${compressed_nnet/\.nnet/\_$sigfig.nnet}
		python3 python/round_nnet.py $nnet $compressed_nnet $sigfig
		python3 python/subtract_nnets.py $nnet $compressed_nnet \
			${compressed_nnet/compressed_nnets/..\/ReluVal-for-comparison/subbed_nnets}
	done
done
