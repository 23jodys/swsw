#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>

#include <cmocka.h>

#include "alignment.h"

static void test_alignment_create(void **state) {
	(void) state; /* unused */
	PairAlignment * pa = pair_alignment_create(10);
	assert_true(pa);
}

static void test_alignment_free(void **state) {
	(void) state; /* unused */
	PairAlignment * pa = pair_alignment_create(10);
	pair_alignment_free(&pa);
	assert_null(pa);
	assert_null(pa->s2);

}


int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_alignment_create),
		cmocka_unit_test(test_alignment_free),
	};
	int result = cmocka_run_group_tests(tests, NULL, NULL);
}
