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

void
eznot_send_not_job(void* payload);

#endif /* end of include guard: EZNOT_JOBS_SEND_NOT_H */
