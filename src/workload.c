#include <stdlib.h>
#include <stdio.h>

#include "../include/workload.h"

//#define BIG_NUMBER_1 777777
#define BIG_NUMBER_1 10000000

int contiguous_array[BIG_NUMBER_1];

static void init_contiguous_array()
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        contiguous_array[i] = i;
    }
}

static void clean_contiguous_array(void)
{
    return;
}

__attribute__((noinline))
static void contiguous_array_func(void)
{
    volatile int sum;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += contiguous_array[i];
    }
}

int *scattered_array[BIG_NUMBER_1];

static void init_scattered_array(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        scattered_array[i] = malloc(sizeof(int));
        *scattered_array[i] = i;
    }
}

static void clean_scattered_array(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        free(scattered_array[i]);
    }
}

__attribute__((noinline))
static void scattered_array_func(void)
{
    volatile int sum;

    sum = 0;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += *scattered_array[i];
    }
}

const workload_t all_workloads[NUMBER_OF_WORKLOADS] = {

    [WL_CONTIGUOUS_ARRAY] = {
        .id = WL_CONTIGUOUS_ARRAY,
        .name = "CONTIGUOUS_ARRAY",
        .init = init_contiguous_array,
        .clean = clean_contiguous_array,
        .workload = contiguous_array_func,
    },

    [WL_SCATTERED_ARRAY] = {
        .id = WL_SCATTERED_ARRAY,
        .name = "SCATTERED_ARRAY",
        .init = init_scattered_array,
        .clean = clean_scattered_array,
        .workload = scattered_array_func,
    },

};
