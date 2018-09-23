#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "swsw.h"

SwswAlignment* swsw_sw_align(SwswScoreConfig score_config, char * seq1, int seq1_len, char * seq2, int seq2_len) {
	// For smith waterman we create a matrix with each dimension one larger that the sequence
	// length so that we can have the first row and column set to zero so that the traceback
	// is guaranteed to stop.
	ScoreMatrix* score_matrix = score_matrix_create(seq1_len + 1, seq2_len + 1);
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

	score_matrix_free(&score_matrix);

	result->success=true;
	result->error_number=0;
	result->alignment=alignment_result;

	return result;
}

ScoreMatrixError swsw_sw_score(
		ScoreMatrix * score_matrix,
		SwswScoreConfig score_config,
		char * seq1, int seq1_len,
		char * seq2, int seq2_len) {

	int max = 0;
	int seq1_index = 0; // Index into actual sequence, not into the scoring matrix
	for (int s1_index=1; s1_index < score_matrix->S1; s1_index++) {
		int seq2_index = 0; // Index into actual sequence, not into the scoring matrix
		for (int s2_index=1; s2_index < score_matrix->S2; s2_index++) {
			//printf("Comparing %c with %c at %d,%d\n", seq1[seq1_index], seq2[seq2_index], seq1_index, seq2_index);

			int nw_score = 0;
			if (seq1[seq1_index] == seq2[seq2_index]) {
				score_matrix_geta(score_matrix, s1_index - 1, s2_index - 1, nw_score);
				nw_score += score_config.match;
			} else {
				score_matrix_geta(score_matrix, s1_index - 1, s2_index - 1, nw_score);
				nw_score += score_config.mismatch;
			}

			int w_score = 0;
			score_matrix_geta(score_matrix, s1_index, s2_index - 1, w_score);
			w_score += score_config.gap;

			int n_score = 0;
			score_matrix_geta(score_matrix, s1_index - 1, s2_index, n_score);
			n_score += score_config.gap;

			//printf("nw_score: %d, w_score: %d, n_score %d\n", nw_score, w_score, n_score);
			
			int score;
			if (nw_score >= w_score && nw_score >= n_score && nw_score >0 ) {
				score = nw_score;
			} else if (w_score >= nw_score && w_score >= n_score  && w_score >0 ) {
				score = w_score;
			} else if (n_score >= nw_score && n_score >= w_score  && n_score >0 ) {
				score = n_score;
			} else {
				score = 0;
			}

			if (score > max) {
				score_matrix->highest_s1 = s1_index;
				score_matrix->highest_s2 = s2_index;
			}

			seq2_index++;

			score_matrix_add(score_matrix, s1_index, s2_index, score);
		}
		seq1_index++;
	}

	ScoreMatrixError error = {.success=true, .error_number=0};
	return error;
}


PairAlignment* swsw_sw_traceback(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len) {
	int current_score = 0;
	int s1_index = score_matrix->highest_s1;
	int s2_index = score_matrix->highest_s2;
	PairAlignment* pa = pair_alignment_create(seq1_len + seq2_len);
	if (NULL == pa) {
		return NULL;
	}
	do {
		DEBUGLOG("Looking at %d, %d\n", s1_index, s2_index);
		Score nw_score = 0;
		score_matrix_geta(score_matrix, s1_index - 1, s2_index - 1, nw_score);

		Score w_score = 0;
		score_matrix_geta(score_matrix, s1_index, s2_index - 1, w_score);

		Score n_score = 0;
		score_matrix_geta(score_matrix, s1_index - 1, s2_index, n_score);

		if (nw_score >= w_score && nw_score >= n_score) {
			DEBUGLOG(" nw_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", nw_score, seq1[s1_index - 1], seq2[s2_index - 1], s1_index -1, s2_index -1);
			current_score = nw_score;
			pair_alignment_prepend(pa, seq1[s1_index - 1], seq2[s2_index - 1]);
			s1_index--;
			s2_index--;
		} else if (w_score >= nw_score && w_score >= n_score) {
			DEBUGLOG(" w_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", w_score, ' ', seq2[s2_index], s1_index, s2_index -1);
			pair_alignment_prepend(pa, ' ', seq2[s2_index - 1]);
			current_score = w_score;
			s2_index--;
		} else {
			DEBUGLOG(" n_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", n_score, seq1[s1_index], ' ', s1_index -1 , s2_index);
			pair_alignment_prepend(pa, seq1[s1_index], ' ');
			current_score = n_score;
			s1_index--;
		}
	} while (current_score > 0); 
	return pa;
}

void swsw_alignment_free(SwswAlignment** sa) {
	pair_alignment_free(&(*(*sa)).alignment);
	(*(*sa)).alignment = NULL;
	free(*sa);
	*sa = NULL;
}
