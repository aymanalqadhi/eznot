#include "config/config.h"
#include "server.h"

#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/******************************************************************************/

int
main(int argc, char* argv[])
{
	/* Get command line arguments */
	app_config_t config;
	config_parse_argv(&config, argc, argv, true);

	/* Create an initialize a server struct */
	eznot_server_t server;
	int rc;

	/* Initialize the server */
	log_info("Initializing server...");
	if ((rc = init_eznot_server(&server, &config)) != 0) {
		log_fatal("Could not initialize server!");
		log_debug("init_eznot_server() returned %d.", rc);

		exit(EXIT_FAILURE);
	}

	/* Start the server */
	log_info("Starting server...");
	if ((rc = start_eznot_server(&server)) != 0) {
		log_fatal("Could not start server!");
		log_debug("start_eznot_server() returned %d.", rc);

		exit(EXIT_FAILURE);
	}

	/* Free used resources */
	log_debug("Cleaning...");
	free_eznot_server(&server);

	return EXIT_SUCCESS;
}

/******************************************************************************/
