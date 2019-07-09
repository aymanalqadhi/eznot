#include "address.h"
#include "callbacks.h"
#include "publishers.h"
#include "server.h"
#include "subscribers.h"

#include "jobs/send_not.h"
#include "jobs_runner.h"

#include "log.h"
#include "uv.h"

#include <assert.h>
#include <stdlib.h>

#define UV_CHECK(r, msg)                                                       \
	if (r) {                                                                   \
		log_error("%s: %s", msg, uv_strerror(r));                              \
		return -1;                                                             \
	}

/******************************************************************************/

int
init_eznot_server(eznot_server_t* server, app_config_t* config)
{
	log_trace("init_eznot_server()");

	if (server == NULL) {
		return -1;
	}

	server->config = config;
	server->loop = uv_default_loop();
	server->handle.data = server;

	int rc = uv_udp_init(server->loop, &server->handle);
	UV_CHECK(rc, "uv_udp_init");

	return 0;
}

/******************************************************************************/

int
start_eznot_server(const eznot_server_t* server)
{
	log_trace("start_eznot_server()");
	int rc;

	if (server->config->use_ipv6) {
		struct sockaddr_in6 addr;

		rc = uv_ip6_addr(EZNOT_IPV6_ANY, server->config->listen_port, &addr);
		UV_CHECK(rc, "uv_ip6_addr");
		rc =
			uv_udp_bind((uv_udp_t*)&server->handle, (struct sockaddr*)&addr, 0);
		UV_CHECK(rc, "uv_udp_bind");
	} else {
		struct sockaddr_in addr;

		rc = uv_ip4_addr(EZNOT_IPV4_ANY, server->config->listen_port, &addr);
		UV_CHECK(rc, "uv_ip4_addr");
		rc =
			uv_udp_bind((uv_udp_t*)&server->handle, (struct sockaddr*)&addr, 0);
		UV_CHECK(rc, "uv_udp_bind");
	}

	UV_CHECK(uv_udp_recv_start(
				 (uv_udp_t*)&server->handle, &eznot_on_alloc, &eznot_on_recv),
			 "uv_udp_recv_start");
	
	log_info("Listening on: %s:%d",
			 server->config->use_ipv6 ? EZNOT_IPV6_ANY : EZNOT_IPV4_ANY,
			 server->config->listen_port);

	UV_CHECK(uv_run(server->loop, UV_RUN_DEFAULT), "uv_run");

	return 0;
}

/******************************************************************************/

void
free_eznot_server(eznot_server_t* server)
{
	log_trace("free_eznot_server()");

	log_debug("Freeing thread pool");
	eznot_destroy_jobs_runner();

	log_debug("Destroying app jobs");
	eznot_destroy_send_not_job();
}

/******************************************************************************/
