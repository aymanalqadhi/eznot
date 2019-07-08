#ifndef EZNOT_SERVICES_INIT_H
#define EZNOT_SERVICES_INIT_H

#include "config/config.h"

#include <stdbool.h>

typedef int (*eznot_init_func)(const app_config_t*);
typedef void (*eznot_destroy_func)();

typedef struct service
{
	const char* name;
	bool is_required;

	eznot_init_func init_func;
	eznot_destroy_func destroy_func;
} service_t;

service_t*
eznot_add_service(const char* name,
				  eznot_init_func init,
				  eznot_destroy_func destroy,
				  bool required);

int
eznot_init_services(const app_config_t* conf);

#endif /* end of include guard: EZNOT_SERVICES_INIT_H */
