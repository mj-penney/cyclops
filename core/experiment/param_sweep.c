#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/experiment.h"
#include "../../include/metric_grp.h"
#include "../../include/workload.h"
#include "./internal.h"

static unsigned long long ps_n_batches_arithmetic(param_sweep_t *ps)
{
    unsigned long long low = strtoull(ps->wl_param_low, NULL, 10);
    unsigned long long high = strtoull(ps->wl_param_high, NULL, 10);
    unsigned long long step = strtoull(ps->wl_param_step, NULL, 10);

    unsigned long long diff = high - low;
    unsigned long long mod = diff % step;

    if (mod != 0) {
        return (diff / step) + 2;
    }
    return (diff / step) + 1;
}

/*
 * Calculates the number of batches needed to perform the parameter sweep.
 */
static unsigned long long ps_n_batches(param_sweep_t *ps)
{
    return ps_n_batches_arithmetic(ps);
}

static unsigned long long ps_get_nth_param_val_arithmetic(param_sweep_t *ps,
                                                          unsigned long long n)
{
    unsigned long long low = strtoull(ps->wl_param_low, NULL, 10);
    unsigned long long high = strtoull(ps->wl_param_high, NULL, 10);
    unsigned long long step = strtoull(ps->wl_param_step, NULL, 10);

    if (n == ps->n_batches - 1) {
        return high;
    }
    return low + (n * step);
}

/* n is zero-based */
static unsigned long long ps_get_nth_param_val(param_sweep_t *ps,
                                               unsigned long long n)
{
    return ps_get_nth_param_val_arithmetic(ps, n);
}

static param_sweep_t *ps_init(cyclops_cfg_t *cyclops_cfg)
{
    param_sweep_t *ps = NULL;
    if (!(ps = calloc(1, sizeof(param_sweep_t)))) {
        perror("Failed to allocate memory for param_sweep_t struct");
        exit(1);
    }

    metric_grp_t *mg = mg_get_by_name(cyclops_cfg->mg_name);
    workload_t *wl = wl_get_by_name(cyclops_cfg->wl_name);

    for (int i = 0; i < cyclops_cfg->n_wl_params; i++) {
        wl_set_param_val(wl,
                         cyclops_cfg->wl_param_keys[i],
                         cyclops_cfg->wl_param_args[i]);
    }

    ps->wl = wl;
    ps->mg = mg;
    ps->warmup_runs = cyclops_cfg->warmup_runs;
    ps->batch_runs = cyclops_cfg->batch_runs;
    ps->wl_param_key = cyclops_cfg->ps_wl_param_key;
    ps->wl_param_low = cyclops_cfg->ps_wl_param_low;
    ps->wl_param_high = cyclops_cfg->ps_wl_param_high;
    ps->wl_param_step = cyclops_cfg->ps_wl_param_step;
    ps->n_batches = ps_n_batches(ps);
    ps->to_csv = cyclops_cfg->param_sweep_csv;

    if (!(ps->metrics = calloc(mg->n_metrics, sizeof(param_sweep_metric_t)))) {
        perror("Failed to allocate memory for param_sweep_metric_t array");
        exit(1);
    }

    for (int i = 0; i < mg->n_metrics; i++) {
        ps->metrics[i].batch_vals = calloc(ps->n_batches,
                                           sizeof(param_sweep_batch_val_t));
        if (!ps->metrics[i].batch_vals) {
            perror("Failed to allocate memory for "
                                                "param_sweep_batch_val array");
            exit(1);
        }

        ps->metrics[i].metric = metric_get_by_id(mg->metrics[i]);
    }

    return ps;
}

static void ps_destroy(param_sweep_t *ps)
{
    for (int i = 0; i < ps->mg->n_metrics; i++) {
        free(ps->metrics[i].batch_vals);
        ps->metrics[i].batch_vals = NULL;
    }

    free(ps->metrics);
    ps->metrics = NULL;

    free(ps);
    ps = NULL;
}

static void ps_set_batch_vals(param_sweep_t *ps, batch_t *b,
                              unsigned long long batch_num,
                              unsigned long long param_val)
{
    param_sweep_metric_t *ps_metric;
    batch_metric_t *batch_metric;
    int metric_num;

    for (metric_num = 0; metric_num < ps->mg->n_metrics; metric_num++) {
        ps_metric = &ps->metrics[metric_num];
        batch_metric = batch_get_batch_metric_by_id(b,ps_metric->metric->id);

        assert(batch_metric);
        assert(batch_num < ps->n_batches);

        ps_metric->batch_vals[batch_num].agg = batch_metric->agg;
        ps_metric->batch_vals[batch_num].param_sweep_val = param_val;
    }
}

void ps_run(cyclops_cfg_t *cyclops_cfg)
{
    param_sweep_t *ps = ps_init(cyclops_cfg);

    static char param_val_buf[64];
    unsigned long long param_val;

    unsigned long long batch_num;
    for (batch_num = 0; batch_num < ps->n_batches; batch_num++) {

        batch_t *b = batch_init(cyclops_cfg);

        /* 
         * must set param to current sweep value after calling
         * batch_init(), not before
         */
        param_val = ps_get_nth_param_val(ps, batch_num);
        snprintf(param_val_buf, sizeof(param_val_buf), "%llu", param_val);
        wl_set_param_val(ps->wl, ps->wl_param_key, param_val_buf);

        batch_param_sweep_run(b, batch_num);

        ps_set_batch_vals(ps, b, batch_num, param_val);

        batch_destroy(b);
    }

    /* write to csv */
    ps_to_csv(ps);

    /* clean up */
    ps_destroy(ps);
}
