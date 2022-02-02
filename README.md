This artifact contains the code for the ASE 2020 Paper "NeuroDiff: Differential Verification of Deep Neural Networks" and documentation on how to use it. We provide a VM working out of the box [here](). The username is "neurodiff" and the password is " " (i.e. a single space character). **Note on the VM: if you reconfigure the number of cpus on the VM, you _must_ recompile OpenBLAS (see below section). Failing to do so will cause buffer overflows and may casue NeuroDiff to produce unsound results**

# Installation
## Dependencies
The following debian and python packages must be install to build and run neurodiff:
```console
sudo apt install make git gcc g++ python3 python3-pip
pip3 install tabulate
```
In addition, if you want to compress your own neural networks, you will need the following python packages:
```console
pip3 install numpy==1.18.1 tensorflow==1.14.0 
```
In addition, NeuroDiff depends on OpenBLAS 0.3.6. We include the source code in this repo, and a script to install. To install, from this directory, run:
```console
bash install_OpenBLAS.sh
```

## Building
In our paper, we evaluated five different versions of our tool: Full NeuroDiff, NeuroDiff with only convex approximations, NeuroDiff with only intermediate symbolic variables, ReluDiff+, and ReluDiff. The version to build is specified using different make rules, which we document here. The following commands assume the user in the DiffNN-Code/ directory. Note that you can specify the parallelism level using CFLAGS=-DMAX_THREAD=$NUM_THREADS. In our experiments we used NUM_THREADS=12

### Building Full NeuroDiff
```console
CFLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineqall extravarssym all
```
### Building NeuroDiff w/ only intermediate variables
```console
CFLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineq extravars all
```
### Building NeuroDiff w/ only convex approximations
```console
CFLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineqall all
```
### Building ReluDiff+
```console
CFLAGS=-DMAX_THREAD=$NUM_THREADS make clean lineq all
```
### Building ReluDiff
```console
CFLAGS=-DMAX_THREAD=$NUM_THREADS make clean all
```

