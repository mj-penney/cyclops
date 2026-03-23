#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/metric.h"
#include "../include/data_processing.h"

const metric_t metrics[N_METRICS] = {
    [METRIC_CPU_CYCLES] = {
        .id = METRIC_CPU_CYCLES,
        .name = "CPU_CYCLES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_REF_CPU_CYCLES] = {
        .id = METRIC_REF_CPU_CYCLES,
        .name = "REF_CPU_CYCLES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_INSTRUCTIONS] = {
        .id = METRIC_INSTRUCTIONS,
        .name = "INSTRUCTIONS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_LLC_READ_ACCESSES] = {
        .id = METRIC_LLC_READ_ACCESSES,
        .name = "LLC_READ_ACCESSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_LLC_READ_MISSES] = {
        .id = METRIC_LLC_READ_MISSES,
        "LLC_READ_MISSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_L1D_READ_ACCESSES] = {
        .id = METRIC_L1D_READ_ACCESSES,
        "L1D_READ_ACCESSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_L1D_READ_MISSES] = {
        .id = METRIC_L1D_READ_MISSES,
        "L1D_READ_MISSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_L1I_READ_ACCESSES] = {
        .id = METRIC_L1I_READ_ACCESSES,
        "L1I_READ_ACCESSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_L1I_READ_MISSES] = {
        .id = METRIC_L1I_READ_MISSES,
        "L1I_READ_MISSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_DTLB_READ_ACCESSES] = {
        .id = METRIC_DTLB_READ_ACCESSES,
        "DTLB_READ_ACCESSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_DTLB_READ_MISSES] = {
        .id = METRIC_DTLB_READ_MISSES,
        "DTLB_READ_MISSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_ITLB_READ_ACCESSES] = {
        .id = METRIC_ITLB_READ_ACCESSES,
        "ITLB_READ_ACCESSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_ITLB_READ_MISSES] = {
        .id = METRIC_ITLB_READ_MISSES,
        "ITLB_READ_MISSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_BPU_READ_ACCESSES] = {
        .id = METRIC_BPU_READ_ACCESSES,
        "BPU_READ_ACCESSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_BPU_READ_MISSES] = {
        .id = METRIC_BPU_READ_MISSES,
        "BPU_READ_MISSES",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_BRANCH_INSTRUCTIONS] = {
        .id = METRIC_BRANCH_INSTRUCTIONS,
        "BRANCH_INSTRUCTIONS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_BRANCH_MISPREDICTIONS] = {
        .id = METRIC_BRANCH_MISPREDICTIONS,
        "BRANCH_MISPREDICTIONS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_STALLED_CYCLES_FRONTEND] = {
        .id = METRIC_STALLED_CYCLES_FRONTEND,
        "STALLED_CYCLES_FRONTEND",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_STALLED_CYCLES_BACKEND]  = {
        .id = METRIC_STALLED_CYCLES_BACKEND,
        "STALLED_CYCLES_BACKEND",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_PAGE_FAULTS] = {
        .id = METRIC_PAGE_FAULTS,
        "PAGE_FAULTS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_PAGE_FAULTS_MAJ] = {
        .id = METRIC_PAGE_FAULTS_MAJ,
        "PAGE_FAULTS_MAJ",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_PAGE_FAULTS_MIN] = {
        .id = METRIC_PAGE_FAULTS_MIN,
        "PAGE_FAULTS_MIN",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_CPU_CLOCK_NS] = {
        .id = METRIC_CPU_CLOCK_NS,
        "CPU_CLOCK_NS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_TASK_CLOCK_NS] = {
        .id = METRIC_TASK_CLOCK_NS,
        "TASK_CLOCK_NS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },
    [METRIC_ALIGNMENT_FAULTS] = {
        .id = METRIC_ALIGNMENT_FAULTS,
        "ALIGNMENT_FAULTS",
        .type = METRIC_TYPE_PERF_COUNTER,
    },

    [METRIC_TIMER] = {
        .id = METRIC_TIMER,
        "TIMER",
        .type = METRIC_TYPE_TIMER,
    },

    [METRIC_INSTRUCTIONS_PER_CYCLE] = {
        .id = METRIC_INSTRUCTIONS_PER_CYCLE,
        .name = "IPC",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_INSTRUCTIONS,
        .denominator_id = METRIC_CPU_CYCLES,
    },
    [METRIC_CYCLES_PER_INSTRUCTION] = {
        .id = METRIC_CYCLES_PER_INSTRUCTION,
        .name = "CPI",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_CPU_CYCLES,
        .denominator_id = METRIC_INSTRUCTIONS,
    },
    [METRIC_LLC_READ_MISS_RATE] = {
        .id = METRIC_LLC_READ_MISS_RATE,
        .name = "LLC_READ_MISS_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_LLC_READ_MISSES,
        .denominator_id = METRIC_LLC_READ_ACCESSES,
    },
    [METRIC_L1D_READ_MISS_RATE] = {
        .id = METRIC_L1D_READ_MISS_RATE,
        .name = "L1D_READ_MISS_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_L1D_READ_MISSES,
        .denominator_id = METRIC_L1D_READ_ACCESSES,
    },
    [METRIC_L1I_READ_MISS_RATE] = {
        .id = METRIC_L1I_READ_MISS_RATE,
        .name = "L1I_READ_MISS_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_L1I_READ_MISSES,
        .denominator_id = METRIC_L1I_READ_ACCESSES,
    },
    [METRIC_DTLB_READ_MISS_RATE] = {
        .id = METRIC_DTLB_READ_MISS_RATE,
        .name = "DTLB_READ_MISS_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_DTLB_READ_MISSES,
        .denominator_id = METRIC_DTLB_READ_ACCESSES,
    },
    [METRIC_ITLB_READ_MISS_RATE] = {
        .id = METRIC_ITLB_READ_MISS_RATE,
        .name = "ITLB_READ_MISS_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_ITLB_READ_MISSES,
        .denominator_id = METRIC_ITLB_READ_ACCESSES,
    },
    [METRIC_BPU_READ_MISS_RATE] = {
        .id = METRIC_BPU_READ_MISS_RATE,
        .name = "BPU_READ_MISS_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_BPU_READ_MISSES,
        .denominator_id = METRIC_BPU_READ_ACCESSES,
    },
    [METRIC_BRANCH_MISPRED_RATE] = {
        .id = METRIC_BRANCH_MISPRED_RATE,
        .name = "BRANCH_MISPRED_RATE",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_BRANCH_MISPREDICTIONS,
        .denominator_id = METRIC_BRANCH_INSTRUCTIONS,
    },
    [METRIC_FE_VS_BE_STALLS] = {
        .id = METRIC_FE_VS_BE_STALLS,
        .name = "FE_VS_BE_STALLS",
        .type = METRIC_TYPE_PERF_RATIO,
        .numerator_id = METRIC_STALLED_CYCLES_FRONTEND,
        .denominator_id = METRIC_STALLED_CYCLES_BACKEND,
    },
};

const metric_grp_t metric_grps[N_METRIC_GRPS] = {

    {
        .name = "TEST",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 11,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_REF_CPU_CYCLES],
            &metrics[METRIC_INSTRUCTIONS],
            &metrics[METRIC_LLC_READ_ACCESSES],
            &metrics[METRIC_LLC_READ_MISSES],
            &metrics[METRIC_L1D_READ_ACCESSES],
            &metrics[METRIC_L1D_READ_MISSES],

            &metrics[METRIC_INSTRUCTIONS_PER_CYCLE],
            &metrics[METRIC_CYCLES_PER_INSTRUCTION],
            &metrics[METRIC_LLC_READ_MISS_RATE],
            &metrics[METRIC_L1D_READ_MISS_RATE],
        },
    },

    {
        .name = "IPC",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 5,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_REF_CPU_CYCLES],
            &metrics[METRIC_INSTRUCTIONS],

            &metrics[METRIC_INSTRUCTIONS_PER_CYCLE],
            &metrics[METRIC_CYCLES_PER_INSTRUCTION],
        },
    },

    {
        .name = "LLC_READS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_LLC_READ_ACCESSES],
            &metrics[METRIC_LLC_READ_MISSES],

            &metrics[METRIC_LLC_READ_MISS_RATE],
        },
    },

    {
        .name = "L1D_READS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_L1D_READ_ACCESSES],
            &metrics[METRIC_L1D_READ_MISSES],

            &metrics[METRIC_L1D_READ_MISS_RATE],
        },
    },

    {
        .name = "L1I_READS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_L1I_READ_ACCESSES],
            &metrics[METRIC_L1I_READ_MISSES],

            &metrics[METRIC_L1I_READ_MISS_RATE],
        },
    },

    {
        .name = "DTLB_READS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_DTLB_READ_ACCESSES],
            &metrics[METRIC_DTLB_READ_MISSES],

            &metrics[METRIC_DTLB_READ_MISS_RATE],
        },
    },

    {
        .name = "ITLB_READS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_ITLB_READ_ACCESSES],
            &metrics[METRIC_ITLB_READ_MISSES],

            &metrics[METRIC_ITLB_READ_MISS_RATE],
        },
    },

    {
        .name = "BPU_READS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_BPU_READ_ACCESSES],
            &metrics[METRIC_BPU_READ_MISSES],

            &metrics[METRIC_BPU_READ_MISS_RATE],
        },
    },

    {
        .name = "BRANCH",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_BRANCH_INSTRUCTIONS],
            &metrics[METRIC_BRANCH_MISPREDICTIONS],

            &metrics[METRIC_BRANCH_MISPRED_RATE],
        },
    },

    {
        .name = "STALLED_CYCLES",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 4,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_STALLED_CYCLES_FRONTEND],
            &metrics[METRIC_STALLED_CYCLES_BACKEND],

            &metrics[METRIC_FE_VS_BE_STALLS],
        },
    },

    {
        .name = "PAGE_FAULTS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 3,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_PAGE_FAULTS],
            &metrics[METRIC_PAGE_FAULTS_MAJ],
            &metrics[METRIC_PAGE_FAULTS_MIN],
        },
    },

    {
        .name = "ALIGNMENT_FAULTS",
        .type = METRIC_GRP_TYPE_PERF,
        .n_metrics = 2,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_CPU_CYCLES],
            &metrics[METRIC_ALIGNMENT_FAULTS],
        },
    },

    {
        .name = "TIMER",
        .type = METRIC_GRP_TYPE_TIMER,
        .n_metrics = 1,
        .metrics = (const metric_t *const[]){
            &metrics[METRIC_TIMER],
        },
    },

};

