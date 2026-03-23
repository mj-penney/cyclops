#ifndef BATCH_H
#define BATCH_H

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"
#include "./data_processing.h"

typedef struct batch_conf {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    const metric_grp_t *mg;
} batch_conf_t;

typedef struct {
    const metric_t *metric;
    uint64_t *run_vals;
    uint64_agg_t agg;
} perf_counter_data_t;

typedef struct {
    const metric_t *metric;
    double *run_vals;
    double_agg_t agg;
} perf_ratio_data_t;

typedef struct {
    const metric_t *metric;
    uint64_t *run_vals;
    uint64_agg_t agg;
} timer_data_t;

// TODO: split into perf_batch_data and timer_batch_data
typedef struct batch_data {

    /* these two are always active for perf batches */
    perf_counter_data_t time_enabled;
    perf_counter_data_t time_running;

    int n_perf_counters;
    perf_counter_data_t *perf_counters;

    int n_perf_ratios;
    perf_ratio_data_t *perf_ratios;

    timer_data_t timer;
} batch_data_t;

void run_batch(unsigned long long warmup_runs,
               unsigned long long batch_runs,
               workload_t *wl,
               const metric_grp_t *mg);

#endif
