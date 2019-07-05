#ifndef EZNOT_REQUEST_MESSAGE_H
#define EZNOT_REQUEST_MESSAGE_H

#include <stddef.h>
#include <stdint.h>

#ifndef UDP_PACKET_SIZE
#define UDP_PACKET_SIZE 508
#endif

#define REQUEST_MESSAGE_HEADER_SIZE (0x40)
#define REQUEST_MESSAGE_PAYLOAD_SIZE                                           \
	(UDP_PACKET_SIZE - REQUEST_MESSAGE_HEADER_SIZE)
#define REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE 0x40
#define REQUEST_MESSAGE_PAYLOAD_DATA_SIZE                                      \
	(REQUEST_MESSAGE_PAYLOAD_SIZE - REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE)
#define REQUEST_MESSAGE_TOTAL_SZIE                                             \
	(REQUEST_MESSAGE_PAYLOAD_SIZE + REQUEST_MESSAGE_HEADER_SIZE)

enum request_message_type
{
	SUBSCRIBE_MESSAGE = 0,
	PUBLISH_MESSAGE = 1
};

typedef struct request_message_header
{
	uint8_t message_type;
	uint8_t flags;
	uint16_t __hchecksum;
	uint32_t __pchecksum;
} request_message_header_t;

typedef struct request_message_payload
{
	uint8_t tags[REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE];
	uint8_t data[REQUEST_MESSAGE_PAYLOAD_DATA_SIZE];
} request_message_payload_t;

typedef struct request_message
{
	request_message_header_t header;
	request_message_payload_t payload;
} request_message_t;

int
eznot_encode_request_message(const request_message_t* msg,
                             char* buff,
                             size_t len);

int
eznot_decode_request_message(const char* buff,
                             size_t len,
                             request_message_t* msg);

#endif /* end of include guard: EZNOT_REQUEST_MESSAGE_H */
