#ifndef WORKLOAD_H
#define WORKLOAD_H

#include <stdbool.h>

#include "./registry.h"

typedef struct {
    const char *key;
    bool arg_provided;
    unsigned long long arg;
    unsigned long long default_value;
} wl_param_t;

typedef struct workload workload_t;
typedef struct workload {
    const char* name;

    const int n_params;
    wl_param_t *params;

    void (*pre_batch)(workload_t *wl);
    void (*post_batch)(void);
    void (*execute)(void);
} workload_t;

#define REGISTER_WORKLOAD(wl_ptr) \
    static void __attribute((constructor)) _registry_register_object(void) { \
        registry_register_object((void *)wl_ptr, REG_ID_WORKLOAD); \
    }

void print_workload_guide(void);

workload_t *wl_get_by_name(const char *name);

unsigned long long wl_param_get_val(wl_param_t *wl_param);
unsigned long long wl_get_param_val(workload_t *wl, const char *key);
void wl_set_param_val(workload_t *wl, const char *key, unsigned long long arg);

#endif
