#!/bin/bash

./delta_network_test 1 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_1.nnet 0.1 0 -1


#reluval cmd
./network_test 1 subbed_nnets/ACASXU_run2a_1_1_batch_2000_1.nnet 0.1 0 -1
