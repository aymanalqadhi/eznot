#include "check.h"
#include "services_init.h"
#include "config/config.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
dummy_destroy()
{}

static int
good_svc_init(const app_config_t* config)
{
	return 0;
}

static int
bad_svc_init()
{
	return -1;
}

START_TEST(test_services_initer)
{
	app_config_t dummy_conf;

	eznot_add_service("Valid service 1", &good_svc_init, &dummy_destroy, true);
	eznot_add_service("Valid service 2", &good_svc_init, &dummy_destroy, false);
	eznot_add_service("Valid service 3", &bad_svc_init, &dummy_destroy, false);

	printf("xd1");
	ck_assert(eznot_init_services(&dummy_conf) == 0);
	printf("xd2");

	eznot_destroy_services();
	eznot_add_service("Valid service 1", &good_svc_init, &dummy_destroy, true);
	eznot_add_service("Valid service 2", &good_svc_init, &dummy_destroy, false);
	eznot_add_service("Invalid service 3", &bad_svc_init, &dummy_destroy, true);

	ck_assert(eznot_init_services(&dummy_conf) != 0);
}
END_TEST

Suite*
services_initer_suite(void)
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("Services Intializer");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_services_initer);
	suite_add_tcase(s, tc_core);

	return s;
}

int
main(void)
{
	int number_failed;
	Suite* s;
	SRunner* sr;

	s = services_initer_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
