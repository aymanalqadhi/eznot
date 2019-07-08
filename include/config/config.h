#ifndef EZSRV_CONFIG_CONFIG_H
#define EZSRV_CONFIG_CONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Errors returned by `config_parse_argv' function */
#define CONFIG_UNKNOWN_ERROR -1
#define CONFIG_MISSING_VALUE -2
#define CONFIG_INVALID_AEGUMENT -3
#define CONFIG_INVALID_VALUE -4
#define CONFIG_HELP_MESSAGE -5
#define CONFIG_VERSION_MESSAGE -6

/**
 * A structure to hold the application config
 */
typedef struct app_config
{
	const char* executable_name; /* The executing file name (argv[0]) */

	uint16_t listen_port;   /* The port the server listens on */
	uint16_t send_port;		/* The port the server sends notifications on */
	uint16_t threads_count; /* The number of the threads in thr threadpool */

	bool use_ipv6; /* If set, IPv6 is used, default is IPv4 */
	FILE* trusted_publishers_file;
} app_config_t;

void
eznot_config_parse_argv(app_config_t* config, int argc, char** argv);

#endif /* EZSRV_CONFIG_CONFIG_H */
