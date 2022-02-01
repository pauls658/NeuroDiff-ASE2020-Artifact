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

#ifndef INTERVAL_H
#define INTERVAL_H

/* define the structure of interval */
struct Interval
{
	struct Matrix lower_matrix;
	struct Matrix upper_matrix;
};

void cpIntervalAlloc(struct Interval *dst, struct Interval *src);

void freeInterval(struct Interval *i);

#endif

