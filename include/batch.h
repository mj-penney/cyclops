#ifndef BATCH_H
#define BATCH_H

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"

typedef struct batch_conf {
    int warmup_runs;
    int batch_runs;
    int workload_id;
    metric_grp_id_t metric_grp_id;
} batch_conf_t;

typedef struct batch_data {

    int n_counters;
    counter_metric_t counters[MAX_PERF_COUNTERS];

    int n_ratios;
    ratio_metric_t ratios[MAX_PERF_RATIOS];

    /*
     * Poor man's map - a sparse array which maps a counter id to an index of
     * the batch_data.counters[] array.
     * Unused array elements should be set to -1.
     *
     * Usage:
     *
     *     int counter_array_idx = counter_id_map[COUNTER_ID];
     *     counter_metric_t counter = counters[counter_array_idx];
     *     if (counter == -1) {
     *         return -1; // error
     *     }
     */
    int counter_id_map[NUMBER_OF_COUNTERS];
} batch_data_t;

int init_batch_conf(batch_conf_t *batch_conf, int warmup_runs,
                                              int batch_runs,
                                              int workload_id,
                                              metric_grp_id_t id);

void run_batch(batch_conf_t batch_conf);

#endif
