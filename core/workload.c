#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/workload.h"

static workload_t **workloads = NULL;
static size_t n_workloads = 0;

void register_workload(workload_t *wl)
{
    workloads = realloc(workloads, (n_workloads + 1) * sizeof(*workloads));
    workloads[n_workloads++] = wl;
}

void print_workload_guide(void)
{
    printf("Workloads:\n\n");

    for (int i = 0; i < n_workloads; i++) {
        printf("  %s\n", workloads[i]->name);
    }
    printf("\n");
}

workload_t *get_workload_by_name(const char *name)
{
    for (size_t i = 0; i < n_workloads; i++) {
        if (strcmp(name, workloads[i]->name) == 0) {
            return workloads[i];
        }
    }
    return NULL;
}

/*
 * Returns the workload parameter value given a parameter key.
 *
 * If no arg is provided for this param by the user (with --param key=value),
 * use the param's default value.
 */
int get_wl_param_val(workload_t *wl, wl_arg_slice_t *wl_args, const char *key)
{
    const int n_params = wl->n_params;
    const wl_param_t *params = wl->params;

    for (int i = 0; i < wl_args->n_args; i++) {
        if (strcmp(key, wl_args->args[i].key) == 0) {
            return atoi(wl_args->args[i].value);
        }
    }

    for (int i = 0; i < n_params; i++) {
        if (strcmp(key, params[i].key) == 0) {
            return atoi(params[i].default_value);
        };
    }

    fprintf(stderr, "Invalid workload param key '%s'\n", key);
    exit(1);
    return 0;
}
