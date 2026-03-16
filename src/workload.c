#include <stdlib.h>
#include <stdio.h>

#include "../include/workload.h"

workload_t *all_workloads[N_WORKLOADS] = {
    [WL_CONTIGUOUS_ARRAY] = &wl_contiguous_array,
    [WL_SCATTERED_ARRAY] = &wl_scattered_array,
    [WL_STRIDED_ARRAY] = &wl_strided_array,
};
