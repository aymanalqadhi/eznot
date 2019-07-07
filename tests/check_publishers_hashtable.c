#include "check.h"
#include "publishers.h"

#include <stdlib.h>
#include <string.h>

START_TEST(test_publishers_hashtable)
{
	const char *valid1 = "ayman";
	const char *valid2 = "192.168.1.1";
	const char *valid3 = "1200:0000:AB00:1234:0000:2552:7777:1313";
	const char *valid4 = "a trusted publisher";
	const char *valid5 = "trust-this";

	ck_assert(eznot_add_publisher(valid1) == 0);
	ck_assert(eznot_add_publisher(valid2) == 0);
	ck_assert(eznot_add_publisher(valid3) == 0);
	ck_assert(eznot_add_publisher(valid4) == 0);
	ck_assert(eznot_add_publisher(valid5) == 0);

	ck_assert(eznot_publishers_count() == 5);

	const char *invalid1 = NULL;
	const char *invalid2 = "";
	const char *invalid3 = "12010:20232000:AB2300:1234:00000:2552:7777:1313";

	ck_assert(eznot_add_publisher(invalid1) != 0);
	ck_assert(eznot_add_publisher(invalid2) != 0);
	ck_assert(eznot_add_publisher(invalid3) != 0);

	ck_assert(eznot_publishers_count() == 5);

	ck_assert(eznot_is_valid_publisher(valid1));
	ck_assert(eznot_is_valid_publisher(valid2));
	ck_assert(eznot_is_valid_publisher(valid3));
	ck_assert(eznot_is_valid_publisher(valid4));
	ck_assert(eznot_is_valid_publisher(valid5));

	ck_assert(!eznot_is_valid_publisher(invalid1));
	ck_assert(!eznot_is_valid_publisher(invalid1));
	ck_assert(!eznot_is_valid_publisher(invalid1));
}
END_TEST

Suite*
publishers_hashtable_suite(void)
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("Request Message");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_publishers_hashtable);
	suite_add_tcase(s, tc_core);

	return s;
}

int
main(void)
{
	int number_failed;
	Suite* s;
	SRunner* sr;

	s = publishers_hashtable_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
