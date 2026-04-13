#ifndef METRIC_BACKEND_H
#define METRIC_BACKEND_H

#include "../../include/metric.h"

void register_backend(backend_t *backend);

#define REGISTER_BACKEND(be_ptr) \
    static void __attribute((constructor)) register_be(void) { \
        register_backend(be_ptr); \
    }

#endif