# File Contents
Here we document the source code, which is located in the folder ~/DiffNN-Code in the VM, and DiffNN-Code in this repository. The documentation for running NeuroDiff and ReluDiff can be found in the [next section](#tool-usage-documentation).
### DiffNN-Code/ 
This is the source code of NeuroDiff/ReluDiff. It has the following files:
#### delta\_network\_test.c
The main entry point for the tool. It handles parsing the input arguments and starting the verification process.
#### nnet.c
Contains the code for:
- loading a network (function: *load\_network*)
- loading an input property (function: *load\_inputs*)
- performing the forward pass and backward propagation (functions: *forward\_prop\_delta\_neurodiff* and *backward\_prop*)
#### nnet.h
Declarations for the functions in nnet.c.
#### split.c
Contains code for:
- The main function that starts the verification process (function: *direct\_run\_check\_delta*) which does the following:
	- Performs the forward pass
	- Checks if the property is satisfied
	- If not, then calling the function to refine the analysis by splitting the input interval (see the next bullet)
- The function that determines which input property to split on, and allocating new memory for the split (function: *split\_interval\_delta*)
#### split.h
Contains the definitions for the above functions.
#### matrix.h
Contains the definition for the struct that stores our matrices and declarations of our matrix mutliplication functions.
#### matrix.c
Contains the functions that call the OpenBLAS matrix multiplication functions on our matrix structs.
#### interval.h
Contains the definition for the struct that represents the intervals during the forward pass.
#### mnist\_tests.h
Defines three data structures for the mnist properties, which are:
- A 2-D array *mnist\_test* that contains 100 test MNIST images (an image is stored as an array of 784 integers)
- An array *correct\_class* of 100 integers corresponding to the correct classification of the 100 test MNIST images
- A 3x100 2-D array *random\_pixels* to determine which pixels to perturb in the 3 pixel experiment. Each entry in this array is a triple of integers each in the range 0-783 which correspond to the three pixels to perturb
#### HAR\_tests.h
Defines two data structures for the HAR properties, which are 
- A 2-D array *HAR\_test* that contains 100 test inputs from the HAR data set
- An array *HAR\_correct\_class* of 100 integers corresponding to the correct classification of the 100 test inputs
#### python/
This directory contains python scripts for
- Create a truncated network (file: *round\_nnet.py*)
## DiffNN-Code/python/round\_nnet.py
This script takes in a neural net file in the .nnet format, truncates its weights to 16 bits, and then outputs the result.
```console
python3 round_nnet.py NNET OUTPUT-PATH
```
- NNET: the file path to the input neural network
- OUTPUT-PATH: the location to write the truncated neural net to
#### scripts/
This directory contains scripts for:
- Creating all of the truncated and subtracted networks used in our evaluation (file: *make\_all\_compressed\_nnets.sh*)
- Running all of the experiments in our paper (files: *run\_&#10033;\_exec-time\_experiments.sh* and *run\_ACAS\_prop4\_depth\_exp.sh*)
- Demonstrating example usages of ReluDiff (files: *run\_&#10033;\_artifact.sh*)
#### nnet/
This directory contains all of the neural networks used in our examples. The file format for the neural network is described [here](https://github.com/sisl/NNet/).


# Tool Usage Documentation
Here we document how to use the various scripts and tools for each of the techniques we evaluated in our paper.
## DiffNN-code/delta\_network\_test
This is NeuroDiff/ReluDiff's main executable. To get a help menu, run *./delta\_network\_test* without any parameters. We also provide the documentation below.
```console
./delta_network_test PROPERTY NNET1 NNET2 EPSILON [OPTIONS]
```
- PROPERTY: an integer value indicating which pre-defined property to verify. The value can be:
	- 1-5, 7-15, 16, 26: These values correspond to the ACAS Xu properties orginally defined by Katz et al. in their [Reluplex](https://arxiv.org/pdf/1702.01135.pdf) and Wang et al. in their [ReluVal](https://arxiv.org/pdf/1804.10829.pdf) paper. See the appendix of [this](https://arxiv.org/pdf/1804.10829.pdf) paper for desciptions. The integers 1-5 and 7-15 directly correspond to properties 1-5 and 7-15 respectively. Property 6 is broken into two parts, corresponding to 16 and 26.
	- 400-499: These values are used for the MNIST properties. Each of the 100 values corresponds to a single input image, and then a perturbation is applied to the image to generate the input property. These properties require that either '-p' or '-t' is specified as an option. If '-p' is specified (see below), a global perturbation will be applied as described in the evaluation of our paper. If '-x' is specified, then the given number random pixels will be perturbed (see below). 
	- 1000-1099: These values are used for the HAR properties. Each of the 100 values corresponds to a single test input, and then a perturbation is applied to the input to generate the input property. These properties require that '-p' is specified as an option.
- NNET1, NNET2: file paths to \*.nnet files to compare.
- EPSILON: A floating point value. ReluDiff will attempt to verify that NNET1 and NNET2 cannot differ by more than EPSILON over the input region defined by PROPERTY
- OPTIONS<br/>
	-p PERTURB : specifies the strength of the global perturbation to apply to the MNIST and HAR properties. For MNIST this should be an integer between 0-254. For HAR, this should be a float between -1 to 1.<br/>
	-x NUM_PIX : Performs pixel perturbation on the MNIST images instead of global perturbation. NUM_PIX is the number of pixels to randomly select for perturbation.<br/>
	-e NUM_EXTRA_VARS : the max number of extra variables that can be used during a forward pass. Setting NUM_EXTRA_VARS=0 will use the heuristic described in the paper (this is probably what you want). Only valid when compiled with "extravars" or "extravarssym" make rule.
	-m DEPTH : (not used for our paper) forces the analysis to 2^DEPTH splits and then prints region verified at each depth<br/>
### Output
Running the above execuatable will produce some output about the verification results. We illustrate on an example:
```console
./delta_network_test 1 nnet/ACASXU_run2a_1_1_batch_2000.nnet compressed_nnets/ACASXU_run2a_1_1_batch_2000_16bit.nnet 0.05 -e 0
Total difference: 1.0736
Average difference: 0.0001
Normalizing input
Initial output delta:
[-269.650666 -323.034150 -343.905884 -359.871644 -358.394318 ]
[270.809356 324.417053 345.055053 360.276184 359.276062 ]
progress: [===                                               ] 4.10%
No adv!
time: 0.331189 


numSplits: 213
```
The total difference and average difference lines refer to the total absolute weight differences between the two networks, and the average difference between each pair of weights. The two arrays after "Initial output delta:" are the lower and upper bounds of the difference between the two networks computed on the _first forward pass_. "No adv!" indicates that the property was verified, followed by total time taken to verify the property. If there is no line beginning with "time:", then ReluDiff could neither verify nor disprove the property. The last line shows the total number of times the original input interval was split.

# Running the Experiments
We provide scripts with the commands used to produce Tables 1, 2, 3, and Figure 11. All commands below assume the user is in the directory DiffNN-Code/. The printed result tables are taken from the paper, which were run on the hardware described in Section 5.2 with 12 threads. We include the logs from these experiments in the directory logs_ASE/.
### Table 1
```console
# runs the verification, and collects output in the file neurodiff_acas_0.05 and reludiffp_acas_0.05
bash table_scripts/table_1.sh
# prints tables with results
bash table_scripts/print_acas_res.sh neurodiff_acas_0.05 reludiffp_acas_0.05
                        ReluDiff+                                               NeuroDiff
|   Property |   Proved |   Undet |    Time (s) |       |   Property |   Proved |   Undet |   Time (s) |
|------------|----------|---------|-------------|       |------------|----------|---------|------------|
|          1 |       44 |       1 | 4800.6      |       |          1 |       45 |       0 | 343.332    |
|          3 |       42 |       0 |    4.09981  |       |          3 |       42 |       0 |   1.68917  |
|          4 |       42 |       0 |    2.79513  |       |          4 |       42 |       0 |   1.24225  |
|          5 |        1 |       0 |    0.220824 |       |          5 |        1 |       0 |   0.100485 |
|          6 |        2 |       0 |    0.407    |       |          6 |        2 |       0 |   0.309289 |
|          7 |        0 |       1 | 1800        |       |          7 |        1 |       0 | 542.018    |
|          8 |        1 |       0 |  361.803    |       |          8 |        1 |       0 |  71.7295   |
|          9 |        1 |       0 |    2.31705  |       |          9 |        1 |       0 |   0.38704  |
|         10 |        1 |       0 |    0.706996 |       |         10 |        1 |       0 |   0.51966  |
|         11 |        1 |       0 |    0.265006 |       |         11 |        1 |       0 |   0.12171  |
|         12 |        1 |       0 |  360.85     |       |         12 |        1 |       0 |   1.7467   |
|         13 |        1 |       0 |    5.1239   |       |         13 |        1 |       0 |   3.41288  |
|         14 |        2 |       0 |   95.8638   |       |         14 |        2 |       0 |   0.311279 |
|         15 |        2 |       0 |   64.9665   |       |         15 |        2 |       0 |   0.376886 |

```

### Table 2
```console
# runs the verification, and collects output in the file neurodiff_acas_0.01 and reludiffp_acas_0.01
bash table_scripts/table_2.sh
# prints tables with results
bash table_scripts/print_acas_res.sh neurodiff_acas_0.01 reludiffp_acas_0.01
                        ReluDiff+                                               NeuroDiff
|   Property |   Proved |   Undet |     Time (s) |      |   Property |   Proved |   Undet |     Time (s) |
|------------|----------|---------|--------------|      |------------|----------|---------|--------------|
|          1 |       15 |      30 | 55778.6      |      |          1 |       41 |       4 | 11400.1      |
|          3 |       35 |       7 | 13642.2      |      |          3 |       42 |       0 |    14.2523   |
|          4 |       37 |       5 |  9114.96     |      |          4 |       42 |       0 |     3.81368  |
|          5 |        0 |       1 |  1800        |      |          5 |        1 |       0 |     0.326058 |
|          6 |        2 |       0 |     0.838987 |      |          6 |        2 |       0 |     1.04212  |
|          7 |        0 |       1 |  1800        |      |          7 |        0 |       1 |  1800        |
|          8 |        0 |       1 |  1800        |      |          8 |        1 |       0 |  1115.94     |
|          9 |        0 |       1 |  1800        |      |          9 |        1 |       0 |     2.43849  |
|         10 |        1 |       0 |     1.13708  |      |         10 |        1 |       0 |     1.55229  |
|         11 |        0 |       1 |  1800        |      |         11 |        1 |       0 |     0.317247 |
|         12 |        0 |       1 |  1800        |      |         12 |        1 |       0 |   132.233    |
|         13 |        1 |       0 |    14.7514   |      |         13 |        1 |       0 |    15.9159   |
|         14 |        0 |       2 |  3600        |      |         14 |        2 |       0 |  1589.27     |
|         15 |        0 |       2 |  3600        |      |         15 |        2 |       0 |   579.387    |
```

### Table 3
```console
# runs the verification, and collects output in the file reludiffp_mnist_pix and neurodiff_mnist_pix
bash table_scripts/table_3.sh
# print the results
bash table_scripts/print_mnist_pix_res.sh reludiffp_mnist_pix neurodiff_mnist_pix
                        ReluDiff+                                               NeuroDiff
|   Num Pixels |   Proved |   Undet |   Time (s) |      |   Num Pixels |   Proved |   Undet |   Time (s) |
|--------------|----------|---------|------------|      |--------------|----------|---------|------------|
|           15 |      100 |       0 |    1610.16 |      |           15 |      100 |       0 |    236.461 |
|           18 |       88 |      12 |   34505.8  |      |           18 |      100 |       0 |    540.827 |
|           21 |       30 |      70 |  145064    |      |           21 |      100 |       0 |   1004.04  |
|           24 |        1 |      99 |  179715.9  |      |           24 |       99 |       1 |   7860.14  |
|	    27 |	0 |	100 |  180000.0  |      |           27 |       83 |      17 |  49824     |
```

### Figure 11
```console
# runs the verification, and collects output in the file reludiffp_mnist_global and neurodiff_mnist_global
bash table_scripts/fig11.sh
# print the results
bash table_scripts/print_mnist_global_res.sh reludiffp_mnist_global neurodiff_mnist_global
                ReluDiff+                               NeuroDiff
|   Perturbation |   Proved |   Undet | |   Perturbation |   Proved |   Undet |
|----------------|----------|---------| |----------------|----------|---------|
|              3 |      100 |       0 | |              3 |      100 |       0 |
|              4 |       55 |      45 | |              4 |      100 |       0 |
|              5 |        0 |     100 | |              5 |      100 |       0 |
|              6 |        0 |     100 | |              6 |       93 |       7 |
|              7 |        0 |     100 | |              7 |       43 |      57 |
|	       8 |        0 |     100 | |	       8 |        4 |      96 |

```
