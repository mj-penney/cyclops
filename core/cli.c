#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "../include/cli.h"
#include "../include/cyclops.h"

enum {
    OPT_CSV = 256,
    OPT_CSV_ALL = 257,
    OPT_REPORT = 258,
    OPT_GEOMETRIC = 259,
};

static struct option long_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"workload", required_argument, 0, 'w'},
    {"metric-grp", required_argument, 0, 'm'},
    {"batch-runs", required_argument, 0, 'r'},
    {"warmup-runs", required_argument, 0, 'u'},
    {"param", required_argument, 0, 'p'},
    {"param-sweep", required_argument, 0, 's'},

    /* long opts only */
    {"csv", no_argument, 0, OPT_CSV},
    {"csv-all", no_argument, 0, OPT_CSV_ALL},
    {"report", no_argument, 0, OPT_REPORT},
    {"geometric", no_argument, 0, OPT_GEOMETRIC},

    {0, 0, 0, 0}
};

int cli_cfg_parse_wl_arg(cyclops_cfg_t *cfg, char *_optarg)
{
    int len  = strlen(optarg);
    char *p0, *p1;

    if (cfg->n_wl_args >= MAX_WL_ARGS) {
        fprintf(stderr, "Too many workload param args\n");
        return -1;
    }

    /* parse the wl param key */

    p0 = _optarg;
    if (!(p1 = strchr(_optarg, '='))) {
        return -1;
    }
    cfg->wl_args[cfg->n_wl_args].key = strndup(p0, p1 - p0);

    /* parse the arg value */

    if (p1 - _optarg >= len) {
        return -1;
    }
    cfg->wl_args[cfg->n_wl_args].value = strdup(p1 + 1);
    cfg->n_wl_args++;

    return 0;
}

int cli_cfg_parse_param_sweep_args(cyclops_cfg_t *cfg, char *_optarg)
{
    int len  = strlen(optarg);
    char *p0, *p1;

    /* parse the wl param key */

    p0 = _optarg;
    if (!(p1 = strchr(_optarg, '='))) {
        return -1;
    }
    cfg->ps_wl_param_key = strndup(p0, p1 - p0);

    /* parse the low value */

    if (p1 - _optarg >= len) {
        return -1;
    }

    p0 = p1 + 1;
    if (!(p1 = strchr(p0, ':'))) {
        return -1;
    }
    cfg->ps_wl_param_low = strndup(p0, p1 - p0);

    /* parse the high value */

    if (p1 - _optarg >= len) {
        return -1;
    }

    p0 = p1 + 1;
    if (!(p1 = strchr(p0, ':'))) {
        return -1;
    }
    cfg->ps_wl_param_high = strndup(p0, p1 - p0);

    /* parse the step value */

    if (p1 - _optarg >= len) {
        return -1;
    }
    cfg->ps_wl_param_step = strdup(p1 + 1);

    return 0;
}

cyclops_cfg_t *cli_cfg_init(int argc, char *argv[])
{
    cyclops_cfg_t *cfg = calloc(1, sizeof(cyclops_cfg_t));
    if (!cfg) {
        perror("Failed to allocate memory for the cyclops config struct");
        exit(1);
    }

    bool csv = false;
    bool csv_all = false;

    int opt;
    while ((opt = getopt_long(argc, argv, "hw:m:r:u:p:s:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                cfg->display_help = true;
                break;
            case 'w':
                cfg->wl_name = strdup(optarg);
                break;
            case 'm':
                cfg->mg_name = strdup(optarg);
                break;
            case 'r':
                cfg->batch_runs = strtoull(optarg, NULL, 10);
                break;
            case 'u':
                cfg->warmup_runs = strtoull(optarg, NULL, 10);
                break;
            case 'p':
                if (cli_cfg_parse_wl_arg(cfg, optarg) != 0) {
                    return NULL;
                }
                break;
            case 's':
                if (cli_cfg_parse_param_sweep_args(cfg, optarg) != 0) {
                    return NULL;
                }
                break;
            case OPT_CSV:
                csv = true;
                break;
            case OPT_CSV_ALL:
                csv_all = true;
                break;
            case OPT_REPORT:
                cfg->report = true;
                break;
            case OPT_GEOMETRIC:
                cfg->geometric = true;
                break;
            default:
                fprintf(stderr, "Usage 1\n");
                return NULL;
        }
    }

    if (csv_all) {
        cfg->param_sweep_csv = true;
        cfg->batch_csv = true;
    } else if (csv && cfg->ps_wl_param_key) {
        cfg->param_sweep_csv = true;
        cfg->batch_csv = false;
    } else if (csv) {
        cfg->batch_csv = true;
    }

    return cfg;
}

// TODO: free all the allocated memory for duplicated strings
void cli_cfg_destroy(cyclops_cfg_t *cli_cfg)
{
    free(cli_cfg);
}

bool cli_cfg_validate(cyclops_cfg_t *cli_cfg)
{
    /* Help requested: other args are irrelevant */
    if (cli_cfg->display_help) {
        return true;
    }

    if (!cli_cfg->wl_name) {
        fprintf(stderr, "Usage: Please select a workload\n");
        return false;
    }

    if (!cli_cfg->mg_name) {
        fprintf(stderr, "Usage: Please select a metric group\n");
        return false;
    }

    return true;
}
