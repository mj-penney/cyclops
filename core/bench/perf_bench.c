#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "../../include/bench.h"
#include "./internal.h"

static struct perf_event_attr create_perf_config(int metric)
{
    struct perf_event_attr pea;

    memset(&pea, 0, sizeof(struct perf_event_attr));

    switch (metric) {
        case METRIC_CPU_CYCLES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CPU_CYCLES;
            break;
        case METRIC_REF_CPU_CYCLES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_REF_CPU_CYCLES;
            break;
        case METRIC_INSTRUCTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_INSTRUCTIONS;
            break;
        case METRIC_LLC_READ_ACCESSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_LL
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case METRIC_LLC_READ_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_LL
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_L1D_READ_ACCESSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1D
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case METRIC_L1D_READ_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1D
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_L1I_READ_ACCESSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1I
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case METRIC_L1I_READ_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1I
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_DTLB_READ_ACCESSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_DTLB
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case METRIC_DTLB_READ_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_DTLB
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_ITLB_READ_ACCESSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_ITLB
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case METRIC_ITLB_READ_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_ITLB
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_BPU_READ_ACCESSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_BPU
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case METRIC_BPU_READ_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_BPU
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_BRANCH_INSTRUCTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
            break;
        case METRIC_BRANCH_MISPREDICTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_MISSES;
            break;
        case METRIC_STALLED_CYCLES_FRONTEND:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND;
            break;
        case METRIC_STALLED_CYCLES_BACKEND:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND;
            break;
        case METRIC_PAGE_FAULTS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_PAGE_FAULTS;
            break;
        case METRIC_PAGE_FAULTS_MAJ:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_PAGE_FAULTS_MAJ;
            break;
        case METRIC_PAGE_FAULTS_MIN:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_PAGE_FAULTS_MIN;
            break;
        case METRIC_CPU_CLOCK_NS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_CPU_CLOCK;
            break;
        case METRIC_TASK_CLOCK_NS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_TASK_CLOCK;
            break;
        case METRIC_ALIGNMENT_FAULTS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_ALIGNMENT_FAULTS;
            break;
        default:
            break;
    }
    pea.size = sizeof(struct perf_event_attr);
    pea.disabled = 0;
    //if (is_leader)
    //    pea.pinned = 1;
    pea.exclude_kernel = 1;
    pea.exclude_hv = 1;
    pea.exclude_idle = 1;
    pea.read_format = PERF_FORMAT_GROUP |
        PERF_FORMAT_ID |
        PERF_FORMAT_TOTAL_TIME_ENABLED |
        PERF_FORMAT_TOTAL_TIME_RUNNING;

    return pea;
}

static void open_perf_counters(struct perf_event_attr attrs[],
                               int counter_fds[],
                               uint64_t counter_ids[],
                               int event_group_size)
{
    counter_fds[0] = syscall(SYS_perf_event_open, &(attrs[0]), 0, -1, -1, 0);

    if (counter_fds[0] == -1) {
        perror("Failed to open perf counter group leader");
        exit(1);
    }

    ioctl(counter_fds[0], PERF_EVENT_IOC_ID, &counter_ids[0]);

    for (int evt_idx = 1; evt_idx < event_group_size; evt_idx++) {

        counter_fds[evt_idx] = syscall(SYS_perf_event_open,
                                &(attrs[evt_idx]), 0, -1, counter_fds[0], 0);

        if (counter_fds[evt_idx] == -1) {
            perror("Failed to open perf counters");
            exit(1);
        }

        ioctl(counter_fds[evt_idx], PERF_EVENT_IOC_ID, &counter_ids[evt_idx]);
    }
}

/* matches kernel abi */
typedef struct perf_result {
    uint64_t nr;
    uint64_t time_enabled;
    uint64_t time_running;
    struct {
        uint64_t value;
        uint64_t id;
    } values[MAX_PERF_COUNTERS];
} perf_result_t;

