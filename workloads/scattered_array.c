#include <stdlib.h>

#include "../include/workload.h"

static const int array_size = 1000000;
static int **scattered_array;

static void init(workload_t *wl)
{
    (void)wl;

    scattered_array = (int **)calloc(array_size, sizeof(int *));
    for (int i = 0; i < array_size; i++) {
        scattered_array[i] = malloc(sizeof(int));
        *scattered_array[i] = i;
    }
}

static void clean(void)
{
    for (int i = 0; i < array_size; i++) {
        free(scattered_array[i]);
    }
    free(scattered_array);
}

__attribute__((noinline)) static void workload(void)
{
    volatile int sum;

    sum = 0;
    for (int i = 0; i < array_size; i++) {
        sum += *scattered_array[i];
    }
}

static workload_t wl = {
    .name = "SCATTERED_ARRAY",

    .init = init,
    .clean = clean,
    .workload = workload,
};

REGISTER_WORKLOAD(&wl)
