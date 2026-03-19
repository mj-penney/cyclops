#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "../include/batch.h"
#include "../include/workload.h"
#include "../include/metric.h"

static const char help_text[] =
"Usage: ./microbench [OPTIONS]\n"
"\n"
"Options:\n\n"
"  -h, --help                       Display this message\n"
"  -w, --workload WORKLOAD          Select workload to benchmark\n"
"  -g, --metric-group GROUP         Select a group of metrics to record\n"
"  -r, --batch-runs RUNS            Number of runs in the batch\n"
"  -u, --warmup-runs RUNS           Number of warmup runs\n"
"\n";

static void print_metric_grp_guide(void)
{
    metric_grp_t metric_grp;
    counter_id_t counter_id;
    ratio_id_t ratio_id;

    printf("Metric groups:\n\n");

    for (int i = 0; i < N_METRIC_GRPS; i++) {
        metric_grp = metric_grps[i];
        printf("  %s:\n", metric_grp.name);

        printf("    Counters: ");
        for (int j = 0; j < metric_grp.n_counters; j++) {
            counter_id = metric_grp.counter_ids[j];
            printf("%s  ", counter_confs[counter_id]);
        }
        printf("\n");

        if (!metric_grp.n_ratios) {
            printf("\n");
            continue;
        }

        printf("    Derived:  ");
        for (int j = 0; j < metric_grp.n_ratios; j++) {
            ratio_id = metric_grp.ratio_ids[j];
            printf("%s  ", ratio_confs[ratio_id].name);
        }
        printf("\n\n");
    }
}

int get_metric_grp_id_from_name(const char *name)
{
    for (int i = 0; i < N_METRIC_GRPS; i++) {
        if (strcmp(name, metric_grps[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{

    char *workload_str = NULL;
    char *metric_grp_str  = NULL;
    int batch_runs = 0;
    int warmup_runs = 0;
    wl_arg_slice_t wl_args;

    memset(&wl_args, 0, sizeof(wl_arg_slice_t));

    static struct option long_opts[] = {
        {"help",    no_argument, 0, 'h'},
        {"workload",    required_argument, 0, 'w'},
        {"metric-grp", required_argument, 0, 'g'},
        {"batch-runs", required_argument, 0, 'r'},
        {"warmup-runs", required_argument, 0, 'u'},
        {"param", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int opt;

    while ((opt = getopt_long(argc, argv, "hw:g:r:u:p:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                fputs(help_text, stdout);
                print_workload_guide();
                print_metric_grp_guide();
                return 0;
            case 'w':
                workload_str = optarg;
                break;
            case 'g':
                metric_grp_str = optarg;
                break;
            case 'r':
                batch_runs = atoi(optarg);
                break;
            case 'u':
                warmup_runs = atoi(optarg);
                break;
            case 'p':
                if (wl_args.n_args >= MAX_WL_PARAMS) {
                    fprintf(stderr, "Too many workload params");
                    return 1;
                }

                char *arg = optarg;
                char *eq = strchr(arg, '=');

                if (!eq) {
                    fprintf(stderr, "Invalid param format: %s\n", arg);
                    return 1;
                }

                *eq = '\0'; // split into two strings

                wl_args.args[wl_args.n_args].key = arg;
                wl_args.args[wl_args.n_args].value = eq + 1;
                wl_args.n_args++;
                break;
            default:
                fprintf(stderr, "Usage 1\n");
                return 1;
        }
    }

    workload_t *wl = get_workload_by_name(workload_str);
    int metric_grp_id = get_metric_grp_id_from_name(metric_grp_str);

    if (!wl || metric_grp_id < 0) {
        fprintf(stderr, "Usage 2\n");
        return 1;
    }

    batch_conf_t batch_conf;
    init_batch_conf(&batch_conf, warmup_runs, batch_runs, wl, metric_grp_id);
    run_batch(batch_conf, &wl_args);

    return 0;
}
