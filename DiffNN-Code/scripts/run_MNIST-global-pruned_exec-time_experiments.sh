#!/bin/bash

perturb=3
eps=1
extra_vars=100
# timeout in seconds
timeout=10

orig_nnet="nnet/3x100/3x100_Ori.nnet" # TODO: set this to the proper nnet
compressed_nnets="compressed_nnets/3x100/3x100_1epo_Adadelta_20Pru.nnet"

IFS=$'\n'
for compressed_nnet in $compressed_nnets; do
	out="exec-time_out_${compressed_nnet##*\/}"
	> $out
	for testcase in $( seq 400 499 ); do
		echo "./delta_network_test $testcase $orig_nnet $compressed_nnet $eps -p $perturb" >> $out
		timeout $timeout ./delta_network_test \
			$testcase \
			$orig_nnet $compressed_nnet\
			$eps $extra_vars -p $perturb 2>> $out
	done
done
