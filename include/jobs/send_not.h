#ifndef EZNOT_JOBS_SEND_NOT_H
#define EZNOT_JOBS_SEND_NOT_H

#include "subscribers.h"

typedef struct send_not_payload
{
	subscriber_t *subscriber;
	char *not_data;
	
	bool *start;
	int *refcount;
} send_not_payload_t;

int
eznot_init_send_not_job();

void
eznot_send_not_job(void* payload);

void
eznot_send_not_job_ready();

void
eznot_destroy_send_not_job();

#endif /* end of include guard: EZNOT_JOBS_SEND_NOT_H */
