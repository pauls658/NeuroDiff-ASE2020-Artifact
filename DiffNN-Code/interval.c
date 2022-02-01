#include "interval.h"
#include "matrix.h"
#include <string.h>

void cpIntervalAlloc(struct Interval *dst, struct Interval *src) {
    dst->lower_matrix.col = src->lower_matrix.col;
    dst->lower_matrix.row = src->lower_matrix.row;
    dst->upper_matrix.col = src->upper_matrix.col;
    dst->upper_matrix.row = src->upper_matrix.row;
    int dataLen = src->lower_matrix.col*src->lower_matrix.row;
    if (dataLen != 0) {
        dst->lower_matrix.data = malloc(sizeof(float) * dataLen);
        dst->upper_matrix.data = malloc(sizeof(float) * dataLen);
        memcpy(dst->lower_matrix.data, src->lower_matrix.data, dataLen * sizeof(float));
        memcpy(dst->upper_matrix.data, src->upper_matrix.data, dataLen * sizeof(float));
    } else {
        dst->lower_matrix.data = NULL;
        dst->upper_matrix.data = NULL;
    }
}

void freeInterval(struct Interval *i) {
    free(i->lower_matrix.data);
    free(i->upper_matrix.data);
}

