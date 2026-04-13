#ifndef BATCH_H
#define BATCH_H

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"
#include "./data_processing.h"

typedef struct metric_grp metric_grp_t;

typedef struct batch_conf {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    const metric_grp_t *mg;
} batch_conf_t;

typedef struct {
    metric_id_t metric_id;
    double *run_vals;
    double_agg_t agg;
} metric_data_t;

typedef struct batch_data {

    int n_raw;
    metric_data_t *raw_data;

    int n_derived;
    metric_data_t *derived_data;
} batch_data_t;

/*** TIMER ***/

void run_batch(unsigned long long warmup_runs,
               unsigned long long batch_runs,
               workload_t *wl,
               const metric_grp_t *mg);

#endif
