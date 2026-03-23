#ifndef BATCH_INTERNAL_H
#define BATCH_INTERNAL_H

uint64_t *alloc_uint64_array(unsigned long long length);
double *alloc_double_array(unsigned long long length);
void run_perf_batch(batch_conf_t batch_conf);
void run_timer_batch(batch_conf_t batch_conf);

#endif
