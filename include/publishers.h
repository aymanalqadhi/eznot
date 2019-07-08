#ifndef EZNOT_PUBLISHERS_H
#define EZNOT_PUBLISHERS_H

#include "address.h"
#include "config/config.h"

#include "uthash.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct publisher
{
	char name[INET6_ADDRSTRLEN];
	UT_hash_handle hh;
} publisher_t;

int
eznot_add_publisher(const char* name);

bool
eznot_is_valid_publisher(const char *name);

int
eznot_publishers_count();

int
eznot_init_publishers_hashtable(const app_config_t* conf);

void
eznot_destroy_publishers_hashtable();

#endif /* end of include guard: EZNOT_PUBLISHERS_H */
