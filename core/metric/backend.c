#include <stdlib.h>
#include <stddef.h>

#include "../../include/metric.h"
#include "./backend.h"

static backend_t **backends = NULL;
static size_t n_backends = 0;

void register_backend(backend_t *be)
{
    backends = realloc(backends, (n_backends + 1) * sizeof(*backends));
    backends[n_backends++] = be;
}

backend_t *get_backend(metric_backend_t id)
{
    for (size_t i = 0; i < n_backends; i++) {
        if (backends[i]->id == id) {
            return backends[i];
        }
    }
    return NULL;
}
