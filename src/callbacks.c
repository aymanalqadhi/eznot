#include "address.h"
#include "callbacks.h"
#include "memory.h"
#include "request_message.h"
#include "subscribers.h"
#include "publishers.h"

#include "log.c"
#include "uv.h"

#include <stdlib.h>

/******************************************************************************/

static int
get_ip_str(const struct sockaddr* sa, char* s, size_t maxlen);

/******************************************************************************/

void
eznot_on_recv(uv_udp_t* handle,
			  ssize_t nread,
			  const uv_buf_t* buff,
			  const struct sockaddr* from,
			  unsigned int flags)
{
	/* TODO:
	 * 1. Write a real implementation (20%)
	 * 2. Handle invalid requests (50%)
	 * 3. Replay to requests
	 */

	if (nread < 0) {
		log_error("eznot_on_recv: %s", uv_strerror(nread));
	} else if (nread >= REQUEST_MESSAGE_TOTAL_SIZE) {
		request_message_t req;
		int ret = eznot_decode_request_message(
			buff->base, REQUEST_MESSAGE_TOTAL_SIZE, &req);

		if (ret != REQUEST_MESSAGE_TOTAL_SIZE) {
			/* TODO: Handle invalid request */
			goto clean_exit;
		}

		char fromaddr[INET6_ADDRSTRLEN];
		if (get_ip_str(from, fromaddr, INET6_ADDRSTRLEN) != 0) {
			/* TODO: Handle error */
			goto clean_exit;
		}

		switch (req.header.message_type) {
		case PUBLISH_MESSAGE:
			goto clean_exit;
		case SUBSCRIBE_MESSAGE:
			goto clean_exit;
		}
	}

clean_exit:
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

static int
get_ip_str(const struct sockaddr* sa, char* s, size_t maxlen)
{
	switch (sa->sa_family) {
	case AF_INET:
		inet_ntop(AF_INET, &(((struct sockaddr_in*)sa)->sin_addr), s, maxlen);
		break;

	case AF_INET6:
		inet_ntop(
			AF_INET6, &(((struct sockaddr_in6*)sa)->sin6_addr), s, maxlen);
		break;

	default:
		return -1;
	}

	return 0;
}

/******************************************************************************/
