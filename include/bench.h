#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"
#include "./metric.h"

typedef void (*bench_func_t)(
    batch_conf_t *,
    batch_data_t *,
    void (*)(void)
);

typedef struct {
    metric_backend_t id;
    bench_func_t bench_func;
} backend_t;

/*
 * TODO: move thes two functions static to a specific backend.c file and
 * only accessable from a backend_t object.
 */
void run_cpu_instruction_be(batch_conf_t *batch_cfg,
                            batch_data_t *batch_data,
                            void (*workload)(void));
void run_perf_be(batch_conf_t *batch_cfg,
                 batch_data_t *batch_data,
                 void (*workload)(void));

const backend_t *get_backend(metric_backend_t id);

#endif
