#include <stdlib.h>
#include <string.h>

#include "../include/workload.h"

static unsigned long long pattern_len;
static unsigned long long pattern_reps;
static unsigned long long bias;

static char *array;

/*
 * Random number generator
 */
static unsigned long long xorshift64()
{
    static unsigned long long x = 88172645463325252ull;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

static void pre_batch(workload_t *wl)
{
    pattern_len = wl_get_param_val(wl, "pattern-len");
    pattern_reps = wl_get_param_val(wl, "pattern-reps");
    bias = wl_get_param_val(wl, "bias");

    /* sanitise input */
    if (bias > 100) {
        bias = 100;
    }

    char *pattern = malloc(pattern_len * sizeof(char));
    array = malloc(pattern_reps * pattern_len * sizeof(char));

    for (unsigned long long i = 0; i < pattern_len; i++) {
        unsigned long long random = xorshift64() % 100;
        pattern[i] = (random < bias) ? 1 : 0;
    }

    for (unsigned long long i = 0; i < pattern_reps; i++) {
        memcpy(&array[i * pattern_len], pattern, pattern_len * sizeof(char));
    }

    free(pattern);
}

static void post_batch(void)
{
    free(array);
}

__attribute__((noinline)) static void execute(void)
{
    volatile unsigned long long sum = 0;
    for (unsigned long long i = 0; i < pattern_reps * pattern_len; i++) {
        if (array[i] == 1) {
            sum += 1;
        } else {
            sum -= 1;
        }
    }
}

static wl_param_t params[] = {
    {
        .key = "pattern-len",
        .default_value = 16,
    }, {
        .key = "pattern-reps",
        .default_value = 1000,
    }, {
        .key = "bias",
        .default_value = 50, // range: 0 - 100 (50 is 'unbiased')
    }
};

static workload_t wl = {
    .name = "BRANCH_PATTERN_2",

    .n_params = 3,
    .params = params,

    .pre_batch = pre_batch,
    .post_batch = post_batch,
    .execute = execute,
};

REGISTER_WORKLOAD(&wl)
