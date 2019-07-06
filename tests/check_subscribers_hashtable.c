#include "check.h"
#include "subscribers.h"
#include "address.h"

#include "uv.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>

static union ip_address addr;

static int print_subscriber(subscriber_t *s)
{
	log_debug("-----------------------------");
	log_debug("Address: %s", s->address);
	log_debug("-----------------------------");

	return 0;
}

START_TEST(test_subscribers_hashtable)
{
	const char *test_address = "127.0.0.1";
	const unsigned short port = 3434;
	subscriber_t *s;

	memset(&addr, 0, sizeof(addr));

	addr.addrv4.sin_family = AF_INET;
	addr.addrv4.sin_port = htons(port);

	ck_assert(uv_inet_pton(AF_INET, test_address, &(addr.addrv4.sin_addr)) >= 0);
	ck_assert(addr.addrv4.sin_family == addr.storage.ss_family);

	ck_assert(eznot_add_subscriber(&addr.storage, NULL, 0) == 0);
	ck_assert(eznot_subscribers_count() == 1);

	ck_assert(eznot_iterate_subscribers(&print_subscriber) == 1);

	ck_assert((s = eznot_get_subscriber(test_address)) != NULL);
	struct sockaddr_in *saddr = (struct sockaddr_in *)&s->endpoint;

	ck_assert(strcmp(s->address, test_address) == 0);
	ck_assert(saddr->sin_port == htons(port));
	ck_assert(s->endpoint.ss_family == AF_INET);
}
END_TEST

Suite*
subscribers_hashtable_suite(void)
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("Request Message");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_subscribers_hashtable);
	suite_add_tcase(s, tc_core);

	return s;
}

int
main(void)
{
	int number_failed;
	Suite* s;
	SRunner* sr;

	s = subscribers_hashtable_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
