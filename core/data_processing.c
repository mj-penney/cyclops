#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../include/data_processing.h"
#include "../include/metric.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;

    if (x < y)
        return -1;

    if (x > y)
        return 1;

    return 0;
}

static int cmp_double(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;

    if (x < y)
        return -1;

    if (x > y)
        return 1;

    return 0;
}

uint64_agg_t aggregate_uint64(uint64_t array[], int size)
{
    uint64_agg_t agg;
    uint64_t *array_cpy = calloc(size, sizeof(uint64_t));
    if (!array_cpy) {
        perror("Failed to allocate array for uint64 aggregate calculations");
        exit(1);
    }

    memcpy(array_cpy, array, size * sizeof(uint64_t));

    qsort(array_cpy, size, sizeof(uint64_t), cmp_uint64);

    memset(&agg, 0, sizeof(uint64_agg_t));

    agg.min = array_cpy[0];
    agg.max = array_cpy[size - 1];
    agg.median = array_cpy[(size - 1) / 2]; // lower median

    free(array_cpy);
    return agg;
}

double_agg_t aggregate_double(double array[], int size)
{
    double_agg_t agg;
    double *array_cpy = calloc(size, sizeof(double));
    if (!array_cpy) {
        perror("Failed to allocate array for double aggregate calculations");
        exit(1);
    }

    memcpy(array_cpy, array, size * sizeof(double));

    qsort(array_cpy, size, sizeof(double), cmp_double);

    memset(&agg, 0, sizeof(double_agg_t));

    agg.min = array_cpy[0];
    agg.max = array_cpy[size - 1];
    agg.median = array_cpy[(size - 1) / 2]; // lower median

    free(array_cpy);
    return agg;
}

void calc_ratios(double results[], const uint64_t numerators[],
                                   const uint64_t denominators[],
                                   int size)
{
    for (int i = 0; i < size; i++) {
        results[i] = (double)numerators[i] / denominators[i];
    }
}