const metric_grp_t *get_mg_by_name(const char *name)
{
    for (int i = 0; i < N_METRIC_GRPS; i++) {
        if (strcmp(name, metric_grps[i].name) == 0) {
            return &metric_grps[i];
        }
    }

    fprintf(stderr, "No metric group with name '%s'\n", name);
    return NULL;
}

void mg_list_metrics_by_type(const metric_grp_t *mg,
                             int type,
                             int max_metrics,
                             const metric_t *metrics_buff[],
                             int *n_metrics)
{
    *n_metrics = 0;

    for (int i = 0; i < mg->n_metrics && *n_metrics < max_metrics; i++) {
        if (mg->metrics[i]->type == type) {
            metrics_buff[*n_metrics] = mg->metrics[i];
            (*n_metrics)++;
        }
    }
}

void print_metric_grp_guide(void)
{
    metric_grp_t metric_grp;

    printf("Metric groups:\n\n");

    for (int i = 0; i < N_METRIC_GRPS; i++) {
        metric_grp = metric_grps[i];
        printf("  %s:\n", metric_grp.name);

        if (metric_grp.type == METRIC_GRP_TYPE_TIMER) {
            printf("    Raw:  %s\n\n", metric_grp.metrics[0]->name);
            continue;
        }

        int n_perf_counters;
        int n_perf_ratios;
        const metric_t *counter_metric_buff[MAX_PERF_COUNTERS];
        const metric_t *ratio_metric_buff[MAX_PERF_RATIOS];

        mg_list_metrics_by_type(&metric_grp, METRIC_TYPE_PERF_COUNTER,
            MAX_PERF_COUNTERS, counter_metric_buff, &n_perf_counters);
        mg_list_metrics_by_type(&metric_grp, METRIC_TYPE_PERF_RATIO,
                MAX_PERF_RATIOS, ratio_metric_buff, &n_perf_ratios);

        printf("    Raw:  ");
        for (int j = 0; j < n_perf_counters; j++) {
            printf("%s  ", counter_metric_buff[j]->name);
        }
        printf("\n");

        printf("    Derived:  ");
        for (int j = 0; j < n_perf_ratios; j++) {
            printf("%s  ", ratio_metric_buff[j]->name);
        }
        printf("\n\n");
    }
}
