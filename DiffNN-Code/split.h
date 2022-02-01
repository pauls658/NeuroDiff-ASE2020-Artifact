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

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include<signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "nnet.h"

#ifndef SPLIT_H
#define SPLIT_H

extern struct Interval *verified_region_for_depth;

extern int RUN_TO_DEPTH;

/* set 1 if a concrete adversarial example is
found */
extern int adv_found;

/* Bisection tree info */
extern float max_depth;
extern int leaf_num;
extern long long numSplits;

/* Progress record out of 1024 */
extern int progress;

/* Time record */
extern struct timeval start,finish, last_finish;

/* If the input range is less than ADV_THRESHOLD, 
then it will check for concrete adversarial example*/
#define ADV_THRESHOLD  0.00001

/* Thread locker */
pthread_mutex_t lock;

/* Active threads number */
extern int count;


/*
 * Define the argument structure of main function
 * direct_run_check used for parallelization.
 */
struct direct_run_check_args
{
    struct VerificationParams *verificationParams;
    struct InputSpace *inputSpace;
    int depth;
};


/*
 * Checks if the given input produces an output such
 * that the difference in outputs between nnet1 and nnet2
 * is greater than nnet1->epsilon.
 */
void check_adv_delta(struct VerificationParams *verificationParams, struct Interval *input);

int direct_run_check_delta(struct VerificationParams *verificationParams, struct InputSpace *inputSpace,
                           int depth);

void *direct_run_check_delta_thread(void *args);

/*
 * Function for splitting and creating new threads
 * for newly split input ranges.
 */
int split_interval_delta(struct VerificationParams *verificationParams, struct InputSpace *inputSpace,
                struct RefinementParams *refinementParams, int depth);


#endif