#include "server.h"

#include "log.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************/

int
init_eznot_server(eznot_server_t *server, app_config_t *config)
{
	log_trace("init_eznot_server()");

	if (server == NULL)
		return -1;

	return 0;
}

/******************************************************************************/

int
start_eznot_server(const eznot_server_t *server)
{
	log_trace("start_eznot_server()");

	return 0;
}

/******************************************************************************/

void
free_eznot_server(eznot_server_t *server)
{
	log_trace("free_eznot_server()");
}

/******************************************************************************/
