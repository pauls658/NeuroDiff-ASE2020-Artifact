/*
 ------------------------------------------------------------------
 ** Top contributors:
 **   Shiqi Wang and Suman Jana
 ** This file is part of the ReluVal project.
 ** Copyright (c) 2018-2019 by the authors listed in the file LICENSE
 ** and their institutional affiliations.
 ** All rights reserved.
 -----------------------------------------------------------------
 */

#include <assert.h>
#include "nnet.h"
#include "split.h"
#include <math.h>
#include <float.h>
#include <fenv.h>
#include <stdlib.h>
#include "mnist_tests.h"
#include "cifar10_test.h"
#include "HAR_tests.h"
//#include <lp_lib.h>

#ifdef STATS
#define NEURONSTATE
#define NEURONSTATETOTALS
#define NEURONEQ
#define NEURONBOUNDS
#define FWDINTERVAL
#endif

#if defined(STATS) || defined(LATEXTABLE) || defined(NEURONSTATETOTALS)
#define LOGGING
#endif


void *memset(void *ptr, int value, size_t numBytes);

void *memcpy(void *to, const void *from, size_t numBytes);

void computeAllBounds(float *eqLow, float *eqUp,
                      struct Interval *input, int inputSize,
                      float *low, float *lowsUp, float *upsLow, float *up);

#define max_error(low, up) -low*up/(up - low)

void print_eq(float *arr, int len) {
    fprintf(stderr, "[");
    for (int i = 0; i < len; i++) {
        if (arr[i] != 0.0) {
            if (i == 0 )
                fprintf(stderr, "%4.2f", arr[i]);
            else
                fprintf(stderr, "%+4.2f", arr[i]);
            if (i < len - 1) { // variables
                fprintf(stderr, "x_%d ", i+1);
            } else {
                fprintf(stderr, " ");
            }
        }
    }
    fprintf(stderr, "]");
}

void print_farray(float *arr, int len) {
    fprintf(stderr, "[ ");
    for (int i = 0; i < len; i++) {
        fprintf(stderr, "%4.12f ", arr[i]);
    }
    fprintf(stderr, "]");
}

void printLatexEq(struct Interval *interval, int offset, int eqSize) {
    fprintf(stderr, "[");
    print_eq(interval->lower_matrix.data + offset, eqSize);
    fprintf(stderr, ",");
    print_eq(interval->upper_matrix.data + offset, eqSize);
    fprintf(stderr, "]");
}

int **neuronStateCounts;

void log_neuron_pre_relu(int layer, int neuron, struct Interval *input,
                         struct Interval *interval, struct Interval *intervalPrime, struct Interval *intervalDelta,
                         float low, float lowsUp, float upsLow, float up,
                         float lowPrime, float lowsUpPrime, float upsLowPrime, float upPrime, float deltaLow,
                         float deltaUp,
                         struct NNet *network1) {
#ifdef LOGGING
    int eqSize = interval->lower_matrix.row;
    int state, statePrime;
    state = up <= 0.0 ? 0 : low >= 0.0 ? 1 : 2;
    statePrime = upPrime <= 0.0 ? 0 : lowPrime >= 0.0 ? 1 : 2;
#endif
#ifdef NEURONSTATE
    fprintf(stderr, "## neuron_%d,%d [%d,%d]\n",
            layer + 1, neuron + 1, state, statePrime);
#endif
#ifdef NEURONSTATETOTALS
    int s;
    if (state == 0) {
        if (statePrime == 0) {
            s = 0;
        } else if (statePrime == 1) {
            s = 1;
        } else if (statePrime == 2) {
            s = 2;
        }
    } else if (state == 1) {
        if (statePrime == 0) {
            s = 3;
        } else if (statePrime == 1) {
            s = 4;
        } else if (statePrime == 2) {
            s = 5;
        }
    } else if (state == 2) {
        if (statePrime == 0) {
            s = 6;
        } else if (statePrime == 1) {
            s = 7;
        } else if (statePrime == 2) {
            s = 8;
        }
    }
    if (layer != network1->numLayers - 1) {
        neuronStateCounts[layer][s] += 1;
    }
#endif
#ifdef NEURONEQ
    fprintf(stderr, "### PRE-RELU\n");
#endif
#ifdef NEURONBOUNDS
    fprintf(stderr, "#### abs bounds: [%4.12f,%4.12f] [%4.12f,%4.12f] [%4.12f,%4.12f]\n",
            low, up, lowPrime, upPrime, deltaLow, deltaUp);
#endif
#ifdef NEURONEQ
    fprintf(stderr, "#### eq: ");
    print_eq(interval->lower_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, " ");
    print_eq(interval->upper_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, "\n#### eq': ");
    print_eq(intervalPrime->lower_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, " ");
    print_eq(intervalPrime->upper_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, "\n");
    fprintf(stderr, "#### eqD: ");
    print_eq(intervalDelta->lower_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, " ");
    print_eq(intervalDelta->upper_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, "\n");
    //fprintf(stderr, "#### error: [%4.4f,%4.4f] [%4.4f,%4.4f]\n",
    //        max_error(low, lowsUp), max_error(upsLow, up),
    //        max_error(lowPrime, lowsUpPrime), max_error(upsLowPrime, upPrime));


#endif
#ifdef LATEXTABLE
    fprintf(stderr, "\t$\\IntIn{\\nd{%d}{%d}}$ & ", layer+1, neuron+1);
    //fprintf(stderr, "\\makecell{");
    //printLatexEq(interval, neuron*eqSize, eqSize);
    //fprintf(stderr, "} & \\makecell{");
    //printLatexEq(intervalPrime, neuron*eqSize, eqSize);
    //fprintf(stderr, "} & \\makecell{");
    printLatexEq(intervalDelta, neuron*eqSize, eqSize);
    //fprintf(stderr, "}");
    fprintf(stderr, " \\\\ \\hline \n");
#endif
}

void log_neuron_post_relu(int layer, int neuron, struct Interval *input, \
        struct Interval *interval, struct Interval *intervalPrime, struct Interval *intervalDelta) {
#ifdef LOGGING
    int eqSize = interval->lower_matrix.row;
#endif
#ifdef NEURONEQ
    fprintf(stderr, "### POST-RELU\n");
    fprintf(stderr, "#### eq: ");
    print_eq(interval->lower_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, " ");
    print_eq(interval->upper_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, "\n#### eq': ");
    print_eq(intervalPrime->lower_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, " ");
    print_eq(intervalPrime->upper_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, "\n");
    fprintf(stderr, "#### eqD: ");
    print_eq(intervalDelta->lower_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, " ");
    print_eq(intervalDelta->upper_matrix.data + neuron*eqSize, eqSize);
    fprintf(stderr, "\n");

#endif
#ifdef LATEXTABLE
    fprintf(stderr, "\t$\\IntOut{\\nd{%d}{%d}}$ & ", layer+1, neuron+1);
    //fprintf(stderr, "\\makecell{");
    //printLatexEq(interval, neuron*eqSize, eqSize);
    //fprintf(stderr, "} & \\makecell{");
    //printLatexEq(intervalPrime, neuron*eqSize, eqSize);
//    fprintf(stderr, "} & \\makecell{");
    printLatexEq(intervalDelta, neuron*eqSize, eqSize);
    //fprintf(stderr, "}");
    fprintf(stderr, " \\\\ \\hline \\hline \n");
#endif
}

void log_forward_pass_enter(struct NNet *network1, struct Interval *input) {
#ifdef LOGGING
    int inputSize = input->lower_matrix.col;
#endif
#ifdef FWDINTERVAL
    fprintf(stderr, "# Forward pass: ");
    print_farray(input->lower_matrix.data, inputSize);
    fprintf(stderr, " ");
    print_farray(input->upper_matrix.data, inputSize);
    fprintf(stderr, "\n");
#endif
#ifdef NEURONSTATETOTALS
    neuronStateCounts = (int**)malloc((network1->numLayers-1) * sizeof(int*));
    for (int i = 0; i < network1->numLayers - 1; i++) {
        neuronStateCounts[i] = (int*)malloc(9 * sizeof(int));
        for (int j = 0; j < 9; j++) {
            neuronStateCounts[i][j] = 0;
        }
    }
#endif
}

