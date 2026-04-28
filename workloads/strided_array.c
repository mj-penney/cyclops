#include <stdlib.h>

#include "../include/workload.h"

unsigned long long stride_bytes;
unsigned long long array_elements;

static int *array;
static unsigned long long *indices;

static void init_indices()
{
    indices = malloc(array_elements * sizeof(unsigned long long));
    for (unsigned long long i = 0; i < array_elements; i++) {
        indices[i] = i;
    }

    srand(42);
    for (unsigned long long i = array_elements - 1; i > 0; i--) {
        unsigned long long j = rand() % (i + 1);

        unsigned long long tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
}

static void init(workload_t *wl)
{
    stride_bytes = wl_get_param(wl, "stride-bytes");
    array_elements = wl_get_param(wl, "array-elements");

    init_indices();

    array = (int *)aligned_alloc(stride_bytes, stride_bytes * array_elements);
    for (unsigned long long i = 0; i < array_elements; i++) {
        array[i * (stride_bytes / sizeof(int))] = i;
    }
}

static void clean(void)
{
    free(array);
    free(indices);
}

__attribute__((noinline)) static void workload(void)
{
    volatile unsigned long long sum = 0;
    for (unsigned long long i = 0; i < array_elements; i++) {
        sum += array[indices[i] * (stride_bytes / sizeof(int))];
    }
}

static wl_param_t params[] = {
    {
        .key = "stride-bytes",
        .default_value = "64", // 1 element per cache line
    }, {
        .key = "array-elements",
        .default_value = "1000000",
    },
};

static workload_t wl = {
    .name = "STRIDED_ARRAY",

    .n_params = 2,
    .params = params,

    .init = init,
    .clean = clean,
    .workload = workload,
};

REGISTER_WORKLOAD(&wl)
