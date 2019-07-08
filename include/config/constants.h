#ifndef EZNOT_CONFIG_CONSTANTS_H
#define EZNOT_CONFIG_CONSTANTS_H

/* Default config values */
#define CONFIG_DEFAULT_PORT 		3434
#define CONFIG_DEFAULT_SEND_PORT 	3435
#define CONFIG_DEFAULT_THREAD_COUNT 8
#define CONFIG_DEFAULT_IPV6_ENABLED false


/**** App name and version ******/
#define CONFIG_APP_NAME    "eznot"
#define CONFIG_APP_VERSION "0.1 ALPHA"

/* App Description */
#define CONFIG_APP_DESCRIPTION \
  "A high-performance, configurable notifications server"

/* String used by getopt function to interpret 
 * command line arguments */
#define CONFIG_ARGS_LIST "s:p:t:6hV"

/* Comannd line usage string */
#define CONFIG_COMMAND_LINE_USAGE \
  "[ -p port ] [ -s send-port ] [ -r threads ] [ -t trusted ] [ -6] [ -h ] [ -V ]"

/* Command line arguments help list.
 * Starts with the usage, and followed by each argument description.
 */
#define CONFIG_COMMAND_LINE_HELP \
  "  -p, --port      The port to listen on.\n"\
  "  -s, --send-port The port to send notifications on.\n"\
  "  -r, --threads   The number of threads to create in the threadpool.\n"\
  "  -t, --trusted   A file containing trusted publishers.\n"\
  "  -6, --ipv6      Enforce IPv6 usage.\n"\
  "  -h, --help      Show this message and exit.\n"\
  "  -V, --version   Show app version and exit.\n"

#endif /* EZNOT_CONFIG_CONSTANTS */
