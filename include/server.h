#ifndef EZNOT_SEVER_H
#define EZNOT_SEVER_H

#include "config/config.h"

#include <uv.h>

typedef struct eznot_server {
	app_config_t *config;

	uv_loop_t *loop;
	uv_udp_t handle;
} eznot_server_t;

int
init_eznot_server(eznot_server_t *server, app_config_t *config);

int
start_eznot_server(const eznot_server_t *server);

void
free_eznot_server(eznot_server_t *server);

#endif /* end of include guard: EZNOT_SEVER_H */
