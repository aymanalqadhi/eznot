#include "jobs_runner.h"
#include "config/config.h"

#include "thpool.h"
#include "log.h"

#include <stdbool.h>
#include <pthread.h>

/******************************************************************************/

static threadpool thpool;
static bool inited = false, destroyed = false;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/******************************************************************************/

void
eznot_enqueue_job(void (*job_func)(void*), void* data)
{
	thpool_add_work(thpool, job_func, data);
}

/******************************************************************************/

int
eznot_init_jobs_runner(const app_config_t *conf)
{
	log_trace("eznot_init_jobs_runner()");

	if (inited) {
		log_error("Thread pool was already initialized.");
		return -1;
	}

	pthread_mutex_lock(&lock);

	thpool = thpool_init(conf->threads_count);
	inited = true;
	destroyed = false;

	pthread_mutex_unlock(&lock);

	return 0;
}

/******************************************************************************/

void
eznot_destroy_jobs_runner()
{
	if (!inited || destroyed) {
		log_warn("The thread pool is not in a valid state");
		return;
	}

	pthread_mutex_lock(&lock);

	thpool_destroy(thpool);
	destroyed = true;

	pthread_mutex_unlock(&lock);
}

/******************************************************************************/
