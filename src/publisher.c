#include "memory.h"
#include "publishers.h"

#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/

static publisher_t* publishers = NULL;
static int publishers_count = 0;

/******************************************************************************/

int
eznot_add_publisher(const char* name)
{
	log_trace("eznot_add_publisher()");

	int len;
	if (name == NULL) {
		log_error("Publisher name cannot be NULL.");
		return -1;
	}

	len = strlen(name);
	if (len <= 0 || len > INET6_ADDRSTRLEN) {
		log_error("Invalid publisher name length, got %d.", len);
		return -1;
	}

	publisher_t* pub = NULL;
	HASH_FIND_STR(publishers, name, pub);

	if (pub) {
		log_error("Publisher has been already added before.");
		return -1;
	}

	pub = xmalloc(sizeof(*pub));
	memset(pub, 0, sizeof(*pub));

	strncpy(pub->name, name, INET6_ADDRSTRLEN);
	HASH_ADD_STR(publishers, name, pub);
	++publishers_count;

	return 0;
}

/******************************************************************************/

bool
eznot_is_valid_publisher(const char* name)
{
	log_trace("eznot_is_valid_publisher()");

	if (name == NULL || strlen(name) <= 0 || strlen(name) > INET6_ADDRSTRLEN) {
		log_error("Invalid name was given.");
		return false;
	}

	publisher_t* pub = NULL;
	HASH_FIND_STR(publishers, name, pub);

	return pub != NULL;
}

/******************************************************************************/

int
eznot_publishers_count()
{
	log_trace("eznot_publishers_count()");
	return publishers_count;
}

/******************************************************************************/

int
eznot_init_publishers_hashtable(const app_config_t* conf)
{
	log_trace("eznot_load_publishers()");

	if (conf->trusted_publishers_file == NULL) {
		log_error("Cannot use NULL as a source file!");
		return -1;
	}

	char namebuf[INET6_ADDRSTRLEN];
	size_t len;

	while (fgets(namebuf, INET6_ADDRSTRLEN, conf->trusted_publishers_file)) {
		len = strlen(namebuf);

		/* Ignore empty entries */
		if (len <= 0) {
			continue;
		}

		namebuf[strcspn(namebuf, "\n")] = 0;
		if (eznot_add_publisher(namebuf) != 0) {
			log_error("Could not add publisher %s.", namebuf);
			return -1;
		}
	}

	return 0;
}

/******************************************************************************/

void
eznot_destroy_publishers_hashtable()
{
	log_trace("eznot_destroy_publishers_hashtable()");

	publisher_t *pub, *tmp;
	HASH_ITER(hh, publishers, pub, tmp) {
		HASH_DELETE(hh, publishers, pub);
		free(pub);
	}

	publishers = NULL;
}

/******************************************************************************/
