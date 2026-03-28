#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"
#include "./metric.h"

int bench_timer(batch_conf_t *batch_cfg,
                batch_data_t *batch_data,
                void (*workload)(void));

int bench_perf_event_open(batch_conf_t *batch_cfg,
                          batch_data_t *batch_data,
                          void (*workload)(void));

#endif
