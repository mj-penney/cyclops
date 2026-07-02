#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./internal.h"

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

static void print_batch_info(batch_t *b)
{
    printf("    Warmup runs: %llu\n", b->warmup_runs);
    printf("    Batch runs:  %llu\n", b->batch_runs);
    putchar('\n');
}

void run_report(batch_t *b)
{
    printf("\n");
    print_batch_info(b);
    print_table_column_headers();

    print_double_agg_table_row(b->raw_metric_scaling.agg, "SCALING");

    for (int i = 0; i < b->n_raw; i++) {
        batch_metric_t *metric_data = &b->raw_metrics[i];
        const metric_t *m = metric_data->metric;
        print_double_agg_table_row(metric_data->agg, m->name);
    }

    for (int i = 0; i < b->n_derived; i++) {
        batch_metric_t *metric_data = &b->derived_metrics[i];
        const metric_t *m = metric_data->metric;
        print_double_agg_table_row(metric_data->agg, m->name);
    }

    printf("\n");
}

static void write_batch_metadata(FILE *file, batch_t *b)
{
    fprintf(file,
            "# ./cyclops -w %s -m %s -u %llu -r %llu",
            b->wl->name,
            b->mg->name,
            b->warmup_runs,
            b->batch_runs);

    for (int i = 0; i < b->wl->n_params; i++) {
        wl_param_t *wl_param = &b->wl->params[i];
        fprintf(file,
                " -p %s=%llu",
                wl_param->key,
                wl_param_get_val(wl_param));
    }
    fprintf(file, "\n");
}

static void write_full_batch(FILE *file, batch_t *b)
{
    /* raw data column names */
    fprintf(file, "%s,", "SCALING");
    for (int i = 0; i < b->n_raw; i++) {
        batch_metric_t *metric_data = &b->raw_metrics[i];
        const metric_t *m = metric_data->metric;
        fprintf(file, "%s,", m->name);
    }

    /* derived data column names */
    for (int i = 0; i < b->n_derived; i++) {
        batch_metric_t *metric_data = &b->derived_metrics[i];
        const metric_t *m = metric_data->metric;
        fprintf(file, "%s,", m->name);
    }

    fputc('\n', file);

    for (unsigned long long r = 0; r < b->batch_runs; r++) {
        fprintf(file, "%.6f,", b->raw_metric_scaling.run_vals[r]);
        for (int i = 0; i < b->n_raw; i++) {
            fprintf(file, "%.6f,", b->raw_metrics[i].run_vals[r]);
        }

        for (int i = 0; i < b->n_derived; i++) {
            fprintf(file, "%.6f,", b->derived_metrics[i].run_vals[r]);
        }

        fputc('\n', file);
    }
}

void batch_to_csv(batch_t *b,
                  unsigned long long batch_no)
{
    char file_name[128];

    snprintf(file_name, sizeof(file_name), "batch_%llu.csv", batch_no);

    FILE *file = fopen(file_name, "w");
    if (!file) {
        perror("Failed to open csv file");
        exit(1);
    }

    write_batch_metadata(file, b);
    write_full_batch(file, b);

    fclose(file);
}

static void ps_write_metadata(FILE *file, param_sweep_t *ps)
{
    fprintf(file,
            "# ./cyclops -w %s -m %s -u %llu -r %llu -s %s=%llu:%llu:%llu",
            ps->wl->name,
            ps->mg->name,
            ps->warmup_runs,
            ps->batch_runs,
            ps->wl_param_key,
            ps->wl_param_low,
            ps->wl_param_high,
            ps->wl_param_step);

    for (int i = 0; i < ps->wl->n_params; i++) {
        wl_param_t *wl_param = &ps->wl->params[i];
        if (strcmp(wl_param->key, ps->wl_param_key) != 0) {
            fprintf(file,
                    " -p %s=%llu",
                    wl_param->key,
                    wl_param_get_val(wl_param));
        }
    }
    fprintf(file, "\n");
}

void ps_to_csv(param_sweep_t *ps)
{
    FILE *file = fopen("param_sweep.csv", "w");
    if (!file) {
        perror("Failed to open csv file");
        exit(1);
    }

    ps_write_metadata(file, ps);

    /* column headers */
    fprintf(file, "%s,", ps->wl_param_key);
    for (int i = 0; i < ps->mg->n_metrics; i++) {
        const metric_t *m = ps->metrics[i].metric;

        fprintf(file, "%s:MIN,%s:MAX,%s:MEDIAN,",
                m->name,
                m->name,
                m->name);
    }
    fprintf(file, "\n");

    /* write aggregate values for each batch */
    for (unsigned long long b = 0; b < ps->n_batches; b++) {
        for (int i = 0; i < ps->mg->n_metrics; i++) {
            param_sweep_batch_val_t *batch = &ps->metrics[i].batch_vals[b];
            fprintf(file,
                    "%llu,%.6f,%.6f,%.6f",
                    batch->param_sweep_val,
                    batch->agg.min,
                    batch->agg.max,
                    batch->agg.median);
        }
        fprintf(file, "\n");

    }

    fclose(file);
}
