#include "subscribers.h"

#include "config/config.h"
#include "address.h"
#include "memory.h"

#include "log.h"
#include "uthash.h"
#include "uv.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/

static subscriber_t* subscribers = NULL;
static unsigned int subscribers_count = 0;
static const app_config_t *config;

/******************************************************************************/

int
eznot_add_subscriber(struct sockaddr_storage* endpoint,
					 const char* tags,
					 size_t tagslen)
{
	assert(endpoint != NULL);

	subscriber_t* s;
	char address[INET6_ADDRSTRLEN];
	int ret;

	memset(address, 0, INET6_ADDRSTRLEN);

	/* Handle IPv4 */
	if (endpoint->ss_family == AF_INET) {
		((struct sockaddr_in *)endpoint)->sin_port = htons(config->send_port);
		ret = uv_inet_ntop(AF_INET,
						   &((union ip_address*)endpoint)->addrv4.sin_addr,
						   address,
						   INET_ADDRSTRLEN);
	} else if (endpoint->ss_family == AF_INET6) {
		((struct sockaddr_in6 *)endpoint)->sin6_port = htons(config->send_port);
		ret = uv_inet_ntop(AF_INET6,
						   &((union ip_address*)endpoint)->addrv6.sin6_addr,
						   address,
						   INET6_ADDRSTRLEN);
	} else {
		log_error("Unsupported address family %d.", endpoint->ss_family);
		return -1;
	}

	if (ret < 0) {
		log_error("uv_inet_ntop: %s", uv_strerror(ret));
		return -1;
	}

	HASH_FIND_STR(subscribers, address, s);

	if (s != NULL) {
		if (s->tags != NULL && s->tags != tags) {
			free(s->tags);
		}

		if (tags != NULL) {
			s->tags = strndup(tags, tagslen);
			s->tags_len = tagslen;
		} else {
			s->tags = NULL;
			s->tags_len = 0;
		}

		return 0;
	}

	s = xmalloc(sizeof(*s));

	memcpy(s->address, address, INET6_ADDRSTRLEN);
	memcpy(&s->endpoint, endpoint, sizeof(s->endpoint));
	
	s->tags = strndup(tags, tagslen);
	s->tags_len = tagslen;

	HASH_ADD_STR(subscribers, address, s);
	++subscribers_count;

	return 0;
}

/******************************************************************************/

subscriber_t*
eznot_get_subscriber(const char* addr)
{
	assert(addr != NULL);

	subscriber_t* s = NULL;
	HASH_FIND_STR(subscribers, addr, s);

	return s;
}

/******************************************************************************/

int
eznot_iterate_subscribers(int (*action)(subscriber_t*))
{
	assert(action != NULL);
	int done = 0;

	subscriber_t *s, *tmp;
	HASH_ITER(hh, subscribers, s, tmp) {
		if (action(s) != 0) {
			return done;
		}
		++done;
	}

	return done;
}

/******************************************************************************/

subscriber_t*
eznot_get_subscribers()
{
	return subscribers;
}

/******************************************************************************/

unsigned int
eznot_subscribers_count()
{
	return subscribers_count;
}

/******************************************************************************/

int
eznot_init_subscribers_hashtable(const app_config_t* conf)
{
	config = conf;
	return 0;
}

/******************************************************************************/

void
eznot_destroy_subscribers_hashtable()
{
	log_trace("eznot_destroy_subscribers_hashtable()");

	subscriber_t *subs, *tmp;
	HASH_ITER(hh, subscribers, subs, tmp) {
		HASH_DELETE(hh, subscribers, subs);
		free(subs);
	}

	subscribers = NULL;
}

/******************************************************************************/
