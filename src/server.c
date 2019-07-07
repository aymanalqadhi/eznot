#include "server.h"
#include "address.h"
#include "callbacks.h"
#include "publishers.h"

#include "log.h"
#include "uv.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************/

int
init_eznot_server(eznot_server_t *server, app_config_t *config)
{
	log_trace("init_eznot_server()");

	if (server == NULL) {
		return -1;
	}

	server->config      = config;
	server->loop        = uv_default_loop();
	server->handle.data = server;

	int rc;
	if (config->trusted_publishers_file == NULL) {
		log_error("You need to provied a trusted publishers file!");
		return -1;
	} else {
		rc = eznot_load_publishers(config->trusted_publishers_file);
		if (rc != 0) {
			log_error("Could not load publishers!");
			return -1;
		}
	}

	rc = uv_udp_init(server->loop, &server->handle);
	UV_CHECK(rc, "uv_udp_init");

	return 0;
}

/******************************************************************************/

int
start_eznot_server(const eznot_server_t *server)
{
	log_trace("start_eznot_server()");
	int rc;

	if (server->config->ipv6) {
		struct sockaddr_in6 addr;

		rc = uv_ip6_addr(EZNOT_IPV6_ANY, server->config->listen_port, &addr);
		UV_CHECK(rc, "uv_ip6_addr");
		rc = uv_udp_bind((uv_udp_t *)&server->handle,
						(struct sockaddr *)&addr, 0);
		UV_CHECK(rc, "uv_udp_bind");
	} else {
		struct sockaddr_in addr;

		rc = uv_ip4_addr(EZNOT_IPV4_ANY, server->config->listen_port, &addr);
		UV_CHECK(rc, "uv_ip4_addr");
		rc = uv_udp_bind((uv_udp_t *)&server->handle,
						(struct sockaddr *)&addr, 0);
		UV_CHECK(rc, "uv_udp_bind");
	}

	rc = uv_udp_recv_start((uv_udp_t *)&server->handle,
						   &eznot_on_alloc,
						   &eznot_on_recv);
	UV_CHECK(rc, "uv_udp_recv_start");
	rc = uv_run(server->loop, UV_RUN_DEFAULT);
	UV_CHECK(rc, "uv_run");

	return 0;
}

/******************************************************************************/

void
free_eznot_server(eznot_server_t *server)
{
	log_trace("free_eznot_server()");
}

/******************************************************************************/
