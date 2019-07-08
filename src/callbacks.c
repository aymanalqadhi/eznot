#include "address.h"
#include "callbacks.h"
#include "memory.h"
#include "publishers.h"
#include "request_message.h"
#include "subscribers.h"

#include "jobs/send_not.h"
#include "jobs_runner.h"

#include "log.c"
#include "uthash.h"
#include "uv.h"

#include <stdlib.h>
#include <string.h>

/******************************************************************************/

static int
get_ip_str(const struct sockaddr* sa, char* s, size_t maxlen);

/******************************************************************************/

static void
handle_publish_requests(const request_message_t* req,
						uv_udp_t* handle,
						const struct sockaddr* addr);

static void
handle_subscribe_requests(const request_message_t* req,
						  uv_udp_t* handle,
						  const struct sockaddr* addr);

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

		/* Handle requests */
		switch (req.header.message_type) {
		case PUBLISH_MESSAGE:
			handle_publish_requests(&req, handle, from);
			goto clean_exit;
		case SUBSCRIBE_MESSAGE:
			handle_subscribe_requests(&req, handle, from);
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

static void
handle_publish_requests(const request_message_t* req,
						uv_udp_t* handle,
						const struct sockaddr* from)
{
	log_trace("handle_publish_requests()");

	/* Get the ip address string */
	char fromaddr[INET6_ADDRSTRLEN];
	if (get_ip_str(from, fromaddr, INET6_ADDRSTRLEN) != 0) {
		log_error("Could not get ip address of client.");
		return;
	}

	/* Check if this is a valid publisher */
	if (!eznot_is_valid_publisher(fromaddr)) {
		log_warn("Got a publish request from an unauthorized client %s",
				 fromaddr);
		return;
	}

	/* Check if the tags are valid */
	if (!eznot_are_valid_tags((char*)req->payload.tags,
							  REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE)) {
		log_warn("Got malformed tags in request from client %s", fromaddr);
		return;
	}

	subscriber_t *subscribers = eznot_get_subscribers(), *s, *tmp;
	if (req->header.flags & EZNOT_REQUEST_BROADCAST) {
		/* Create a data buffer */
		char* data = xmalloc(REQUEST_MESSAGE_TOTAL_SIZE);
		memcpy(data, req->payload.data, REQUEST_MESSAGE_TOTAL_SIZE);
		/* Create state variables */
		bool* start = xmalloc(sizeof(*start));
		int* refcount = xmalloc(sizeof(*refcount));
		/* Set state variables values */
		*start = false;
		*refcount = 0;

		if (strlen((char*)req->payload.tags) > 0) {
			/* To all subscribers */
			HASH_ITER(hh, subscribers, s, tmp)
			{
				send_not_payload_t* payload = xmalloc(sizeof(*payload));
				payload->subscriber = s;
				payload->not_data = data;
				payload->start = start;
				payload->refcount = refcount;

				eznot_enqueue_job(&eznot_send_not_job, payload);
				++(*refcount);
			}
		} else {
			/* TODO: Implement this */
		}

		*start = true;
		eznot_send_not_job_ready();
	} else {
		/* TODO: Implement this */
		HASH_ITER(hh, subscribers, s, tmp) {}
	}
}

/******************************************************************************/

static void
handle_subscribe_requests(const request_message_t* req,
						  uv_udp_t* handle,
						  const struct sockaddr* from)
{
	log_trace("handle_subscribe_requests()");

	/* Get the ip address string */
	char fromaddr[INET6_ADDRSTRLEN];
	if (get_ip_str(from, fromaddr, INET6_ADDRSTRLEN) != 0) {
		log_error("Could not get ip address of client.");
		return;
	}

	/* Check if the client has subscribed before */
	if (eznot_get_subscriber(fromaddr) != NULL) {
		/* TODO: replay to client with message */
		log_warn("Client %s tries to subscribe again.", fromaddr);
		return;
	}

	/* Check if the tags are valid */
	if (!eznot_are_valid_tags((char*)req->payload.tags,
							  REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE)) {
		log_warn("Got malformed tags in request from client %s", fromaddr);
		return;
	}

	/* Add subscriber */
	if (eznot_add_subscriber((struct sockaddr_storage*)from,
							 (char *)req->payload.tags,
							 REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE) == 0) {
		log_info("Client %s has subscribed successfully.", fromaddr);
	} else {
		log_error("Cound not subscribe client %s.", fromaddr);
	}
}

/******************************************************************************/
