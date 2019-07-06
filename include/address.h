#ifndef EZNOT_ADDRESS_H
#define EZNOT_ADDRESS_H

#include "uv.h"

#define EZNOT_IPV6_ANY      "::"
#define EZNOT_IPV4_ANY      "0.0.0.0"
#define EZNOT_IPV6_LOOPBACK "::"
#define EZNOT_IPV4_LOOPBACK "127.0.0.1"

union ip_address {
	struct sockaddr addr;
	struct sockaddr_in addrv4;
	struct sockaddr_in6 addrv6;
	struct sockaddr_storage storage;
};

#endif /* end of include guard: EZNOT_ADDRESS_H */
