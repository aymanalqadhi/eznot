#include "config/config.h"

#include <stdio.h>
#include <stdbool.h>

int
main(int argc, char* argv[])
{
	app_config_t config;
	config_parse_argv(&config, argc, argv, true);

	return 0;
}
