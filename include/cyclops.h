#ifndef CYCLOPS_H
#define CYCLOPS_H

#include <stdbool.h>

#define MAX_BATCH_RUNS 100
#define MAX_PERF_COUNTERS 15
#define MAX_PERF_RATIOS 15

typedef struct {
    bool display_help;
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    char *wl_name;
    char *mg_name;
    int n_wl_params;
    char **wl_param_keys;
    char **wl_param_args;
    char *ps_wl_param_key;
    char *ps_wl_param_low;
    char *ps_wl_param_high;
    char *ps_wl_param_step;
    bool batch_csv;
    bool param_sweep_csv;
    bool report;
    bool geometric;
} cyclops_cfg_t;

#endif
