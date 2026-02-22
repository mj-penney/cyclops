#ifndef WORKLOAD_H
#define WORKLOAD_H

enum {
    WL_CONTIGUOUS_ARRAY,
    WL_SCATTERED_ARRAY,
    NUMBER_OF_WORKLOADS,
};

typedef struct workload {
    int id;
    const char* name;
    void (*init)(void);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

extern const workload_t all_workloads[NUMBER_OF_WORKLOADS];

#endif
