#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define UNIT_TESTING 1

#include <cmocka.h>


#include "alignment.h"

/**
 * @brief Given that we call pair_alignment_prepend more times that we have allocated, verify that we do not crash 
 */
static void test_pa_too_many_prepend(void **state) {
	(void) state; /* unused */
	PairAlignment * pa = pair_alignment_create(4);
	int i = 0;
	PairAlignmentError error = {.success=true, .error_number=0};
	while(i < 1000) {
		PairAlignmentError error = pair_alignment_prepend(pa, 'A', 'A');
		if (!error.success) {
			break;
		}
		i += 1;
	}
	pair_alignment_free(&pa);
}

/**
 * @brief Given that we call pair_alignment_create, verify that we don't crash.
 */
static void test_alignment_create(void **state) {
	(void) state; /* unused */
	PairAlignment * pa = pair_alignment_create(10);
	assert_true(pa);
}

/** 
 * @brief Given that we've created a pair alignment structure, verify that
 * after it has been freed, all pointers have been set to NULL
 */
static void test_alignment_free(void **state) {
	PairAlignment * pa = pair_alignment_create(10);
	// Keep pointers to the sequence arrays
	char** s1 = &(pa->s1);
	char** s2 = &(pa->s2);
	// Confirm that they were properly allocated
	assert_non_null(*s1);
	assert_non_null(*s2);
	assert_non_null(pa);

	pair_alignment_free(&pa);

	// Verify that all pointers have been set to NULL
	assert_null(pa);
	assert_null(*s1);
	assert_null(*s2);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_alignment_create),
		cmocka_unit_test(test_alignment_free),
		cmocka_unit_test(test_pa_too_many_prepend),
	};
	int result = cmocka_run_group_tests(tests, NULL, NULL);
}
