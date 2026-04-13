#include "../../include/bench.h"

static const backend_t backends[N_METRIC_BACKENDS] = {
    [METRIC_BE_PERF] = {
        .id = METRIC_BE_PERF,
        .bench_func = run_perf_be,
    },
    [METRIC_BE_CPU_INSTRUCTION] = {
        .id = METRIC_BE_CPU_INSTRUCTION,
        .bench_func = run_cpu_instruction_be,
    },
};

const backend_t *get_backend(metric_backend_t id)
{
    return &backends[id];
}
