#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/batch.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

int init_batch_conf(batch_conf_t *batch_conf, int warmup_runs,
                                                            int batch_runs,
                                                            int workload_id,
                                                            metric_grp_id_t id)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_RUNS)
        return -1;

    batch_conf->warmup_runs   = warmup_runs;
    batch_conf->batch_runs    = batch_runs;
    batch_conf->workload_id   = workload_id;
    batch_conf->metric_grp_id = id;

    return 0;
}

static batch_data_t *init_batch_data(batch_conf_t batch_conf)
{
    metric_grp_t metric_grp;
    counter_metric_t counter;
    ratio_metric_t ratio;
    uint64_t *uint64_ptr;
    double *double_ptr;

    batch_data_t *batch_data = calloc(1, sizeof(batch_data_t));

    metric_grp = metric_grps[batch_conf.metric_grp_id];

    batch_data->n_counters = metric_grp.n_counters;
    batch_data->n_ratios = metric_grp.n_ratios;

    if ((uint64_ptr = calloc(MAX_BATCH_RUNS, sizeof(uint64_t))) == 0) {
        perror("Failed to allocate array");
        exit(1);
    } else {
        batch_data->time_enabled.run_vals = uint64_ptr;
    }

    if ((uint64_ptr = calloc(MAX_BATCH_RUNS, sizeof(uint64_t))) == 0) {
        perror("Failed to allocate array");
        exit(1);
    } else {
        batch_data->time_running.run_vals = uint64_ptr;
    }

    for (int i = 0; i < N_COUNTER_METRICS; i++) {
        batch_data->counter_id_map[i] = -1;
    }

    for (int i = 0; i < metric_grp.n_counters; i++) {
        counter.id = metric_grp.counter_ids[i];
        if ((uint64_ptr = calloc(MAX_BATCH_RUNS, sizeof(uint64_t))) == 0) {
            perror("Failed to allocate array");
            exit(1);
        } else {
            counter.run_vals = uint64_ptr;
        }
        batch_data->counters[i] = counter;
        batch_data->counter_id_map[metric_grp.counter_ids[i]] = i;
    }

    for (int i = 0; i < metric_grp.n_ratios; i++) {
        ratio.id = metric_grp.ratio_ids[i];
        if ((double_ptr = calloc(MAX_BATCH_RUNS, sizeof(double_ptr))) == 0) {
            perror("Failed to allocate array");
            exit(1);
        } else {
            ratio.run_vals = double_ptr;
        }
        batch_data->ratios[i] = ratio;
    }

    return batch_data;
}

static int destroy_batch_data(batch_data_t *batch_data)
{
    free(batch_data->time_enabled.run_vals);
    batch_data->time_enabled.run_vals = NULL;

    free(batch_data->time_running.run_vals);
    batch_data->time_running.run_vals = NULL;

    for (int i = 0; i < batch_data->n_counters; i++) {
        free(batch_data->counters[i].run_vals);
        batch_data->counters[i].run_vals = NULL;
    }

    for (int i = 0; i < batch_data->n_ratios; i++) {
        free(batch_data->ratios[i].run_vals);
        batch_data->ratios[i].run_vals = NULL;
    }

    free(batch_data);
    batch_data = NULL;

    return 0;
}

static int process_batch_ctrs(batch_conf_t batch_conf,
                               batch_data_t *batch_data)
{
    uint64_agg_t agg;
    int batch_runs;

    batch_runs = batch_conf.batch_runs;

    for (int i = 0; i < batch_data->n_counters; i++) {
        agg = aggregate_uint64(batch_data->counters[i].run_vals, batch_runs);
        batch_data->counters[i].agg = agg;
    }

    return 0;
}

static int process_batch_ratios(batch_conf_t batch_conf,
                                 batch_data_t *batch_data)
{
    metric_grp_id_t metric_grp_id;
    ratio_id_t ratio_id;
    counter_id_t numerator_id, denominator_id;
    double_agg_t agg;
    counter_metric_t *counters;
    ratio_metric_t *ratios;
    int numerator_idx, denominator_idx, batch_runs, *counter_id_map;
    uint64_t *numerators, *denominators;

    counter_id_map = batch_data->counter_id_map;
    metric_grp_id = batch_conf.metric_grp_id;

    for (int i = 0; i < batch_data->n_ratios; i++) {

        ratio_id = metric_grps[metric_grp_id].ratio_ids[i];
        numerator_id = ratio_confs[ratio_id].numerator_id;
        denominator_id = ratio_confs[ratio_id].denominator_id;

        numerator_idx = counter_id_map[numerator_id];
        denominator_idx = counter_id_map[denominator_id];

        if (numerator_idx == -1 || denominator_idx == -1) {
            return -1;
        }

        counters = batch_data->counters;
        ratios = batch_data->ratios;

        numerators = counters[numerator_idx].run_vals;
        denominators = counters[denominator_idx].run_vals;

        batch_runs = batch_conf.batch_runs;
        calc_ratios(ratios[i].run_vals, numerators, denominators, batch_runs);

        agg = aggregate_double(ratios[i].run_vals, batch_runs);
        ratios[i].agg = agg;
    }

    return 0;
}

static int process_batch_data(batch_conf_t batch_conf,
                              batch_data_t *batch_data)
{
    if(process_batch_ctrs(batch_conf, batch_data) != 0) {
        return -1;
    }

    if(process_batch_ratios(batch_conf, batch_data) != 0) {
        return -1;
    }

    return 0;
}

void run_batch(batch_conf_t batch_conf)
{
    batch_data_t *batch_data = (batch_data_t*)init_batch_data(batch_conf);

    workload_t workload = *all_workloads[batch_conf.workload_id];

    workload.init();
    bench_perf_event(batch_conf, batch_data, workload.workload);
    workload.clean();

    process_batch_data(batch_conf, batch_data);

    run_report(batch_conf, batch_data);

    destroy_batch_data(batch_data);
}
