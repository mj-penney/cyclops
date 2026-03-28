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

uint64_t *alloc_uint64_array(unsigned long long length)
{
    uint64_t *array = calloc(length, sizeof(uint64_t));
    if (!array) {
        perror("Failed to allocate uint64 array");
        exit(1);
    }
    return array;
}

double *alloc_double_array(unsigned long long length)
{
    double *array = calloc(length, sizeof(double));
    if (!array) {
        perror("Failed to allocate double array");
        exit(1);
    }
    return array;
}

void run_batch(unsigned long long warmup_runs,
               unsigned long long batch_runs,
               workload_t *wl,
               const metric_grp_t *mg)
{
    batch_conf_t *cfg = init_batch_conf(warmup_runs, batch_runs, wl, mg);

    // TODO: pass batch config pointer rather than passing by value
    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        run_timer_batch(cfg);
    } else {
        run_perf_batch(cfg);
    }

    free(cfg);
}
