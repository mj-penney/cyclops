#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"

typedef enum {

    /* perf metrics */

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

    /* timer metric */

    METRIC_TIMER,

    /* ratio metrics */

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

    N_METRICS,

} metric_id_t;

enum {
    METRIC_TYPE_PERF_COUNTER,
    METRIC_TYPE_PERF_RATIO,
    METRIC_TYPE_TIMER,
};

typedef struct {
    metric_id_t id;
    const char *name;
    int type;
    metric_id_t numerator_id;
    metric_id_t denominator_id;
} metric_t;

enum {
    METRIC_GRP_TYPE_PERF,
    METRIC_GRP_TYPE_TIMER,
};

typedef struct metric_grp {
    const char *name;
    int type;

    int n_metrics;
    const metric_t *const *metrics;
} metric_grp_t;

enum {
    METRIC_GRP_TEST,
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
    METRIC_GRP_TIMER,

    N_METRIC_GRPS,
};

const metric_grp_t *get_mg_by_name(const char *name);
void mg_list_metrics_by_type(const metric_grp_t *mg,
                             int type,
                             int max_metrics,
                             const metric_t *metrics_buff[],
                             int *n_metrics);
void print_metric_grp_guide(void);

#endif
