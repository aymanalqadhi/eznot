#ifndef EZNOT_JOBS_RUNNER_H
#define EZNOT_JOBS_RUNNER_H

#include "config/config.h"

#include <stddef.h>

void
eznot_enqueue_job(void (*job_func)(void*), void*);

int
eznot_init_jobs_runner(const app_config_t *conf);

void
eznot_destroy_jobs_runner();

#endif /* end of include guard: EZNOT_JOBS_RUNNER_H */
