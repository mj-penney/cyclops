#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"
#include "./batch.h"

typedef enum {
    METRIC_TYPE_RAW,
    METRIC_TYPE_DERIVED,
} metric_type_t;

typedef enum {
    METRIC_BE_PERF,
    METRIC_BE_CPU_INSTRUCTION,
    N_METRIC_BACKENDS,
} metric_backend_t;

typedef struct {
    const char *name;
    metric_type_t type;
    metric_backend_t backend;
    metric_id_t numerator;
    metric_id_t denominator;
    /* TODO: add supported architectures */
} metric_t;

/*** METRIC GROUPS ***/

typedef struct metric_grp {
    const char *name;
    metric_backend_t backend;
    int n_metrics;
    metric_id_t *metrics;
} metric_grp_t;

const metric_t *get_metric_by_id(metric_id_t id);
metric_id_t mg_get_nth_raw_id(const metric_grp_t *mg, int n);
metric_id_t mg_get_nth_derived_id(const metric_grp_t *mg, int n);

const metric_grp_t *get_mg_by_name(const char *name);

int mg_n_raw(const metric_grp_t *mg);
int mg_n_derived(const metric_grp_t *mg);

void print_metric_grp_guide(void);

/** METRIC BACKEND ***/

typedef struct batch_conf batch_conf_t;
typedef struct batch_data batch_data_t;

typedef void (*bench_func_t)(
    batch_conf_t *,
    batch_data_t *,
    void (*)(void)
);

typedef struct {
    metric_backend_t id;
    bench_func_t bench_func;
} backend_t;

backend_t *get_backend(metric_backend_t id);

#endif
