#ifndef EZNOT_CALLBACKS_H
#define EZNOT_CALLBACKS_H

#include "uv.h"

void
eznot_on_recv(uv_udp_t*,
			  ssize_t,
			  const uv_buf_t*,
			  const struct sockaddr*,
			  unsigned int);

void
eznot_on_alloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf);

#endif /* end of include guard: EZNOT_CALLBACKS_H */
