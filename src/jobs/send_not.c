#include "jobs/send_not.h"
#include "config/config.h"

#include "log.h"
#include "uv.h"

#include <pthread.h>
#include <stdbool.h>

#include <sys/socket.h>

/******************************************************************************/

static bool inited = false;
static int send_socket4, send_socket6;
static pthread_cond_t not_ready = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static const app_config_t *app_config;

/******************************************************************************/

int
eznot_init_send_not_job(const app_config_t *conf)
{
	pthread_mutex_lock(&lock);
	log_trace("eznot_init_send_not_job()");

	if (inited) {
		log_error("Notification sender job is already initialzed.");
		pthread_mutex_unlock(&lock);
		return -1;
	}

	send_socket4 = socket(AF_INET, SOCK_DGRAM, 0);
	send_socket6 = socket(AF_INET6, SOCK_DGRAM, 0);
	app_config = conf;

	if (send_socket4 < 0 || send_socket6 < 0) {
		log_error("Could not create send sockets!");
		pthread_mutex_unlock(&lock);
		return -1;
	}

	pthread_mutex_unlock(&lock);
	return 0;
}

/******************************************************************************/

void
eznot_send_not_job(void* payload)
{
	pthread_mutex_lock(&lock);
	log_trace("thread %s: eznot_send_not_job()", pthread_self());

	if (payload == NULL) {
		log_warn("Passed NULL for payload, skipping.");
		pthread_mutex_unlock(&lock);
		return;
	}

	send_not_payload_t* p = payload;
	if (!p->subscriber || !p->not_data || !p->start || !p->refcount) {
		log_warn("Malformed payload passed, skipping.");
		pthread_mutex_unlock(&lock);
		return;
	}

	while (!*(p->start)) {
		pthread_cond_wait(&not_ready, &lock);
	}

	log_debug("Sending notification to: %s", p->subscriber->address);
	pthread_mutex_unlock(&lock);

	int sent;
	switch (p->subscriber->endpoint.ss_family) {
	case AF_INET:
		sent = sendto(send_socket4,
					  p->not_data,
					  REQUEST_MESSAGE_PAYLOAD_DATA_SIZE,
					  0,
					  (struct sockaddr*)&p->subscriber->endpoint,
					  sizeof(struct sockaddr_in));
		break;

	case AF_INET6:
		sent = sendto(send_socket6,
					  p->not_data,
					  REQUEST_MESSAGE_PAYLOAD_DATA_SIZE,
					  0,
					  (struct sockaddr*)&p->subscriber->endpoint,
					  sizeof(struct sockaddr_in6));
		break;

	default:
		log_error("Unsupported address family");
	}

	if (sent < 0) {
		log_warn("Could not sent notification to client %s",
				 p->subscriber->address);
	}

	/* Free the payload if the refcount is equal or less than 0 */
	if (--*(p->refcount) <= 0) {
		pthread_mutex_lock(&lock);

		log_debug("Cleaning send notification job payload");
		free(p->not_data);
		free(p->start);
		free(p->refcount);
		free(p);

		pthread_mutex_unlock(&lock);
	} else {
		free(p);
	}
}

/******************************************************************************/

void
eznot_send_not_job_ready()
{
	pthread_cond_broadcast(&not_ready);
}

/******************************************************************************/

void
eznot_destroy_send_not_job()
{
	pthread_mutex_lock(&lock);
	log_trace("eznot_destroy_send_not_job()");
	log_debug("Closing send sockets...");

	shutdown(send_socket4, SHUT_RDWR);
	shutdown(send_socket6, SHUT_RDWR);
	pthread_mutex_unlock(&lock);
}

/******************************************************************************/
