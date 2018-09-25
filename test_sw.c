#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>
#include "sds/sds.h"
#include "swsw.h"

/** 
 * @brief Given that we ask for a perfectly reasonable alignment, verify that we get a successful result and a 0 error number and a non null alignment
 */
static void test_sw_align_golden(void **state) {
	char* seq1 = "ABCDEFGHIJ";
	char* seq2 = "ABCEFGHIJ";
	SwswScoreConfig score_config = {.gap=-1, .match=3, .mismatch=-3};
	SwswAlignment* result = swsw_sw_align(score_config, seq1, 10, seq2, 9);
	assert_true(result->success);
	assert_int_equal(result->error_number, 0);
	assert_non_null(result->alignment);
	pair_alignment_sprint(result->alignment);
	swsw_alignment_free(&result);
}

/**
 * @brief Given that we have two short sequences, verify that we get a correct cigar string
 */
static void test_cigar_golden(void** state) {
	// https://sites.google.com/site/bioinformaticsremarks/bioinfo/sam-bam-format/what-is-a-cigar
	sds seq1 = sdsnew("CCATACTGAACTGACTAAC");
	sds seq2 = sdsnew("ACTAGAATGGCT");

	size_t seq1_len = sdslen(seq1);
	size_t seq2_len = sdslen(seq2);

	SwswScoreConfig score_config = {.gap=-1, .match=3, .mismatch=-3};
	SwswAlignment* result = swsw_sw_align(score_config, seq1, seq1_len, seq2, seq2_len);
	assert_true(result->success);
	
	pair_alignment_sprint(result->alignment);

	CigarString* cigar = cigar_string_create(result->alignment);
	printf("%s\n", cigar->cigar);

	sds expected = sdsnew("3M1I3M1D5M");

	assert_int_equal(cigar->pos, 5);
	assert_string_equal(cigar->cigar, expected);

	cigar_string_free(&cigar);

	swsw_alignment_free(&result);
	sdsfree(seq1);
	sdsfree(seq2);
	sdsfree(expected);
}

static void test_traceback(void **state) {
	ScoreMatrix* s = score_matrix_create(10,7);

	char* seq1 = "ABCDEFGHIJ";
	char* seq2 = "ABCDHIJ";

	SwswScoreConfig score_config;
	score_config.gap      = -1;
	score_config.match    =  3; 
	score_config.mismatch = -3;

	ScoreMatrixError error = swsw_sw_score(s, score_config, seq1, 9, seq2, 6);
	//score_matrix_printf(s, seq1, 9, seq2, 6);
	PairAlignment* pa = swsw_sw_traceback(s, seq1, 9, seq2, 6);
	assert_non_null(pa);
	score_matrix_free(&s);
	pair_alignment_free(&pa);
	assert_null(pa);
	
}

static void test_score(void **state) {
	ScoreMatrix * s = score_matrix_create(10,10);
	char * seq1 = "ABCDDDDHIJ";
	char * seq2 = "ABCDEFGHIJ";

	SwswScoreConfig score_config;
	score_config.gap = 2;
	score_config.match=3; 
	score_config.mismatch=-3;

	ScoreMatrixError error = swsw_sw_score(s, score_config, seq1, 9, seq2, 9);
	assert_true(error.success);
	// Confirm that the highest was found at index 9 in both dimensions
	assert_int_equal(s->highest_s1, 9);
	assert_int_equal(s->highest_s2, 9);

	// Confirm one of the scores
	Score result = 0;
	score_matrix_geta(s, 5, 5, result);
	assert_int_equal(result, 19);
	score_matrix_free(&s);
}


//
int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_score),
		cmocka_unit_test(test_sw_align_golden),
		cmocka_unit_test(test_traceback),
		cmocka_unit_test(test_cigar_golden),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
