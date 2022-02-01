#!/bin/bash

set -e

out_dir="logs_ASE"
mkdir -p $out_dir

perturb=5

# mnist experiments first
out_dir_mnist="$out_dir/mnist_p$perturb"
mkdir -p $out_dir_mnist


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

nnets=(
"mnist_relu_2_512.nnet mnist_relu_2_512_16bit.nnet 300"
#"mnist_relu_3_100.nnet mnist_relu_3_100_16bit.nnet 100" 
#"mnist_relu_4_1024.nnet mnist_relu_4_1024_16bit.nnet 800"
)

for pair in "${nnets[@]}"; do 
	pair=( $pair )
	nnet=${pair[0]}
	nnetPrime=${pair[1]}
	extraVars=${pair[2]}

	# ReluDiff original
	#make clean  all
	#out="$out_dir_mnist/reludiff_orig_$nnet"
	#run_for_nnet $nnet $nnetPrime 0 $out
	
	# ReluDiff with linear relax on abs intervals
	make clean  lineq all 
	out="$out_dir_mnist/reludiff_lineq_$nnet"
	#run_for_nnet $nnet $nnetPrime 0 $out
	
	# ReluDiff with linear relax on all
	#make clean  lineqall all
	#out="$out_dir_mnist/reludiff_lineqall_$nnet"
	#run_for_nnet $nnet $nnetPrime 0 $out
	
	# ReluDiff with linear relax on abs intervals and intermediate sym vars
	#make clean  lineq extravars all
	#out="$out_dir_mnist/reludiff_lineq_extravars_$nnet"
	#run_for_nnet $nnet $nnetPrime $extraVars $out
	
	# ReluDiff with linear relax on all and intermediate sym vars
	#make clean  lineqall extravars all
	#out="$out_dir_mnist/reludiff_lineqall_extravars_$nnet"
	#run_for_nnet $nnet $nnetPrime $extraVars $out

	# ReluDiff with linear relax on all and intermediate sym vars and heuristic
	make clean  allopts all 
	out="$out_dir_mnist/reludiff_allopts_$nnet"
	run_for_nnet $nnet $nnetPrime $extraVars $out

done
