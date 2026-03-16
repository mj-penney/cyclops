#include <stdlib.h>

#include "../include/workload.h"

static const int array_size = 1000;
static int *contiguous_array;

static void init(void)
{
    contiguous_array = (int *)calloc(array_size, sizeof(int));
    for (int i = 0; i < array_size; i++) {
        contiguous_array[i] = i;
    }
}

static void clean(void)
{
    free(contiguous_array);
}

__attribute__((noinline))
static void workload(void)
{
    volatile int sum;
    for (int i = 0; i < array_size; i++) {
        sum += contiguous_array[i];
    }
}

workload_t wl_contiguous_array = {
    .id = WL_CONTIGUOUS_ARRAY,
    .name = "CONTIGUOUS_ARRAY",
    .init = init,
    .clean = clean,
    .workload = workload,
};
