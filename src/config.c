#include "config/config.h"
#include "config/constants.h"

#include <getopt.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/******************************************************************************/

/* Holds the last error message.
 * NULL indicates that there were no erros in the last operation*/
static const char*error_message;

/******************************************************************************/

static int
parse_long(const char* str, long* ret);

/******************************************************************************/

void
eznot_config_parse_argv(app_config_t* config, int argc, char** argv)
{
	char arg;
	long parsed_value;

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
			if (parse_long(optarg, &parsed_value) == -1 || parsed_value < 0 ||
				parsed_value > 0xFFFF) {
				fprintf(stderr, "ERROR: %s!\n", error_message);
				exit(EXIT_FAILURE);
			}

			config->listen_port = (uint16_t)parsed_value;
			break;

		/* Send port option */
		case 's':
			if (parse_long(optarg, &parsed_value) == -1 || parsed_value < 0 ||
				parsed_value > 0xFFFF) {
				fprintf(stderr, "ERROR: %s!\n", error_message);
				exit(EXIT_FAILURE);
			}

			config->threads_count = (uint16_t)parsed_value;
			break;

		/* Send port option */
		case 'r':
			if (parse_long(optarg, &parsed_value) == -1 || parsed_value < 0 ||
				parsed_value > 0xFFFF) {
					fprintf(stderr, "ERROR: %s!\n", error_message);
					exit(EXIT_FAILURE);
			}

			config->send_port = (uint16_t)parsed_value;
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
					"ERROR: %s!\nUsage: %s %s\n",
					error_message,
					argv[0],
					CONFIG_COMMAND_LINE_USAGE);
			exit(EXIT_FAILURE);
		}
	}
}

/******************************************************************************/

static int
parse_long(const char* str, long* ret)
{
	errno = 0;
	char* endp;
	*ret = strtol(str, &endp, 10);

	if (errno != 0 || *endp != '\0') {
		return -1;
	} else {
		return 0;
	}
}

/******************************************************************************/
