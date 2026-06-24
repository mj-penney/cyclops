#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/experiment.h"
#include "../../include/metric_grp.h"
#include "../../include/workload.h"
#include "./internal.h"

static double *alloc_double_array(unsigned long long length)
{
    double *array = calloc(length, sizeof(double));
    if (!array) {
        perror("Failed to allocate double array");
        exit(1);
    }
    return array;
}

batch_t *batch_init(cyclops_cfg_t *cyclops_cfg)
{
    batch_t *b;
    if (!(b = calloc(1, sizeof(batch_t)))) {
        perror("Failed to allocate memory for batch struct");
        exit(1);
    }

    metric_grp_t *mg = mg_get_by_name(cyclops_cfg->mg_name);
    workload_t *wl = wl_get_by_name(cyclops_cfg->wl_name);

    for (int i = 0; i < cyclops_cfg->n_wl_params; i++) {
        wl_set_param_val(wl,
                         cyclops_cfg->wl_param_keys[i],
                         cyclops_cfg->wl_param_args[i]);
    }

    b->warmup_runs   = cyclops_cfg->warmup_runs;
    b->batch_runs    = cyclops_cfg->batch_runs;
    b->wl            = wl;
    b->mg            = mg;

    b->n_raw = mg_n_metrics_by_type(mg, METRIC_TYPE_RAW);
    b->n_derived = mg_n_metrics_by_type(mg, METRIC_TYPE_DERIVED);
    b->to_csv = cyclops_cfg->batch_csv;
    b->report = cyclops_cfg->report;

    assert(b->n_raw > 0);

    if (!(b->raw_metrics = calloc(b->n_raw, sizeof(batch_metric_t)))) {
        perror("Failed to allocate memory for perf counters");
        exit(1);
    }

    if (b->n_derived) {
        if (!(b->derived_metrics = calloc(b->n_derived,
                                          sizeof(batch_metric_t)))) {
            perror("Failed to allocate memory for perf ratios");
            exit(1);
        }
    }

    b->raw_metric_scaling.run_vals = alloc_double_array(b->batch_runs);

    for (int i = 0; i < b->n_raw; i++) {
        b->raw_metrics[i].batch_runs = b->batch_runs;
        b->raw_metrics[i].run_vals = alloc_double_array(b->batch_runs);
        b->raw_metrics[i].metric = mg_get_nth_metric_by_type(mg, i,
                                                            METRIC_TYPE_RAW);
    }

    for (int i = 0; i < b->n_derived; i++) {
        b->derived_metrics[i].batch_runs = b->batch_runs;
        b->derived_metrics[i].run_vals = alloc_double_array(b->batch_runs);
        b->derived_metrics[i].metric = mg_get_nth_metric_by_type(mg, i,
                                                        METRIC_TYPE_DERIVED);
    }

    return b;
}

void batch_destroy(batch_t *b)
{
    for (int i = 0; i < b->n_raw; i++) {
        free(b->raw_metrics[i].run_vals);
        b->raw_metrics[i].run_vals = NULL;
    }

    for (int i = 0; i < b->n_derived; i++) {
        free(b->derived_metrics[i].run_vals);
        b->derived_metrics[i].run_vals = NULL;
    }

    free(b->raw_metric_scaling.run_vals);
    b->raw_metric_scaling.run_vals = NULL;

    free(b->raw_metrics);
    b->raw_metrics = NULL;

    free(b->derived_metrics);
    b->derived_metrics = NULL;

    free(b);
    b = NULL;
}

void batch_metric_set_run_val(batch_metric_t *batch_metric,
                              unsigned long long run,
                              double val)
{
    assert(run < batch_metric->batch_runs);

    batch_metric->run_vals[run] = val;
}

batch_metric_t *batch_get_batch_metric_by_id(batch_t *b,
                                             metric_id_t metric_id)
{
    batch_metric_t *batch_metric;

    for (int i = 0; i < b->n_raw; i++) {
        batch_metric = &b->raw_metrics[i];

        if (batch_metric->metric->id == metric_id) {
            return batch_metric;
        }
    }

    for (int i = 0; i < b->n_derived; i++) {
        batch_metric = &b->derived_metrics[i];

        if (batch_metric->metric->id == metric_id) {
            return batch_metric;;
        }
    }

    return NULL;
}

static void batch_aggregate_raw_metrics(batch_t *b)
{
    batch_metric_t *batch_metric = &b->raw_metric_scaling;

    batch_metric->agg = aggregate_double(batch_metric->run_vals,
                                         b->batch_runs);

    for (int i = 0; i < b->n_raw; i++) {
        batch_metric = &b->raw_metrics[i];

        batch_metric->agg = aggregate_double(batch_metric->run_vals,
                                             b->batch_runs);
    }
}

static void batch_process_derived_metrics(batch_t *b)
{
    batch_metric_t *ratio, *numerator, *denominator;

    for (int i = 0; i < b->n_derived; i++) {

        ratio = &b->derived_metrics[i];
        const metric_t *m = ratio->metric;

        numerator = batch_get_batch_metric_by_id(b, m->numerator);
        denominator = batch_get_batch_metric_by_id(b, m->denominator);

        assert(numerator != NULL);
        assert(denominator != NULL);

        calc_ratios(ratio->run_vals,
                    numerator->run_vals,
                    denominator->run_vals,
                    b->batch_runs);

        ratio->agg = aggregate_double(ratio->run_vals, b->batch_runs);
    }
}

static void batch_run(batch_t *b, unsigned long long batch_no)
{
    const metric_backend_t *backend = metric_backend_get(b->mg->backend);

    b->wl->pre_batch(b->wl);
    backend->bench_func(b, b->wl->execute);
    b->wl->post_batch();

    batch_aggregate_raw_metrics(b);
    batch_process_derived_metrics(b);

    if (b->to_csv) {
        batch_to_csv(b, batch_no);
    }

    if (b->report) {
        run_report(b);
    }
}

void batch_single_run(cyclops_cfg_t *cyclops_cfg)
{
    batch_t *b = batch_init(cyclops_cfg);
    batch_run(b, 0);
    batch_destroy(b);
}

void batch_param_sweep_run(batch_t *b, unsigned long long batch_no)
{
    batch_run(b, batch_no);
}
