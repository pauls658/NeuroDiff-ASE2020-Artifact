if [[ -z $1 ]]; then
	echo "usage: bash $0 <eps>"
	exit 1
fi
timeout=1800
eps=$1
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 16 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 16 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 26 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 26 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
