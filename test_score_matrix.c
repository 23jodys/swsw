#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>

#include <cmocka.h>

#include "score_matrix.h"

static void test_s1_to_large(void **state) {
	/* Given that the s1 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(&s, 10, 0);
	assert_false(result.success);
}

static void test_s2_to_large(void **state) {
	/* Given that the s2 value is exactly the size of the array, confirm
	 * that the result is not a success */ 
	(void) state; /* unused */
	ScoreMatrix s = score_matrix_create(10, 10);
	ScoreMatrixResult result = score_matrix_get(&s, 0, 10);
	assert_false(result.success);
}

static void test_adds(void **state) {
	/* Given that we attempt to adds a score at a given position sequence

}

static void test_round_trip(void **state) {
	/* Given that the matrix was initialized and a value added to a position,
	 * verify that that value can be retrieved */
	(void) state; /* unused */
	ScoreMatrix s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(&s, 5, 5, 999);
	assert_true(add_result.success);
	ScoreMatrixResult get_result = score_matrix_get(&s, 5, 5);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
}

static void test_create_add_free(void **state) {
	/* Give that we create a matrix and add a value to it, confirm that after it is freed we cannot retrieve results */
	(void) state;
	ScoreMatrix s = score_matrix_create(10, 10);
	ScoreMatrixError add_result= score_matrix_add(&s, 5, 5, 999);
	/* Should work because this is a valid matrix */
	ScoreMatrixResult get_result = score_matrix_get(&s, 5, 5);
	assert_true(get_result.success);
	assert_int_equal(get_result.value, 999);
	/* Free the matrix */
	score_matrix_free(&s);

	/* Verify that we get a false success */
	ScoreMatrixResult get2_result = score_matrix_get(&s, 5, 5);
	assert_false(get2_result.success);
}

static void test_many_matrices(void **state) {
	/* Given that many matrices are created, confirm that values can be added and  retrieved from all of them */
	(void) state; /* unused */
	ScoreMatrix matrices[200];
	for (int i=0; i < 200; i++) {
		matrices[i] = score_matrix_create(100,100);	
		score_matrix_add(&(matrices[i]), 57, 57, i * 10);
	}

	for (int i=0; i < 200; i++) {
		ScoreMatrixResult result = score_matrix_get(&(matrices[i]), 57, 57);
		assert_true(result.success);
		assert_int_equal(result.value, i * 10);
	}

}
int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_s1_to_large), 
		cmocka_unit_test(test_s2_to_large), 
		cmocka_unit_test(test_round_trip), 
		cmocka_unit_test(test_many_matrices), 
		cmocka_unit_test(test_create_add_free), 
	};
	int result = cmocka_run_group_tests(tests, NULL, NULL);
	printf("result %d\n", result);
}
