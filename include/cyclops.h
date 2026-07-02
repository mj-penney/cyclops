#ifndef CYCLOPS_H
#define CYCLOPS_H

#include <stdbool.h>

#define MAX_BATCH_RUNS 100
#define MAX_PERF_COUNTERS 15
#define MAX_PERF_RATIOS 15
#define MAX_WL_ARGS 5

typedef struct {
    char *key;
    unsigned long long value;
} wl_arg_t;

typedef struct {
    bool display_help;
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    char *wl_name;
    char *mg_name;

    int n_wl_args;
    wl_arg_t wl_args[MAX_WL_ARGS];

    bool geometric;
    char *ps_wl_param_key;
    unsigned long long ps_wl_param_low;
    unsigned long long ps_wl_param_high;
    unsigned long long ps_wl_param_step;

    bool batch_csv;
    bool param_sweep_csv;
    bool report;
} cyclops_cfg_t;

#endif
