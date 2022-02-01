#!/bin/bash

# property number -> the networks this should be run on
declare -a exps
# property number -> networks this should _not_ be run on
# a value of "x" means skip nothing
declare -a skips

exps["1"]=".*"
skips["1"]="x"
#exps["2"]="[1-9]_[37]"
#skips["2"]="5_3|4_2"
exps["3"]="[1-9]_[37]"
skips["3"]="1_[7-9]"
exps["4"]=".*"
skips["4"]="[1-9]_[37]"
exps["5"]="1_1"
skips["5"]="x"
exps["16"]="1_1"
skips["16"]="x"
exps["26"]="1_1"
skips["26"]="x"
exps["7"]="1_9"
skips["7"]="x"
exps["8"]="2_9"
skips["8"]="x"
exps["9"]="3_3"
skips["9"]="x"
exps["10"]="4_5"
skips["10"]="x"
exps["11"]="1_1"
skips["11"]="x"
exps["12"]="3_3"
skips["12"]="x"
exps["13"]="1_1"
skips["13"]="x"
exps["14"]="4_1|5_1"
skips["14"]="x"
exps["15"]="4_1|5_1"
skips["15"]="x"

outfile="result"
touch $outfile
> $outfile

ACAS_depth=10
# ACAS experiments
# iterate array keys
for prop in "${!exps[@]}"; do
	for compressed_nnet in compressed_nnets/ACAS*; do
		model=$( echo $compressed_nnet | sed -e 's/^.*run2a_\([1-9]_[1-9]\).*$/\1/' )
		if [[ $model =~ ${exps[$prop]} && ! $model =~ ${skips[$prop]} ]]; then
			orig_nnet=${compressed_nnet/compressed_nnets/nnet}
			orig_nnet=${orig_nnet/\_[0-9]\.nnet/\.nnet}

			echo "./delta_network_test $prop $orig_nnet $compressed_nnet 0 0 $ACAS_depth" >> $outfile
			./delta_network_test $prop $orig_nnet $compressed_nnet 0 0 $ACAS_depth 2>> $outfile
		fi
	done
	exit 0
done

# MNIST experiments
# Run on the first 30
mnist_depth=0
perturbs="1
2"
for prop in $( seq 400 499 ); do
	for compressed_nnet in compressed_nnets/*mnist*; do
		for perturb in $perturbs; do
			orig_nnet=${compressed_nnet/compressed_nnets/nnet}
			orig_nnet=${orig_nnet/\_[0-9]\.nnet/\.nnet}

			echo "./delta_network_test $prop $orig_nnet $compressed_nnet 0 $perturb $mnist_depth" >> $outfile
			./delta_network_test $prop $orig_nnet $compressed_nnet 0 $perturb $mnist_depth 2>> $outfile
		done
	done
done
