#include <string.h>
#include <stdlib.h>

#include "../include/metric.h"
#include "../include/data_processing.h"

const char *counter_confs[N_COUNTER_METRICS] = {

    /* counter metrics */

    [COUNTER_CPU_CYCLES]              = "CPU_CYCLES",
    [COUNTER_REF_CPU_CYCLES]          = "REF_CPU_CYCLES",
    [COUNTER_INSTRUCTIONS]            = "INSTRUCTIONS",
    [COUNTER_LLC_READ_ACCESSES]       = "LLC_READ_ACCESSES",
    [COUNTER_LLC_READ_MISSES]         = "LLC_READ_MISSES",
    [COUNTER_L1D_READ_ACCESSES]       = "L1D_READ_ACCESSES",
    [COUNTER_L1D_READ_MISSES]         = "L1D_READ_MISSES",
    [COUNTER_L1I_READ_ACCESSES]       = "L1I_READ_ACCESSES",
    [COUNTER_L1I_READ_MISSES]         = "L1I_READ_MISSES",
    [COUNTER_DTLB_READ_ACCESSES]      = "DTLB_READ_ACCESSES",
    [COUNTER_DTLB_READ_MISSES]        = "DTLB_READ_MISSES",
    [COUNTER_ITLB_READ_ACCESSES]      = "ITLB_READ_ACCESSES",
    [COUNTER_ITLB_READ_MISSES]        = "ITLB_READ_MISSES",
    [COUNTER_BPU_READ_ACCESSES]       = "BPU_READ_ACCESSES",
    [COUNTER_BPU_READ_MISSES]         = "BPU_READ_MISSES",
    [COUNTER_BRANCH_INSTRUCTIONS]     = "BRANCH_INSTRUCTIONS",
    [COUNTER_BRANCH_MISPREDICTIONS]   = "BRANCH_MISPREDICTIONS",
    [COUNTER_STALLED_CYCLES_FRONTEND] = "STALLED_CYCLES_FRONTEND",
    [COUNTER_STALLED_CYCLES_BACKEND]  = "STALLED_CYCLES_BACKEND",
    [COUNTER_PAGE_FAULTS]             = "PAGE_FAULTS",
    [COUNTER_PAGE_FAULTS_MAJ]         = "PAGE_FAULTS_MAJ",
    [COUNTER_PAGE_FAULTS_MIN]         = "PAGE_FAULTS_MIN",
    [COUNTER_CPU_CLOCK_NS]            = "CPU_CLOCK_NS",
    [COUNTER_TASK_CLOCK_NS]           = "TASK_CLOCK_NS",
    [COUNTER_ALIGNMENT_FAULTS]        = "ALIGNMENT_FAULTS",

    [COUNTER_RDTSCP]                  = "RDTSCP",

};

const ratio_conf_t ratio_confs[N_RATIO_METRICS] = {
    [RATIO_INSTRUCTIONS_PER_CYCLE] = {
        .name = "IPC",
        .numerator_id = COUNTER_INSTRUCTIONS,
        .denominator_id = COUNTER_CPU_CYCLES,
    },
    [RATIO_CYCLES_PER_INSTRUCTION] = {
        .name = "CPI",
        .numerator_id = COUNTER_CPU_CYCLES,
        .denominator_id = COUNTER_INSTRUCTIONS,
    },
    [RATIO_LLC_READ_MISS_RATE] = {
        .name = "LLC_READ_MISS_RATE",
        .numerator_id = COUNTER_LLC_READ_MISSES,
        .denominator_id = COUNTER_LLC_READ_ACCESSES,
    },
    [RATIO_L1D_READ_MISS_RATE] = {
        .name = "L1D_READ_MISS_RATE",
        .numerator_id = COUNTER_L1D_READ_MISSES,
        .denominator_id = COUNTER_L1D_READ_ACCESSES,
    },
    [RATIO_L1I_READ_MISS_RATE] = {
        .name = "L1I_READ_MISS_RATE",
        .numerator_id = COUNTER_L1I_READ_MISSES,
        .denominator_id = COUNTER_L1I_READ_ACCESSES,
    },
    [RATIO_DTLB_READ_MISS_RATE] = {
        .name = "DTLB_READ_MISS_RATE",
        .numerator_id = COUNTER_DTLB_READ_MISSES,
        .denominator_id = COUNTER_DTLB_READ_ACCESSES,
    },
    [RATIO_ITLB_READ_MISS_RATE] = {
        .name = "ITLB_READ_MISS_RATE",
        .numerator_id = COUNTER_ITLB_READ_MISSES,
        .denominator_id = COUNTER_ITLB_READ_ACCESSES,
    },
    [RATIO_BPU_READ_MISS_RATE] = {
        .name = "BPU_READ_MISS_RATE",
        .numerator_id = COUNTER_BPU_READ_MISSES,
        .denominator_id = COUNTER_BPU_READ_ACCESSES,
    },
    [RATIO_BRANCH_MISPRED_RATE] = {
        .name = "BRANCH_MISPRED_RATE",
        .numerator_id = COUNTER_BRANCH_MISPREDICTIONS,
        .denominator_id = COUNTER_BRANCH_INSTRUCTIONS,
    },
    [RATIO_FE_VS_BE_STALLS] = {
        .name = "FE_VS_BE_STALLS",
        .numerator_id = COUNTER_STALLED_CYCLES_FRONTEND,
        .denominator_id = COUNTER_STALLED_CYCLES_BACKEND,
    },
};

