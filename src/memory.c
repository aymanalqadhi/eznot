#include "memory.h"

#include "log.h"

#include <stddef.h>
#include <stdlib.h>

/******************************************************************************/

void*
xmalloc(size_t size)
{
	log_trace("xmalloc()");

	void* ret = malloc(size);
	if (!ret) {
		log_fatal("Could not allocate memory!");
		exit(EXIT_FAILURE);
	}

	log_debug("Allocated %d bytes in %p", size, ret);
	return ret;
}

/******************************************************************************/
