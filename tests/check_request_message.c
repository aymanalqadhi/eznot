#include "check.h"
#include "request_message.h"

#include <stdlib.h>
#include <string.h>


START_TEST(test_are_valid_tags)
{
	const char *valid_tags1 = "test,tag-1,test_3,test-xxxxxx-last";
	const char *valid_tags2 = "these,are,valid,tags";
	const char *valid_tags3 = "__,_,__,--,tags,--";
	const char *valid_tags4 = NULL;

	const char *invalid_tags1 = "th!s,is,a-tag";
	const char *invalid_tags2 = "a malicous code with spaces";
	const char *invalid_tags3 = "1 + 1 = 2";
	const char *invalid_tags4 = "";

	ck_assert(eznot_are_valid_tags(valid_tags1, strlen(valid_tags1)));
	ck_assert(eznot_are_valid_tags(valid_tags2, strlen(valid_tags2)));
	ck_assert(eznot_are_valid_tags(valid_tags3, strlen(valid_tags3)));
	ck_assert(eznot_are_valid_tags(valid_tags4, 0));

	ck_assert(!eznot_are_valid_tags(invalid_tags1, strlen(invalid_tags1)));
	ck_assert(!eznot_are_valid_tags(invalid_tags2, strlen(invalid_tags2)));
	ck_assert(!eznot_are_valid_tags(invalid_tags3, strlen(invalid_tags3)));
	ck_assert(!eznot_are_valid_tags(invalid_tags4, strlen(invalid_tags4)));
}
END_TEST

START_TEST(test_request_message_encode)
{
	request_message_t msg;
	memset(&msg, 0, sizeof(msg));

	/* Test encoding */
	const char* tags = "these,are,test,flags,separated,with,commas";
	const char* data = "This is the notification message!";
	uint8_t type = PUBLISH_MESSAGE;
	uint8_t flags = 123;

	msg.header.message_type = type;
	msg.header.flags = flags;
	strcpy((char*)msg.payload.tags, tags);
	strcpy((char*)msg.payload.data, data);

	char encoded_buffer[REQUEST_MESSAGE_TOTAL_SIZE];
	int rslt = eznot_encode_request_message(
	    &msg, encoded_buffer, REQUEST_MESSAGE_TOTAL_SIZE);

	/* Check return value */
	ck_assert_msg(rslt == REQUEST_MESSAGE_TOTAL_SIZE,
	              "eznot_encode_request_message() must return %d, got %d",
	              REQUEST_MESSAGE_TOTAL_SIZE,
	              rslt);

	/* Check the big endian conversion macro */
	ck_assert(123 == HTOBE16(HTOBE16(123)));
	ck_assert(0xF0FFF0F0FF == HTOBE32(HTOBE32(0xF0FFF0F0FF)));

	/* Check header values */
	ck_assert(encoded_buffer[0] == type);
	ck_assert(encoded_buffer[1] == flags);

	/* Get big endian checksums */
	uint16_t hchecksum = *(uint16_t*)(encoded_buffer + 2);
	uint32_t pchecksum = *(uint32_t*)(encoded_buffer + 4);
	ck_assert_msg(HTOBE16(hchecksum) == type + flags,
	              "Expected value: %d, got %d.",
	              HTOBE16(hchecksum),
	              type + flags);
	rslt = eznot_calculate_checksum(
	    (const uint8_t*)(encoded_buffer + REQUEST_MESSAGE_HEADER_SIZE),
	    REQUEST_MESSAGE_PAYLOAD_SIZE);
	ck_assert_msg(HTOBE32(pchecksum) == rslt,
	              "Expected %d, got %d.",
	              HTOBE32(pchecksum),
	              rslt);

	/* Check payload */
	ck_assert(strncmp(tags,
	                  encoded_buffer + REQUEST_MESSAGE_HEADER_SIZE,
	                  REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE) == 0);
	ck_assert(strncmp(data,
	                  encoded_buffer + REQUEST_MESSAGE_HEADER_SIZE +
	                      REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE,
	                  REQUEST_MESSAGE_PAYLOAD_DATA_SIZE) == 0);
}
END_TEST

START_TEST(test_request_message_decode)
{
	request_message_t msg, decoded;
	memset(&msg, 0, sizeof(msg));

	/* Test encoding */
	const char* tags = "these,are,test,flags,separated,with,commas";
	const char* data = "This is the notification message!";
	uint8_t type = PUBLISH_MESSAGE;
	uint8_t flags = 123;

	msg.header.message_type = type;
	msg.header.flags = flags;
	strcpy((char*)msg.payload.tags, tags);
	strcpy((char*)msg.payload.data, data);

	char encoded_buffer[REQUEST_MESSAGE_TOTAL_SIZE];
	int rslt = eznot_encode_request_message(
	    &msg, encoded_buffer, REQUEST_MESSAGE_TOTAL_SIZE);

	/* Check return value */
	ck_assert_msg(rslt == REQUEST_MESSAGE_TOTAL_SIZE,
	              "eznot_encode_request_message() must return %d, got %d",
	              REQUEST_MESSAGE_TOTAL_SIZE,
	              rslt);
	ck_assert(eznot_decode_request_message(encoded_buffer,
	                                       REQUEST_MESSAGE_TOTAL_SIZE,
	                                       &decoded) == rslt);

	/* Check the header */
	ck_assert_msg(decoded.header.message_type == msg.header.message_type,
	              "Expected: %d, got %d.",
	              msg.header.message_type,
	              decoded.header.message_type);
	ck_assert(decoded.header.flags == msg.header.flags);
	ck_assert_msg(decoded.header.__hchecksum ==
	          msg.header.message_type + msg.header.flags,
			  "Expected: %d, got %d.",
			  decoded.header.__hchecksum,
			  msg.header.message_type + msg.header.flags);
	ck_assert(decoded.header.__pchecksum ==
	          eznot_calculate_checksum(msg.payload.tags, REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE) +
			  eznot_calculate_checksum(msg.payload.data, REQUEST_MESSAGE_PAYLOAD_DATA_SIZE));

	/* Check the payload */
	ck_assert(strncmp(decoded.payload.tags,
	                  msg.payload.tags,
	                  REQUEST_MESSAGE_PAYLOAD_TAGS_SIZE) == 0);
	ck_assert(strncmp(decoded.payload.data,
	                  msg.payload.data,
	                  REQUEST_MESSAGE_PAYLOAD_DATA_SIZE) == 0);
}
END_TEST

Suite*
request_message_suite(void)
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("Request Message");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_request_message_encode);
	tcase_add_test(tc_core, test_request_message_decode);
	tcase_add_test(tc_core, test_are_valid_tags);

	suite_add_tcase(s, tc_core);

	return s;
}

int
main(void)
{
	int number_failed;
	Suite* s;
	SRunner* sr;

	s = request_message_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
