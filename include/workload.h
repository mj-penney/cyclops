#ifndef WORKLOAD_H
#define WORKLOAD_H

enum {
    WL_CONTIGUOUS_ARRAY,
    WL_SCATTERED_ARRAY,
    WL_STRIDED_ARRAY,
    N_WORKLOADS,
};

typedef struct workload {
    int id;
    const char* name;
    void (*init)(void);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

extern workload_t *all_workloads[N_WORKLOADS];

extern workload_t wl_contiguous_array;
extern workload_t wl_scattered_array;
extern workload_t wl_strided_array;

#endif
