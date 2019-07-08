#ifndef EZNOT_SUBSCRIBERS_H
#define EZNOT_SUBSCRIBERS_H

#include "config/config.h"
#include "request_message.h"

#include "uthash.h"
#include "uv.h"

#include <stddef.h>

typedef struct subscriber
{
	char address[INET6_ADDRSTRLEN];
	struct sockaddr_storage endpoint;

	char* tags;
	size_t tags_len;

	UT_hash_handle hh;
} subscriber_t;

int
eznot_init_subscribers_hashtable(const app_config_t* conf);

int
eznot_add_subscriber(struct sockaddr_storage*, const char*, size_t);

subscriber_t*
eznot_get_subscriber(const char*);

int
eznot_iterate_subscribers(int (*)(subscriber_t*));

subscriber_t*
eznot_get_subscribers();

unsigned int
eznot_subscribers_count();

#endif /* end of include guard: EZNOT_SUBSCRIBERS_H */
