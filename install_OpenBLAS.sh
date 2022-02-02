#!/bin/bash

install_dir="$( pwd )/DiffNN-Code"
unzip v0.3.6.zip
cd OpenBLAS-0.3.6
make
make PREFIX=$install_dir install
