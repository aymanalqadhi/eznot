#include "config/config.h"
#include "config/constants.h"

#include <getopt.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/******************************************************************************/

static bool
parse_uint16(const char *str, uint16_t* ret);

/******************************************************************************/

void
eznot_config_parse_argv(app_config_t* config, int argc, char** argv)
{
	char arg;
	uint16_t parsed_short;

	/* Application options definitions */
	static struct option options[] = {
		{"port",      required_argument, NULL, 'p'},
		{"send-port", required_argument, NULL, 's'},
		{"threads",   required_argument, NULL, 'r'},
		{"trusted",   required_argument, NULL, 't'},
		{"ipv6",      no_argument, NULL,       '6'},
		{"help",      no_argument, NULL,       'h'},
		{"version",   no_argument, NULL,       'V'}
	};

	/* Set default values */
	config->executable_name         = argv[0];
	config->listen_port             = CONFIG_DEFAULT_PORT;
	config->send_port               = CONFIG_DEFAULT_SEND_PORT;
	config->threads_count           = CONFIG_DEFAULT_THREAD_COUNT;
	config->use_ipv6                = CONFIG_DEFAULT_IPV6_ENABLED;
	config->trusted_publishers_file = NULL;

	while ((arg = getopt_long_only(
				argc, argv, CONFIG_ARGS_LIST, options, NULL)) != -1) {
		switch (arg) {

		/* Port option */
		case 'p':
			if (!parse_uint16(optarg, &parsed_short)) {
				fprintf(stderr, "ERROR: Invalid listen port value!\n");
				exit(EXIT_FAILURE);
			}

			config->listen_port = parsed_short;
			break;

		/* Send port option */
		case 's':
			if (!parse_uint16(optarg, &parsed_short)) {
				fprintf(stderr, "ERROR: Invalid send port value!\n");
				exit(EXIT_FAILURE);
			}

			config->threads_count = parsed_short;
			break;

		/* Send port option */
		case 'r':
			if (!parse_uint16(optarg, &parsed_short)) {
					fprintf(stderr, "ERROR: Invalid threadpool size!\n");
					exit(EXIT_FAILURE);
			}

			config->send_port = parsed_short;
			break;

		/* Trusted publishers file option */
		case 't':
			if (access(optarg, F_OK) == -1) {
				fprintf(stderr, "ERROR: Cannot access file %s\n", optarg);
				exit(EXIT_FAILURE);
			}

			config->trusted_publishers_file = fopen(optarg, "r");
			if (config->trusted_publishers_file == NULL) {
				fprintf(stderr, "ERROR: Could not open file %s\n", optarg);
				exit(EXIT_FAILURE);
			}

			break;

		/* Help option */
		case 'h':
			printf("Usage: %s %s\n\n%s\n\nOptions:\n%s\n",
				   argv[0],
				   CONFIG_COMMAND_LINE_USAGE,
				   CONFIG_APP_DESCRIPTION,
				   CONFIG_COMMAND_LINE_HELP);
			exit(EXIT_SUCCESS);

		/* Version option */
		case 'V':
			printf("%s version %s\n", CONFIG_APP_NAME, CONFIG_APP_VERSION);
			exit(EXIT_SUCCESS);

		/* IPv6 option */
		case '6':
			config->use_ipv6 = true;
			break;

			/* An error occured */
		default:
			fprintf(stderr,
					"Usage: %s %s\n",
					argv[0],
					CONFIG_COMMAND_LINE_USAGE);
			exit(EXIT_FAILURE);
		}
	}

	/* Check for required options */
	if (config->trusted_publishers_file == NULL) {
		fprintf(stderr, "ERROR: You must provide a trusted publishers file!\n");
		exit(EXIT_FAILURE);
	}
}

/******************************************************************************/

static bool
parse_uint16(const char *str, uint16_t* ret)
{
	errno = 0;
	char* endp;
	*ret = strtod(str, &endp);

	return !(errno != 0 || *endp != '\0') && *ret >= 0 && *ret <= 0xFFFF;
}

/******************************************************************************/
