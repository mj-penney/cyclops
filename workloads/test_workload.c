#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/workload.h"

typedef struct {
    unsigned long long array_len;
    unsigned long long *array;
} wl_state_t;

static wl_state_t wl_state = { 0 };

static void pre_batch(workload_t *wl)
{
    wl_state.array_len = wl_get_param_val(wl, "array-len");
    wl_state.array = malloc(wl_state.array_len * sizeof(unsigned long long));

    for (unsigned long long i = 0; i < wl_state.array_len; i++) {
        wl_state.array[i] = i;
    }
}

static void post_batch(void)
{
    free(wl_state.array);
}


#if defined(__x86_64__) || defined(__amd64__)

extern void test_workload_execute_x86_64(wl_state_t *wl_state);

__attribute__((noinline))
static void execute(void)
{
    test_workload_execute_x86_64(&wl_state);
}

#elif defined(__aarch64__) || defined(__arm__)

extern void test_workload_execute_aarch64(wl_state_t *wl_state);

__attribute__((noinline))
static void execute(void)
{
    test_workload_execute_aarch64(&wl_state);
}

#else

__attribute__((noinline))
static void execute(void)
{
    for (unsigned long long i = 0; i < wl_state.array_len; i++) {
        printf("%llu\n", wl_state.array[i]);
    }
}

#endif

static wl_param_t params[] = {
    {
        .key = "array-len",
        .default_value = 5,
    },
};

static workload_t wl = {
    .name = "TEST_ASM",

    .n_params = 1,
    .params = params,

    .pre_batch = pre_batch,
    .post_batch = post_batch,
    .execute = execute,
};

REGISTER_WORKLOAD(&wl)
