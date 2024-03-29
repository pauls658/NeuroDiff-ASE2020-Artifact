#!/bin/bash

NUM_THREADS=4
eps=0.01
FLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineqall extravarssym all
bash artifact_scripts/acas_cmds.sh $eps |& tee neurodiff_acas_$eps

FLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineq all
bash artifact_scripts/acas_cmds.sh $eps |& tee reludiffp_acas_$eps
