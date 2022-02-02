#!/bin/bash

NUM_THREADS=4
FLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineqall extravarssym all
bash artifact_scripts/mnist_global_cmds.sh |& tee neurodiff_mnist_global

FLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineq all
bash artifact_scripts/mnist_global_cmds.sh $eps |& tee reludiffp_mnist_global
