#!/bin/bash
reludiff_res=$1
neurodiff_res=$2

echo "			ReluDiff+" > reludiff_tmp
python3 python/read_mnist_pix_res.py $reludiff_res >> reludiff_tmp
echo "					NeuroDiff" > neurodiff_tmp
python3 python/read_mnist_pix_res.py $neurodiff_res >> neurodiff_tmp
paste neurodiff_tmp reludiff_tmp 
rm neurodiff_tmp reludiff_tmp
