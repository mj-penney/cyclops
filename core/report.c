#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/report.h"
#include "../include/metric.h"
#include "../include/data_processing.h"

#define TABLE_COLUMN_WIDTH 18

static void print_table_cell_right_align(const char *text)
{
    int text_len = strlen(text);

    if (text_len > TABLE_COLUMN_WIDTH)
        text_len = TABLE_COLUMN_WIDTH;

    char cell_buf[TABLE_COLUMN_WIDTH + 1] = { '\0' };

    memset(cell_buf, ' ', sizeof(cell_buf) - 1);
    memcpy(&cell_buf[TABLE_COLUMN_WIDTH - text_len], text, text_len);

    printf("%s", cell_buf);
}

static void print_double_agg_table_row(double_agg_t agg, const char *name)
{
    char name_buf[32];
    char min_buf[32];
    char max_buf[32];
    char median_buf[32];

    snprintf(name_buf, sizeof(name_buf), "%s", name);
    snprintf(min_buf, sizeof(min_buf), "%.6f", agg.min);
    snprintf(max_buf, sizeof(max_buf), "%.6f", agg.max);
    snprintf(median_buf, sizeof(median_buf), "%.6f", agg.median);

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

static void print_table_column_headers()
{
    print_table_cell_right_align("Metric");
    print_table_cell_right_align("Min");
    print_table_cell_right_align("Max");
    print_table_cell_right_align("Median");
    printf("\n");

    for (int i = 0; i < TABLE_COLUMN_WIDTH * 4; i++) {
        putchar('=');
    }
    putchar('\n');
}

static void print_batch_info(batch_conf_t *cfg)
{
    printf("    Warmup runs: %llu\n", cfg->warmup_runs);
    printf("    Batch runs:  %llu\n", cfg->batch_runs);
    putchar('\n');
}

void run_report(batch_conf_t *cfg, batch_data_t *batch_data)
{
    printf("\n");
    print_batch_info(cfg);
    print_table_column_headers();

    for (int i = 0; i < batch_data->n_raw; i++) {
        const metric_t *m = get_metric_by_id(
                                            batch_data->raw_data[i].metric_id);
        print_double_agg_table_row(batch_data->raw_data[i].agg, m->name);
    }

    for (int i = 0; i < batch_data->n_derived; i++) {
        const metric_t *m = get_metric_by_id(
                                        batch_data->derived_data[i].metric_id);
        print_double_agg_table_row(batch_data->derived_data[i].agg, m->name);
    }

    printf("\n");
}

static void write_batch_metadata(FILE *file, batch_conf_t *cfg)
{
    fprintf(file, "#workload=%s\n", cfg->wl->name);
    fprintf(file, "#metric-group=%s\n", cfg->mg->name);
    fprintf(file, "#warmup-runs=%llu\n", cfg->warmup_runs);
    fprintf(file, "#batch-runs=%llu\n", cfg->batch_runs);

    workload_t *wl = cfg->wl;
    if (!wl->params) {
        return;
    }

    for (int i = 0; i < wl->n_params; i++) {
        fprintf(file, "#workload-params.%s=", wl->params[i].key);
        fprintf(file, "%llu\n", wl_get_param(wl, wl->params[i].key));
    }
}

void batch_to_csv(batch_conf_t *cfg, batch_data_t *batch_data)
{
    FILE *file = fopen("batch.csv", "w");
    if (!file) {
        perror("Failed to open csv file");
        exit(1);
    }

    write_batch_metadata(file, cfg);

    for (int i = 0; i < batch_data->n_raw; i++) {
        const metric_t *m = get_metric_by_id(
                                            batch_data->raw_data[i].metric_id);
        fprintf(file, "%s,", m->name);
    }

    for (int i = 0; i < batch_data->n_derived; i++) {
        const metric_t *m = get_metric_by_id(
                                        batch_data->derived_data[i].metric_id);
        fprintf(file, "%s,", m->name);
    }

    fputc('\n', file);

    for (unsigned long long r = 0; r < cfg->batch_runs; r++) {
        for (int i = 0; i < batch_data->n_raw; i++) {
            fprintf(file, "%.6f,", batch_data->raw_data[i].run_vals[r]);
        }

        for (int i = 0; i < batch_data->n_derived; i++) {
            fprintf(file, "%.6f,", batch_data->derived_data[i].run_vals[r]);
        }

        fputc('\n', file);
    }

    fclose(file);
}
