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
#include "HAR_tests.h"
#include <float.h>


int main( int argc, char *argv[])
{

    char *FULL_NET_PATH;

    if (argc != 2) {
        printf("please specify a network\n");
        printf("./network_test [network-path]\n");
        exit(1);
    }

    for (int i=1;i<argc;i++) {

        if (i == 1) {
            FULL_NET_PATH = argv[i];
        }

    }

    openblas_set_num_threads(1);

    //clock_t start, end;
    srand((unsigned)time(NULL));



    struct NNet* nnet = load_network(FULL_NET_PATH);

    int outputSize   = nnet->outputSize;

    float in[nnet->inputSize];
    struct Matrix input = {in, 1, nnet->inputSize};

    float o[nnet->outputSize];
    struct Matrix output = {o, outputSize, 1};

    float max;
    int max_i;
    int incorrect = 0;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < nnet->inputSize; j++) {
            in[j] = HAR_test[i][j];
        }
        forward_prop(nnet, &input, &output);
		printMatrix(&output);
        max_i = -1;
        max = -FLT_MIN;
        for (int k = 0; k < nnet->outputSize; k++) {
            if (o[k] > max) {
                max_i = k;
                max = o[k];
            }
        }
        if (max_i != HAR_correct_class[i]) {
            printf("Wrong class for test %d.\n", i);
            incorrect++;
        }
    }

    printf("Total incorrect: %d\n", incorrect);

}
