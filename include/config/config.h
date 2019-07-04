#ifndef EZSRV_CONFIG_CONFIG_H
#define EZSRV_CONFIG_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/* Errors returned by `config_parse_argv' function */
#define CONFIG_UNKNOWN_ERROR    -1
#define CONFIG_MISSING_VALUE    -2
#define CONFIG_INVALID_AEGUMENT -3
#define CONFIG_INVALID_VALUE    -4
#define CONFIG_HELP_MESSAGE     -5
#define CONFIG_VERSION_MESSAGE  -6

/**
 * A structure to hold the application config
 */
typedef struct app_config {
  const char *executable_name;  /* The executing file name (argv[0]) */

  uint16_t listen_port;         /* The port the server listens on */
  int ipv6;                     /* If set, IPv6 is used, default is IPv4 */
} app_config_t;

/**
 * Parses command line arguments and sets the results in `config' parameter
 *
 * @param config    The structure to store the results in
 * @param argc      Command line arguments count
 * @param argv      Command line arguments values vector
 * @param handle_errors If true, the function will handle errors on its own
 *
 * @return 0 on success, negative value on failure
 */
int config_parse_argv(
    app_config_t *config,
    int argc,
    char ** argv,
    bool handle_errors);

/**
 * Gets the last error message
 *
 * @return NULL if there was no error message, or
 *         a pointer to the error message string
 */
const char* config_last_error();

#endif /* EZSRV_CONFIG_CONFIG_H */
