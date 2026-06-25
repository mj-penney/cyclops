#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "./cyclops.h"
#include "./metric_grp.h"
#include "./workload.h"

///////////////////////////////////////////////////////////////////////////////
/// BATCH

typedef struct double_agg {
    double min;
    double max;
    double median;
} double_agg_t;

typedef struct metric_grp metric_grp_t;

typedef struct {
    const metric_t *metric;
    unsigned long long batch_runs;
    double *run_vals;
    double_agg_t agg;
} batch_metric_t;

typedef struct batch {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    metric_grp_t *mg;

    batch_metric_t raw_metric_scaling;

    int n_raw;
    batch_metric_t *raw_metrics;

    int n_derived;
    batch_metric_t *derived_metrics;

    bool to_csv;
    bool report;
} batch_t;

void batch_metric_set_run_val(batch_metric_t *batch_metric,
                              unsigned long long run,
                              double val);

void batch_single_run(cyclops_cfg_t *cyclops_cfg);

///////////////////////////////////////////////////////////////////////////////
/// PARAM SWEEP

typedef struct {
    unsigned long long param_sweep_val;
    double_agg_t agg;
} param_sweep_batch_val_t;

typedef struct {
    const metric_t *metric;
    param_sweep_batch_val_t *batch_vals;
} param_sweep_metric_t;

typedef struct {

    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    metric_grp_t *mg;

    char *wl_param_key;
    char *wl_param_low;
    char *wl_param_high;
    char *wl_param_step;
    bool geometric;

    unsigned long long n_batches;
    param_sweep_metric_t *metrics;

    bool to_csv;
} param_sweep_t;

void ps_run(cyclops_cfg_t *cyclops_cfg);

#endif
