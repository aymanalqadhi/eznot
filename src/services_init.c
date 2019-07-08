#include "services_init.h"

#include "config/config.h"
#include "memory.h"

#include "log.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/queue.h>

/******************************************************************************/

typedef struct service_container
{
	service_t service;
	STAILQ_ENTRY(service_container) entries;
} service_container_t;

/******************************************************************************/

static STAILQ_HEAD(services_head,
				   service_container) head = STAILQ_HEAD_INITIALIZER(head);

/******************************************************************************/

service_t*
eznot_add_service(const char* name,
				  eznot_init_func init,
				  eznot_destroy_func destroy,
				  bool required)
{
	log_trace("eznot_add_service()");

	assert(name != NULL);
	assert(init != NULL);
	assert(destroy != NULL);

	service_container_t* svc = malloc(sizeof(*svc));

	svc->service.name = name;
	svc->service.init_func = init;
	svc->service.destroy_func = destroy;
	svc->service.is_required = required;

	STAILQ_INSERT_TAIL(head, svc, entries);

	return &svc->service;
}

/******************************************************************************/

int
eznot_init_services(const app_config_t* conf)
{
	log_trace("eznot_init_services()");

	service_container_t *svc, tmp;
	int rc;

	STAILQ_FOREACH(svc, head, tmp)
	{
		log_debug("Initializing service %s.", svc->service.name);

		rc = svc->service.init_func(conf);
		if (rc != 0 && svc->service.is_required) {
			log_error("Could not initialize required service %s.",
					  svc->service.name);
			return -1;
		} else {
			log_warn("Could not initialize service %s.",
					  svc->service.name);
		}
	}

	return 0;
}

/******************************************************************************/
