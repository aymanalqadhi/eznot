#include "request_message.h"

#include "log.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/******************************************************************************/

uint32_t
eznot_calculate_checksum(const uint8_t* buff, size_t len)
{
	uint32_t checksum = 0;
	for (int i = 0; i < len; ++i)
		checksum += buff[i];
	return checksum;
}

/******************************************************************************/

int
eznot_encode_request_message(const request_message_t* msg,
                             char* buff,
                             size_t len)
{
	assert(buff != NULL);
	assert(msg != NULL);

	log_trace("eznot_encode_request_message()");
	if (len < UDP_PACKET_SIZE) {
		log_error("Buffer size must be %d, got %d.", UDP_PACKET_SIZE, len);
		return -1;
	}

	/* Zero the msg buffer */
	memset(buff, 0, UDP_PACKET_SIZE);

	/* Calculate checksums */
	uint16_t hchecksum = msg->header.message_type + msg->header.flags;
	uint32_t pchecksum =
	    eznot_calculate_checksum(msg->payload.tags,
	                             REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE) +
	    eznot_calculate_checksum(msg->payload.data,
	                             REQUEST_MESSAGE_PAYLOAD_DATA_SIZE);
	/* Convert checksums to big endian */
	hchecksum = HTOBE16(hchecksum);
	pchecksum = HTOBE32(pchecksum);

	/* Encode header */
	int pos = 0;
	buff[pos++] = msg->header.message_type;
	buff[pos++] = msg->header.flags;

	memcpy(buff + pos, (void*)&hchecksum, sizeof(hchecksum));
	pos += sizeof(hchecksum);
	memcpy(buff + pos, (void*)&pchecksum, sizeof(pchecksum));
	pos += sizeof(pchecksum);

	/* Encode payload */
	memcpy(buff + pos, msg->payload.tags, REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE);
	pos += REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE;
	memcpy(buff + pos, msg->payload.data, REQUEST_MESSAGE_PAYLOAD_DATA_SIZE);
	pos += REQUEST_MESSAGE_PAYLOAD_DATA_SIZE;

	return pos;
}

/******************************************************************************/

int
eznot_decode_request_message(const char* buff,
                             size_t len,
                             request_message_t* msg)
{
	assert(buff != NULL);
	assert(msg != NULL);

	log_trace("eznot_decode_request_message()");
	if (len < UDP_PACKET_SIZE) {
		log_error("Buffer size must be %d, got %d.", UDP_PACKET_SIZE, len);
		return -1;
	}

	/* Decode Header */
	int pos = 0;
	msg->header.message_type = buff[pos++];
	msg->header.flags = buff[pos++];

	/* Decode checksums */
	uint16_t hchecksum;
	uint32_t pchecksum;

	memcpy((void*)&hchecksum, buff + pos, sizeof(hchecksum));
	pos += sizeof(hchecksum);
	memcpy((void*)&pchecksum, buff + pos, sizeof(pchecksum));
	pos += sizeof(pchecksum);

	msg->header.__hchecksum = HTOBE16(hchecksum);
	msg->header.__pchecksum = HTOBE32(pchecksum);

	/* Decode body */
	memcpy(msg->payload.tags, buff + pos, REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE);
	pos += REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE;
	memcpy(msg->payload.data, buff + pos, REQUEST_MESSAGE_PAYLOAD_DATA_SIZE);
	pos += REQUEST_MESSAGE_PAYLOAD_DATA_SIZE;

	return pos;
}

/******************************************************************************/
