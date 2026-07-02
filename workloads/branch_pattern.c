#include <stdlib.h>
#include <string.h>

#include "../include/workload.h"

static unsigned long long pattern_len;
static unsigned long long n_branches;
static unsigned long long bias;

static char *array;

/*
 * Random number generator
 */
static unsigned long long xorshift64(unsigned long long seed)
{
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return seed;
}

static void pre_batch(workload_t *wl)
{
    pattern_len = wl_get_param_val(wl, "pattern-len");
    n_branches = wl_get_param_val(wl, "n-branches");
    bias = wl_get_param_val(wl, "bias");

    /* sanitise input */
    if (pattern_len > n_branches) {
        pattern_len = n_branches;
    }
    if (bias > 100) {
        bias = 100;
    }

    char *pattern = malloc(pattern_len * sizeof(char));
    array = malloc(n_branches * sizeof(char));

    unsigned long long random = 88172645463325252ull;
    for (unsigned long long i = 0; i < pattern_len; i++) {
        random = xorshift64(random);
        pattern[i] = ((random % 100) < bias) ? 1 : 0;
    }

    unsigned long long full_repetitions = n_branches / pattern_len;
    unsigned long long remainder = n_branches % pattern_len;

    for (unsigned long long i = 0; i < full_repetitions; i++) {
        memcpy(&array[i * pattern_len], pattern, pattern_len * sizeof(char));
    }

    memcpy(&array[full_repetitions * pattern_len], pattern,
                                                    remainder * sizeof(char));

    free(pattern);
}

static void post_batch(void)
{
    free(array);
}

__attribute__((noinline)) static void execute(void)
{
    volatile unsigned long long sum = 0;
    for (unsigned long long i = 0; i < n_branches; i++) {
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
        .key = "n-branches",
        .default_value = 1000,
    }, {
        .key = "bias",
        .default_value = 50, // range: 0 - 100 (50 is 'unbiased')
    }
};

static workload_t wl = {
    .name = "BRANCH_PATTERN",

    .n_params = 3,
    .params = params,

    .pre_batch = pre_batch,
    .post_batch = post_batch,
    .execute = execute,
};

REGISTER_WORKLOAD(&wl)
