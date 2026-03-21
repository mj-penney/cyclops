#ifndef WORKLOAD_H
#define WORKLOAD_H

typedef struct {
    const char *key;
    const char *arg;
    const char *default_value;
} wl_param_t;

typedef struct workload workload_t;
typedef struct workload {
    const char* name;

    const int n_params;
    wl_param_t *params;

    void (*init)(workload_t *wl);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

void register_workload(workload_t *wl);

#define REGISTER_WORKLOAD(wl_ptr) \
    static void __attribute((constructor)) register_wl(void) { \
        register_workload(wl_ptr); \
    }

void print_workload_guide(void);

workload_t *get_workload_by_name(const char *name);

int wl_get_param(workload_t *wl, const char *key);
void wl_set_param(workload_t *wl, const char *key, const char *arg);

#endif
