#!/bin/bash

out_dir="logs_ASE"
mkdir -p $out_dir

eps="0.05"
extra_vars="0"

out_dir_acas="$out_dir/acas_newheur_$eps"
mkdir -p $out_dir_acas

# ReluDiff original (we already did this)
# make clean bench all
# out="$out_dir_acas/reludiff_orig"
# bash scripts/run_ACAS_exec-time_experiment.sh $eps 0 $out

# ReluDiff with linear relax on abs intervals
#make clean bench lineq all
#out="$out_dir_acas/reludiff_lineq"
#bash scripts/run_ACAS_exec-time_experiment.sh $eps 0 $out

# ReluDiff with linear relax on all
#make clean bench lineqall all
#out="$out_dir_acas/reludiff_lineqall"
#bash scripts/run_ACAS_exec-time_experiment.sh $eps 0 $out

# ReluDiff with linear relax on abs intervals and intermediate sym vars
#make clean bench lineq extravars all
#out="$out_dir_acas/reludiff_lineq_extravars"
#bash scripts/run_ACAS_exec-time_experiment.sh $eps $extra_vars $out

# ReluDiff with linear relax on all and intermediate sym vars
make clean lineqall extravarssym bench all
out="$out_dir_acas/reludiff_lineqall_extravars"
bash scripts/run_ACAS_exec-time_experiment.sh $eps $extra_vars $out
