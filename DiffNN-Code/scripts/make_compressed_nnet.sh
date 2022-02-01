#!/bin/bash

if [[ -z $1 ]]; then
	echo "usage: $0 <orig-nnet>"
	exit 1
fi

nnet=$1

mkdir -p compressed_nnets
mkdir -p ../ReluVal-for-comparison/subbed_nnets

compressed_nnet=${nnet/nnet/compressed_nnets}
compressed_nnet=${compressed_nnet/\.nnet/\_16bit.nnet}
python3 python/round_nnet.py $nnet $compressed_nnet 16bit
python3 python/subtract_nnets.py $nnet $compressed_nnet \
		${compressed_nnet/compressed_nnets/..\/ReluVal-for-comparison/subbed_nnets}
