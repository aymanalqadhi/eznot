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

/**
 * A Helper function to parse string to integer values
 *
 * @param ret A pointer to a variable into which to store the result
 * @return 0 on success, or -1 on failure
 */
static int
parse_long(const char* str, long* ret);

/******************************************************************************/

int
config_parse_argv(app_config_t* config,
				  int argc,
				  char** argv,
				  bool handle_errors)
{
	char arg;
	long parsed_value;

	/* Application options definitions */
	static struct option options[] = {
		{"port",    required_argument, NULL, 'p'},
		{"trusted", required_argument, NULL, 't'},
		{"ipv6",    no_argument, NULL,       '6'},
		{"help",    no_argument, NULL,       'h'},
		{"version", no_argument, NULL,       'V'}
	};

	/* Set default values */
	config->executable_name = argv[0];
	config->ipv6 = CONFIG_DEFAULT_IPV6_ENABLED;
	config->listen_port = CONFIG_DEFAULT_PORT;

	while ((arg = getopt_long_only(
				argc, argv, CONFIG_ARGS_LIST, options, NULL)) != -1) {
		switch (arg) {

		/* Port option */
		case 'p':
			if (parse_long(optarg, &parsed_value) == -1 || parsed_value < 0 ||
				parsed_value > 0xFFFF) {
				error_message = "Invalid port value";
				if (handle_errors) {
					fprintf(stderr, "ERROR: %s!\n", error_message);
					exit(EXIT_FAILURE);
				} else {
					return CONFIG_INVALID_VALUE;
				}
			}

			config->listen_port = (uint16_t)parsed_value;
			break;

		/* Port option */
		case 't':
			if (strncmp(optarg, "-", 1) == 0) {
				config->trusted_publishers_file = stdin;
			} else if (access(optarg, F_OK) == -1) {
				if (handle_errors) {
					fprintf(stderr, "ERROR: Cannot access file %s\n", optarg);
					exit(EXIT_FAILURE);
				} else {
					return CONFIG_INVALID_VALUE;
				}
			}

			config->trusted_publishers_file = fopen(optarg, "r");
			if (config->trusted_publishers_file == NULL) {
				if (handle_errors) {
					fprintf(stderr, "ERROR: Could not open file %s\n", optarg);
					exit(EXIT_FAILURE);
				} else {
					return CONFIG_UNKNOWN_ERROR;
				}
			}

			break;

		/* Help option */
		case 'h':
			if (handle_errors) {
				printf("Usage: %s %s\n\n%s\n\nOptions:\n%s\n",
					   argv[0],
					   CONFIG_COMMAND_LINE_USAGE,
					   CONFIG_APP_DESCRIPTION,
					   CONFIG_COMMAND_LINE_HELP);
				exit(EXIT_SUCCESS);
			} else {
				return CONFIG_HELP_MESSAGE;
			}

		/* Version option */
		case 'V':
			if (handle_errors) {
				printf("%s version %s\n", CONFIG_APP_NAME, CONFIG_APP_VERSION);
				exit(EXIT_SUCCESS);
			} else {
				return CONFIG_VERSION_MESSAGE;
			}

		/* IPv6 option */
		case '6':
			config->ipv6 = 1;
			break;

			/* An error occured */
		default:
			error_message = "Invalid Options";
			if (handle_errors) {
				fprintf(stderr,
						"ERROR: %s!\nUsage: %s %s\n",
						error_message,
						argv[0],
						CONFIG_COMMAND_LINE_USAGE);
				exit(EXIT_FAILURE);
			} else {
				return CONFIG_INVALID_AEGUMENT;
			}
		}
	}

	error_message = NULL;
	return 0;
}

/******************************************************************************/

const char*
config_last_error()
{
	return error_message;
}

/******************************************************************************/

static int
parse_long(const char* str, long* ret)
{
	errno = 0;
	char* endp;
	*ret = strtol(optarg, &endp, 10);

	if (errno != 0 || *endp != '\0') {
		return -1;
	} else {
		return 0;
	}
}

/******************************************************************************/
