#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"
#include "./metric.h"

typedef void (*bench_func_t)(
    batch_conf_t *,
    batch_data_t *,
    void (*)(void)
);

typedef struct {
    metric_backend_t id;
    bench_func_t bench_func;
} backend_t;

void register_backend(backend_t *backend);

#define REGISTER_BACKEND(be_ptr) \
    static void __attribute((constructor)) register_be(void) { \
        register_backend(be_ptr); \
    }

backend_t *get_backend(metric_backend_t id);

#endif
