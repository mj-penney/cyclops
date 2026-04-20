#ifndef REPORT_H
#define REPORT_H

#include "./batch.h"

void run_report(batch_conf_t *cfg, batch_data_t *batch_data);
void batch_to_csv(batch_conf_t *cfg, batch_data_t *batch_data);

#endif
