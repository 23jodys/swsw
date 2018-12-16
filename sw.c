#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "swsw.h"

SwswAlignment* swsw_sw_align(SwswScoreConfig score_config, sds seq1, int seq1_len, sds seq2, int seq2_len) {
	// For smith waterman we create a matrix with each dimension one larger that the sequence
	// length so that we can have the first row and column set to zero so that the traceback
	// is guaranteed to stop.
	ScoreMatrix* score_matrix = score_matrix_create(seq1_len + 1, seq2_len + 1);
	//score_matrix_printf(score_matrix, seq1, seq1_len, seq2, seq2_len);
	SwswAlignment* result = malloc(sizeof(SwswAlignment));
	if (!result) {
		return NULL;
	}
	if (score_matrix->success != true) {
		result->success=false;
		result->error_number=1;
		result->alignment=NULL;
		return result;
	} 

	ScoreMatrixError scoring_error = swsw_sw_score(score_matrix, score_config, seq1, seq1_len, seq2, seq2_len);	
	if (scoring_error.success != true) {
		result->success=false;
		result->error_number=1;
		result->alignment=NULL;
		return result;
	}

	PairAlignment* alignment_result = swsw_sw_traceback(score_matrix, seq1, seq1_len, seq2, seq2_len);

	//score_matrix_printf(score_matrix, seq1, seq1_len, seq2, seq2_len);
	score_matrix_free(&score_matrix);

	result->success=true;
	result->error_number=0;
	result->alignment=alignment_result;

	return result;
}

ScoreMatrixError swsw_sw_score(ScoreMatrix * score_matrix, SwswScoreConfig score_config, sds seq1, int seq1_len, sds seq2, int seq2_len) {

	int max = 0;
	int seq1_index = 0; // Index into actual sequence, not into the scoring matrix
	for (int s1_index=1; s1_index < score_matrix->S1; s1_index++) {
		int seq2_index = 0; // Index into actual sequence, not into the scoring matrix
		for (int s2_index=1; s2_index < score_matrix->S2; s2_index++) {
			DEBUGLOG("Comparing %c with %c at %d,%d\n", seq1[seq1_index], seq2[seq2_index], seq1_index, seq2_index);

			ScoreValue nw_score = 0;
			if (seq1[seq1_index] == seq2[seq2_index]) {
				score_matrix_geta(score_matrix, s1_index - 1, s2_index - 1, nw_score);
				nw_score += score_config.match;
			} else {
				score_matrix_geta(score_matrix, s1_index - 1, s2_index - 1, nw_score);
				nw_score += score_config.mismatch;
			}

			ScoreValue w_score = 0;
			score_matrix_geta(score_matrix, s1_index, s2_index - 1, w_score);
			w_score += score_config.gap;

			ScoreValue n_score = 0;
			score_matrix_geta(score_matrix, s1_index - 1, s2_index, n_score);
			n_score += score_config.gap;

			Score score = {.value = 0, .direction=DirNone};
			if (nw_score >= w_score && nw_score >= n_score && nw_score >0 ) {
				score.value = nw_score;
				score.direction=DirNw;
			} else if (w_score >= nw_score && w_score >= n_score  && w_score >0 ) {
				score.value = w_score;
				score.direction=DirW;
			} else if (n_score >= nw_score && n_score >= w_score  && n_score >0 ) {
				score.value = n_score;
				score.direction=DirN;
			} else {
				score.value = 0;
				score.direction=DirNone;
			}

			if (score.value > max) {
				DEBUGLOG("score %lld was higher that %d, setting highest s1: %d, highest s2: %d\n", 
						score.value, max, s1_index, s2_index
				);
				score_matrix->highest_s1 = s1_index;
				score_matrix->highest_s2 = s2_index;
				max = score.value;
			}

			seq2_index++;

			DEBUGLOG("score.value: %lld, score.direction: %d\n", score.value, score.direction );
			
			score_matrix_add(score_matrix, s1_index, s2_index, score);
		}
		seq1_index++;
	}

	ScoreMatrixError error = {.success=true, .error_number=0};
	return error;
}

PairAlignment* swsw_sw_traceback(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len) {
	ScoreValue next_score = 0;
	enum ScoreDirection current_direction = DirNone;
	size_t s1_index = score_matrix->highest_s1;
	size_t s2_index = score_matrix->highest_s2;
	PairAlignment* pa = pair_alignment_create(seq1_len + seq2_len);
	if (NULL == pa) {
		return NULL;
	}
	do {
		//TOOD: handle error
		//
		ScoreMatrixResult current_result = score_matrix_get(score_matrix, s1_index, s2_index);
		current_direction = (current_result.score).direction;
		DEBUGLOG("Looking at %zu, %zu which points us in %d which contains score %llu\n", s1_index, s2_index, (int)current_direction, next_score);

		if (current_direction == DirNw) {
			// Match or mismatch
			CigarOperator cigar_operator;
			if (seq1[s1_index - 1] == seq2[s2_index - 1]) {
				cigar_operator = CigarOperatorMatch;
			} else {
				cigar_operator = CigarOperatorMisMatch;
			}
			pair_alignment_prepend(pa, seq1[s1_index - 1], seq2[s2_index - 1], cigar_operator);
			s1_index--;
			s2_index--; 
		} else if (current_direction == DirW) {
			// Sequence only in reference
			pair_alignment_prepend(pa, '-', seq2[s2_index - 1], CigarOperatorRefOnly);
			s2_index--;
		} else { // DirN
			// Sequence only in query 
			pair_alignment_prepend(pa, seq1[s1_index - 1], '-', CigarOperatorQueryOnly);
			s1_index--;
		}
		ScoreMatrixResult next_result = score_matrix_get(score_matrix, s1_index, s2_index);

		next_score = next_result.score.value;
 		DEBUGLOG("After following direction %d, next score is %lld\n", current_direction, next_score);

	} while (next_score > 0); 

	pair_alignment_prepend_cigar(pa, CigarOperatorNull);

	return pa;
}

void swsw_alignment_free(SwswAlignment** sa) {
	pair_alignment_free(&(*(*sa)).alignment);
	(*(*sa)).alignment = NULL;
	free(*sa);
	*sa = NULL;
}
