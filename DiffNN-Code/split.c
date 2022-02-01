/*
 -----------------------------------------------------------------
 ** Top contributors:
 **   Shiqi Wang and Suman Jana
 ** This file is part of the ReluVal project.
 ** Copyright (c) 2018-2019 by the authors listed in the file LICENSE
 ** and their institutional affiliations.
 ** All rights reserved.
 -----------------------------------------------------------------
 */

#include "split.h"
#include <math.h>

#define AVG_WINDOW 5

#ifndef MAX_THREAD
#define MAX_THREAD -1
#endif

#define MIN_DEPTH_PER_THREAD 5

int NEED_PRINT = 0;
int RUN_TO_DEPTH = -1;
int input_depth = 0;
int adv_found = 0;
int count = 0;
int thread_tot_cnt  = 0;
int smear_cnt = 0;
long long numSplits = 0;

struct Interval *verified_region_for_depth;

int progress = 0;

int CHECK_ADV_MODE = 0;
int PARTIAL_MODE = 0;

float avg_depth = 50;
float total_avg_depth = 0;
int leaf_num = 0;
float max_depth = 0;

struct timeval start,finish,last_finish;


/*
 * ACAS Xu properties.
 */

int check_between(float x, float lower, float upper) {
    if (x > lower && x < upper) {
        return 1;
    }
    return 0;
}

int check_epsilon_concrete(struct VerificationParams *params, struct Matrix *output) {
    float epsilon = params->epsilon;

        if (!check_between(output->data[params->target], -epsilon, epsilon)) {
            // output is not within boundary
            return 1;
        }
    return 0;
}

int check_epsilon_interval(struct VerificationParams *params, struct Interval *output) {
    float epsilon = params->epsilon;
    if (!check_between(output->lower_matrix.data[params->target], -epsilon, epsilon) ||
        !check_between(output->upper_matrix.data[params->target], -epsilon, epsilon)) {
        return 1;
    }


    return 0;
}


/*
 * multithread for direct_run_check function
*/
void *direct_run_check_delta_thread(void *args)
{

    struct direct_run_check_args *actual_args = args;

    direct_run_check_delta(actual_args->verificationParams,
                    actual_args->inputSpace, actual_args->depth);

    return NULL;
}

int direct_run_check_delta(struct VerificationParams *verificationParams,
                    struct InputSpace *inputSpace, int depth)
{

    pthread_mutex_lock(&lock);

    if (adv_found) {
        pthread_mutex_unlock(&lock);

        return 0;
    }

    pthread_mutex_unlock(&lock);

    struct RefinementParams refinementParams;
    initRefinementParams(&refinementParams);
    int outputSize = verificationParams->nnet->outputSize;

    float lower[outputSize], upper[outputSize];
    struct Interval output = {
            (struct Matrix) {lower, outputSize, 1},
            (struct Matrix) {upper, outputSize, 1}
    };
    float lowerDelta[outputSize], upperDelta[outputSize];
    struct Interval outputDelta = {
            (struct Matrix) {lowerDelta, outputSize, 1},
            (struct Matrix) {upperDelta, outputSize, 1}
    };

    forward_prop_delta_neurodiff(verificationParams, inputSpace, &output, &outputDelta, &refinementParams);

    if (depth == 0) {
        fprintf(stderr, "Initial output delta:\n");
        fprintMatrix(stderr, &outputDelta.lower_matrix);
        fprintMatrix(stderr, &outputDelta.upper_matrix);
    }

    int isOverlap = check_epsilon_interval(verificationParams, &outputDelta);

#ifdef DEPTH_VERIFIED
    pthread_mutex_lock(&lock);
    if (!isOverlap) {
    	fprintf(stderr, "%d\n", depth);
    }
    pthread_mutex_unlock(&lock);
#endif

