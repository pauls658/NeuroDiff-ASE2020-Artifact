#!/bin/bash

for nnet in $( ls compressed_nnets ); do
	if [[ "$nnet" =~ .*mnist.* ]]; then
		./test_mnist nnet/${nnet/_[0-9]\.nnet/.nnet} > out_orig
		./test_mnist compressed_nnets/$nnet > out_compressed
		../ReluVal-for-comparison/test_mnist \
			../ReluVal-for-comparison/subbed_nnets/$nnet > out_subbed
		python3 python/compare_outputs.py out_orig out_compressed out_subbed
	fi # maybe do similar for ACAS?
done
rm out_orig out_compressed out_subbed