const metric_grp_t metric_grps[N_METRIC_GRPS] = {

    [METRIC_GRP_IPC] = {
        .id = METRIC_GRP_IPC,
        .name = "IPC",
        .n_counters = 3,
        .n_ratios = 2,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_REF_CPU_CYCLES,
            COUNTER_INSTRUCTIONS,
        },
        .ratio_ids = {
            RATIO_INSTRUCTIONS_PER_CYCLE,
            RATIO_CYCLES_PER_INSTRUCTION,
        },
    },

    [METRIC_GRP_LLC_READS] = {
        .id = METRIC_GRP_LLC_READS,
        .name = "LLC_READS",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_LLC_READ_ACCESSES,
            COUNTER_LLC_READ_MISSES,
        },
        .ratio_ids = {
            RATIO_LLC_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_L1D_READS] = {
        .id = METRIC_GRP_L1D_READS,
        .name = "L1D_READS",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_L1D_READ_ACCESSES,
            COUNTER_L1D_READ_MISSES,
        },
        .ratio_ids = {
            RATIO_L1D_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_L1I_READS] = {
        .id = METRIC_GRP_L1I_READS,
        .name = "L1I_READS",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_L1I_READ_ACCESSES,
            COUNTER_L1I_READ_MISSES,
        },
        .ratio_ids = {
            RATIO_L1I_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_DTLB_READS] = {
        .id = METRIC_GRP_DTLB_READS,
        .name = "DTLB_READS",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_DTLB_READ_ACCESSES,
            COUNTER_DTLB_READ_MISSES,
        },
        .ratio_ids = {
            RATIO_DTLB_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_ITLB_READS] = {
        .id = METRIC_GRP_ITLB_READS,
        .name = "ITLB_READS",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_ITLB_READ_ACCESSES,
            COUNTER_ITLB_READ_MISSES,
        },
        .ratio_ids = {
            RATIO_ITLB_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_BPU_READS] = {
        .id = METRIC_GRP_BPU_READS,
        .name = "BPU_READS",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_BPU_READ_ACCESSES,
            COUNTER_BPU_READ_MISSES,
        },
        .ratio_ids = {
            RATIO_BPU_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_BRANCH] = {
        .id = METRIC_GRP_BRANCH,
        .name = "BRANCH",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_BRANCH_INSTRUCTIONS,
            COUNTER_BRANCH_MISPREDICTIONS,
        },
        .ratio_ids = {
            RATIO_BRANCH_MISPRED_RATE,
        },
    },

    [METRIC_GRP_STALLED_CYCLES] = {
        .id = METRIC_GRP_STALLED_CYCLES,
        .name = "STALLED_CYCLES",
        .n_counters = 3,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_STALLED_CYCLES_FRONTEND,
            COUNTER_STALLED_CYCLES_BACKEND,
        },
        .ratio_ids = {
            RATIO_FE_VS_BE_STALLS,
        },
    },

    [METRIC_GRP_PAGE_FAULTS] = {
        .id = METRIC_GRP_PAGE_FAULTS,
        .name = "PAGE_FAULTS",
        .n_counters = 3,
        .n_ratios = 0,
        .counter_ids = {
            COUNTER_PAGE_FAULTS,
            COUNTER_PAGE_FAULTS_MAJ,
            COUNTER_PAGE_FAULTS_MIN,
        },
        //.ratio_ids = {
        //},
    },

    [METRIC_GRP_ALIGNMENT_FAULTS] = {
        .id = METRIC_GRP_ALIGNMENT_FAULTS,
        .name = "ALIGNMENT_FAULTS",
        .n_counters = 2,
        .n_ratios = 0,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_ALIGNMENT_FAULTS,
        },
        //.ratio_ids = {
        //},
    },

    [METRIC_GRP_RDTSCP] = {
        .id = METRIC_GRP_RDTSCP,
        .name = "RDTSCP",
        .n_counters = 1,
        .n_ratios = 0,
        .counter_ids = {
            COUNTER_RDTSCP,
        },
        //.ratio_ids = {
        //},
    },

};
