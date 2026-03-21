#ifndef BATCH_H
#define BATCH_H

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"

typedef struct batch_conf {
    int warmup_runs;
    int batch_runs;
    workload_t *wl;
    metric_grp_id_t metric_grp_id;
} batch_conf_t;

typedef struct batch_data {

    /* these two are always active for perf_event batches */
    raw_metric_t time_enabled;
    raw_metric_t time_running;

    int n_raw_metrics;
    raw_metric_t raw_metrics[MAX_ACTIVE_PERF_COUNTERS];

    int n_ratios;
    ratio_metric_t ratios[MAX_ACTIVE_RATIOS];

    /*
     * Poor man's map - a sparse array which maps a raw metric id to an index
     * of the batch_data.raw_metrics[] array.
     * Unused array elements should be set to -1.
     *
     * Usage:
     *
     *     int raw_metric_array_idx = raw_metric_id_map[RAW_METRIC_ID];
     *     raw_metric_t raw_metric = raw_metrics[raw_metric_array_idx];
     *     if (raw_metric == -1) {
     *         return -1; // error
     *     }
     */
    int raw_metric_id_map[N_RAW_METRICS];
} batch_data_t;

int init_batch_conf(batch_conf_t *batch_conf, int warmup_runs,
                                              int batch_runs,
                                              workload_t *wl,
                                              metric_grp_id_t id);

void run_batch(batch_conf_t batch_conf);

#endif
