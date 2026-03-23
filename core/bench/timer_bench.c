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

static uint64_t rdtscp()
{
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}

/*
 * This is just a placeholder for now.
 * Might make this function usable at some point.
 */
int bench_timer(batch_conf_t batch_conf, batch_data_t *batch_data,
                                          void (*workload)(void))
{
    uint64_t start, end;

    pin_thread();

    for (unsigned long long i = 0; i < batch_conf.warmup_runs; i++) {
        workload();
    }

    for (unsigned long long i = 0; i < batch_conf.batch_runs; i++) {
        start = rdtscp();
        workload();
        end = rdtscp();
        batch_data->timer.run_vals[i] = end - start;
    }

    return 0;
}

