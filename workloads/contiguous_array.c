#include <stdlib.h>

#include "../include/workload.h"

static const int array_size = 1000;
static int *contiguous_array;

static void init(workload_t *wl, wl_arg_slice_t *wl_args)
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

static workload_t wl = {
    .name = "CONTIGUOUS_ARRAY",
    .init = init,
    .clean = clean,
    .workload = workload,
};

REGISTER_WORKLOAD(&wl);
