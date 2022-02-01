/*
 ------------------------------------------------------------------
 ** Top contributors:
 **   Shiqi Wang and Suman Jana
 ** This file is part of the ReluVal project.
 ** Copyright (c) 2018-2019 by the authors listed in the file LICENSE
 ** and their institutional affiliations.
 ** All rights reserved.
 -----------------------------------------------------------------
 *
 * This file to load and evaluate a neural network. The usage is:
 *  ./run_network [network-path] [input1] [input2] ...
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "split.h"

//extern int thread_tot_cnt;

/* print the progress if getting SIGQUIT */
void sig_handler(int signo)
{

    if (signo == SIGQUIT) {
        printf("progress: %d/1024\n", progress);
    }

}



int main( int argc, char *argv[])
{
    char *FULL_NET_PATH;
    int PROPERTY = -1;

    if (argc < 4) {
        printf("please specify a network and input\n");
        printf("./network_test [network-path] [property] [input1] ... \n");
        exit(1);
    }

    int numInputs = argc - 3;
    float *input_test = malloc(sizeof(float)*(numInputs));

    for (int i=1;i<argc;i++) {

        if (i == 1) {
            FULL_NET_PATH = argv[i];
        }

        if (i == 2) {
            PROPERTY = atoi(argv[i]);
        }

        if (i >= 3) {
            input_test[i - 3] = atof(argv[i]);
        }

    }

    openblas_set_num_threads(1);

    //clock_t start, end;
    srand((unsigned)time(NULL));



    struct NNet* nnet = load_network(FULL_NET_PATH);

    int outputSize   = nnet->outputSize;
    
    struct Matrix input = {input_test, 1, numInputs};

    if (PROPERTY < 300 || PROPERTY > 399) {
        normalize_input(nnet, &input);
    }

    float o[nnet->outputSize];
    struct Matrix output = {o, outputSize, 1};

    forward_prop(nnet, &input, &output);

    printMatrix(&output);

}
