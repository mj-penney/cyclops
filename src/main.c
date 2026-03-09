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
"  -m, --metric-group GROUP         Select a group of metrics to record\n"
"\n";

static void print_workload_guide(void)
{
    printf("Workloads:\n\n");

    for (int i = 0; i < NUMBER_OF_WORKLOADS; i++) {
        printf("  %s\n", all_workloads[i]->name);
    }
    printf("\n");
}

static void print_metric_grp_guide(void)
{
    metric_grp_t metric_grp;
    counter_id_t counter_id;
    ratio_id_t ratio_id;

    printf("Metric groups:\n\n");

    for (int i = 0; i < NUMBER_OF_METRIC_GRPS; i++) {
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
            return;
        }

        printf("    Derived:  ");
        for (int j = 0; j < metric_grp.n_ratios; j++) {
            ratio_id = metric_grp.ratio_ids[j];
            printf("%s  ", ratio_confs[ratio_id].name);
        }
        printf("\n\n");
    }
}

int main(int argc, char *argv[])
{

    char *workload_str = NULL;
    char *metric_grp_str  = NULL;

    static struct option long_opts[] = {
        {"help",    required_argument, 0, 'h'},
        {"workload",    required_argument, 0, 'w'},
        {"metric-grp", required_argument, 0, 'm'},
        {0, 0, 0, 0}
    };

    int opt;

    while ((opt = getopt_long(argc, argv, "w:m:h", long_opts, NULL)) != -1) {
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
            default:
                fprintf(stderr, "Usage 1\n");
                return 1;
        }
    }

    int workload_id = -1;
    int metric_grp_id = -1;

    if (strcmp(workload_str, all_workloads[WL_CONTIGUOUS_ARRAY]->name) == 0) {
        workload_id = WL_CONTIGUOUS_ARRAY;

    } else if (strcmp(workload_str, all_workloads[WL_SCATTERED_ARRAY]->name) == 0) {
        workload_id = WL_SCATTERED_ARRAY;

    }

    if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_IPC].name) == 0) {
        metric_grp_id = METRIC_GRP_IPC;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_LLC_READS].name) == 0) {
        metric_grp_id = METRIC_GRP_LLC_READS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_L1D_READS].name) == 0) {
        metric_grp_id = METRIC_GRP_L1D_READS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_L1I_READS].name) == 0) {
        metric_grp_id = METRIC_GRP_L1I_READS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_DTLB_READS].name) == 0) {
        metric_grp_id = METRIC_GRP_DTLB_READS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_ITLB_READS].name) == 0) {
        metric_grp_id = METRIC_GRP_ITLB_READS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_BPU_READS].name) == 0) {
        metric_grp_id = METRIC_GRP_BPU_READS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_BRANCH].name) == 0) {
        metric_grp_id = METRIC_GRP_BRANCH;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_STALLED_CYCLES].name) == 0) {
        metric_grp_id = METRIC_GRP_STALLED_CYCLES;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_PAGE_FAULTS].name) == 0) {
        metric_grp_id = METRIC_GRP_PAGE_FAULTS;

    } else if (strcmp(metric_grp_str, metric_grps[METRIC_GRP_ALIGNMENT_FAULTS].name) == 0) {
        metric_grp_id = METRIC_GRP_ALIGNMENT_FAULTS;

    }

    if (workload_id < 0 || metric_grp_id < 0) {
        fprintf(stderr, "Usage 2\n");
        return 1;
    }

    int warmup_runs = 5;
    int batch_runs = MAX_BATCH_RUNS;

    batch_conf_t batch_conf;
    init_batch_conf(&batch_conf, warmup_runs, batch_runs, workload_id,
                                                          metric_grp_id);
    run_batch(batch_conf);

    return 0;
}
