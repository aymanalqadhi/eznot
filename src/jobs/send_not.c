#include "jobs/send_not.h"

#include "log.h"

#include <pthread.h>

/******************************************************************************/

static pthread_cond_t not_ready = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/******************************************************************************/

void
eznot_send_not_job(void* payload)
{
	log_trace("thread %s: eznot_send_not_job()", pthread_self());

	if (payload == NULL) {
		log_warn("Passed NULL for payload, skipping.");
		return;
	}

	send_not_payload_t *p = payload;
	if (!p->subscriber || !p->not_data || !p->start || !p->refcount) {
		log_warn("Malformed payload passed, skipping.");
		return;
	}

	pthread_mutex_trylock(&lock);
	while (!*(p->start)) {
		pthread_cond_wait(&not_ready, &lock);
	}
	pthread_mutex_unlock(&lock);

	log_info("Processing on thread %d...", pthread_self());

	/* Free the payload if the refcount is equal or less than 0 */
	if (--*(p->refcount) <= 0) {
		pthread_mutex_lock(&lock);

		free(p->not_data);
		free(p->start);
		free(p->refcount);
		free(p);

		pthread_mutex_unlock(&lock);
	}
}

/******************************************************************************/