void log_forward_pass_exit(struct NNet *network1, struct Interval *output) {
#ifdef LOGGING
    int outputSize = output->lower_matrix.col;
#endif
#ifdef NEURONSTATETOTALS
    fprintf(stderr, "# State counts:\n");
    fprintf(stderr, "Neuron state:\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
    for (int i = 0; i < network1->numLayers - 1; i++) {
        fprintf(stderr, "layer %d:", i);
        for (int j = 0; j < 9; j++) {
            fprintf(stderr, "\t%d", neuronStateCounts[i][j]);
        }
        fprintf(stderr, "\n");
        free(neuronStateCounts[i]);
    }
    free(neuronStateCounts);
#endif
#ifdef FWDINTERVAL
    fprintf(stderr, "# Output: ");
    print_farray(output->lower_matrix.data, outputSize);
    fprintf(stderr, " ");
    print_farray(output->upper_matrix.data, outputSize);
    fprintf(stderr, "\n");
#endif
}

/*
 * Load_network is a function modified from Reluplex
 * It takes in a nnet filename with path and load the
 * network from the file
 * Outputs the NNet instance of loaded network.
 */
struct NNet *load_network(const char *filename) {

    FILE *fstream = fopen(filename, "r");

    if (fstream == NULL) {
        printf("Wrong network!\n");
        exit(1);
    }

    /* Largest line is ~39K bytes (for the CIFAR networks),
     * but allocate a bit extra just in case... */
    int bufferSize = 100240;
    char *buffer = (char *) malloc(sizeof(char) * bufferSize);
    char *record, *line;
    int i = 0, layer = 0, row = 0, j = 0, param = 0;

    struct NNet *nnet = (struct NNet *) malloc(sizeof(struct NNet));

    line = fgets(buffer, bufferSize, fstream);

    /* Skip comments in the beginning */
    while (strstr(line, "//") != NULL) {
        line = fgets(buffer, bufferSize, fstream);
    }

    /* First four inputs are number of layers, number of inputs,
     * number of outputs, and maximum layer size. */
    record = strtok(line, ",\n");
    nnet->numLayers = atoi(record);
    nnet->inputSize = atoi(strtok(NULL, ",\n"));
    nnet->outputSize = atoi(strtok(NULL, ",\n"));
    nnet->maxLayerSize = atoi(strtok(NULL, ",\n"));

    /* Read array sizes */
    nnet->layerSizes = (int *) malloc(sizeof(int) * (nnet->numLayers + 1));
    line = fgets(buffer, bufferSize, fstream);
    record = strtok(line, ",\n");
    for (i = 0; i < ((nnet->numLayers) + 1); i++) {
        nnet->layerSizes[i] = atoi(record);
        record = strtok(NULL, ",\n");
    }

    /* Deprecated property... */
    line = fgets(buffer, bufferSize, fstream);
    record = strtok(line, ",\n");
    nnet->symmetric = atoi(record);

    /* Mins, maxes, means, and ranges used for normalizing inputs...*/
    /* Read mins */
    nnet->mins = (float *) malloc(sizeof(float) * nnet->inputSize);
    line = fgets(buffer, bufferSize, fstream);
    record = strtok(line, ",\n");
    for (i = 0; i < (nnet->inputSize); i++) {
        nnet->mins[i] = (float) atof(record);
        record = strtok(NULL, ",\n");
    }

    /* Read maxes */
    nnet->maxes = (float *) malloc(sizeof(float) * nnet->inputSize);
    line = fgets(buffer, bufferSize, fstream);
    record = strtok(line, ",\n");
    for (i = 0; i < (nnet->inputSize); i++) {
        nnet->maxes[i] = (float) atof(record);
        record = strtok(NULL, ",\n");
    }

    /* Read means */
    nnet->means = (float *) malloc(sizeof(float) * (nnet->inputSize + 1));
    line = fgets(buffer, bufferSize, fstream);
    record = strtok(line, ",\n");
    for (i = 0; i < ((nnet->inputSize) + 1); i++) {
        nnet->means[i] = (float) atof(record);
        record = strtok(NULL, ",\n");
    }

    /* Read ranges */
    nnet->ranges = (float *) malloc(sizeof(float) * (nnet->inputSize + 1));
    line = fgets(buffer, bufferSize, fstream);
    record = strtok(line, ",\n");
    for (i = 0; i < ((nnet->inputSize) + 1); i++) {
        nnet->ranges[i] = (float) atof(record);
        record = strtok(NULL, ",\n");
    }

    /* Alloc memory for reading in weights and biases */
    nnet->matrix = (float ****) malloc(sizeof(float *) * nnet->numLayers);
    for (layer = 0; layer < (nnet->numLayers); layer++) {
        nnet->matrix[layer] = \
                (float ***) malloc(sizeof(float *) * 2);
        nnet->matrix[layer][0] = \
                (float **) malloc(sizeof(float *) * nnet->layerSizes[layer + 1]);
        nnet->matrix[layer][1] = \
                (float **) malloc(sizeof(float *) * nnet->layerSizes[layer + 1]);

        for (row = 0; row < nnet->layerSizes[layer + 1]; row++) {
            nnet->matrix[layer][0][row] = \
                    (float *) malloc(sizeof(float) * nnet->layerSizes[layer]);
            nnet->matrix[layer][1][row] = (float *) malloc(sizeof(float));
        }

    }

    layer = 0;
    param = 0;
    i = 0;
    j = 0;

    char *tmpptr = NULL;

    float w = 0.0;
    /* Read weights and biases */
    while ((line = fgets(buffer, bufferSize, fstream)) != NULL) {

        if (i >= nnet->layerSizes[layer + 1]) {

            if (param == 0) {
                param = 1;
            } else {
                param = 0;
                layer++;
            }

            i = 0;
            j = 0;
        }

        record = strtok_r(line, ",\n", &tmpptr);

        while (record != NULL) {
            w = (float) atof(record);
            nnet->matrix[layer][param][i][j] = w;
            j++;
            record = strtok_r(NULL, ",\n", &tmpptr);
        }

        tmpptr = NULL;
        j = 0;
        i++;
    }


    /* Copy weights and biases into Matrix structs */
    struct Matrix *weights = malloc(nnet->numLayers * sizeof(struct Matrix));
    struct Matrix *bias = malloc(nnet->numLayers * sizeof(struct Matrix));
    for (int layer = 0; layer < nnet->numLayers; layer++) {
        weights[layer].row = nnet->layerSizes[layer];
        weights[layer].col = nnet->layerSizes[layer + 1];
        weights[layer].data = (float *) malloc(sizeof(float)\
 * weights[layer].row * weights[layer].col);

        int n = 0;


        for (int i = 0; i < weights[layer].col; i++) {

            for (int j = 0; j < weights[layer].row; j++) {
                weights[layer].data[n] = nnet->matrix[layer][0][i][j];
                n++;
            }

        }

        bias[layer].col = nnet->layerSizes[layer + 1];
        bias[layer].row = (float) 1;
        bias[layer].data = (float *) malloc(sizeof(float) * bias[layer].col);

        for (int i = 0; i < bias[layer].col; i++) {
            bias[layer].data[i] = nnet->matrix[layer][1][i][0];
        }


    }

    nnet->weights = weights;
    nnet->bias = bias;

    free(buffer);
    fclose(fstream);

    return nnet;
}


/*
 * Subtracts the weights of nnet2 from the weights of nnet1.
 */
void compute_network_delta(struct NNet *nnet1, struct NNet *nnet2) {
	float totalDiff = 0.0;
	int numParams = 0;
    for (int k = 0; k < nnet1->numLayers; k++) {
        struct Matrix new = nnet1->weights[k];
        struct Matrix orig = nnet2->weights[k];
        for (int i = 0; i < new.col * new.row; i++) {
			numParams++;
            new.data[i] -= orig.data[i];
			totalDiff += fabs(new.data[i]);
        }

        new = nnet1->bias[k];
        orig = nnet2->bias[k];
        for (int i = 0; i < new.col * new.row; i++) {
            new.data[i] -= orig.data[i];
        }
    }
	fprintf(stderr, "Total difference: %4.4f\n", totalDiff);
	fprintf(stderr, "Average difference: %4.4f\n", totalDiff/((float)numParams));
}

/*
 * Allocates memory for and loads the positive/negative weights.
 */
void load_positive_and_negative_weights(struct NNet *nnet) {
    struct Matrix *posWeights = malloc(nnet->numLayers * sizeof(struct Matrix));
    struct Matrix *negWeights = malloc(nnet->numLayers * sizeof(struct Matrix));
    struct Matrix weights;
    for (int layer = 0; layer < nnet->numLayers; layer++) {
        weights = nnet->weights[layer];

        posWeights[layer].row = weights.row;
        posWeights[layer].col = weights.col;
        negWeights[layer].row = weights.row;
        negWeights[layer].col = weights.col;
        posWeights[layer].data = (float *) malloc(sizeof(float) * weights.row * weights.col);
        negWeights[layer].data = (float *) malloc(sizeof(float) * weights.row * weights.col);
        memset(posWeights[layer].data, 0, sizeof(float) * weights.row * weights.col);
        memset(negWeights[layer].data, 0, sizeof(float) * weights.row * weights.col);

        for (int i = 0; i < weights.row * weights.col; i++) {
            if (weights.data[i] >= 0) {
                posWeights[layer].data[i] = weights.data[i];
            } else {
                negWeights[layer].data[i] = weights.data[i];
            }
        }
    }
    nnet->posWeights = posWeights;
    nnet->negWeights = negWeights;
}

/*
 * destroy_network is a function modified from Reluplex
 * It release all the memory allocated to the network instance
 * It takes in the instance of nnet
 */
void destroy_network(struct NNet *nnet) {

    int i = 0, row = 0;
    if (nnet != NULL) {

        for (i = 0; i < (nnet->numLayers); i++) {

            for (row = 0; row < nnet->layerSizes[i + 1]; row++) {
                free(nnet->matrix[i][0][row]);
                free(nnet->matrix[i][1][row]);
            }

            free(nnet->matrix[i][0]);
            free(nnet->matrix[i][1]);
            free(nnet->weights[i].data);
            free(nnet->posWeights[i].data);
            free(nnet->negWeights[i].data);
            free(nnet->bias[i].data);
            free(nnet->matrix[i]);
        }

        free(nnet->weights);
        free(nnet->posWeights);
        free(nnet->negWeights);
        free(nnet->bias);
        free(nnet->layerSizes);
        free(nnet->mins);
        free(nnet->maxes);
        free(nnet->means);
        free(nnet->ranges);
        free(nnet->matrix);
        free(nnet);
    }

}


/*
 * Load the inputs of all the predefined properties
 * It takes in the property and input pointers
 */
void load_inputs(int PROPERTY, int inputSize, struct InputSpace *inputSpace, struct VerificationParams *verificationParams) {
    float *u = inputSpace->inputs.upper_matrix.data;
    float *l = inputSpace->inputs.lower_matrix.data;
    float *ud = inputSpace->deltas.upper_matrix.data;
    float *ld = inputSpace->deltas.lower_matrix.data;

    // Other properties don't start with an initial delta
    inputSpace->deltas.lower_matrix.data = NULL;
    inputSpace->deltas.upper_matrix.data = NULL;
    inputSpace->deltas.lower_matrix.row = inputSpace->deltas.upper_matrix.row =
            inputSpace->deltas.lower_matrix.col = inputSpace->deltas.upper_matrix.col = 0;

    // all inputs min max
    if (PROPERTY == 800) {
        struct NNet *nnet = verificationParams->nnet;
        float upper[nnet->inputSize];
        float lower[nnet->inputSize];
        for (int i = 0; i < inputSize; i++) {
            upper[i] = nnet->maxes[i];
            lower[i] = nnet->mins[i];
        }
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 3;
        return;
    }

    // HAR properties
    float perturb = verificationParams->perturb;
    if (PROPERTY >= 1000 && PROPERTY < 2000) {
        int testNum = PROPERTY - 1000;
        for (int i = 0; i < 561; i++) {
            u[i] = fmin((HAR_test[testNum][i] + perturb), 1.0);
            l[i] = fmax((HAR_test[testNum][i] - perturb), -1.0);
        }
        verificationParams->target = HAR_correct_class[testNum];
    }

    if (verificationParams->mnistPixExp == 0) {
        // MNIST properties global exp
        if (PROPERTY >= 400 && PROPERTY <= 499) {
            int testNum = PROPERTY - 400;
            for (int i = 0; i < 784; i++) {
                u[i] = fmin((mnist_test[testNum][i] + perturb) / 255.0, 1.0);
                l[i] = fmax((mnist_test[testNum][i] - perturb) / 255.0, 0.0);
            }
            verificationParams->target = correct_class[testNum];
            return;
        }
    } else {
        // MNIST properties pixel exp
        if (PROPERTY >= 400 && PROPERTY <= 499) {
            // pixel experiment
            int num_pixels = verificationParams->mnistPixExp;
            int testNum = PROPERTY - 400;
            for (int i = 0; i < 784; i++) {
                u[i] = mnist_test[testNum][i] / 255.0;
                l[i] = mnist_test[testNum][i] / 255.0;
            }
            for (int i = 0; i < num_pixels; i++) {
                u[random_pixels[testNum][i]] = 1.0;
                l[random_pixels[testNum][i]] = 0.0;
            }
            verificationParams->target = correct_class[testNum];
            return;
        }
    }

    /* mnist but with uniform dist inputs */
    if (PROPERTY >= 500 && PROPERTY <= 599) {
        int testNum = PROPERTY - 500;
        for (int i = 0; i < 784; i++) {
            u[i] = (fmin((mnist_test[testNum][i] + perturb) / 255.0, 1.0) - 0.1307)/0.3081;
            l[i] = (fmax((mnist_test[testNum][i] - perturb) / 255.0, 0.0) - 0.1307)/0.3081;
        }
        verificationParams->target = correct_class[testNum];
        return;
    }

    // cifar 10
    if (PROPERTY >= 600 && PROPERTY <= 699) {
        int testNum = PROPERTY - 600;
        for (int i = 0; i < 3072; i++) {
            u[i] = fmin((cifar10_test[testNum][i] + perturb) / 255.0, 1.0);
            l[i] = fmax((cifar10_test[testNum][i] - perturb) / 255.0, 0.0);
        }
        verificationParams->target = cifar10_correct_class[testNum];
        return;
    }

    // cifar 10 with normalization
    if (PROPERTY >= 700 && PROPERTY <= 800) {
        int testNum = PROPERTY - 700;
        for (int i = 0; i < 3072; i++) {
            if (i % 3 == 0) {
                u[i] = (fmin((cifar10_test[testNum][i] + perturb) / 255.0, 1.0) - 0.4914)/0.2023;
                l[i] = (fmax((cifar10_test[testNum][i] - perturb) / 255.0, 0.0) - 0.4914)/0.2023;
            } else if (i % 3 == 1) {
                u[i] = (fmin((cifar10_test[testNum][i] + perturb) / 255.0, 1.0) - 0.4822)/0.1994;
                l[i] = (fmax((cifar10_test[testNum][i] - perturb) / 255.0, 0.0) - 0.4822)/0.1994;
            } else {
                u[i] = (fmin((cifar10_test[testNum][i] + perturb) / 255.0, 1.0) - 0.4465)/0.201;
                l[i] = (fmax((cifar10_test[testNum][i] - perturb) / 255.0, 0.0) - 0.4465)/0.201;
            }
        }
        verificationParams->target = cifar10_correct_class[testNum];
        return;
    }

    if (PROPERTY == 301) {
        float upper[] = {1.5, 0.5};
        float lower[] = {0.5, -0.5};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
        return;
    }

    if (PROPERTY == 300 || PROPERTY == 302) {
        float upper[] = {2.0, 2.0};
        float lower[] = {-2.0, -2.0};
        /*float upper[] = {6.0, 5.0};
        float lower[] = {4.0, 1.0};*/
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
        return;
    }

    if (PROPERTY == 303) {
        float upper[] = {1.0, 1.0};
        float lower[] = {-1.0, -1.0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
        return;
    }

    if (PROPERTY == 304) {
        float upper[] = {1.0, 2.0};
        float lower[] = {0.0, 1.0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
        return;
    }

    if (PROPERTY == 305) {
        float upper[] = {0.7, 1.0, 1.0};
        float lower[] = {0.0, 0.6, 0.0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
        return;
    }

    if (PROPERTY == 306) {
        /* float upper[] = {0.679857, -0.054688, 0.499999, 0.500000, -0.450000 };
        float lower[] = {0.639928, -0.062500, 0.468749, 0.449999, -0.475000 }; */
        float upper[] = {0.679857, -0.046875, 0.468749, 0.500000, -0.475000 };
        float lower[] = {0.600000, -0.062500, 0.437499, 0.449999, -0.500000 };
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
        return;
    }

    if (PROPERTY == 1) {
        float upper[] = {60760, 3.141592, 3.141592, 1200, 60};
        float lower[] = {55947.691, -3.141592, -3.141592, 1145, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 2) {
        float upper[] = {60760, 3.141592, 3.141592, 1200, 60};
        float lower[] = {55947.691, -3.141592, -3.141592, 1145, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 3) {
        float upper[] = {1800, 0.06, 3.141592, 1200, 1200};
        float lower[] = {1500, -0.06, 3.10, 980, 960};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 4) {
        float upper[] = {1800, 0.06, 0, 1200, 800};
        float lower[] = {1500, -0.06, 0, 1000, 700};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 5) {
        float upper[] = {400, 0.4, -3.1415926 + 0.005, 400, 400};
        float lower[] = {250, 0.2, -3.1415926, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 4;
    }

    if (PROPERTY == 16) {
        float upper[] = {62000, -0.7, -3.141592 + 0.005, 200, 1200};
        float lower[] = {12000, -3.141592, -3.141592, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;

    }

    if (PROPERTY == 26) {
        float upper[] = {62000, 3.141592, -3.141592 + 0.005, 200, 1200};
        float lower[] = {12000, 0.7, -3.141592, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 7) {
        float upper[] = {60760, 3.141592, 3.141592, 1200, 1200};
        float lower[] = {0, -3.141592, -3.141592, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 4;
    }

    if (PROPERTY == 8) {
        float upper[] = {60760, -3.141592 * 0.75, 0.1, 1200, 1200};
        float lower[] = {0, -3.141592, -0.1, 600, 600};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 1;
    }

    if (PROPERTY == 9) {
        float upper[] = {7000, -0.14, -3.141592 + 0.01, 150, 150};
        float lower[] = {2000, -0.4, -3.141592, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 3;
    }

    if (PROPERTY == 10) {
        float upper[] = {60760, 3.141592, -3.141592 + 0.01, 1200, 1200};
        float lower[] = {36000, 0.7, -3.141592, 900, 600};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 11) {
        float upper[] = {400, 0.4, -3.1415926 + 0.005, 400, 400};
        float lower[] = {250, 0.2, -3.1415926, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 4;
    }

    if (PROPERTY == 12) {
        float upper[] = {60760, 3.141592, 3.141592, 1200, 60};
        float lower[] = {55947.691, -3.141592, -3.141592, 1145, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 13) {
        float upper[] = {60760, 3.141592, 3.141592, 360, 360};
        float lower[] = {60000, -3.141592, -3.141592, 0, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 0;
    }

    if (PROPERTY == 14) {
        float upper[] = {400, 0.4, -3.1415926 + 0.005, 400, 400};
        float lower[] = {250, 0.2, -3.1415926, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 4;
    }

    if (PROPERTY == 15) {
        float upper[] = {400, -0.2, -3.1415926 + 0.005, 400, 400};
        float lower[] = {250, -0.4, -3.1415926, 100, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
        verificationParams->target = 3;
    }

    if (PROPERTY == 100) {
        float upper[] = {400, 0, -3.1415926 + 0.025, 250, 200};
        float lower[] = {250, 0, -3.1415926 + 0.025, 250, 200};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
    }

    if (PROPERTY == 101) {
        float upper[] = {400, 0.4, -3.1415926 + 0.025, 250, 200};
        float lower[] = {250, 0.2, -3.1415926 + 0.025, 250, 200};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
    }

    if (PROPERTY == 102) {
        float upper[] = {400, 0.2, -3.1415926 + 0.05, 0, 0};
        float lower[] = {250, 0.2, -3.1415926 + 0.05, 0, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
    }

    if (PROPERTY == 110) {
        float upper[] = {10000, 3.141592, -3.141592 + 0.01, 1200, 1200};
        float lower[] = {1000, 3.141592, -3.141592 + 0.01, 1200, 1200};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
    }

    if (PROPERTY == 111) {
        float upper[] = {1000, 3.141592, -3.141592 + 0.01, 1200, 1200};
        float lower[] = {1000, 3.141592, -3.141592 + 0.01, 0, 1200};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
    }

    if (PROPERTY == 112) {
        float upper[] = {1000, 3.141592, -3.141592 + 0.01, 1200, 1200};
        float lower[] = {1000, 3.141592, -3.141592 + 0.01, 1200, 0};
        memcpy(u, upper, sizeof(float) * inputSize);
        memcpy(l, lower, sizeof(float) * inputSize);
    }


}


/*
 * Following functions denomalize and normalize the concrete inputs
 * and input intervals.
 * They take in concrete inputs or input intervals.
 * Output normalized or denormalized concrete inputs or input intervals.
 */
void denormalize_input(struct NNet *nnet, struct Matrix *input) {

    for (int i = 0; i < nnet->inputSize; i++) {
        input->data[i] = input->data[i] * (nnet->ranges[i]) + nnet->means[i];
    }

}


void denormalize_input_interval(struct NNet *nnet, struct Interval *input) {
    denormalize_input(nnet, &input->upper_matrix);
    denormalize_input(nnet, &input->lower_matrix);
}


void normalize_input(struct NNet *nnet, struct Matrix *input) {

    for (int i = 0; i < nnet->inputSize; i++) {

        if (input->data[i] > nnet->maxes[i]) {
            input->data[i] = (nnet->maxes[i] - nnet->means[i]) / (nnet->ranges[i]);
        } else if (input->data[i] < nnet->mins[i]) {
            input->data[i] = (nnet->mins[i] - nnet->means[i]) / (nnet->ranges[i]);
        } else {
            input->data[i] = (input->data[i] - nnet->means[i]) / (nnet->ranges[i]);
        }

    }

}


void normalize_input_interval(struct NNet *nnet, struct Interval *input) {

    normalize_input(nnet, &input->upper_matrix);
    normalize_input(nnet, &input->lower_matrix);

}

void initRefinementParams(struct RefinementParams *refinementParams) {
    refinementParams->numFeatures = 0;
    refinementParams->mono = 0;
    refinementParams->testConcreteInput = 0;
    refinementParams->split_feature = -500;
    refinementParams->mid = -5000;
    refinementParams->refineDeltaInput = 0;
    refinementParams->numExtraVars = 0;
}


/*
 * Concrete forward propagation with openblas
 * It takes in network and concrete input matrix.
 * Outputs the concrete outputs.
 */
int forward_prop(struct NNet *network, struct Matrix *input, struct Matrix *output) {

    int layer;

    struct NNet *nnet = network;
    int numLayers = nnet->numLayers;
    int inputSize = nnet->inputSize;


    float z[nnet->maxLayerSize];
    float a[nnet->maxLayerSize];
    struct Matrix Z = {z, 1, inputSize};
    struct Matrix A = {a, 1, inputSize};

    memcpy(Z.data, input->data, nnet->inputSize * sizeof(float));

    for (layer = 0; layer < numLayers; layer++) {
        A.row = nnet->bias[layer].row;
        A.col = nnet->bias[layer].col;
        memcpy(A.data, nnet->bias[layer].data, A.row * A.col * sizeof(float));

        matmul_with_bias(&Z, &nnet->weights[layer], &A);
        if (layer < numLayers - 1) {
            relu(&A);
        }
        memcpy(Z.data, A.data, A.row * A.col * sizeof(float));
        Z.row = A.row;
        Z.col = A.col;

    }

    memcpy(output->data, A.data, A.row * A.col * sizeof(float));
    output->row = A.row;
    output->col = A.col;

    return 1;
}


int forward_prop_delta(struct NNet *nnet1, struct NNet *nnet2, struct Matrix *input, struct Matrix *output) {
    float o1[nnet1->outputSize];
    float o2[nnet1->outputSize];
    struct Matrix output1 = {o1, nnet1->outputSize, 1};
    struct Matrix output2 = {o2, nnet1->outputSize, 1};

    forward_prop(nnet1, input, &output1);
    forward_prop(nnet2, input, &output2);

    for (int i = 0; i < nnet1->outputSize; i++) {
        output->data[i] = output2.data[i] - output1.data[i];
    }
    return 1;
}


void backward_prop(struct NNet *nnet, int target, int maxLayerSize, struct Interval *grad,
                   int R[][maxLayerSize], int subtract) {
    int i, j, layer;
    int numLayers = nnet->numLayers;
    int inputSize = nnet->inputSize;

    float gradLower[maxLayerSize];
    float gradUpper[maxLayerSize];
    float newGradLower[maxLayerSize];
    float newGradUpper[maxLayerSize];

    /* Initialize gradient with weights of last (output) layer */
    memcpy(gradLower, nnet->matrix[numLayers - 1][0][target], \
             sizeof(float) * nnet->layerSizes[numLayers - 1]);
    memcpy(gradUpper, nnet->matrix[numLayers - 1][0][target], \
             sizeof(float) * nnet->layerSizes[numLayers - 1]);

    if (subtract) {
        for (i = 0; i < nnet->layerSizes[numLayers - 1]; i++) {
            gradLower[i] = -gradLower[i];
            gradUpper[i] = -gradUpper[i];
        }
    }

#ifdef BACKPROP
    fprintf(stderr, "# Gradient computation\n");
    fprintf(stderr, "## Layer %d\n", numLayers - 1);
    print_farray(gradLower, nnet->layerSizes[numLayers-1]);
    fprintf(stderr, "\n");
    print_farray(gradUpper, nnet->layerSizes[numLayers-1]);
    fprintf(stderr, "\n");
#endif

    /* Process second to last layer to first layer */
    for (layer = numLayers - 2; layer >= 0; layer--) {
        float **weights = nnet->matrix[layer][0];
        memset(newGradUpper, 0, sizeof(float) * maxLayerSize);
        memset(newGradLower, 0, sizeof(float) * maxLayerSize);

        if (layer != 0) { // general case
            /* For each neuron in the _current_ layer
             * (layerSizes includes the input layer, hence the layer + 1).
             * In the nnet->weights matrix, layer is the _current_ layer.
             * In the nnet->layerSizes, layer is the _previous_ layer. */
            for (j = 0; j < nnet->layerSizes[layer + 1]; j++) {
                /* In backward propagation, we first perform ReLU, then do matrix mult */
                /* Perform ReLU */
                if (R[layer][j] == 0) {
                    /* both slopes where 0 */
                    gradUpper[j] = gradLower[j] = 0;
                } else if (R[layer][j] == 1) {
                    /* This neuron could be either linear (the original equations) or 0.
                     * We have to ensure 0 is in the gradient. */
                    /* min(grad_lower, 0) */
                    gradLower[j] = (gradLower[j] < 0) ? gradLower[j] : 0;
                    /* max(grad_upper, 0) */
                    gradUpper[j] = (gradUpper[j] > 0) ? gradUpper[j] : 0;
                } else {
                    /* Both upper and lower slopes are linear, no need to do anything. */
                }

                /* Perform matrix multiplication */
                /* For each neuron in the _previous_ layer,
                 * add the _current_ neuron's gradient to it.
                 * layer is the _previous_ layer in layerSizes */
                for (i = 0; i < nnet->layerSizes[layer]; i++) {
                    if (weights[j][i] >= 0) {
                        /* Weight is positive
                         * Lower to lower, upper to upper */
                        newGradLower[i] += weights[j][i] * gradLower[j];
                        newGradUpper[i] += weights[j][i] * gradUpper[j];
                    } else {
                        /* Else flip */
                        newGradLower[i] += weights[j][i] * gradUpper[j];
                        newGradUpper[i] += weights[j][i] * gradLower[j];
                    }
                }
            }

        } else { /* Input layer */
            /* For each neuron in second layer */
            for (j = 0; j < nnet->layerSizes[layer + 1]; j++) {
                /* Perform ReLU */
                if (R[layer][j] == 0) {
                    gradUpper[j] = gradLower[j] = 0;
                } else if (R[layer][j] == 1) {
                    /* min(gradLower[j], 0) */
                    gradLower[j] = (gradLower[j] < 0) ? gradLower[j] : 0;
                    /* max(gradUpper[j], 0) */
                    gradUpper[j] = (gradUpper[j] > 0) ? gradUpper[j] : 0;
                } else {
                    /* Neuron was linear, no need to do anything */
                }

                /* For each input neuron's gradient, add the current neuron's
                 * gradient to it. */
                for (i = 0; i < inputSize; i++) {
                    if (weights[j][i] >= 0) { /* Weight is positive */
                        newGradLower[i] += weights[j][i] * gradLower[j];
                        newGradUpper[i] += weights[j][i] * gradUpper[j];
                    } else {
                        newGradLower[i] += weights[j][i] * gradUpper[j];
                        newGradUpper[i] += weights[j][i] * gradLower[j];
                    }
                }
            }
        }

#ifdef BACKPROP
        fprintf(stderr, "## Layer %d\n", layer);
        print_farray(newGradLower, nnet->layerSizes[layer]);
        fprintf(stderr, "\n");
        print_farray(newGradUpper, nnet->layerSizes[layer]);
        fprintf(stderr, "\n");
#endif


        if (layer != 0) {
            /* layer is the _previous_ layer in the layerSizes array */
            memcpy(gradLower, newGradLower, sizeof(float) * nnet->layerSizes[layer]);
            memcpy(gradUpper, newGradUpper, sizeof(float) * nnet->layerSizes[layer]);
        } else {
            memcpy(grad->lower_matrix.data, newGradLower, sizeof(float) * inputSize);
            memcpy(grad->upper_matrix.data, newGradUpper, sizeof(float) * inputSize);
        }
    }
}


void computeBounds(float *new_equation_lower, float *new_equation_upper,
                   struct Interval *input, int inputSize, int i, float *bounds) {
    float tempVal_lower = 0.0, tempVal_upper = 0.0;
    for (int k = 0; k < inputSize; k++) {
        if (new_equation_lower[k + i * (inputSize + 1)] >= 0) {
            tempVal_lower += new_equation_lower[k + i * (inputSize + 1)] * input->lower_matrix.data[k] - OUTWARD_ROUND;
        } else {
            tempVal_lower += new_equation_lower[k + i * (inputSize + 1)] * input->upper_matrix.data[k] - OUTWARD_ROUND;
        }
        if (new_equation_upper[k + i * (inputSize + 1)] >= 0) {
            tempVal_upper += new_equation_upper[k + i * (inputSize + 1)] * input->upper_matrix.data[k] + OUTWARD_ROUND;
        } else {
            tempVal_upper += new_equation_upper[k + i * (inputSize + 1)] * input->lower_matrix.data[k] + OUTWARD_ROUND;
        }
    }
    tempVal_lower += new_equation_lower[inputSize + i * (inputSize + 1)];
    tempVal_upper += new_equation_upper[inputSize + i * (inputSize + 1)];
    bounds[0] = tempVal_lower;
    bounds[1] = tempVal_upper;
}

/* Computes the bounds for an equation with terms of input variables */
void computeAllBounds(float *eqLow, float *eqUp,
                      struct Interval *input, int equationSize,
                      float *low, float *lowsUp, float *upsLow, float *up) {
    float tempVal_lower = 0, tempVal_upper = 0, lower_s_upper = 0, upper_s_lower = 0;

    fesetround(FE_UPWARD); // Compute upper bounds
    for (int k = 0; k < equationSize - 1; k++) {
        /* upper bound */
        if (eqUp[k] > 0) {
            tempVal_upper += eqUp[k] * input->upper_matrix.data[k];
            if (input->upper_matrix.data[k] < 0) tempVal_upper += OUTWARD_ROUND;
            upper_s_lower += eqUp[k] * input->lower_matrix.data[k];
            if (input->lower_matrix.data[k] < 0) upper_s_lower += OUTWARD_ROUND;
        } else if (eqUp[k] < 0) {
            tempVal_upper += eqUp[k] * input->lower_matrix.data[k];
            if (input->lower_matrix.data[k] < 0) tempVal_upper += OUTWARD_ROUND;
            upper_s_lower += eqUp[k] * input->upper_matrix.data[k];
            if (input->upper_matrix.data[k] < 0) upper_s_lower += OUTWARD_ROUND;
        }
    }
    lower_s_upper += eqLow[equationSize - 1];
    tempVal_upper += eqUp[equationSize - 1];

    fesetround(FE_DOWNWARD); // Compute lower bounds
    for (int k = 0; k < equationSize - 1; k++) {
        /* lower bound */
        if (eqLow[k] > 0) {
            /* If coefficient is positive, multiply by lower bound
             * of the input. */

            tempVal_lower += eqLow[k] * input->lower_matrix.data[k];
            if (input->lower_matrix.data[k] < 0) tempVal_lower -= OUTWARD_ROUND;
            lower_s_upper += eqLow[k] * input->upper_matrix.data[k];
            if (input->upper_matrix.data[k] < 0) lower_s_upper -= OUTWARD_ROUND;
        } else if (eqLow[k] < 0) {
            /* Otherwise, multiply by upper bound. */
            tempVal_lower += eqLow[k] * input->upper_matrix.data[k];
            if (input->upper_matrix.data[k] < 0) tempVal_lower -= OUTWARD_ROUND;
            lower_s_upper += eqLow[k] * input->lower_matrix.data[k];
            if (input->lower_matrix.data[k] < 0) lower_s_upper -= OUTWARD_ROUND;
        }

    }
    tempVal_lower += eqLow[equationSize - 1];
    upper_s_lower += eqUp[equationSize - 1];

    *low = tempVal_lower;
    *lowsUp = lower_s_upper;
    *upsLow = upper_s_lower;
    *up = tempVal_upper;
}

void computeSingleEqBounds(float *eq, struct Interval *input, int equationSize,
                      float *low, float *up) {
    float tempVal_lower = 0, tempVal_upper = 0;

    fesetround(FE_UPWARD); // Compute upper bounds
    for (int k = 0; k < equationSize - 1; k++) {
        if (eq[k] >= 0) {
            // positive coeff, take upper bound to maximize
            tempVal_upper += eq[k] * input->upper_matrix.data[k];
        } else {
            // negative coeff, take lower bound to maximize
            tempVal_upper += eq[k] * input->lower_matrix.data[k];
        }
    }
    tempVal_upper += eq[equationSize - 1];

    fesetround(FE_DOWNWARD); // Compute lower bounds
    for (int k = 0; k < equationSize - 1; k++) {
        /* lower bound */
        if (eq[k] >= 0) {
            /* If coefficient is positive, multiply by lower bound to minimize. */
            tempVal_lower += eq[k] * input->lower_matrix.data[k];
        } else {
            /* If negative, multiply by upper bound to minimize. */
            tempVal_lower += eq[k] * input->upper_matrix.data[k];
        }
    }
    tempVal_lower += eq[equationSize - 1];

    *low = tempVal_lower;
    *up = tempVal_upper;
}

void zero_interval(struct Interval *interval, int eqSize, int neuron) {
    for (int k = 0; k < eqSize; k++) {
        interval->lower_matrix.data[k + neuron * (eqSize)] = 0;
        interval->upper_matrix.data[k + neuron * (eqSize)] = 0;
    }
}

/*
 * Implements the ReLUVal style concretization of a neuron's interval equation.
 * In particular, the lower bound is always concretized to zero, but the upper
 * bound can be left alone if the lower bound of the upper bound's eqaution is
 * greater than 0.
 */
void concretizeNeuronEq(struct Interval *interval, int eqSize, int neuron, float upsLower, float up) {
    int eqOffset = neuron * eqSize;
    if (upsLower < 0.0) { // concretize
        zero_interval(interval, eqSize, neuron);
        interval->upper_matrix.data[eqSize - 1 + eqOffset] = up;
    } else { // upper bound is linear, so leave it alone
        for (int k = 0; k < eqSize; k++) {
            interval->lower_matrix.data[k + eqOffset] = 0.0;
        }
    }
}

/*
 * Transforms the LB equation so that it's upper bound is 0.
 */
void linearizeDeltaLB(struct Interval *interval, int eqSize, int neuron,
                       float lower, float lowersUpper) {
    float *eqLower = &interval->lower_matrix.data[eqSize*neuron];
    for (int k = 0; k < eqSize; k++) {
        eqLower[k] = eqLower[k]*(-lower)/(lowersUpper - lower);
    }
    eqLower[eqSize - 1] -= lowersUpper*(-lower)/(lowersUpper - lower);
}

/*
 * Transforms an equation so that it's lower bound is at newLB */
void linearizeAudjustLB(float *eq, int eqSize,
                      float origLow, float origUp, float newLow) {
    float mu = (newLow - origUp)/(origLow - origUp);
    for (int k = 0; k < eqSize; k++) {
        eq[k] = eq[k]*mu;
    }    
    eq[eqSize - 1] -= mu*origUp;
    eq[eqSize - 1] += origUp;
}

/*
 * Transform the UB equation so that it's lower bound is 0.
 */
void linearizeDeltaUB(struct Interval *interval, int eqSize, int neuron,
                      float uppersLower, float upper) {
    float *eqUpper = &interval->upper_matrix.data[eqSize*neuron];
    for (int k = 0; k < eqSize; k++) {
        eqUpper[k] = eqUpper[k]*upper/(upper - uppersLower);
    }
    eqUpper[eqSize - 1] -= uppersLower*upper/(upper - uppersLower);
}

void linearizeNeuronEq(struct Interval *interval, int eqSize, int neuron,
                  float lower, float lowersUpper, float uppersLower, float upper) {
    if (uppersLower < 0.0) {
        for (int i = 0; i < eqSize; ++i) {
            interval->upper_matrix.data[i+neuron*eqSize] =
                    interval->upper_matrix.data[i+neuron*eqSize]*
                    upper/(upper-uppersLower);
        }
        interval->upper_matrix.data[eqSize-1 + neuron*eqSize] -= upper*uppersLower/
                                                                 (upper - uppersLower);
    }

    if (lowersUpper <= 0.0) {
        for (int i = 0; i < eqSize; ++i) {
            interval->lower_matrix.data[i+neuron*eqSize] = 0.0;
        }
    } else {
        for (int i = 0; i < eqSize; ++i) {
            interval->lower_matrix.data[i+neuron*eqSize] =
                    interval->lower_matrix.data[i+neuron*eqSize]*
                    lowersUpper / (lowersUpper - lower);
        }
    }
}

/*
 * Performs an affine multiplication transform to the array of input intervals.
 */
void affineTransform(struct Interval *interval, struct Matrix *posMatrix, struct Matrix *negMatrix,
                     struct Interval *outInterval, int overWrite) {
    if (overWrite) {
        /* source neuron --weight--> dest neuron */
        fesetround(FE_UPWARD); // compute upper bound
        /* For source neurons with a positive incoming weight, we multiply
         * by the source neuron's upper bound to maximize it. */
        matmul(&interval->upper_matrix, posMatrix, &outInterval->upper_matrix);
        /* For source neurons with a negative incoming weight, we multiply
         * by the source neuron's lower bound to minimize it. */
        matmul_with_bias(&interval->lower_matrix, negMatrix, &outInterval->upper_matrix);

        fesetround(FE_DOWNWARD); // compute lower bound
        /* Use the lower bound when multiplying by a positive weight to minimize */
        matmul(&interval->lower_matrix, posMatrix, &outInterval->lower_matrix);
        /* Use the upper bound when multiplying by negative weight to minimize */
        matmul_with_bias(&interval->upper_matrix, negMatrix, &outInterval->lower_matrix);
    } else {
        fesetround(FE_UPWARD);
        matmul_with_bias(&interval->upper_matrix, posMatrix, &outInterval->upper_matrix);
        matmul_with_bias(&interval->lower_matrix, negMatrix, &outInterval->upper_matrix);
        fesetround(FE_DOWNWARD);
        matmul_with_bias(&interval->lower_matrix, posMatrix, &outInterval->lower_matrix);
        matmul_with_bias(&interval->upper_matrix, negMatrix, &outInterval->lower_matrix);
    }
}

void computeGradient(struct VerificationParams *verificationParams,
        int maxLayerSize, int R[][maxLayerSize], int RPrime[][maxLayerSize],
        struct Interval *gradientIntRet) {
    int inputSize = verificationParams->nnet->inputSize;

    float glower[inputSize], gupper[inputSize];
    struct Interval gradInt = {
            (struct Matrix) {glower, 1, inputSize},
            (struct Matrix) {gupper, 1, inputSize}
    };
    float gplower[inputSize], gpupper[inputSize];
    struct Interval gradPrimeInt = {
            (struct Matrix) {gplower, 1, inputSize},
            (struct Matrix) {gpupper, 1, inputSize}
    };

    backward_prop(verificationParams->nnet, verificationParams->target, maxLayerSize, &gradInt, R, 0);
    backward_prop(verificationParams->nnetPrime, verificationParams->target, maxLayerSize, &gradPrimeInt, RPrime, 1);
    for (int k = 0; k < inputSize; k++) {
        gradInt.lower_matrix.data[k] += gradPrimeInt.lower_matrix.data[k];
        gradInt.upper_matrix.data[k] += gradPrimeInt.upper_matrix.data[k];
    }

    memcpy(gradientIntRet->lower_matrix.data, gradInt.lower_matrix.data, inputSize* sizeof(float));
    memcpy(gradientIntRet->upper_matrix.data, gradInt.upper_matrix.data, inputSize* sizeof(float));
}

void smearBasedRefinement(struct VerificationParams *verificationParams, struct Interval *input, struct Interval *grad,
                          struct RefinementParams *refinementParams) {
    int inputSize = verificationParams->nnet->inputSize;
    float smear;
    float largest_smear;
    int *inputTypes = verificationParams->nnet->inputTypes;

    /*
     * Check if any of the inputs are monotonic.
     */

    largest_smear = -FLT_MAX;
    for (int i = 0; i < inputSize; i++) {
        if (input->upper_matrix.data[i] == input->lower_matrix.data[i]) {
			// don't split a feature with no width
            continue;
        } else if (grad->upper_matrix.data[i]<=0 ||
            grad->lower_matrix.data[i]>=0) { // The input is monotonic

            refinementParams->mono = 1;

            smear = ((grad->upper_matrix.data[i] > - grad->lower_matrix.data[i]) ?
                     grad->upper_matrix.data[i] :
                     -grad->lower_matrix.data[i]) *
                    (input->upper_matrix.data[i] - input->lower_matrix.data[i]);

            if (smear >= largest_smear) {
                largest_smear = smear;
                refinementParams->split_feature = i;
            }

        }
        refinementParams->numFeatures++;
    }

    /*
     * If none are monotonic, find the input with the highest smear value.
     * For now, we don't apply monotonic when there are categorical features.
     */
    if (refinementParams->mono == 0 || inputTypes != NULL) {
        refinementParams->mono = 0;

        float e, width;
        largest_smear = -FLT_MAX;

        for (int i = 0; i < inputSize; i++) {
        	if (input->upper_matrix.data[i] == input->lower_matrix.data[i]) {
				// don't split a feature with no width
				continue;
			}

            width = input->upper_matrix.data[i] - input->lower_matrix.data[i];

            e = (fabs(grad->upper_matrix.data[i]) > fabs(grad->lower_matrix.data[i])) ?
                fabs(grad->upper_matrix.data[i]) :
                fabs(grad->lower_matrix.data[i]);

            smear = e*width;

            if (inputTypes != NULL && inputTypes[i] != 0) {
                int curType = inputTypes[i];
                int j;
                for (j = i + 1; j < inputSize && inputTypes[j] == curType; j++) {
                    width = input->upper_matrix.data[j] - input->lower_matrix.data[j];

                    e = (fabs(grad->upper_matrix.data[j]) > fabs(grad->lower_matrix.data[j])) ?
                        fabs(grad->upper_matrix.data[j]) :
                        fabs(grad->lower_matrix.data[j]);

                    smear += e*width;
                }
                i = j - 1;
            }

            if (largest_smear < smear) {
                largest_smear = smear;
                refinementParams->split_feature = i;
            }

        }

        float upper = input->upper_matrix.data[refinementParams->split_feature];
        float lower = input->lower_matrix.data[refinementParams->split_feature];

        float middle;

        if (upper != lower) {
            middle = (upper + lower) / 2;
        }
        else {
            middle = upper;
        }
        refinementParams->mid = middle;

        if (upper - middle <= ADV_THRESHOLD) {
            refinementParams->testConcreteInput = 1;
        }
    }
}

/*
 * Removes the intermediate variables from an interval's equation by replacing them
 * with their original stored equation. Copies the result into dstLower and dstUpper.
 * @param interval          The interval equations of all neurons in the current layer
 * @param extraVarInterval  The symbolic equations of the extra vars
 * @param eqSize            The number of variables (including the constant) in the equations
 *                          stored in interval
 * @param inputSize         The number of inputs to the neural network. This should also be the
 *                          number of variables in each equation in extraVarInterval (w/o constant).
 * @param neuron            The neuron in interval that we want to canonicalize.
 * @param dstLower,dstUpper The return arrays
 */
void canonicalizeInterval(struct Interval *interval, struct Interval *extraVarInterval,
                          int eqSize, int inputSize, int neuron, int flip, float *dstLower, float *dstUpper) {
    // Store the neuron's original equation
    // First do the input variables
    int offsetEq = eqSize * neuron;
    memcpy(dstLower, interval->lower_matrix.data + offsetEq, inputSize * sizeof(float));
    memcpy(dstUpper, interval->upper_matrix.data + offsetEq, inputSize * sizeof(float));
    // the constant is always the last element
    dstLower[inputSize] = interval->lower_matrix.data[offsetEq + eqSize - 1];
    dstUpper[inputSize] = interval->upper_matrix.data[offsetEq + eqSize - 1];
    float flipSign = flip == 1 ? -1.0 : 1.0;
    // Now do the other extra symbolic vars
    // We already handled i == eqSize - 1 above
    for (int i = inputSize; i < eqSize - 1; i++) {
        int offsetExtraVarEq = (inputSize + 1) * (i - inputSize);
        /* First do lower bound */
        float scalar = interval->lower_matrix.data[offsetEq + i];
        fesetround(FE_DOWNWARD);
        if (scalar != 0.0) {
            if (scalar*flipSign > 0) {
                /* Scalar is positive (unless flipped),
                 * lower goes to lower, upper goes to upper */
                for (int j = 0; j < inputSize; j++) {
                    dstLower[j] +=
                            scalar * extraVarInterval->lower_matrix.data[offsetExtraVarEq + j];
                }
                dstLower[inputSize] +=
                        scalar * extraVarInterval->lower_matrix.data[offsetExtraVarEq + inputSize];

            } else {
                for (int j = 0; j < inputSize; j++) {
                    dstLower[j] +=
                            scalar * extraVarInterval->upper_matrix.data[offsetExtraVarEq + j];
                }
                dstLower[inputSize] +=
                        scalar * extraVarInterval->upper_matrix.data[offsetExtraVarEq + inputSize];
            }
        }

        /* Now do upper */
        scalar = interval->upper_matrix.data[offsetEq + i];
        fesetround(FE_UPWARD);
        if (scalar != 0.0) {
            if (scalar*flipSign > 0) {
                /* Scalar is positive (unless flipped),
                 * lower goes to lower, upper goes to upper */
                for (int j = 0; j < inputSize; j++) {
                    dstUpper[j] +=
                            scalar * extraVarInterval->upper_matrix.data[offsetExtraVarEq + j];
                }
                dstUpper[inputSize] +=
                        scalar * extraVarInterval->upper_matrix.data[offsetExtraVarEq + inputSize];
            } else {
                for (int j = 0; j < inputSize; j++) {
                    dstUpper[j] +=
                            scalar * extraVarInterval->lower_matrix.data[offsetExtraVarEq + j];
                }
                dstUpper[inputSize] +=
                        scalar * extraVarInterval->lower_matrix.data[offsetExtraVarEq + inputSize];
            }
        }
    }
}


void makeIntermediateVar(struct Interval *interval, struct Interval *extraVarInterval, struct Interval *input,
                          int extraVarIndex, int inputSize, int equationSize, int neuron, float lb, float ub) {
    int eqOffset = equationSize*neuron;
#ifdef EXTRAVARSSYM
    /* Canonicalize the output interval of this neuron */
    float canonEqLow[inputSize + 1];
    float canonEqUp[inputSize + 1];
#ifdef EXTRAVARSSYMCONC
    /* this is for testing the symbolic var implementation, it should not
     * be used for verification */
    memset(canonEqLow, 0, sizeof(float)*(inputSize+1));
    memset(canonEqUp, 0, sizeof(float)*(inputSize+1));
    canonEqLow[inputSize] = lb;
    canonEqUp[inputSize] = ub;
#else
    canonicalizeInterval(interval, extraVarInterval, equationSize, inputSize, neuron, 0,
                         canonEqLow, canonEqUp);
#endif
#endif
    /* Set the output equal to the new variable */
    memset(interval->lower_matrix.data + eqOffset, 0, equationSize * sizeof(float));
    memset(interval->upper_matrix.data + eqOffset, 0, equationSize * sizeof(float));
    interval->lower_matrix.data[eqOffset + extraVarIndex] = 1.0;
    interval->upper_matrix.data[eqOffset + extraVarIndex] = 1.0;
#ifdef EXTRAVARSSYM
    int extraVarEqOffset = (extraVarIndex - inputSize)*(inputSize+1);
    memcpy(extraVarInterval->lower_matrix.data + extraVarEqOffset, canonEqLow, (inputSize+1)*sizeof(float));
    memcpy(extraVarInterval->upper_matrix.data + extraVarEqOffset, canonEqUp, (inputSize+1)*sizeof(float));
#else
    input->lower_matrix.data[extraVarIndex] = lb;
    input->upper_matrix.data[extraVarIndex] = ub;
#endif
}


/*
 * Computes the lower and upper concrete bounds for both the lower and upper interval equations.
 * @param interval          The interval equations of all neurons in the current layer
 * @param extraVarInterval  The symbolic equations of the extra vars
 * @param input             The input intervals of the network (this may include intervals for extraVars)
 * @param inputSize         The number of inputs to the neural network (SHOULD NOT include extraVars)
 * @param eqSize            The number of variables (including the constant) in the equations
 *                          stored in interval
 * @param neuron            The neuron in the layer to compute the concrete bounds for
 * @param low, lowsUp,      The return variables
 *        upsLow, up
 */
void computeIntervalBoundsExtraSymbolic(struct Interval *interval, struct Interval *extraVarInterval, struct Interval *input,
                                        int inputSize, int eqSize, int neuron,
                                        float *low, float *lowsUp, float *upsLow, float *up) {
    float tmpEqLow[inputSize + 1];
    float tmpEqUp[inputSize + 1];

    float tmp;
    canonicalizeInterval(interval, extraVarInterval, eqSize, inputSize, neuron, 0,
                         tmpEqLow, tmpEqUp);
    computeAllBounds(tmpEqLow, tmpEqUp, input, inputSize + 1, low, &tmp, &tmp, up);
    canonicalizeInterval(interval, extraVarInterval, eqSize, inputSize, neuron, 1,
                         tmpEqLow, tmpEqUp);
    computeAllBounds(tmpEqLow, tmpEqUp, input, inputSize + 1, &tmp, lowsUp, upsLow, &tmp);
}

float computeSymbolicInfo(struct Interval *interval, struct Interval *extraVarInterval, struct Interval *input,
                                        int inputSize, int eqSize, int neuron) {
    /* float tmpEqLow[inputSize + 1];
    float tmpEqUp[inputSize + 1];
    canonicalizeInterval(interval, extraVarInterval, eqSize, inputSize, neuron, 0,
                         tmpEqLow, tmpEqUp); */
    float symInfo = 0;
    int eqOffset = eqSize*neuron;
    float *eqUp = (interval->upper_matrix.data + eqOffset);
    float *eqLow = (interval->lower_matrix.data + eqOffset);
    for (int i = 0; i < eqSize - 1; i++) {
        symInfo += fabs(eqUp[i]);
        symInfo += fabs(eqLow[i]);
    }
    return (symInfo/(symInfo + fabs(eqUp[eqSize - 1]) + fabs(eqLow[eqSize - 1])));
}

int is_between(float x, float lower, float upper) {
    if (x > lower && x < upper) {
        return 1;
    }
    return 0;
}

/*
 * Implements the ReLUDiff algorithm using ReLUVal to compute the neuron's absolute bounds.
 * All input data structures should be properly initialized!
 */
int forward_prop_delta_neurodiff(struct VerificationParams *verificationParams, struct InputSpace *inputSpace,
        struct Interval *output, struct Interval *outputDelta, struct RefinementParams *refinementParams) {

    int i, k, layer, eqOffset, constantIndex;
    int numUnstable = 0;
    int nextEVS = 0;
    struct Interval *input = &inputSpace->inputs;
    struct Interval *inputDeltas = &inputSpace->deltas;
    if (inputDeltas->lower_matrix.data == NULL)
        inputDeltas = NULL;

    struct NNet *nnet = verificationParams->nnet;
    struct NNet *nnetPrime = verificationParams->nnetPrime;
    struct NNet *deltas = verificationParams->nnetDelta;
    int numLayers = nnet->numLayers;
    int inputSize = nnet->inputSize;

    log_forward_pass_enter(nnet, input);

    int maxLayerSize = nnet->maxLayerSize;

    int R[numLayers][maxLayerSize];
    int RPrime[numLayers][maxLayerSize];
    memset(R, 0, sizeof(float) * numLayers * maxLayerSize);
    memset(RPrime, 0, sizeof(float) * numLayers * maxLayerSize);


    // Initialize equation matrices
    int equationSize = inputSize + 1; // number of variables in equation

    int curLayerNumExtraVars = 0, prevLayerNumExtraVars = 0;
#ifdef EXTRAVARS
    /* Add in a new variable for the delta inputs only (for right now)*/
    int remainingExtraVars = 0;
    if (inputDeltas != NULL) {
        for (k = 0; k < inputSize; k++) {
            if (inputDeltas->upper_matrix.data[k] != inputDeltas->lower_matrix.data[k]) {
                remainingExtraVars++;
                /* For now we don't care about the split order when refinement the
                 * delta input intervals. */
                if (refinementParams->refineDeltaInput != 1) {
                    refinementParams->refineDeltaInput = 1;
                    refinementParams->split_feature = k;
                }
            }
        }
    }

    remainingExtraVars += inputSpace->numExtraVars;
    equationSize += remainingExtraVars;

    float newUp[equationSize], newLow[equationSize];
    struct Interval newInput = {
            (struct Matrix) {(float *) newLow, 1, equationSize},
            (struct Matrix) {(float *) newUp, 1, equationSize}
    };
    // Init all variable intervals to 0
    memset(newLow, 0, sizeof(float)*equationSize);
    memset(newUp, 0, sizeof(float)*equationSize);
    // Copy over original inputs to new
    memcpy(newLow, input->lower_matrix.data, inputSize* sizeof(float));
    memcpy(newUp, input->upper_matrix.data, inputSize* sizeof(float));
    input = &newInput;

    /* Why is this outside the ifdef? */
    struct Interval *extraVarInterval = NULL;
#ifdef EXTRAVARSSYM
    int totalExtraVarEqSize = (inputSize + 1)*remainingExtraVars*sizeof(float);
    int totalExtraVars = remainingExtraVars;
    float *extraVarEqLow = malloc(totalExtraVarEqSize);
    float *extraVarEqUp = malloc(totalExtraVarEqSize);
    memset(extraVarEqLow, 0, totalExtraVarEqSize);
    memset(extraVarEqUp, 0, totalExtraVarEqSize);
    struct Interval eInt = {
            (struct Matrix) {(float *) extraVarEqLow, inputSize +1, totalExtraVars},
            (struct Matrix) {(float *) extraVarEqUp, inputSize +1, totalExtraVars}
    };
    extraVarInterval = &eInt;
#endif

#endif

    // equation is the temp equation for each layer
    // Each arrays is divided into sections of size equationSize.
    // The coefficient of variable k for neuron i is stored at
    // index k + i*equationSize, and the constant value is stored
    // at k = equationSize - 1.
    // number of bytes needed to store matrix of equations
    int maxEquationMatrixSize = equationSize * nnet->maxLayerSize * sizeof(float);
    float *eqLow = malloc(maxEquationMatrixSize);
    float *eqUp = malloc(maxEquationMatrixSize);
    float *newEqLow = malloc(maxEquationMatrixSize);
    float *newEqUp = malloc(maxEquationMatrixSize);

    float *eqPrimeLow = malloc(maxEquationMatrixSize);
    float *eqPrimeUp = malloc(maxEquationMatrixSize);
    float *newEqPrimeLow = malloc(maxEquationMatrixSize);
    float *newEqPrimeUp = malloc(maxEquationMatrixSize);

    float *eqDeltaLow = malloc(maxEquationMatrixSize);
    float *eqDeltaUp = malloc(maxEquationMatrixSize);
    float *newEqDeltaLow = malloc(maxEquationMatrixSize);
    float *newEqDeltaUp = malloc(maxEquationMatrixSize);

    memset(eqUp, 0, maxEquationMatrixSize);
    memset(eqLow, 0, maxEquationMatrixSize);
    memset(newEqUp, 0, maxEquationMatrixSize);
    memset(newEqLow, 0, maxEquationMatrixSize);

    memset(eqPrimeUp, 0, maxEquationMatrixSize);
    memset(eqPrimeLow, 0, maxEquationMatrixSize);
    memset(newEqPrimeUp, 0, maxEquationMatrixSize);
    memset(newEqPrimeLow, 0, maxEquationMatrixSize);

    memset(eqDeltaUp, 0, maxEquationMatrixSize);
    memset(eqDeltaLow, 0, maxEquationMatrixSize);
    memset(newEqDeltaUp, 0, maxEquationMatrixSize);
    memset(newEqDeltaLow, 0, maxEquationMatrixSize);


    struct Interval eqInterval = {
            (struct Matrix) {(float *) eqLow, equationSize, inputSize},
            (struct Matrix) {(float *) eqUp, equationSize, inputSize}
    };
    // the row/col initialization doesn't matter for the new equations
    // because it gets overwritten in the matmul functions
    struct Interval newEqInterval = {
            (struct Matrix) {(float *) newEqLow, equationSize, maxLayerSize},
            (struct Matrix) {(float *) newEqUp, equationSize, maxLayerSize}
    };
    struct Interval eqPrimeInterval = {
            (struct Matrix) {(float *) eqPrimeLow, equationSize, inputSize},
            (struct Matrix) {(float *) eqPrimeUp, equationSize, inputSize}
    };
    struct Interval newEqPrimeInterval = {
            (struct Matrix) {(float *) newEqPrimeLow, equationSize, maxLayerSize},
            (struct Matrix) {(float *) newEqPrimeUp, equationSize, maxLayerSize}
    };
    struct Interval eqDeltaInterval = {
            (struct Matrix) {(float *) eqDeltaLow, equationSize, inputSize},
            (struct Matrix) {(float *) eqDeltaUp, equationSize, inputSize}
    };
    struct Interval newEqDeltaInterval = {
            (struct Matrix) {(float *) newEqDeltaLow, equationSize, maxLayerSize},
            (struct Matrix) {(float *) newEqDeltaUp, equationSize, maxLayerSize}
    };

    float concUp = 0.0, concLow = 0.0;
    float concUpsLow = 0.0, concLowsUp = 0.0;

    float concPrimeUp = 0.0, concPrimeLow = 0.0;
    float concPrimeUpsLow = 0.0, concPrimeLowsUp = 0.0;

    float concDeltaLow = 0.0, concDeltaUp = 0.0;
    float concDeltaLowsUp = 0.0, concDeltaUpsLow = 0.0; // for unused parameters

    /* Initialize the equations for each input neuron.
     * Note that the delta equation starts as all 0's. */
    for (i = 0; i < inputSize; i++) {
        eqLow[i * equationSize + i] = 1;
        eqUp[i * equationSize + i] = 1;
        eqPrimeLow[i * equationSize + i] = 1;
        eqPrimeUp[i * equationSize + i] = 1;
    }

    if (inputDeltas != NULL) {
        /* Initialize the delta intervals
         * TODO: Initialize the extraVar eq intervals when EXTRAVARSSYM is defined. */
#ifdef EXTRAVARS
        /* Index of next available input var */
        int varOffset = (equationSize - 1) - remainingExtraVars;
        for (i = 0; i < inputSize; i++) {
            if (inputDeltas->upper_matrix.data[i] != inputDeltas->lower_matrix.data[i]) {
                /* Initialize bounds of input variables. */
                input->lower_matrix.data[varOffset] = inputDeltas->lower_matrix.data[i];
                input->upper_matrix.data[varOffset] = inputDeltas->upper_matrix.data[i];
                /* Set delta equation to be equal to the input var. */
                eqDeltaLow[i * equationSize + varOffset] = 1;
                eqDeltaUp[i * equationSize + varOffset] = 1;
                varOffset++;
                remainingExtraVars--;
            }
        }
#else
        for (i = 0; i < inputSize; i++) {
            if (inputDeltas->upper_matrix.data[i] != inputDeltas->lower_matrix.data[i]) {
                eqDeltaLow[i * equationSize + equationSize - 1] = inputDeltas->lower_matrix.data[i];
                eqDeltaUp[i * equationSize + equationSize - 1] = inputDeltas->upper_matrix.data[i];
            }

        }
#endif
    }


    for (layer = 0; layer < (numLayers); layer++) {
        numUnstable = 0;
        memset(newEqUp, 0, maxEquationMatrixSize);
        memset(newEqLow, 0, maxEquationMatrixSize);
        memset(newEqPrimeUp, 0, maxEquationMatrixSize);
        memset(newEqPrimeLow, 0, maxEquationMatrixSize);
        memset(newEqDeltaLow, 0, maxEquationMatrixSize);
        memset(newEqDeltaUp, 0, maxEquationMatrixSize);

        struct Matrix bias = nnet->bias[layer];
        struct Matrix biasPrime = nnetPrime->bias[layer];
        struct Matrix biasDelta = deltas->bias[layer];

        struct Matrix pWeights = nnet->posWeights[layer];
        struct Matrix nWeights = nnet->negWeights[layer];
        struct Matrix pWeightsPrime = nnetPrime->posWeights[layer];
        struct Matrix nWeightsPrime = nnetPrime->negWeights[layer];
        struct Matrix pWeightsDelta = deltas->posWeights[layer];
        struct Matrix nWeightsDelta = deltas->negWeights[layer];

        // Apply weights to symbolic intervals

        /* nnet */
        affineTransform(&eqInterval, &pWeights, &nWeights, &newEqInterval, 1);

        /* nnetPrime */
        affineTransform(&eqPrimeInterval, &pWeightsPrime, &nWeightsPrime, &newEqPrimeInterval, 1);

        /* nnetDelta */
        affineTransform(&eqInterval, &pWeightsDelta, &nWeightsDelta, &newEqDeltaInterval, 1);
        affineTransform(&eqDeltaInterval, &pWeightsPrime, &nWeightsPrime, &newEqDeltaInterval, 0);

        // layer + 1 because layerSizes array includes input layer
        for (i = 0; i < nnet->layerSizes[layer + 1]; i++) {
            concUp = concLow = 0.0;
            concLowsUp = concUpsLow = 0.0;

            concPrimeLow = concPrimeUp = 0.0;
            concPrimeLowsUp = concPrimeUpsLow = 0.0;

            concDeltaLow = concDeltaUp = 0.0;

            eqOffset = i * equationSize;
            constantIndex = eqOffset + equationSize - 1;

            /* Add bias to the constant */
            fesetround(FE_DOWNWARD); // lower bounds
            newEqLow[constantIndex] += bias.data[i];
            newEqPrimeLow[constantIndex] += biasPrime.data[i];
            newEqDeltaLow[constantIndex] += biasDelta.data[i];
            fesetround(FE_UPWARD); // upper bounds
            newEqUp[constantIndex] += bias.data[i];
            newEqPrimeUp[constantIndex] += biasPrime.data[i];
            newEqDeltaUp[constantIndex] += biasDelta.data[i];

            computeAllBounds(newEqLow + eqOffset, newEqUp + eqOffset,
                             input, equationSize, &concLow, &concLowsUp, &concUpsLow, &concUp);
            computeAllBounds(newEqPrimeLow + eqOffset, newEqPrimeUp + eqOffset,
                             input, equationSize, &concPrimeLow, &concPrimeLowsUp, &concPrimeUpsLow, &concPrimeUp);
#ifndef EXTRAVARSSYM
            computeAllBounds(newEqDeltaLow + eqOffset, newEqDeltaUp + eqOffset,
                             input, equationSize, &concDeltaLow, &concDeltaLowsUp, &concDeltaUpsLow, &concDeltaUp);
#else
            computeIntervalBoundsExtraSymbolic(&newEqDeltaInterval, extraVarInterval, input,
                    inputSize, equationSize, i, &concDeltaLow, &concDeltaLowsUp, &concDeltaUpsLow, &concDeltaUp);
#endif

            log_neuron_pre_relu(layer, i, input, &newEqInterval, &newEqPrimeInterval, &newEqDeltaInterval,
                                concLow, concLowsUp, concUpsLow, concUp,
                                concPrimeLow, concPrimeLowsUp, concPrimeUpsLow, concPrimeUp, concDeltaLow, concDeltaUp,
                                nnet);

			
			//assert(concDeltaUp >= concDeltaLow);
			//assert(concDeltaUpsLow >= concDeltaLow);
            //if (concDeltaUp - concDeltaLow < -1e-7) {
            //    fprintf(stderr, "it happened: %f\n", concDeltaLow - concDeltaUp);
            //}

            if (layer < (numLayers - 1)) { // ReLU Transform
                /*
                float newUp;
                int ret = lpRefineBounds(&newEqInterval, &newEqPrimeInterval, &newEqDeltaInterval, input, i, equationSize,
                               &newUp);
                if (ret == 0 && newUp < concDeltaUp) {
                    fprintf(stderr, "tightened: %4.8f,%4.8f,%d\n", concDeltaUp, newUp, layer);
                    //concDeltaUp = newUp;
                } else if (ret == 0 && newUp > concDeltaUp + FLT_MIN) {
                    fprintf(stderr, "increased: %4.8f,%4.8f,%d,%d\n", concDeltaUp, newUp, layer, i);
				}*/

                if (concUp <= 0.0) { // eq is 0
                    R[layer][i] = 0;
                    zero_interval(&newEqInterval, equationSize, i);

                    if (concPrimeUp <= 0.0) { // case 1, both are zero
                        RPrime[layer][i] = 0;

                        zero_interval(&newEqPrimeInterval, equationSize, i);

                        zero_interval(&newEqDeltaInterval, equationSize, i);

                    } else if (concPrimeLow >= 0.0) { // case 2, eqPrime is linear
                        RPrime[layer][i] = 2;
                        /* No need to change eqPrime */
                        /* newEqDelta = newEqPrime - 0 */
                        for (k = eqOffset; k < eqOffset + equationSize; k++) {
                            newEqDeltaLow[k] = newEqPrimeLow[k];
                            newEqDeltaUp[k] = newEqPrimeUp[k];
                        }

                    } else { // case 3, eqPrime is non-linear
                        RPrime[layer][i] = 1;

#ifdef LINEQ
                        linearizeNeuronEq(&newEqPrimeInterval, equationSize, i,
                                          concPrimeLow, concPrimeLowsUp, concPrimeUpsLow, concPrimeUp);

                        for (k = eqOffset; k < eqOffset + equationSize; k++) {
                            newEqDeltaLow[k] = 0.0;
                            newEqDeltaUp[k] = newEqPrimeUp[k];
                        }
#else
                        concretizeNeuronEq(&newEqPrimeInterval, equationSize, i, concPrimeUpsLow, concPrimeUp);

                        /* lower bound is always 0 */
                        if (concPrimeUpsLow >= 0.0) {
                            /* upper bound is linear, so save the equation. */
                            for (k = eqOffset; k < eqOffset + equationSize; k++) {
                                newEqDeltaLow[k] = 0.0;
                                newEqDeltaUp[k] = newEqPrimeUp[k];
                            }
                        } else {
                            /* otherwise both a concrete */
                            zero_interval(&newEqDeltaInterval, equationSize, i);
                            newEqDeltaLow[constantIndex] = 0.0;
                            newEqDeltaUp[constantIndex] = concPrimeUp;
                        }
#endif

                    }

                } else if (concLow >= 0.0) { // eq is linear
                    R[layer][i] = 2;
                    /* no need to do anything to eq */

                    if (concPrimeUp <= 0.0) { // case 4, eqPrime is 0
                        RPrime[layer][i] = 0;

                        zero_interval(&newEqPrimeInterval, equationSize, i);

                        for (k = eqOffset; k < eqOffset + equationSize; k++) {
                            newEqDeltaLow[k] = -newEqUp[k];
                            newEqDeltaUp[k] = -newEqLow[k];
                        }


                    } else if (concPrimeLow >= 0.0) { // case 5, both are linear, we can do nothing
                        RPrime[layer][i] = 2;

                    } else { // case 6
                        RPrime[layer][i] = 1;
#ifdef LINEQ
                        linearizeNeuronEq(&newEqPrimeInterval, equationSize, i,
                                          concPrimeLow, concPrimeLowsUp, concPrimeUpsLow, concPrimeUp);
#else
                        concretizeNeuronEq(&newEqPrimeInterval, equationSize, i, concPrimeUpsLow, concPrimeUp);
#endif

                        /* More precise lower bound which we may use later...
                        float tmpLbEq[equationSize];
                        float tmpLow, tmpUp;
                        memcpy(tmpLbEq, newEqLow + eqOffset, equationSize* sizeof(float))
                        for (k = eqOffset; k < eqOffset + equationSize; k++)
                            tmpLbEq[k] += newEqDeltaLow[k];
                        computeSingleEqBounds(tmpLbEq, input, inputSize, &tmpLow, &tmpUp);*/

                        /* we can optimize when -concLow < concDeltaUp, but it almost never happens... */
#ifdef LINEQDELTA
                        /* deltaLow is always a sound lower bound */

                        /* Use concrete delta upper bounds (there are also valid approximations
                         * for this case too, we just need to prove them sound) */
                        for (k = eqOffset; k < eqOffset + equationSize; k++) {
                            newEqDeltaUp[k] = 0;
                        }
                        newEqDeltaUp[constantIndex] = fmax(-concLow, concDeltaUp);

#else
                        zero_interval(&newEqDeltaInterval, equationSize, i);
                        newEqDeltaLow[constantIndex] = fmax(-concUp, concDeltaLow);
                        newEqDeltaUp[constantIndex] = fmax(-concLow, concDeltaUp);
#endif

                    }
                } else { // eq is non-linear
                    R[layer][i] = 1;

#ifdef LINEQ
                    linearizeNeuronEq(&newEqInterval, equationSize, i,
                                concLow, concLowsUp, concUpsLow, concUp);
#else
                    concretizeNeuronEq(&newEqInterval, equationSize, i, concUpsLow, concUp);
#endif

                    if (concPrimeUp <= 0.0) { // case 7, eq prime is 0
                        RPrime[layer][i] = 0;

                        zero_interval(&newEqPrimeInterval, equationSize, i);
#ifdef LINEQ
                        for (k = eqOffset; k < eqOffset + equationSize; k++) {
                            newEqDeltaLow[k] = -newEqUp[k];
                            newEqDeltaUp[k] = 0.0;
                        }
#else
                        if (concUpsLow >= 0.0) {
                            for (k = eqOffset; k < eqOffset + equationSize; k++) {
                                newEqDeltaLow[k] = -newEqUp[k];
                                newEqDeltaUp[k] = 0.0;
                            }
                        } else {
                            zero_interval(&newEqDeltaInterval, equationSize, i);
                            newEqDeltaLow[constantIndex] = -concUp;
                            newEqDeltaUp[constantIndex] = 0;
                        }
#endif

                    } else if (concPrimeLow >= 0.0) { // case 8
                        RPrime[layer][i] = 2;
                        // eqprime is linear, no need to do anything to it

#ifdef LINEQDELTA
                        /* Use concrete LB for now, but note that there are
                         * valid approximations for the LB too. */
                        for (k = eqOffset; k < eqOffset + equationSize; k++) {
                            newEqDeltaLow[k] = 0;
                        }
                        newEqDeltaLow[constantIndex] = fmin(concPrimeLow, concDeltaLow);
                        /* delta up is always sound */
#else
                        zero_interval(&newEqDeltaInterval, equationSize, i);
                        newEqDeltaLow[constantIndex] = fmin(concPrimeLow, concDeltaLow);
                        newEqDeltaUp[constantIndex] = fmin(concPrimeUp, concDeltaUp);
#endif

                    } else {// case 9
                        numUnstable++;
                        RPrime[layer][i] = 1;
#ifdef LINEQ
                        linearizeNeuronEq(&newEqPrimeInterval, equationSize, i,
                                          concPrimeLow, concPrimeLowsUp, concPrimeUpsLow, concPrimeUp);
#else
                        concretizeNeuronEq(&newEqPrimeInterval, equationSize, i, concPrimeUpsLow, concPrimeUp);
#endif

#ifndef LINEQDELTA
                        if (concDeltaLow >= 0) {
                            zero_interval(&newEqDeltaInterval, equationSize, i);
                            newEqDeltaUp[constantIndex] = concDeltaUp;

                        } else if (concDeltaUp <= 0) {
                            zero_interval(&newEqDeltaInterval, equationSize, i);
                            newEqDeltaLow[constantIndex] = fmax(concDeltaLow, -concUp);
                            newEqDeltaUp[constantIndex] = 0;
                        } else {
                            zero_interval(&newEqDeltaInterval, equationSize, i);
                            newEqDeltaLow[constantIndex] = fmax(concDeltaLow, -concUp);
                            newEqDeltaUp[constantIndex] = concDeltaUp;

                        }
#else
                        if (concDeltaUp < 0) {
                            for (k = eqOffset; k < eqOffset + equationSize; k++) {
                                newEqDeltaUp[k] = 0.0;
                            }
                        } else if (concDeltaUpsLow < 0) {
                            linearizeDeltaUB(&newEqDeltaInterval, equationSize, i, concDeltaUpsLow, concDeltaUp);
                        } // else both up and upslow are great than 0, so we are good

                        if (concDeltaLow > 0) {
                            for (k = eqOffset; k < eqOffset + equationSize; k++) {
                                newEqDeltaLow[k] = 0.0;
                            }
                        } else if (concDeltaLowsUp > 0) {
                            linearizeDeltaLB(&newEqDeltaInterval, equationSize, i, concDeltaLow, concDeltaLowsUp);
                        } // else both low and upsLow are less than 0
#endif
                        /* computeAllBounds(newEqDeltaLow + eqOffset, newEqDeltaUp + eqOffset,
                                         input, equationSize, &concDeltaLow, &concDeltaLowsUp, &concDeltaUpsLow, &concDeltaUp);
                        assert(concDeltaUpsLow >= concDeltaLow); */

#ifdef EXTRAVARS
                        if (remainingExtraVars > 0) {
                            /* Compute the bounds out here (and not in makeIntermediateVar) so
                             * we can do the heuristic check. */
#ifndef EXTRAVARSSYM
                            computeAllBounds(newEqDeltaLow + eqOffset, newEqDeltaUp + eqOffset,
                                    input, equationSize, &concDeltaLow, &concDeltaLowsUp, &concDeltaUpsLow, &concDeltaUp);
#else
                            computeIntervalBoundsExtraSymbolic(&newEqDeltaInterval, extraVarInterval, input,
                                    inputSize, equationSize, i, &concDeltaLow, &concDeltaLowsUp, &concDeltaUpsLow, &concDeltaUp);
#endif

#ifdef EXTRAVARSHEURISTIC
                            /* float threshold = 0.01;
                            float nonsymbolicInfo = 1 - computeSymbolicInfo(&newEqDeltaInterval, extraVarInterval, input,
                                    inputSize, equationSize, i);
                            int maxVarsThisLayer = (1 - ((float)prevLayerNumExtraVars/(float)nnet->layerSizes[layer]))*
                                    (float)nnet->layerSizes[layer + 1];*/

                            if ((layer != layer - 2 ) &&
                                ((float)prevLayerNumExtraVars < ((float)nnet->layerSizes[layer])/2.0)) {
#else
                            if (1) {
#endif
                                curLayerNumExtraVars++;
                                makeIntermediateVar(&newEqDeltaInterval, extraVarInterval, input, equationSize - remainingExtraVars - 1,
                                                    inputSize, equationSize, i, concDeltaLow, concDeltaUp);
                                remainingExtraVars--;
                            }
                        }
#endif

                    }
                }

                log_neuron_post_relu(layer, i, input, &newEqInterval, &newEqPrimeInterval, &newEqDeltaInterval);

            } else {
                output->lower_matrix.data[i] = concLow;
                output->upper_matrix.data[i] = concUp;
                outputDelta->lower_matrix.data[i] = concDeltaLow;
                outputDelta->upper_matrix.data[i] = concDeltaUp;
            }

        }
        //fprintf(stderr, "%d ", numUnstable);
        nextEVS += numUnstable/((layer+1))+1;
        //fprintf(stderr, "%d\n", nextEVS);
        prevLayerNumExtraVars = curLayerNumExtraVars;
        //printf("layer %d extra vars: %d\n", layer, curLayerNumExtraVars);
        curLayerNumExtraVars = 0;
        memcpy(eqLow, newEqLow, maxEquationMatrixSize);
        memcpy(eqUp, newEqUp, maxEquationMatrixSize);
        memcpy(eqPrimeLow, newEqPrimeLow, maxEquationMatrixSize);
        memcpy(eqPrimeUp, newEqPrimeUp, maxEquationMatrixSize);
        memcpy(eqDeltaLow, newEqDeltaLow, maxEquationMatrixSize);
        memcpy(eqDeltaUp, newEqDeltaUp, maxEquationMatrixSize);

        eqInterval.lower_matrix.row = eqInterval.upper_matrix.row = \
                                                         newEqInterval.lower_matrix.row;
        eqInterval.lower_matrix.col = eqInterval.upper_matrix.col = \
                                                         newEqInterval.lower_matrix.col;
        eqPrimeInterval.lower_matrix.row = eqPrimeInterval.upper_matrix.row = \
                                                         newEqPrimeInterval.lower_matrix.row;
        eqPrimeInterval.lower_matrix.col = eqPrimeInterval.upper_matrix.col = \
                                                         newEqPrimeInterval.lower_matrix.col;
        eqDeltaInterval.lower_matrix.row = eqDeltaInterval.upper_matrix.row = \
                                                         newEqDeltaInterval.lower_matrix.row;
        eqDeltaInterval.lower_matrix.col = eqDeltaInterval.upper_matrix.col = \
                                                         newEqDeltaInterval.lower_matrix.col;
    }

    log_forward_pass_exit(nnet, outputDelta);

    if (refinementParams->refineDeltaInput == 0) {
        float gradLower[inputSize], gradUpper[inputSize];
        struct Interval gradInt = {
                (struct Matrix) {gradLower, 1, inputSize},
                (struct Matrix) {gradUpper, 1, inputSize}
        };

        computeGradient(verificationParams, maxLayerSize, R, RPrime, &gradInt);

        smearBasedRefinement(verificationParams, &inputSpace->inputs, &gradInt, refinementParams);
    } // else {
        // we already decided which delta input to refine
    //}

    float epsilon = verificationParams->epsilon;
    int target = verificationParams->target;
    float thresh = verificationParams->useExtraVarsThresh;
    refinementParams->numExtraVars = nextEVS;
    /*if (thresh == 0.0) {
        //refinementParams->numExtraVars = verificationParams->extraVars;

        //fprintf(stderr, "evs: %d\n", refinementParams->numExtraVars);
    } else if (is_between(outputDelta->lower_matrix.data[target], -epsilon*thresh, epsilon*thresh) &&
        is_between(outputDelta->upper_matrix.data[target], -epsilon*thresh, epsilon*thresh)) {

        if (verificationParams->extraVars != 0) {
            refinementParams->numExtraVars = verificationParams->extraVars;
        } else {
            //TODO: use our heuristic
        }
    }*/

    free(eqLow);
    free(eqUp);
    free(newEqLow);
    free(newEqUp);

    free(eqPrimeLow);
    free(eqPrimeUp);
    free(newEqPrimeLow);
    free(newEqPrimeUp);

    free(eqDeltaLow);
    free(eqDeltaUp);
    free(newEqDeltaLow);
    free(newEqDeltaUp);

#ifdef EXTRAVARSSYM
    free(extraVarEqLow);
    free(extraVarEqUp);
#endif

    return 1;
}
