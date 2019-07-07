#ifndef EZNOT_JOBS_RUNNER_H
#define EZNOT_JOBS_RUNNER_H

#include <stddef.h>

int
eznot_init_jobs_runner(size_t threads_count);

void
eznot_enqueue_job(void (*job_func)(void*), void*);

void
eznot_destroy_jobs_runner();

#endif /* end of include guard: EZNOT_JOBS_RUNNER_H */
