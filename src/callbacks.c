#include "callbacks.h"
#include "memory.h"

#include "log.c"
#include "uv.h"

#include <stdlib.h>

/******************************************************************************/

void
eznot_on_recv(uv_udp_t* handle,
			  ssize_t nread,
			  const uv_buf_t *buff,
			  const struct sockaddr* from,
			  unsigned int flags)
{
	if (nread < 0) {
		log_error("eznot_on_recv: %s", uv_strerror(nread));
	} else if (nread > 0) {
		fwrite(buff->base, sizeof(char), nread, stdout);
		putchar('\n');
	}

	free(buff->base);
}

/******************************************************************************/

void
eznot_on_alloc(uv_handle_t* client, size_t suggested_size, uv_buf_t *buff)
{
	log_trace("eznot_on_alloc()");

	buff->base = xmalloc(suggested_size);
	buff->len  = suggested_size;

	log_debug("Allocated %lu bytes in %p.", suggested_size, buff->base);
}

/******************************************************************************/