static perf_result_t *calc_run_delta(perf_result_t *start_result,
                                     perf_result_t *end_result)
{
    end_result->time_enabled -= start_result->time_enabled;
    end_result->time_running -= start_result->time_running;

    for (uint64_t i = 0; i < end_result->nr; i++) {
        end_result->values[i].value -= start_result->values[i].value;
    }

    return end_result;
}

static void store_perf_results(batch_data_t *batch_data,
                               perf_result_t perf_start_results[],
                               perf_result_t perf_end_results[],
                               uint64_t perf_ctr_ids[],
                               unsigned long long batch_runs)
{
    for (unsigned long long run = 0; run < batch_runs; run++) {

        /* verify that the kernel did not reorder the counters */
        for (uint64_t i = 0; i < perf_end_results[run].nr; i++) {
            assert(perf_start_results[run].values[i].id == perf_ctr_ids[i]);
            assert(perf_end_results[run].values[i].id == perf_ctr_ids[i]);
        }

        perf_result_t *pr = calc_run_delta(&perf_start_results[run],
                                           &perf_end_results[run]);

        batch_data->time_enabled.run_vals[run] = pr->time_enabled;
        batch_data->time_running.run_vals[run] = pr->time_running;

        if (pr->time_running == 0) {
            fprintf(stderr,
                    "Time running is 0 - the kernel was probably "
                    "unable to schedule the perf event counter group\n");
            exit(1);
        }

        if (pr->time_running != pr->time_enabled) {
            fprintf(stdout, "Multiplexing occured on run %llu\n", run);
        }

        double scaling = (double)pr->time_enabled / pr->time_running;

        for (unsigned int pr_idx = 0; pr_idx < pr->nr; pr_idx++) {
            double value = scaling * pr->values[pr_idx].value;
            batch_data->perf_counters[pr_idx].run_vals[run] = value;
        }
    }
}

int bench_perf_event_open(batch_conf_t *batch_cfg,
                          batch_data_t *batch_data,
                          void (*workload)(void))
{
    struct perf_event_attr attrs[MAX_PERF_COUNTERS];
    int                    perf_ctr_fds[MAX_PERF_COUNTERS];
    uint64_t               perf_ctr_ids[MAX_PERF_COUNTERS];

    perf_result_t *perf_start_results = calloc(batch_cfg->batch_runs,
                                               sizeof(perf_result_t));
    perf_result_t *perf_end_results = calloc(batch_cfg->batch_runs,
                                             sizeof(perf_result_t));
    if (!perf_start_results || !perf_end_results) {
        perror("Failed to allocate buffer for perf results");
        exit(1);
    }

    for (int i = 0; i < batch_data->n_perf_counters; i++) {
        int metric_id = batch_data->perf_counters[i].metric->id;
        attrs[i] = create_perf_config(metric_id);
    }

    pin_thread();

    open_perf_counters(attrs, perf_ctr_fds, perf_ctr_ids,
                                                batch_data->n_perf_counters);

    for (unsigned long long i = 0; i < batch_cfg->warmup_runs; i++) {
        workload();
    }

    /*
     * This is the main benchmark loop.
     * Keep it as clean and minimal as possible
     * to reduce noise.
     */
    for (unsigned long long run = 0; run < batch_cfg->batch_runs; run++) {

        read(perf_ctr_fds[0], &perf_start_results[run], sizeof(perf_result_t));

        asm volatile("" ::: "memory");
        workload();
        asm volatile("" ::: "memory");

        read(perf_ctr_fds[0], &perf_end_results[run], sizeof(perf_result_t));
    }

    for (int i = 0; i < batch_data->n_perf_counters; i++) {
        if (close(perf_ctr_fds[i]) == -1) {
            exit(1);
        }
    }

    store_perf_results(batch_data,
                       perf_start_results,
                       perf_end_results,
                       perf_ctr_ids,
                       batch_cfg->batch_runs);

    free(perf_start_results);
    free(perf_end_results);

    return 0;
}
