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

#include "../../include/metric.h"
#include "./backend.h"
#include "./backend_common.h"

#if defined(__x86_64__) || defined(__amd64__)

static uint64_t rdtscp()
{
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}

static void bench_rdtscp(batch_conf_t *batch_cfg,
                 batch_data_t *batch_data,
                 void (*workload)(void))
{
    uint64_t start, end;

    pin_thread();

    for (unsigned long long i = 0; i < batch_cfg->warmup_runs; i++) {
        workload();
    }

    for (unsigned long long i = 0; i < batch_cfg->batch_runs; i++) {
        start = rdtscp();
        workload();
        end = rdtscp();
        batch_data->raw_data[0].run_vals[i] = (double)(end - start);
    }
}

#endif

void run_be(batch_conf_t *batch_cfg,
                            batch_data_t *batch_data,
                            void (*workload)(void))
{
    const metric_grp_t *mg = batch_cfg->mg;
    assert(mg != NULL);

    switch (mg->metrics[0]) {

        case METRIC_RDTSCP:
#if defined(__x86_64__) || defined(__i386__) || defined(__amd64__)
            bench_rdtscp(batch_cfg, batch_data, workload);
#else
            fprintf(stderr, "RDTSCP is x86-only\n");
            exit(1);
#endif
            break;

        case METRIC_ARM:
#if defined(__aarch64__) || defined(__arm__)
            fprintf(stderr, "Arm timer not implemented yet\n");
#else
            fprintf(stderr, "Arm counter is arm-only\n");
            exit(1);
#endif
            break;

        default:
            exit(1);
    }
}

static backend_t be = {
    .id = METRIC_BE_CPU_INSTRUCTION,
    .bench_func = run_be,
};

REGISTER_BACKEND(&be)
