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


#include "matrix.h"
#include <string.h>
#include "interval.h"

#ifndef NNET_H
#define NNET_H

//#define OUTWARD_ROUND 0.0000005
#define OUTWARD_ROUND 0.0

/* which property to test */
extern int PROPERTY;
extern float perturb;

/* log file */
extern char *LOG_FILE;
extern FILE *fp;

//typedef int bool;
//enum { false, true };


/*
 * Network instance modified from Reluplex
 * malloc all the memory needed for network
 */
struct NNet 
{
    int symmetric;     
    int numLayers;     
    int inputSize;     
    int outputSize;    
    int maxLayerSize;  
    int *layerSizes;   
    int *inputTypes;

    float *mins;      
    float *maxes;     
    float *means; 
    float *ranges;
    /*
     * first dimension: the layer (k)
     * second dimension: is bias (0 = no, 1 = yes)
     * third dimension: neuron in layer (k)
     * fourth dimension: source neuron in layer (k - 1)
     */
    float ****matrix;

    struct Matrix* weights;
    struct Matrix* posWeights;
    struct Matrix* negWeights;
    struct Matrix* bias;

};

/* These are the verification parameters __that never change__,
 * hence they do not include the input regions. */
struct VerificationParams {
    struct NNet *nnet;
    struct NNet *nnetPrime;
    struct NNet *nnetDelta;
    float epsilon;
    float perturb;
    int target;
    int extraVars; /* 0 means automatically determine number of vars */
    int mnistPixExp;
    float useExtraVarsThresh;
};

/* Describes the input space we are interested in. This
 * should really just be called ForwardPassParams */
struct InputSpace {
    struct Interval inputs;
    struct Interval deltas;
    int numExtraVars;
};

/* Data structure to pass information from the forward pass
 * to the refinement. */
struct RefinementParams
{
    int split_feature;
    float mid;
    int mono;
    int numFeatures;
    int testConcreteInput;
    int numExtraVars;
    /* Whether or not to refine the delta input interval,
     * or the input intervals. */
    int refineDeltaInput;
};

void initRefinementParams(struct RefinementParams *refinementParams);

/* load the network from file */
struct NNet *load_network(const char *filename);

/*
 * Subtracts the weights of nnet2 from nnet1.
 */
void compute_network_delta(struct NNet *nnet1, struct NNet *nnet2);

/*
 * Allocates the arrays for positive and negative weights, and
 * fills these weight matrices.
 */
void load_positive_and_negative_weights(struct NNet *nnet);

/* free all the memory for the network */
void destroy_network(struct NNet *network);


/* load the input range of the property */
void load_inputs(int PROPERTY, int inputSize, struct InputSpace *inputSpace, struct VerificationParams *verificationParams);


/* denormalize input */
void denormalize_input(struct NNet *nnet, struct Matrix *input);


/* denormalize input range */
void denormalize_input_interval(struct NNet *nnet, struct Interval *input);


/* normalize input */
void normalize_input(struct NNet *nnet, struct Matrix *input);


/* normalize input range */
void normalize_input_interval(struct NNet *nnet, struct Interval *input);


/*  
 * Uses for loop to calculate the output
 * 0.00002607 sec for one run with one core
*/
int evaluate(struct NNet *network, struct Matrix *input, struct Matrix *output);


/*  
 * Uses sgemm to calculate the output
 * 0.00001359 sec for one run with one core
*/
int forward_prop(struct NNet *network, struct Matrix *input, struct Matrix *output);

/*
 * The back prop to calculate the gradient
 * 0.000249 sec for one run with one core
*/
void backward_prop(struct NNet *nnet, int target, int maxLayerSize, struct Interval *grad, int R[][maxLayerSize], int subtract);


/*
 * Computes the difference in output between in nnet1 and nnet2
 * and stores it in the given output matrix.
 * (i.e. output = nnet2 - nnet1)
 */
int forward_prop_delta(struct NNet *nnet1, struct NNet *nnet2, struct Matrix *input, struct Matrix *output);

int forward_prop_delta_neurodiff(struct VerificationParams *params, struct InputSpace *inputSpace,
        struct Interval *output, struct Interval *outputDelta, struct RefinementParams *refinementParams);

/* Function graveyard
 * Brandon's attempt at adding in intermediate symbolic variables.
int forward_prop_delta_intermediate_vars(struct NNet *nnet, struct NNet *nnetPrime, struct NNet *nnetDelta,\
            struct Interval *input, struct Interval *output,\
            struct Interval *grad, struct Interval *outputDelta, int extraVars);
*/

#endif
