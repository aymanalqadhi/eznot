#include "callbacks.h"
#include "memory.h"
#include "request_message.h"

#include "log.c"
#include "uv.h"

#include <stdlib.h>

/******************************************************************************/

void
eznot_on_recv(uv_udp_t* handle,
              ssize_t nread,
              const uv_buf_t* buff,
              const struct sockaddr* from,
              unsigned int flags)
{
	/* TODO:
	 * 1. Write a real implementation
	 * 2. Handle invalid requests
	 * 3. Replay to requests
	 */

	if (nread < 0) {
		log_error("eznot_on_recv: %s", uv_strerror(nread));
	} else if (nread >= REQUEST_MESSAGE_TOTAL_SIZE) {
		request_message_t req;
		int ret = eznot_decode_request_message(
		    buff->base, REQUEST_MESSAGE_TOTAL_SIZE, &req);

		if (ret == REQUEST_MESSAGE_TOTAL_SIZE) {
			log_debug("------------------------------------");
			log_debug("Got a message!");
			log_debug("------------------------------------");
			log_debug("msg.header.message_type = %d", req.header.message_type);
			log_debug("msg.header.flags        = %d", req.header.flags);
			log_debug("msg.header.__hchecksum  = %d", req.header.__hchecksum);
			log_debug("msg.header.__pchecksum  = %d", req.header.__pchecksum);
			log_debug("------------------------------------");
			log_debug("msg.payload.tags = %s", req.payload.tags);
			log_debug("msg.payload.data = %s", req.payload.data);
			log_debug("------------------------------------");
		}
	}

	free(buff->base);
}

/******************************************************************************/

void
eznot_on_alloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buff)
{
	log_trace("eznot_on_alloc()");

	buff->base = xmalloc(suggested_size);
	buff->len = suggested_size;

	log_debug("Allocated %lu bytes in %p.", suggested_size, buff->base);
}

/******************************************************************************/
