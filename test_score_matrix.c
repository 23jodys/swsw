#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define UNIT_TESTING 1
#include <cmocka.h>
#include "sds/sds.h"
#include "swsw.h"


static void test_add_freed_matrix_fails(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	score_matrix_free(&s);
	assert_null(s);
	ScoreMatrixError result = score_matrix_add(s, 0, 0, 10);
	assert_false(result.success);
}

static void test_get_freed_matrix_fails(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	score_matrix_free(&s);
	ScoreMatrixResult result = score_matrix_get(s, 0,0);
	assert_false(result.success);
}

static void test_get_s1_to_large(void **state) {
	/* Given that the s1 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(5, 8);
	ScoreMatrixResult result = score_matrix_get(s, 5, 8);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_get_s2_to_large(void **state) {
	/* Given that the s2 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(s, 0, 10);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_add_s1_to_large(void **state) {
	/* Given that the s1 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError result = score_matrix_add(s, 10, 0, 55);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_add_s2_to_large(void **state) {
	/* Given that the s2 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError result = score_matrix_add(s, 0, 10, 55);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_adds(void **state) {
	/* Given that we attempt to adds a score at a given position sequence */

}

static void test_get_negative_s1(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(s, -10, 0);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_get_negative_s2(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(s, 0, -10);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_add_negative_s1(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError result = score_matrix_add(s, -10, 0, 55);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_add_negative_s2(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError result = score_matrix_add(s, 0, -10, 55);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_round_trip_middle(void **state) {
	/* Given that the matrix was initialized and a value added to a position,
	 * verify that that value can be retrieved from the middle of the matrix*/
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 5, 5, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 5, 5);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the north west corner.
 */
static void test_round_trip_nw(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 0, 0, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 0, 0);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the south west corner.
 */
static void test_round_trip_sw(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 9, 0, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 9, 0);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the south east corner.
 */
static void test_round_trip_se(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 9, 9, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 9, 9);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the north east corner.
 */
static void test_round_trip_ne(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 0, 9, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 0, 9);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	score_matrix_free(&s);
}

static void test_create_add_free(void **state) {
	/* Give that we create a matrix and add a value to it, confirm that after it is freed we cannot retrieve results */
	(void) state;
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 5, 5, 999);
	/* Should work because this is a valid matrix */
	ScoreMatrixResult get_result = score_matrix_get(s, 5, 5);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	/* Free the matrix */
	score_matrix_free(&s);

	/* Verify that the pointer was set to NULL */
	assert_null(s);

	/* Verify that we get a false success */
	ScoreMatrixResult get2_result = score_matrix_get(s, 5, 5);
	assert_false(get2_result.success);
}

static void test_many_matrices(void **state) {
	/* Given that many matrices are created, confirm that values can be added and  retrieved from all of them */
	(void) state; /* unused */
	ScoreMatrix * matrices[200];
	for (int i=0; i < 200; i++) {
		matrices[i] = score_matrix_create(100,100);	
		score_matrix_add(matrices[i], 57, 57, i * 10);
	}

	for (int i=0; i < 200; i++) {
		ScoreMatrixResult result = score_matrix_get(matrices[i], 57, 57);
		assert_true(result.success);
		assert_int_equal(result.value, i * 10);
		score_matrix_free(&(matrices[i]));
	}
}

static void test_printf(void **state) {
	(void) state;

	ScoreMatrix * s = score_matrix_create(10,10);
	char * seq1 = "ABCDEFGHIJ";
	char * seq2 = "ABCDEFGHIJ";
	score_matrix_printf(s, seq1, 9, seq2, 9);
	score_matrix_free(&s);
}


/** 
 * @brief Given that we create a large matrix, verify that all cells in the first column can be filled and that accessing outside the matrix fails
 */
static void test_fill_lh_matrix(void **state) {
	ScoreMatrix* s = score_matrix_create(9999,9999);
	assert_true(s->success);
	for (size_t s1 = 0; s1 < 9999; s1++) {
		DEBUGLOG("count %zu\n", s1); 
		ScoreMatrixError r = score_matrix_add(s, s1, 0, s1);
		if (s1 < 10000) {
			assert_true(r.success);
			ScoreMatrixResult r = score_matrix_get(s, s1, 0);
			assert_int_equal(r.value, s1);
		} else {
			assert_false(r.success);
		}
	}
	score_matrix_free(&s);
}

/** 
 * @brief Given that we create a large matrix, verify that all cells in the first column can be filled and that accessing outside the matrix fails
 */
static void test_fill_rh_matrix(void **state) {
	ScoreMatrix* s = score_matrix_create(9999,9999);
	assert_true(s->success);
	for (size_t s2 = 0; s2 < 9999; s2++) {
		//printf("%s:%d (%s) count: %zu\n", __FILE__, __LINE__, __func__, s2);
		ScoreMatrixError e = score_matrix_add(s, 0, s2, s2);
		if (s2 < 10000) {
			assert_true(e.success);
			ScoreMatrixResult r = score_matrix_get(s, 0, s2);
			assert_int_equal(r.value, s2);
		} else {
			assert_false(e.success);
		}
	}
	score_matrix_free(&s);
}

/**
 * @brief Given that we create a very large matrix, verify that we can fill each cell and retrieve it.
 */
static void test_fill_full_matrix(void **state) {
	ScoreMatrix* s = score_matrix_create(10000,10000);
	assert_true(s->success);
	for (size_t s1 = 0; s1 < 10000; s1++) {
		for (size_t s2 = 0; s2 < 10000; s2++) {
			ScoreMatrixError e = score_matrix_add(s, s1, s2, s1 + s2);
			assert_true(e.success);
		}
	}
	for (size_t s1 = 0; s1 < 10000; s1++) {
		for (size_t s2 = 0; s2 < 10000; s2++) {
			ScoreMatrixResult r = score_matrix_get(s, s1, s2);
			assert_true(r.success);
			assert_int_equal(r.value, s1 + s2);
		}
	}
}

int main(void) {
	// These tests confirm that values can be stored and retrieved correctly.
	const struct CMUnitTest round_trip_tests[] = {
		cmocka_unit_test(test_fill_lh_matrix),
		cmocka_unit_test(test_fill_rh_matrix),
		cmocka_unit_test(test_fill_full_matrix),
		cmocka_unit_test(test_round_trip_nw), 
		cmocka_unit_test(test_round_trip_sw), 
		cmocka_unit_test(test_round_trip_se), 
		cmocka_unit_test(test_round_trip_ne), 
	};

	// These tests confirm that various interfaces perform correctly when given
	// improper values.
	const struct CMUnitTest interface_tests[] = {
		cmocka_unit_test(test_add_negative_s1), 
		cmocka_unit_test(test_add_negative_s2), 
		cmocka_unit_test(test_get_negative_s1), 
		cmocka_unit_test(test_get_negative_s2), 
		cmocka_unit_test(test_get_freed_matrix_fails),
		cmocka_unit_test(test_add_freed_matrix_fails),
		cmocka_unit_test(test_get_s1_to_large), 
		cmocka_unit_test(test_get_s2_to_large), 
		cmocka_unit_test(test_add_s1_to_large), 
		cmocka_unit_test(test_add_s2_to_large), 
	};
	const struct CMUnitTest general_tests[] = {
		cmocka_unit_test(test_printf),
		cmocka_unit_test(test_many_matrices), 
		cmocka_unit_test(test_create_add_free), 
	};


	char* round_trip_env = getenv("TEST_ROUND_TRIP");
	char* interface_env = getenv("TEST_INTERFACE");
	char* general_env = getenv("TEST_GENERAL");

	int round_trip_result = 0; 
	int interface_result = 0; 
	int general_result = 0;

#define run_group(group) group ## _result = cmocka_run_group_tests( group ## _tests , NULL, NULL)

	if (round_trip_env || interface_env || general_env) {
		if (round_trip_env != NULL) {
			run_group(round_trip);
		}

		if (interface_env != NULL) {
			run_group(interface);
		}

		if (general_env != NULL) {
			run_group(general);
		}
	} else {
		run_group(round_trip);
		run_group(interface);
		run_group(general);
	}
	return round_trip_result + interface_result + general_result;
}
