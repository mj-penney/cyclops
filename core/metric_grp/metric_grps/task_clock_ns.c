#include "../../../include/metric_grp.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_INSTRUCTIONS,
    METRIC_TASK_CLOCK_NS,
};

static metric_grp_t mg = {
    .name = "TASK_CLOCK_NS",
    .backend = METRIC_BE_PERF,
    .n_metrics = 3,
    .metrics = metrics,
};

REGISTER_MG(&mg)
