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

    for (size_t i = 0; i < n_workloads; i++) {
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
int wl_get_param(workload_t *wl, const char *key)
{
    for (int i = 0; i < wl->n_params; i++) {
        if (strcmp(wl->params[i].key, key) == 0) {
            if (wl->params[i].arg) {
                return atoi(wl->params[i].arg);
            }
            return atoi(wl->params[i].default_value);
        };
    }

    fprintf(stderr, "Invalid workload parameter '%s'\n", key);
    exit(1);
    return 0;
}

void wl_set_param(workload_t *wl, const char *key, const char *arg)
{
    if (!wl->params) {
        fprintf(stderr, "Workload has no parameters\n");
        return;
    }

    for (int i = 0; i < wl->n_params; i++) {
        if (strcmp(wl->params[i].key, key) == 0) {
            if (wl->params[i].arg) {
                fprintf(stderr,
                        "Value for workload parameter %s already set\n", key);
            }
            wl->params[i].arg = arg;
            return;
        }
    }

    fprintf(stderr, "Invalid workload parameter '%s'\n", key);
}
