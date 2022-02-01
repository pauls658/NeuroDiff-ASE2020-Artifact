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
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_1_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_1_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_3_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_4_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 1 nnet/ACASXU_run2a_5_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_5_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_1_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_1_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_1_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_1_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_1_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_1_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_1_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_1_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_1_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_1_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_3_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_3_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_4_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_4_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 3 nnet/ACASXU_run2a_5_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 3 nnet/ACASXU_run2a_5_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_1_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_1_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_1_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_1_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_1_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_1_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_1_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_1_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_1_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_1_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_3_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_3_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_4_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_4_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_2_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_2_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_2_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_4_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_4_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_4_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_6_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_6_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_6_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_7_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_7_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_8_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_8_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_8_batch_2000_16bit.nnet $eps
echo "./delta_network_test 4 nnet/ACASXU_run2a_5_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 4 nnet/ACASXU_run2a_5_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 5 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 5 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 7 nnet/ACASXU_run2a_1_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 7 nnet/ACASXU_run2a_1_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 8 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 8 nnet/ACASXU_run2a_2_9_batch_2000.nnet compressed_nnets/ACASXU_run2a_2_9_batch_2000_16bit.nnet $eps
echo "./delta_network_test 9 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 9 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 10 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 10 nnet/ACASXU_run2a_4_5_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_5_batch_2000_16bit.nnet $eps
echo "./delta_network_test 11 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 11 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 12 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 12 nnet/ACASXU_run2a_3_3_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_3_batch_2000_16bit.nnet $eps
echo "./delta_network_test 13 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 13 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 14 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 14 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 14 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 14 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 15 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 15 nnet/ACASXU_run2a_4_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 15 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 15 nnet/ACASXU_run2a_5_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_5_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 16 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 16 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
echo "./delta_network_test 26 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps"
timeout -s 2 $timeout ./delta_network_test 26 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet $eps
