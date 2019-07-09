#include "config/config.h"
#include "services_init.h"

#include "jobs/send_not.h"
#include "jobs_runner.h"
#include "publishers.h"
#include "subscribers.h"

#include "server.h"

#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/

static inline void
export_services();

/******************************************************************************/

int
main(int argc, char* argv[])
{
	/* Get command line arguments */
	app_config_t config;
	eznot_config_parse_argv(&config, argc, argv);
	
	/* Config the logger */
	log_set_level(config.log_level);

	/* Export and initialize services */
	export_services();
	if (eznot_init_services(&config) != 0) {
		log_fatal("Could not initialize app services!");
		exit(EXIT_FAILURE);
	}

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

static inline void
export_services()
{
	/* Publishers */
	eznot_add_service("Publishers Hashtable",
					  &eznot_init_publishers_hashtable,
					  &eznot_destroy_publishers_hashtable,
					  true);
	
	/* Subscribers */
	eznot_add_service("Subscribers Hashtable",
					  &eznot_init_subscribers_hashtable,
					  &eznot_destroy_subscribers_hashtable,
					  true);
	/* Job runner */
	eznot_add_service("Jobs Runner",
					  &eznot_init_jobs_runner,
					  &eznot_destroy_jobs_runner,
					  true);
	/* Sent notifications job */
	eznot_add_service("Send Notifications Job",
					  &eznot_init_send_not_job,
					  &eznot_destroy_send_not_job,
					  true);

}

/******************************************************************************/