    if (RUN_TO_DEPTH != -1) {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < outputSize; i++) {
            verified_region_for_depth[depth].upper_matrix.data[i] = fmax(outputDelta.upper_matrix.data[i],
                                                                         verified_region_for_depth[depth].upper_matrix.data[i]);
            verified_region_for_depth[depth].lower_matrix.data[i] = fmin(outputDelta.lower_matrix.data[i],
                                                                         verified_region_for_depth[depth].lower_matrix.data[i]);
        }
        pthread_mutex_unlock(&lock);
    }


    if (depth == 10 && isOverlap == 0) { /* Print progress */
        pthread_mutex_lock(&lock);
            progress++;
            if(!adv_found){
                printf("progress: [");
                for (int pp=0;pp<50;pp++) {

                    if (pp <= ((float)progress/1024)*50) {
                        printf("=");
                    }
                    else {
                        printf(" ");
                    }
                }
                printf("] %0.2f%%\r", ((float)progress/1024)*100);
                fflush(stdout);
            }
        pthread_mutex_unlock(&lock);
    }


    if (RUN_TO_DEPTH != -1) { /* Force to run to a specific depth */
        if (depth < RUN_TO_DEPTH) {
            isOverlap = split_interval_delta(verificationParams, inputSpace, &refinementParams, depth);
        }
    } else if (isOverlap) { /* Else run like normal */
        isOverlap = split_interval_delta(verificationParams, inputSpace, &refinementParams, depth);

    }
#ifdef VERIFIED
    else if (!isOverlap) {
        pthread_mutex_lock(&lock);
        fprintf(stderr, "# verified\n");
        pthread_mutex_unlock(&lock);
    }
#endif

    return isOverlap;

}

/*
 * Check the existance of concrete adversarial examples
 * It takes in the network and input ranges.
 * If a concrete adversarial example is found,
 * global adv_found will be set to 1. 
 */
void check_adv_delta(struct VerificationParams *verificationParams, struct Interval *input)
{
    struct NNet *nnet1 = verificationParams->nnet;
    struct NNet *nnet2 = verificationParams->nnetPrime;
    float a[nnet1->inputSize];
    struct Matrix adv = {a, 1, nnet1->inputSize};

    for (int i=0;i<nnet1->inputSize;i++) {
        float upper = input->upper_matrix.data[i];
        float lower = input->lower_matrix.data[i];
        float middle = (lower+upper)/2;

        a[i] = middle;
    }

    float outDelta[nnet1->outputSize];
    struct Matrix outputDelta = {outDelta, nnet1->outputSize, 1};

    forward_prop_delta(nnet1, nnet2, &adv, &outputDelta);

    int is_adv = 0;
    is_adv = check_epsilon_concrete(verificationParams, &outputDelta);


    if (is_adv) {
        fprintf(stderr, "\nadv found:\n");
        fprintf(stderr, "adv is: ");
        fprintMatrix(stderr, &adv);
        fprintf(stderr, "it's output delta is: ");
        fprintMatrix(stderr, &outputDelta);
        pthread_mutex_lock(&lock);
        adv_found = 1;
        pthread_mutex_unlock(&lock);
    }

}



int split_interval_delta(struct VerificationParams *verificationParams, struct InputSpace *inputSpace,
                struct RefinementParams *refinementParams, int depth)
{

    pthread_mutex_lock(&lock);
    if (adv_found && RUN_TO_DEPTH == -1) {
        pthread_mutex_unlock(&lock);
        return 0;
    }
    numSplits += 1;
    pthread_mutex_unlock(&lock);

    struct NNet *nnet = verificationParams->nnet;
    int inputSize = nnet->inputSize;

    int numPartitions;
    int split_feature = refinementParams->split_feature;
    int refineDeltaInput = refinementParams->refineDeltaInput;
    int *inputTypes = nnet->inputTypes;
    if (inputTypes == NULL) {
        // no input types => all inputs are continuous, bisect
        numPartitions = 2;
    } else if (inputTypes[split_feature] == 0) {
        // input is continuous => bisect
        numPartitions = 2;
    } else {
        // input is categorical
        // split_feature should point to the first input with
        // this category type
        int numLabels = 1;
        while (numLabels + split_feature < inputSize &&
                inputTypes[numLabels + split_feature] == inputTypes[split_feature]) {
            numLabels++;
        }

        if (numLabels == 1) {
            fprintf(stderr, "Refining binary inputs not supported yet!\n");
            exit(1);
        }

        if (refineDeltaInput == 1) {
            numPartitions = pow(2, numLabels);
        } else {
            numPartitions = numLabels;
        }
    }

    /*
     * Allocate all of the partitions.
     */
    struct InputSpace inputSpaces[numPartitions];
    for (int i = 0; i < numPartitions; i++) {
        cpIntervalAlloc(&inputSpaces[i].inputs, &inputSpace->inputs);
        cpIntervalAlloc(&inputSpaces[i].deltas, &inputSpace->deltas);
        inputSpaces[i].numExtraVars = refinementParams->numExtraVars;
        //if (inputSpaces[i].numExtraVars == 0) printf("no vars!\n");
    }

