#!/bin/bash
reludiff_res=$1
neurodiff_res=$2

echo "		ReluDiff+" > reludiff_tmp
python3 python/read_mnist_global_res.py $reludiff_res 1 >> reludiff_tmp
echo "			NeuroDiff" > neurodiff_tmp
python3 python/read_mnist_global_res.py $neurodiff_res 1 >> neurodiff_tmp
paste reludiff_tmp neurodiff_tmp
rm neurodiff_tmp reludiff_tmp
