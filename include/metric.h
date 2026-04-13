#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"

/*** PERF ***/

typedef enum {

    /* counters */

    METRIC_CPU_CYCLES,
    METRIC_REF_CPU_CYCLES,
    METRIC_INSTRUCTIONS,
    METRIC_LLC_READ_ACCESSES,
    METRIC_LLC_READ_MISSES,
    METRIC_L1D_READ_ACCESSES,
    METRIC_L1D_READ_MISSES,
    METRIC_L1I_READ_ACCESSES,
    METRIC_L1I_READ_MISSES,
    METRIC_DTLB_READ_ACCESSES,
    METRIC_DTLB_READ_MISSES,
    METRIC_ITLB_READ_ACCESSES,
    METRIC_ITLB_READ_MISSES,
    METRIC_BPU_READ_ACCESSES,
    METRIC_BPU_READ_MISSES,
    METRIC_BRANCH_INSTRUCTIONS,
    METRIC_BRANCH_MISPREDICTIONS,
    METRIC_STALLED_CYCLES_FRONTEND,
    METRIC_STALLED_CYCLES_BACKEND,
    METRIC_PAGE_FAULTS,
    METRIC_PAGE_FAULTS_MAJ,
    METRIC_PAGE_FAULTS_MIN,
    METRIC_CPU_CLOCK_NS,
    METRIC_TASK_CLOCK_NS,
    METRIC_ALIGNMENT_FAULTS,

    METRIC_RDTSCP,
    METRIC_ARM,

    /* ratios */

    METRIC_INSTRUCTIONS_PER_CYCLE,
    METRIC_CYCLES_PER_INSTRUCTION,
    METRIC_LLC_READ_MISS_RATE,
    METRIC_L1D_READ_MISS_RATE,
    METRIC_L1I_READ_MISS_RATE,
    METRIC_DTLB_READ_MISS_RATE,
    METRIC_ITLB_READ_MISS_RATE,
    METRIC_BPU_READ_MISS_RATE,
    METRIC_BRANCH_MISPRED_RATE,
    METRIC_FE_VS_BE_STALLS,

    N_METRICS

} metric_id_t;

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

typedef struct {
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

#endif
