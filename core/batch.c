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
                                              workload_t *wl,
                                              metric_grp_id_t id)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_RUNS) {
        batch_runs = 100;
    }

    if (warmup_runs < 0 ) {
        warmup_runs = 5;
    }

    batch_conf->warmup_runs   = warmup_runs;
    batch_conf->batch_runs    = batch_runs;
    batch_conf->wl            = wl;
    batch_conf->metric_grp_id = id;

    return 0;
}

static batch_data_t *init_batch_data(batch_conf_t batch_conf)
{
    batch_data_t *data;
    metric_grp_t metric_grp;
    raw_metric_t raw_metric;
    ratio_metric_t ratio;

    int runs = batch_conf.batch_runs;

    
    if (!(data = calloc(1, sizeof(batch_data_t)))) {
        perror("Failed to allocate memory for batch_data_t struct");
        exit(1);
    }

    metric_grp = metric_grps[batch_conf.metric_grp_id];

    data->n_raw_metrics = metric_grp.n_raw_metrics;
    data->n_ratios = metric_grp.n_ratios;

    /* allocate memory for the time_enabled run_vals */
    if (!(data->time_enabled.run_vals = calloc(runs, sizeof(uint64_t)))) {
        perror("Failed to allocate array");
        exit(1);
    }

    /* allocate memory for the time_running run_vals */
    if (!(data->time_running.run_vals = calloc(runs, sizeof(uint64_t)))) {
        perror("Failed to allocate array");
        exit(1);
    }

    /* init poor man's map elements to -1 */
    memset(data->raw_metric_id_map, -1, N_RAW_METRICS * sizeof(int));

    /* init raw metrics */
    for (int i = 0; i < metric_grp.n_raw_metrics; i++) {
        raw_metric.id = metric_grp.raw_metric_ids[i];
        if (!(raw_metric.run_vals = calloc(runs, sizeof(uint64_t)))) {
            perror("Failed to allocate array");
            exit(1);
        }
        data->raw_metrics[i] = raw_metric;
        data->raw_metric_id_map[metric_grp.raw_metric_ids[i]] = i;
    }

    /* init ratio metrics */
    for (int i = 0; i < metric_grp.n_ratios; i++) {
        ratio.id = metric_grp.ratio_ids[i];
        if (!(ratio.run_vals = calloc(runs, sizeof(double)))) {
            perror("Failed to allocate array");
            exit(1);
        }
        data->ratios[i] = ratio;
    }

    return data;
}

static int destroy_batch_data(batch_data_t *batch_data)
{
    free(batch_data->time_enabled.run_vals);
    batch_data->time_enabled.run_vals = NULL;

    free(batch_data->time_running.run_vals);
    batch_data->time_running.run_vals = NULL;

    for (int i = 0; i < batch_data->n_raw_metrics; i++) {
        free(batch_data->raw_metrics[i].run_vals);
        batch_data->raw_metrics[i].run_vals = NULL;
    }

    for (int i = 0; i < batch_data->n_ratios; i++) {
        free(batch_data->ratios[i].run_vals);
        batch_data->ratios[i].run_vals = NULL;
    }

    free(batch_data);
    batch_data = NULL;

    return 0;
}

static int process_batch_raw_metrics(batch_conf_t batch_conf,
                                     batch_data_t *batch_data)
{
    uint64_agg_t agg;
    int batch_runs;

    batch_runs = batch_conf.batch_runs;

    for (int i = 0; i < batch_data->n_raw_metrics; i++) {
        agg = aggregate_uint64(batch_data->raw_metrics[i].run_vals,
                                                                batch_runs);
        batch_data->raw_metrics[i].agg = agg;
    }

    return 0;
}

static int process_batch_ratios(batch_conf_t batch_conf,
                                 batch_data_t *batch_data)
{
    metric_grp_id_t metric_grp_id;
    ratio_id_t ratio_id;
    raw_metric_id_t numerator_id, denominator_id;
    double_agg_t agg;
    raw_metric_t *raw_metrics;
    ratio_metric_t *ratios;
    int numerator_idx, denominator_idx, batch_runs, *raw_metric_id_map;
    uint64_t *numerators, *denominators;

    raw_metric_id_map = batch_data->raw_metric_id_map;
    metric_grp_id = batch_conf.metric_grp_id;

    for (int i = 0; i < batch_data->n_ratios; i++) {

        ratio_id = metric_grps[metric_grp_id].ratio_ids[i];
        numerator_id = ratio_confs[ratio_id].numerator_id;
        denominator_id = ratio_confs[ratio_id].denominator_id;

        numerator_idx = raw_metric_id_map[numerator_id];
        denominator_idx = raw_metric_id_map[denominator_id];

        if (numerator_idx == -1 || denominator_idx == -1) {
            return -1;
        }

        raw_metrics = batch_data->raw_metrics;
        ratios = batch_data->ratios;

        numerators = raw_metrics[numerator_idx].run_vals;
        denominators = raw_metrics[denominator_idx].run_vals;

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
    if(process_batch_raw_metrics(batch_conf, batch_data) != 0) {
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
    workload_t *wl = batch_conf.wl;

    wl->init(wl);

    if (batch_conf.metric_grp_id == METRIC_GRP_TIMER) {
        bench_timer(batch_conf, batch_data, wl->workload);
    } else {
        bench_perf_event_open(batch_conf, batch_data, wl->workload);
    }

    wl->clean();

    process_batch_data(batch_conf, batch_data);

    if (batch_conf.metric_grp_id == METRIC_GRP_TIMER) {
        timer_batch_to_csv(batch_conf, batch_data);
    } else {
        perf_batch_to_csv(batch_conf, batch_data);
    }

    run_report(batch_conf, batch_data);

    destroy_batch_data(batch_data);
}
