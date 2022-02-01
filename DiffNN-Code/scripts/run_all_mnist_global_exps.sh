#!/bin/bash

set -e

out_dir="logs"
mkdir -p $out_dir

perturbs="3
4
5
6
7"


run_for_nnet() {
	nnet=$1
	nnetPrime=$2
	evs=$3
	out=$4
	if [[ -f $out ]]; then
		echo "File already exists: $out"
		echo "Refusing to overwrite it"
		exit 1
	fi
	touch $out
	for testcase in $( seq 400 499 ); do
		echo "./delta_network_test $testcase $nnet $nnetPrime 0 -p $perturb -e $evs" >> $out
		./delta_network_test \
			$testcase \
			nnet/$nnet compressed_nnets/$nnetPrime\
			0 -m 0 -p $perturb -e $evs 2>> $out
	done
}

pair="mnist_relu_4_1024.nnet mnist_relu_4_1024_16bit.nnet 0"
pair=( $pair )
nnet=${pair[0]}
nnetPrime=${pair[1]}
extraVars=${pair[2]}

for perturb in $perturbs; do
	echo $perturb
	out_dir_mnist="$out_dir/mnist_final_p$perturb"
	mkdir -p $out_dir_mnist
	out="$out_dir_mnist/reludiff_allopts_$nnet"
	run_for_nnet $nnet $nnetPrime $extraVars $out &
done
wait
