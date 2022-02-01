#!/bin/bash

out="rerun.out"
> $out
timeout=1800

echo "./delta_network_test 1 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet 0.01 20" >> $out
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_3_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_3_7_batch_2000_16bit.nnet 0.01 20 2>> $out

echo "./delta_network_test 1 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet 0.01 20" >> $out
timeout -s 2 $timeout ./delta_network_test 1 nnet/ACASXU_run2a_4_7_batch_2000.nnet compressed_nnets/ACASXU_run2a_4_7_batch_2000_16bit.nnet 0.01 20 2>> $out

