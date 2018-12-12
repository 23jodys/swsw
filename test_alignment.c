#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmocka.h>
#include "swsw.h"

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
	PairAlignment* pa = pair_alignment_create(10);
	assert_true(pa);
	pair_alignment_free(&pa);
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

	// Verify that the internal pointers to arrays for storing the alignment
	// are set to NULL, as well.
	// Normally these cause issues with valgrind to throw invalid reads of size 8
	// but we supress these normally, see valgrind_suppressions.supp
	assert_null(*s1);
	assert_null(*s2);
}

/**
 * @brief Given that we create and free many pair alignments, verify that we do not crash
 */
static void test_alignment_free_many(void **state) {
	for (int i=0; i < 1000; i++) {
		PairAlignment* pa = pair_alignment_create(i + 1);
		pair_alignment_free(&pa);
	}
}

/**
 * @brief Given that we ask to retrieve the reference alignment, verify that it is returned correctly
 */
static void test_pair_alignment_get_reference_e2e(void **state) {
	PairAlignment* test_alignment = pair_alignment_create(20);

	assert_non_null(test_alignment);

	test_alignment->s1 = sdscat(test_alignment->s1, "        CGAAAGTTTTGC");
	test_alignment->s2 = sdscat(test_alignment->s2, "          AAA TTTT C");

	printf("before: %s\n", test_alignment->s1);

	sds observed_reference = pair_alignment_get_reference(test_alignment);

	assert_string_equal(observed_reference, "          AAA TTTT C");

	pair_alignment_free(&test_alignment);
}


int main(void) {
	const struct CMUnitTest tests[] = {
		//cmocka_unit_test(test_alignment_create),
		//cmocka_unit_test(test_alignment_free),
		//cmocka_unit_test(test_pa_too_many_prepend),
		//cmocka_unit_test(test_alignment_free_many),
		//cmocka_unit_test(test_pair_alignment_get_reference_e2e),
	};
	int result = cmocka_run_group_tests(tests, NULL, NULL);
}
