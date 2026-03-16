#include <stdlib.h>

#include "../include/workload.h"

#define STRIDE_BYTES 64 // 1 element per cache line

static const int array_elements = 10000000;
static int *array;

static void init(void)
{
    array = (int *)aligned_alloc(STRIDE_BYTES, STRIDE_BYTES * array_elements);
    for (int i = 0; i < array_elements; i++) {
        array[i * (STRIDE_BYTES / sizeof(int))] = i;
    }
}

static void clean(void)
{
    free(array);
}

__attribute__((noinline))
static void workload(void)
{
    volatile int sum = 0;
    for (int i = 0; i < array_elements; i++) {
        sum += array[i * (STRIDE_BYTES / sizeof(int))];
    }
}

workload_t wl_strided_array = {
    .id = WL_STRIDED_ARRAY,
    .name = "STRIDED_ARRAY",
    .init = init,
    .clean = clean,
    .workload = workload,
};
