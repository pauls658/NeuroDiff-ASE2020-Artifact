#!/bin/bash
reludiff_res=$1
neurodiff_res=$2

echo "			ReluDiff+" > reludiff_tmp
python3 python/read_acas_res.py $reludiff_res >> reludiff_tmp
echo "					NeuroDiff" > neurodiff_tmp
python3 python/read_acas_res.py $neurodiff_res >> neurodiff_tmp
paste reludiff_tmp neurodiff_tmp
rm neurodiff_tmp reludiff_tmp
