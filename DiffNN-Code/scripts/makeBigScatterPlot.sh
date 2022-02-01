#!/bin/bash

out=scaplot.csv
> $out
logdir=logs_ASE
cmd="python3 python/output_analysis.py scatplot"
set -e

# Original ICSE experiments
logfiles="exec-time_out_mnist_relu_2_512_16bit.nnet
exec-time_out_mnist_relu_3_100_16bit.nnet
exec-time_out_mnist_relu_4_1024_16bit.nnet
exec-time_out_pix_mnist_relu_2_512_16bit.nnet
exec-time_out_pix_mnist_relu_3_100_16bit.nnet
exec-time_out_pix_mnist_relu_4_1024_16bit.nnet
HAR_exec-time_out"
path="$logdir/icse_exps"
for logfile in $logfiles; do
	$cmd $path/neurodiff/$logfile $path/reludiff/$logfile >> $out
done

$cmd $logdir/acas_jiawei_0.05/reludiff_lineqall_extravars $logdir/acas_jiawei_0.05/reludiff_lineq >> $out



# New experiments
# ACAS
$cmd $logdir/acas_jiawei2_0.01/reludiff_lineqall_extravars $logdir/acas_jiawei_0.01/reludiff_lineq >> $out

# MNIST pixel
pixdirs="mnist_x15
mnist_x18
mnist_x21
mnist_x24
mnist_x27
mnist_x30"
for pixdir in $pixdirs; do
	echo "#pixel exp: $pixdir" >> $out
	rdifffile="$logdir/$pixdir/reludiff_lineq_mnist_relu_4_1024.nnet"

	if [[ ! -f $rdifffile ]]; then
		rdifffile="none"
	fi
	$cmd $logdir/$pixdir/reludiff_lineqall_extravarssym_mnist_relu_4_1024.nnet $rdifffile >> $out
done

# MNIST global
gdirs="mnist_final_p3
mnist_final_p4
mnist_final_p5
mnist_final_p6
mnist_final_p7"

for gdir in $gdirs; do
	echo "#global exp: $gdir" >> $out
	#echo "$cmd $logdir/$pixdir/reludiff_lineqall_extravarssym_mnist_relu_4_1024.nnet $logdir/$pixdir/reludiff_lineq_mnist_relu_4_1024.nnet 1"
	$cmd $logdir/$gdir/reludiff_lineqall_extravarssym_mnist_relu_4_1024.nnet $logdir/$gdir/reludiff_lineq_mnist_relu_4_1024.nnet 1 >> $out
done