    if (inputTypes == NULL || inputTypes[split_feature] == 0) {
        // Continous split feature. Either an input interval or a delta input interval
        if (refineDeltaInput == 1) {
            fprintf(stderr, "Refining continuous delta input is not supported yet. Bye bye.\n");
            exit(1);
        } else {
            struct Interval *int1, *int2;
            int1 = &inputSpaces[0].inputs;
            int2 = &inputSpaces[1].inputs;

            if (refinementParams->mono == 1) {
                int1->lower_matrix.data[split_feature] = int1->upper_matrix.data[split_feature] =
                        inputSpace->inputs.upper_matrix.data[split_feature];
                int2->lower_matrix.data[split_feature] = int2->upper_matrix.data[split_feature] =
                        inputSpace->inputs.lower_matrix.data[split_feature];

                if (refinementParams->numFeatures <= 1) {
                    check_adv_delta(verificationParams, int1);
                    check_adv_delta(verificationParams, int2);
                    freeInterval(&inputSpaces[0].inputs);
                    freeInterval(&inputSpaces[0].deltas);
                    freeInterval(&inputSpaces[1].inputs);
                    freeInterval(&inputSpaces[1].deltas);
                    return 0;
                }

            } else { // mono == 0
                if ((depth >= 35 || refinementParams->testConcreteInput) && RUN_TO_DEPTH == -1) {
                    check_adv_delta(verificationParams, &inputSpace->inputs);
                }

                int1->lower_matrix.data[split_feature] = refinementParams->mid;
                int2->upper_matrix.data[split_feature] = refinementParams->mid;
            }
        }
    } else {
        // categorical
        fprintf(stderr, "Refining categorical features is not yet supported. Bye bye.\n");
        exit(1);
    }

    depth = depth + 1;

    if (numPartitions == 2) {
        pthread_mutex_lock(&lock);
        if ((depth <= avg_depth - MIN_DEPTH_PER_THREAD) && \
            (count <= MAX_THREAD)) {
            pthread_mutex_unlock(&lock);


            pthread_t workers1, workers2;
            struct direct_run_check_args args1 = {
                    verificationParams, &inputSpaces[0],
                    depth
            };
            struct direct_run_check_args args2 = {
                    verificationParams, &inputSpaces[1],
                    depth};


            pthread_create(&workers1, NULL, direct_run_check_delta_thread, &args1);

            pthread_mutex_lock(&lock);
            count++;
            thread_tot_cnt++;
            pthread_mutex_unlock(&lock);

            pthread_create(&workers2, NULL, direct_run_check_delta_thread, &args2);

            pthread_mutex_lock(&lock);
            count++;
            thread_tot_cnt++;
            pthread_mutex_unlock(&lock);

            pthread_join(workers1, NULL);

            pthread_mutex_lock(&lock);
            count--;
            pthread_mutex_unlock(&lock);

            pthread_join(workers2, NULL);

            pthread_mutex_lock(&lock);
            count--;
            pthread_mutex_unlock(&lock);


            if (depth == 11) { /* Print progress */
                pthread_mutex_lock(&lock);
                progress++;
                if (!adv_found) {
                    printf("progress: [");
                    for (int pp = 0; pp < 50; pp++) {

                        if (pp <= ((float) progress / 1024) * 50) {
                            printf("=");
                        } else {
                            printf(" ");
                        }
                    }
                    printf("] %0.2f%%\r", ((float) progress / 1024) * 100);
                    fflush(stdout);
                }
                pthread_mutex_unlock(&lock);
            }

        } else {

            pthread_mutex_unlock(&lock);

            direct_run_check_delta(verificationParams, &inputSpaces[0],
                                   depth);
            direct_run_check_delta(verificationParams, &inputSpaces[1],
                                   depth);

        }
        freeInterval(&inputSpaces[0].inputs);
        freeInterval(&inputSpaces[0].deltas);
        freeInterval(&inputSpaces[1].inputs);
        freeInterval(&inputSpaces[1].deltas);
        return 0;
    } else {
        fprintf(stderr, "More than 2 input partition refining is not supported yet. Bye bye\n");
        exit(1);
        return 0;
    }
}
