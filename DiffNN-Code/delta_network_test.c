#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "split.h"
#include <float.h>

#ifdef DEBUG
    #include <fenv.h>
#endif

/* print the progress if getting SIGQUIT */
void sig_handler(int signo)
{

    if (signo == SIGINT) {
        fprintf(stderr, "progress: %d/1024\n", progress);
	    fprintf(stderr, "numSplits: %lld\n", numSplits);
    }
	exit(0);
}


void printUsage(char *argv[]) {
    printf("USAGE\n\t%s PROPERTY NNET1 NNET2 EPSILON ", argv[0]);
    printf("[OPTIONS]\n\n");
    printf("DESCRIPTION\n");
    printf("\tTries to verify that the outputs of NNET1 and NNET2\n");
    printf("\tcannot differ by more than EPSILON over the input region\n");
    printf("\tdefined by PROPERTY.\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("\t-p PERTURB\n\t\tSpecifies the maximum perturbation allowed when applicable.\n\n");
    printf("\t-x NUM_PIX\n\t\tSpecifies the number of pixels to perturb when applicable.\n\n");
    printf("\t-m DEPTH\n\t\tForces the analysis to make DEPTH splits, and then prints the\n");
    printf("\t\tregion verified at each depth.\n\n");
    printf("\t-e NUMEXTRAVARS\n");
    printf("\t\tThe maximum number of extra variables that can\n");
    printf("\t\tbe used during the forward pass. Should be set to 0 \n\t\tto use the hueristic from the paper.\n\n");
}

int main( int argc, char *argv[])
{
#ifdef DEBUG
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW);
#endif

	signal(SIGINT, sig_handler);
    int PROPERTY = -1;
    int extraVars = 0;
    int mnistPixExp = 0;
    float epsilon = 0.0;
    float extraVarThresh = 0.0;
    float perturb = 0.0;
    struct VerificationParams params;

    int opt;
    while ((opt = getopt(argc, argv, "p:m:e:t:x:")) != -1) {
        switch (opt) {
            case 'p':
                perturb = atof(optarg);
                break;
            case 'm':
                RUN_TO_DEPTH = atoi(optarg);
                break;
            case 'e':
                extraVars = atoi(optarg);
                break;
            case 't':
                extraVarThresh = atof(optarg);
                break;
            case 'x':
                mnistPixExp = atoi(optarg);
                break;
            default:
                printf("Invalid option: %c\n", opt);
                printUsage(argv);
                exit(1);
        }
    }


    if (argc - optind != 4) {
        printf("Only recieved %d positional parameters.\n", argc - optind);
        printUsage(argv);
        exit(1);
    }

    char *FULL_NET_PATH1 = NULL, *FULL_NET_PATH2 = NULL;
    PROPERTY = atoi(argv[optind]);
    FULL_NET_PATH1 = argv[optind + 1];
    FULL_NET_PATH2 = argv[optind + 2];
    epsilon = atof(argv[optind + 3]);

    openblas_set_num_threads(1);


    srand((unsigned)time(NULL));
    double time_spent;

    struct NNet* nnet1 = load_network(FULL_NET_PATH1);
    load_positive_and_negative_weights(nnet1);
    struct NNet* nnet2 = load_network(FULL_NET_PATH2);
    load_positive_and_negative_weights(nnet2);
    struct NNet* nnetDelta = load_network(FULL_NET_PATH2);
    compute_network_delta(nnetDelta, nnet1);
    load_positive_and_negative_weights(nnetDelta);
    nnet1->inputTypes =
            nnet2->inputTypes =
            nnetDelta->inputTypes = NULL;

    params.nnet = nnet1;
    params.nnetPrime = nnet2;
    params.nnetDelta = nnetDelta;
    params.epsilon = epsilon;
    params.perturb = perturb;
    params.extraVars = extraVars;
    params.useExtraVarsThresh = extraVarThresh;
    params.mnistPixExp = mnistPixExp;

    int inputSize    = nnet1->inputSize;
    int outputSize   = nnet1->outputSize;


    /* load the input intervals, and put them into matrices */
    float u[inputSize], l[inputSize];
    float ud[inputSize], ld[inputSize];
    struct InputSpace inputSpace = {
            (struct Interval) {
                    (struct Matrix) {l,1,inputSize},
                    (struct Matrix) {u,1,inputSize}
            },
            (struct Interval) {
                    (struct Matrix) {ld,1,inputSize},
                    (struct Matrix) {ud,1,inputSize}
            },
            params.useExtraVarsThresh == 0.0 ? extraVars : 0
    };
    load_inputs(PROPERTY, inputSize, &inputSpace, &params);

    /* If RUN_TO_DEPTH is set (i.e it is >= 0), then we will perform a fixed number of splits
     * and exit. We record the bounds verified at each depth, so we allocate arrays to record
     * the bounds. */
    if (RUN_TO_DEPTH > 0) {
        verified_region_for_depth = malloc(sizeof(struct Interval)*(RUN_TO_DEPTH+1));
        for (int i = 0; i < RUN_TO_DEPTH+1; i++) {
            verified_region_for_depth[i].lower_matrix.data = malloc(sizeof(float)*outputSize);
            verified_region_for_depth[i].upper_matrix.data = malloc(sizeof(float)*outputSize);
            verified_region_for_depth[i].lower_matrix.row = 1;
            verified_region_for_depth[i].lower_matrix.col = outputSize;
            verified_region_for_depth[i].upper_matrix.row = 1;
            verified_region_for_depth[i].upper_matrix.col = outputSize;
            for (int j = 0; j < outputSize; j++) {
                verified_region_for_depth[i].lower_matrix.data[j] = FLT_MAX;
                verified_region_for_depth[i].upper_matrix.data[j] = -FLT_MAX;
            }
        }
    }

    /* Certain properties should not have their inputs normalized.
     * 300 - 399 are toy properties for our motivating example.
     * 400 - 599 are MNIST properties, which are normalized in the
     * load_inputs function. */
    if (PROPERTY < 300 || PROPERTY > 700 && PROPERTY != 800) {
        printf("Normalizing input\n");
        normalize_input_interval(nnet1, &inputSpace.inputs);
    }


    /* Sanity check that their are no intervals with a "negative" width. */
    for (int i=0;i<inputSize;i++) {
        if (inputSpace.inputs.upper_matrix.data[i] <\
                inputSpace.inputs.lower_matrix.data[i]) {
            printf("Input lower bound is greater than upper bound for input: %d!\n", i);
            exit(1);
        }
    }


    gettimeofday(&start, NULL);
    int isOverlap = 0;

    if (RUN_TO_DEPTH == 0) {
        struct RefinementParams refinementParams;
		initRefinementParams(&refinementParams);
        float oDelta_upper[outputSize], oDelta_lower[outputSize];
        struct Interval output_delta_interval = {
                (struct Matrix){oDelta_lower, outputSize, 1},
                (struct Matrix){oDelta_upper, outputSize, 1}
        };
        float o_upper[outputSize], o_lower[outputSize];
        struct Interval output_interval = {
                (struct Matrix){o_lower, outputSize, 1},
                (struct Matrix){o_upper, outputSize, 1}
        };

        forward_prop_delta_neurodiff(&params, &inputSpace, &output_interval, &output_delta_interval, &refinementParams);

        if (check_epsilon_interval(&params, &output_delta_interval)) {
            inputSpace.numExtraVars = refinementParams.numExtraVars;
            forward_prop_delta_neurodiff(&params, &inputSpace, &output_interval, &output_delta_interval, &refinementParams);
        }
        fprintf(stderr, "Output interval: ");
        fprintf(stderr, "[%2.4f, %2.4f]\n",
                output_interval.lower_matrix.data[params.target],
                output_interval.upper_matrix.data[params.target]);
        fprintf(stderr, "Output delta interval: ");
        fprintf(stderr, "[%2.4f, %2.4f]\n",
                output_delta_interval.lower_matrix.data[params.target],
                output_delta_interval.upper_matrix.data[params.target]);
    } else {
        isOverlap = direct_run_check_delta(&params, &inputSpace, 0);
    }


    gettimeofday(&finish, NULL);
    time_spent = ((float)(finish.tv_sec - start.tv_sec) *\
            1000000 + (float)(finish.tv_usec - start.tv_usec)) /\
            1000000;

    if (isOverlap == 0 && adv_found == 0) {
        fprintf(stderr, "\nNo adv!\n");
    }

	if (RUN_TO_DEPTH > 0) {
		for (int i = 0; i < RUN_TO_DEPTH+1; i++) {
			fprintf(stderr, "depth: %d\n", i);
			fprintMatrix(stderr, &verified_region_for_depth[i].lower_matrix);
			fprintMatrix(stderr, &verified_region_for_depth[i].upper_matrix);
			free(verified_region_for_depth[i].lower_matrix.data);
			free(verified_region_for_depth[i].upper_matrix.data);
		}
        free(verified_region_for_depth);
	}



    fprintf(stderr, "time: %f \n\n\n", time_spent);
    fprintf(stderr, "numSplits: %lld\n", numSplits);
    destroy_network(nnet1);
    destroy_network(nnet2);
    destroy_network(nnetDelta);
}
