#include <stdlib.h>

#include "../include/workload.h"

#define STRIDE_BYTES 64 // one element per cache line

static unsigned long long array_elements;
static unsigned long long repeats;
static unsigned long long array_size_kib;

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

static void pre_batch(workload_t *wl)
{
    array_size_kib = wl_get_param_val(wl, "array-size-kib");
    repeats = wl_get_param_val(wl, "repeats");
    array_elements = (array_size_kib << 10) / STRIDE_BYTES;

    init_indices();

    array = (int *)aligned_alloc(STRIDE_BYTES, STRIDE_BYTES * array_elements);

    /* Build random linked list */
    for (unsigned long long i = 0; i < array_elements - 1; i++) {
        unsigned long long curr = indices[i];
        unsigned long long next = indices[i + 1];

        array[curr * (STRIDE_BYTES / sizeof(unsigned int))] = next;
    }

    /* Close the loop */
    array[
        indices[array_elements - 1] *
        (STRIDE_BYTES / sizeof(unsigned int))
    ] = indices[0];

}

static void post_batch(void)
{
    free(array);
    free(indices);
}

__attribute__((noinline)) static void execute(void)
{
    unsigned int p = indices[0];

    for (unsigned long long rep = 0; rep < repeats; rep++) {
        for (unsigned long long i = 0; i < array_elements; i++) {
            p = array[p * (STRIDE_BYTES / sizeof(unsigned int))];
        }
    }

    asm volatile("" : "+r"(p));
}

static wl_param_t params[] = {
    {
        .key = "array-size-kib",
        .default_value = 1,
    }, {
        .key = "repeats",
        .default_value = 10000,
    },
};

static workload_t wl = {
    .name = "STRIDED_ARRAY",

    .n_params = 2,
    .params = params,

    .pre_batch = pre_batch,
    .post_batch = post_batch,
    .execute = execute,
};

REGISTER_WORKLOAD(&wl)
