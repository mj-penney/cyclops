#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/batch.h"
#include "../../include/bench.h"
#include "../../include/workload.h"
#include "../../include/data_processing.h"
#include "../../include/report.h"

#include "./internal.h"

static batch_data_t *init_timer_batch_data(batch_conf_t *cfg)
{
    batch_data_t *data = calloc(1, sizeof(batch_data_t));
    if (!data) {
        perror("Failed to allocate memory for batch data struct");
        exit(1);
    }

    data->timer.run_vals = alloc_uint64_array(cfg->batch_runs);
    data->timer.metric = cfg->mg->metrics[0];

    return data;
}

static void destroy_timer_batch_data(batch_data_t *batch_data)
{
    free(batch_data->timer.run_vals);
    batch_data->timer.run_vals = NULL;

    free(batch_data);
    batch_data = NULL;
}

static void process_timer_batch(batch_conf_t *cfg,
                         batch_data_t *batch_data)
{
    uint64_agg_t agg;
    agg = aggregate_uint64(batch_data->timer.run_vals, cfg->batch_runs);

    batch_data->timer.agg = agg;
}

void run_timer_batch(batch_conf_t *cfg)
{

    workload_t *wl = cfg->wl;
    batch_data_t *batch_data = init_timer_batch_data(cfg);

    // TODO: move this to a function in the bench subsystem
    wl->init(wl);
    bench_timer(cfg, batch_data, wl->workload);
    wl->clean();

    process_timer_batch(cfg, batch_data);

    timer_batch_to_csv(cfg, batch_data);
    // TODO: implement run_timer_report(batch_conf, batch_data);

    destroy_timer_batch_data(batch_data);
}
