#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "../include/batch.h"
#include "../include/workload.h"
#include "../include/metric.h"

static const char help_text[] =
"Usage: ./cyclops [OPTIONS]\n"
"\n"
"Options:\n\n"
"  -h, --help                       Display this message\n"
"  -w, --workload WORKLOAD          Select workload to benchmark\n"
"  -g, --metric-group GROUP         Select a group of metrics to record\n"
"  -r, --batch-runs RUNS            Number of runs in the batch\n"
"  -u, --warmup-runs RUNS           Number of warmup runs\n"
"\n";

#define MAX_WL_ARGS 5

int main(int argc, char *argv[])
{
    char *workload_str = NULL;
    char *metric_grp_str  = NULL;
    unsigned long long batch_runs = 0;
    unsigned long long warmup_runs = 0;
    int n_wl_params = 0;
    char *wl_param_keys[MAX_WL_ARGS];
    char *wl_param_args[MAX_WL_ARGS];

    static struct option long_opts[] = {
        {"help", no_argument, 0, 'h'},
        {"workload", required_argument, 0, 'w'},
        {"metric-grp", required_argument, 0, 'm'},
        {"batch-runs", required_argument, 0, 'r'},
        {"warmup-runs", required_argument, 0, 'u'},
        {"param", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int opt;
    char *key;
    char *eq;
    while ((opt = getopt_long(argc, argv, "hw:m:r:u:p:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                fputs(help_text, stdout);
                print_workload_guide();
                print_metric_grp_guide();
                return 0;
            case 'w':
                workload_str = optarg;
                break;
            case 'm':
                metric_grp_str = optarg;
                break;
            case 'r':
                batch_runs = strtoull(optarg, NULL, 10);
                break;
            case 'u':
                warmup_runs = strtoull(optarg, NULL, 10);
                break;
            case 'p':
                if (n_wl_params >= MAX_WL_ARGS) {
                    fprintf(stderr, "Too many workload param args\n");
                    return 1;
                }

                key = optarg;
                eq = strchr(key, '=');

                if (!eq) {
                    fprintf(stderr, "Invalid param format: %s\n", key);
                    return 1;
                }
                *eq = '\0'; // split into two strings
                wl_param_keys[n_wl_params] = key;
                wl_param_args[n_wl_params] = eq + 1;
                n_wl_params++;
                break;
            default:
                fprintf(stderr, "Usage 1\n");
                return 1;
        }
    }

    workload_t *wl = get_workload_by_name(workload_str);
    const metric_grp_t *mg = get_mg_by_name(metric_grp_str);

    if (!wl) {
        fprintf(stderr, "Usage: Please select a workload\n");
        return 1;
    } else if (!mg) {
        fprintf(stderr, "Usage: Please select a metric group\n");
        return 1;
    }

    for (int i = 0; i < n_wl_params; i++) {
        wl_set_param(wl, wl_param_keys[i], wl_param_args[i]);
    }

    run_batch(warmup_runs, batch_runs, wl, mg);

    return 0;
}
