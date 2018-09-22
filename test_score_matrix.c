#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>

#define UNIT_TESTING 1
#include <cmocka.h>

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
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(s, 10, 0);
	assert_false(result.success);
}

static void test_get_s2_to_large(void **state) {
	/* Given that the s2 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(s, 0, 10);
	assert_false(result.success);
}

static void test_add_s1_to_large(void **state) {
	/* Given that the s1 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError result = score_matrix_add(s, 10, 0, 55);
	assert_false(result.success);
}

static void test_add_s2_to_large(void **state) {
	/* Given that the s2 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError result = score_matrix_add(s, 0, 10, 55);
	assert_false(result.success);
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

static void test_add_many(void **state) {
	for (size_t x=1; x < 10; x++) {
		for (size_t y=1; y < 10; y++) {
			ScoreMatrix* m = score_matrix_create(x, y);
			for (size_t s1=0; s1 <= x; s1++) {
				for (size_t s2=0; s2 <= y; s2++) {
					ScoreMatrixError error = score_matrix_add(m, s1, s2, 9999);
					assert_int_equal(error.error_number, 0);
					ScoreMatrixResult result = score_matrix_get(m, s1, s2);
					assert_int_equal(result.error_number, 0);
					assert_int_equal(result.value, 9999);
				}
			}
		}
	}
	
}

static void test_round_trip(void **state) {
	/* Given that the matrix was initialized and a value added to a position,
	 * verify that that value can be retrieved */
	(void) state; /* unused */
	ScoreMatrix * s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(s, 5, 5, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(s, 5, 5);
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

static void test_align(void **state) {
	char* seq1 = "ABCCCCGHIJ";
	char* seq2 = "ABCDEFGHIJ";
	score_matrix_align(seq1, 9, seq2, 9);
}


static void test_traceback(void **state) {
	ScoreMatrix* s = score_matrix_create(10,7);

	char* seq1 = "ABCDEFGHIJ";
	char* seq2 = "ABCDHIJ";

	ScoreConfig score_config;
	score_config.gap      = -1;
	score_config.match    =  3; 
	score_config.mismatch = -3;

	ScoreMatrixError error = score_matrix_score(s, score_config, seq1, 9, seq2, 6);
	score_matrix_printf(s, seq1, 9, seq2, 6);
	PairAlignment* pa = score_matrix_traceback(s, seq1, 9, seq2, 6);
	//assert_non_null(pa);
	score_matrix_free(&s);
	pair_alignment_free(&pa);
	//assert_null(pa);
	
}

static void test_score(void **state) {
	ScoreMatrix * s = score_matrix_create(10,10);
	char * seq1 = "ABCDDDDHIJ";
	char * seq2 = "ABCDEFGHIJ";

	ScoreConfig score_config;
	score_config.gap = 2;
	score_config.match=3; 
	score_config.mismatch=-3;

	ScoreMatrixError error = score_matrix_score(s, score_config, seq1, 9, seq2, 9);
	assert_true(error.success);
	// Confirm that the highest was found at index 9 in both dimensions
	assert_int_equal(s->highest_s1, 9);
	assert_int_equal(s->highest_s2, 9);

	// Confirm one of the scores
	score_matrix_printf(s, seq1, 9, seq2, 9);
	Score result = 0;
	score_matrix_geta(s, 5, 5, result);
	assert_int_equal(result, 19);
	score_matrix_free(&s);
}

static void test_score2(void **state) {
	ScoreMatrix * s = score_matrix_create(3,3);

	char * seq1 = "ABCABCABCABC";
	char * seq2 = "ABCABCABCABC";

	ScoreConfig score_config;
	score_config.gap = 2;
	score_config.match=3; 
	score_config.mismatch=-3;

	ScoreMatrixError error = score_matrix_score(s, score_config, seq1, 5, seq2, 8);
	assert_true(error.success);
	assert_int_equal(s->highest_s1, 2);
	assert_int_equal(s->highest_s2, 2);

	// Confirm one of the scores
	//score_matrix_printf(s, seq1, 3, seq2, 3);
	//score_matrix_get(s, 0, 0);
	//score_matrix_geta(s, 2, 3, result);
	ScoreMatrixResult result = score_matrix_get(s, 2, 2);
	//assert_int_equal(result, 19);
	printf("success: %d, result: %d\n", result.error_number, result.value);
	score_matrix_free(&s);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		//cmocka_unit_test(test_score2),
		cmocka_unit_test(test_add_many),
		//cmocka_unit_test(test_align),
		//cmocka_unit_test(test_printf),
		//cmocka_unit_test(test_get_freed_matrix_fails),
		//cmocka_unit_test(test_add_freed_matrix_fails),
		//cmocka_unit_test(test_get_s1_to_large), 
		//cmocka_unit_test(test_get_s2_to_large), 
		//cmocka_unit_test(test_add_s1_to_large), 
		//cmocka_unit_test(test_add_s2_to_large), 
		//cmocka_unit_test(test_round_trip), 
		//cmocka_unit_test(test_many_matrices), 
		//cmocka_unit_test(test_create_add_free), 
		//cmocka_unit_test(test_add_negative_s1), 
		//cmocka_unit_test(test_add_negative_s2), 
		//cmocka_unit_test(test_get_negative_s1), 
		//cmocka_unit_test(test_get_negative_s2), 
		//cmocka_unit_test(test_traceback),
	};
	int result = cmocka_run_group_tests(tests, NULL, NULL);
}
