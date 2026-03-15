#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"
#include "./data_processing.h"

typedef enum {

    /* counter metrics */

    COUNTER_CPU_CYCLES,
    COUNTER_REF_CPU_CYCLES,
    COUNTER_INSTRUCTIONS,
    COUNTER_LLC_READ_ACCESSES,
    COUNTER_LLC_READ_MISSES,
    COUNTER_L1D_READ_ACCESSES,
    COUNTER_L1D_READ_MISSES,
    COUNTER_L1I_READ_ACCESSES,
    COUNTER_L1I_READ_MISSES,
    COUNTER_DTLB_READ_ACCESSES,
    COUNTER_DTLB_READ_MISSES,
    COUNTER_ITLB_READ_ACCESSES,
    COUNTER_ITLB_READ_MISSES,
    COUNTER_BPU_READ_ACCESSES,
    COUNTER_BPU_READ_MISSES,
    COUNTER_BRANCH_INSTRUCTIONS,
    COUNTER_BRANCH_MISPREDICTIONS,
    COUNTER_STALLED_CYCLES_FRONTEND,
    COUNTER_STALLED_CYCLES_BACKEND,
    COUNTER_PAGE_FAULTS,
    COUNTER_PAGE_FAULTS_MAJ,
    COUNTER_PAGE_FAULTS_MIN,
    COUNTER_CPU_CLOCK_NS,
    COUNTER_TASK_CLOCK_NS,
    COUNTER_ALIGNMENT_FAULTS,

    N_COUNTER_METRICS,

} counter_id_t;

extern const char *counter_confs[N_COUNTER_METRICS];

typedef enum {

    /* ratio metrics */

    RATIO_INSTRUCTIONS_PER_CYCLE,
    RATIO_CYCLES_PER_INSTRUCTION,
    RATIO_LLC_READ_MISS_RATE,
    RATIO_L1D_READ_MISS_RATE,
    RATIO_L1I_READ_MISS_RATE,
    RATIO_DTLB_READ_MISS_RATE,
    RATIO_ITLB_READ_MISS_RATE,
    RATIO_BPU_READ_MISS_RATE,
    RATIO_BRANCH_MISPRED_RATE,
    RATIO_FE_VS_BE_STALLS,

    N_RATIO_METRICS,

} ratio_id_t;

typedef struct ratio_conf {
    const char *name;
    counter_id_t numerator_id;
    counter_id_t denominator_id;
} ratio_conf_t;

extern const ratio_conf_t ratio_confs[N_RATIO_METRICS];

typedef enum {
    METRIC_GRP_IPC,
    METRIC_GRP_LLC_READS,
    METRIC_GRP_L1D_READS,
    METRIC_GRP_L1I_READS,
    METRIC_GRP_DTLB_READS,
    METRIC_GRP_ITLB_READS,
    METRIC_GRP_BPU_READS,
    METRIC_GRP_BRANCH,
    METRIC_GRP_STALLED_CYCLES,
    METRIC_GRP_PAGE_FAULTS,
    METRIC_GRP_ALIGNMENT_FAULTS,
    N_METRIC_GRPS,
} metric_grp_id_t;

typedef struct counter_metric {
    int id;
    uint64_t *run_vals;
    uint64_agg_t agg;
} counter_metric_t;

typedef struct ratio_metric {
    int id;
    double *run_vals;
    double_agg_t agg;
} ratio_metric_t;

typedef struct metric_grp {
    int id;
    const char *name;
    int n_counters;
    int n_ratios;
    counter_id_t counter_ids[MAX_ACTIVE_COUNTERS];
    ratio_id_t ratio_ids[MAX_ACTIVE_RATIOS];
} metric_grp_t;

extern const metric_grp_t metric_grps[N_METRIC_GRPS];

#endif
