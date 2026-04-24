#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/batch.h"
#include "../include/metric.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

static batch_conf_t *init_batch_conf(unsigned long long warmup_runs,
                              unsigned long long batch_runs,
                              workload_t *wl,
                              const metric_grp_t *mg)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_RUNS) {
        batch_runs = 100;
    }

    batch_conf_t *cfg = calloc(1, sizeof(batch_conf_t));
    if (!cfg) {
        perror("Could not allocate memory for the batch config struct");
        exit(1);
    }

    cfg->warmup_runs   = warmup_runs;
    cfg->batch_runs    = batch_runs;
    cfg->wl            = wl;
    cfg->mg            = mg;

    return cfg;
}

static double *alloc_double_array(unsigned long long length)
{
    double *array = calloc(length, sizeof(double));
    if (!array) {
        perror("Failed to allocate double array");
        exit(1);
    }
    return array;
}

static batch_data_t *init_batch_data(batch_conf_t *cfg)
{
    batch_data_t *data;
    if (!(data = calloc(1, sizeof(batch_data_t)))) {
        perror("Failed to allocate memory for batch_data_t struct");
        exit(1);
    }

    const metric_grp_t *mg = cfg->mg;

    data->n_raw = mg_n_raw(mg);
    data->n_derived = mg_n_derived(mg);

    assert(data->n_raw > 0);

    if (!(data->raw_data = calloc(data->n_raw, sizeof(metric_data_t)))) {
        perror("Failed to allocate memory for perf counters");
        exit(1);
    }

    if (data->n_derived) {
        if (!(data->derived_data = calloc(data->n_derived,
                                          sizeof(metric_data_t)))) {
            perror("Failed to allocate memory for perf ratios");
            exit(1);
        }
    }

    data->raw_data_scaling.run_vals = alloc_double_array(cfg->batch_runs);

    for (int i = 0; i < data->n_raw; i++) {
        data->raw_data[i].run_vals = alloc_double_array(cfg->batch_runs);
        data->raw_data[i].metric_id = mg_get_nth_raw_id(mg, i);
    }

    for (int i = 0; i < data->n_derived; i++) {
        data->derived_data[i].run_vals = alloc_double_array(cfg->batch_runs);
        data->derived_data[i].metric_id = mg_get_nth_derived_id(mg, i);
    }

    return data;
}

static void destroy_batch_data(batch_data_t *data)
{
    for (int i = 0; i < data->n_raw; i++) {
        free(data->raw_data[i].run_vals);
        data->raw_data[i].run_vals = NULL;
    }

    for (int i = 0; i < data->n_derived; i++) {
        free(data->derived_data[i].run_vals);
        data->derived_data[i].run_vals = NULL;
    }

    free(data->raw_data_scaling.run_vals);
    data->raw_data_scaling.run_vals = NULL;

    free(data->raw_data);
    data->raw_data = NULL;

    free(data->derived_data);
    data->derived_data = NULL;

    free(data);
    data = NULL;
}

static void process_perf_counter_data(batch_conf_t *cfg,
                                      batch_data_t *batch_data)
{
    batch_data->raw_data_scaling.agg = aggregate_double(
                    batch_data->raw_data_scaling.run_vals, cfg->batch_runs);

    for (int i = 0; i < batch_data->n_raw; i++) {
        batch_data->raw_data[i].agg = aggregate_double(
                            batch_data->raw_data[i].run_vals, cfg->batch_runs);
    }
}

static void process_perf_ratio_data(batch_conf_t *cfg,
                                    batch_data_t *bd)
{
    for (int i = 0; i < bd->n_derived; i++) {

        metric_data_t *ratio = &bd->derived_data[i];
        const metric_t *m = get_metric_by_id(ratio->metric_id);

        double *numerators = NULL;
        double *denominators = NULL;

        for (int j = 0; j < bd->n_raw; j++) {
            if (numerators && denominators) {
                break;
            }

            if (bd->raw_data[j].metric_id == m->numerator) {
                numerators = bd->raw_data[j].run_vals;
            } else if (bd->raw_data[j].metric_id == m->denominator) {
                denominators = bd->raw_data[j].run_vals;
            }
        }

        calc_ratios(bd->derived_data[i].run_vals,
                    numerators,
                    denominators,
                    cfg->batch_runs);

        ratio->agg = aggregate_double(ratio->run_vals, cfg->batch_runs);
    }
}

void run_batch(unsigned long long warmup_runs,
               unsigned long long batch_runs,
               workload_t *wl,
               const metric_grp_t *mg,
               const char *output_file_name)
{
    batch_conf_t *cfg = init_batch_conf(warmup_runs, batch_runs, wl, mg);
    batch_data_t *batch_data = init_batch_data(cfg);
    const metric_backend_t *backend = get_backend(mg->backend);

    wl->init(wl);
    backend->bench_func(cfg, batch_data, wl->workload);
    wl->clean();

    process_perf_counter_data(cfg, batch_data);
    process_perf_ratio_data(cfg, batch_data);

    batch_to_csv(cfg, batch_data, output_file_name, true);
    run_report(cfg, batch_data);

    destroy_batch_data(batch_data);
    free(cfg);
}
