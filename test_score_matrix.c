#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define UNIT_TESTING 1
#include <cmocka.h>
#include "sds/sds.h"
#include "swsw.h"

/* Simple function to replace malloc for unit testing */
void* null_malloc(size_t size) {
	return NULL;
}

/* Simple function to replace calloc for unit testing */
void* null_calloc(size_t count, size_t size) {
	return NULL;
}

static void test_add_freed_matrix_fails(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	score_matrix_free(&s);
	assert_null(s);
	Score score = {.value=10,.direction=0};
	ScoreMatrixError result = score_matrix_add(s, 0, 0, score);
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
	Score score = {.value=55, .direction=0};
	ScoreMatrixError result = score_matrix_add(s, 10, 0, score);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_add_s2_to_large(void **state) {
	/* Given that the s2 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=55, .direction=0};
	ScoreMatrixError result = score_matrix_add(s, 0, 10, score);
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
	Score score = {.value=55, .direction=0};
	ScoreMatrixError result = score_matrix_add(s, -10, 0, score);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_add_negative_s2(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=55, .direction=DirNone};
	ScoreMatrixError result = score_matrix_add(s, 0, -10, score);
	assert_false(result.success);
	score_matrix_free(&s);
}

static void test_round_trip_middle(void **state) {
	/* Given that the matrix was initialized and a value added to a position,
	 * verify that that value can be retrieved from the middle of the matrix*/
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=999, .direction=DirNone};
	ScoreMatrixError add_result= score_matrix_add(s, 5, 5, score);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 5, 5);
	assert_true(get_result.success);
	assert_int_equal(get_result.score.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the north west corner.
 */
static void test_round_trip_nw(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=999, .direction=DirNone};
	ScoreMatrixError add_result= score_matrix_add(s, 0, 0, score);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 0, 0);
	assert_true(get_result.success);
	assert_int_equal(get_result.score.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the west corner.
 */
static void test_round_trip_sw(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=999, .direction=DirNone};
	ScoreMatrixError add_result= score_matrix_add(s, 9, 0, score);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 9, 0);
	assert_true(get_result.success);
	assert_int_equal(get_result.score.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the south east corner.
 */
static void test_round_trip_se(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=999, .direction=DirNone};
	ScoreMatrixError add_result= score_matrix_add(s, 9, 9, score);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 9, 9);
	assert_true(get_result.success);
	assert_int_equal(get_result.score.value, 999);
	score_matrix_free(&s);
}

/**
 * @brief Given that we have a matrix, verify that we can store and retrieve a value from the north east corner.
 */
static void test_round_trip_ne(void** state) {
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=999, .direction=DirNone};
	ScoreMatrixError add_result= score_matrix_add(s, 0, 9, score);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 0, 9);
	assert_true(get_result.success);
	assert_int_equal(get_result.score.value, 999);
	score_matrix_free(&s);
}

static void test_create_add_free(void **state) {
	/* Give that we create a matrix and add a value to it, confirm that after it is freed we cannot retrieve results */
	(void) state;
	ScoreMatrix * s = score_matrix_create(10, 10);
	Score score = {.value=999, .direction=DirNone};
	ScoreMatrixError add_result= score_matrix_add(s, 5, 5, score);
	/* Should work because this is a valid matrix */
	ScoreMatrixResult get_result = score_matrix_get(s, 5, 5);
	assert_true(get_result.success);
	assert_int_equal(get_result.score.value, 999);
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
		Score score = {.value=i*10, .direction=DirNone};
		score_matrix_add(matrices[i], 57, 57, score);
	}

	for (int i=0; i < 200; i++) {
		ScoreMatrixResult result = score_matrix_get(matrices[i], 57, 57);
		assert_true(result.success);
		assert_int_equal(result.score.value, i * 10);
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
		Score score = {.value=s1, .direction=DirNone};
		ScoreMatrixError r = score_matrix_add(s, s1, 0, score);
		if (s1 < 10000) {
			assert_true(r.success);
			ScoreMatrixResult r = score_matrix_get(s, s1, 0);
			assert_int_equal(r.score.value, s1);
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
		Score score = {.value=s2, .direction=DirNone};
		ScoreMatrixError e = score_matrix_add(s, 0, s2, score);
		if (s2 < 10000) {
			assert_true(e.success);
			ScoreMatrixResult r = score_matrix_get(s, 0, s2);
			assert_int_equal(r.score.value, s2);
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
			Score score = {.value = s1 + s2, .direction = DirNone};
			ScoreMatrixError e = score_matrix_add(s, s1, s2, score);
			assert_true(e.success);
		}
	}
	for (size_t s1 = 0; s1 < 10000; s1++) {
		for (size_t s2 = 0; s2 < 10000; s2++) {
			ScoreMatrixResult r = score_matrix_get(s, s1, s2);
			assert_true(r.success);
			assert_int_equal(r.score.value, s1 + s2);
		}
	}
}

#undef calloc
/** 
 * @brief given that malloc will return NULL, verify that _alloc returns NULL 
 */
static void test_score_matrix_alloc_null_malloc(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_alloc(10, 10, null_malloc, calloc);
	assert_null(s);
}
#define calloc test_calloc

#undef malloc
/**
 * @brief Given that calloc will return NULL, verify that the ScoreMatrix structure indicates this
 */
static void test_score_matrix_alloc_null_calloc(void **state) {
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_alloc(10, 10, malloc, null_calloc);
	assert_false(s->success);
}
#define malloc test_malloc


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

	const struct CMUnitTest alloc_tests[] = {
		cmocka_unit_test(test_score_matrix_alloc_null_malloc),
		cmocka_unit_test(test_score_matrix_alloc_null_calloc)
	};

	char* round_trip_env = getenv("TEST_ROUND_TRIP");
	char* interface_env = getenv("TEST_INTERFACE");
	char* general_env = getenv("TEST_GENERAL");
	char* alloc_env = getenv("TEST_ALLOC");

	int round_trip_result = 0; 
	int interface_result = 0; 
	int general_result = 0;
	int alloc_result = 0;

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
		if (alloc_env != NULL) {
			run_group(alloc);
		}
	} else {
		run_group(round_trip);
		run_group(interface);
		run_group(general);
		run_group(alloc);
	}
	return round_trip_result + interface_result + general_result + alloc_result;
}
