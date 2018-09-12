#include <stdlib.h>
#include <stdio.h>
#include "score_matrix.h"

typedef struct ScoreConfig {
	int gap;
	int match;
	int mismatch;
} ScoreConfig;

void score_matrix_align(char * seq1, int seq1_len, char * seq2, int seq2_len) {
	ScoreMatrix * score_matrix = score_matrix_create(seq1_len + 1, seq2_len + 1);
	if (score_matrix->success != true) {
		return;
	} 

	score_matrix_printf(score_matrix, seq1, seq1_len, seq2, seq2_len);
	score_matrix_free(&score_matrix);
}

ScoreMatrixError score_matrix_score(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len) {

	ScoreConfig score;
	score.gap = 1;
	score.match=2; 
	score.mismatch=-1;

	int highest_s1 = 0;
	int highest_s2 = 0;
	int max = 0;
	int seq1_index = 0; // Index into actual sequence, not into the scoring matrix
	for (int s1_index=1; s1_index < score_matrix->S1; s1_index++) {
		int seq2_index = 0; // Index into actual sequence, not into the scoring matrix
		for (int s2_index=1; s2_index < score_matrix->S2; s2_index++) {
			printf("Comparing %c with %c at %d,%d\n", seq1[seq1_index], seq2[seq2_index], seq1_index, seq2_index);
			int nw_score, w_score, n_score;
			// nw_score
			if (seq1[seq1_index] == seq2[seq2_index]) {
				ScoreMatrixResult result = score_matrix_get(score_matrix, s1_index - 1, s2_index - 1);
				nw_score = result.value + score.match;
			} else {
				ScoreMatrixResult result = score_matrix_get(score_matrix, s1_index - 1, s2_index - 1);
				nw_score = result.value + score.mismatch;
			}

			// w_score
			ScoreMatrixResult result = score_matrix_get(score_matrix, s1_index - 1, s2_index - 1);
			w_score = scoring_matrix[s1_index][s2_index - 1] - score.gap;

			// n_score
			n_score = scoring_matrix[s1_index - 1][s2_index] - score.gap;
			printf("nw_score: %d, w_score: %d, n_score %d\n", nw_score, w_score, n_score);
			
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
	}

	ScoreMatrixError error = {.success=true, .error_number=0};
	return error;
}


ScoreMatrix * score_matrix_create(int S1, int S2) {
	ScoreMatrix * score_matrix;

	score_matrix = malloc(sizeof(ScoreMatrix));

	score_matrix->data = calloc(S1 * S2, sizeof(Score));
	if (score_matrix->data == NULL) {
		score_matrix->success = false;
		score_matrix->S1 = 0;
		score_matrix->S2 = 0;
	} else {
		score_matrix->S1 = S1;
		score_matrix->S2 = S2;
		score_matrix->success = true;
	}

	return score_matrix;
}


ScoreMatrixError score_matrix_add(ScoreMatrix * score_matrix, int s1, int s2, Score value) {
	if (score_matrix->data == NULL) {
		ScoreMatrixError error = {.error_number=3, .success=false};
		return error;
	}
	if (s1 >= score_matrix->S1 || s1 < 0) {
		ScoreMatrixError error = {.error_number=1, .success=false};
		return error;
	}

	if (s2 >= score_matrix->S2 || s2 < 0) {
		ScoreMatrixError error = {.error_number=2, .success=false};
		return error;
	}

	*(score_matrix->data + s1 * score_matrix->S1 + s2) = value;

	ScoreMatrixError error = {.error_number=0, .success=true};
	return error;
}

ScoreMatrixError score_matrix_adds(ScoreMatrix * score_matrix, int i1, int i2, Score value) {
	ScoreMatrixError result = score_matrix_add(score_matrix, i1 + 1, i2 + 1, value);
	return result;
}

ScoreMatrixResult score_matrix_get(ScoreMatrix * score_matrix, int s1, int s2) {
	if (score_matrix->data == NULL) {
		ScoreMatrixResult error = {.error_number=3, .success=false, .value=0};
		return error;
	}
	if (s1 >= score_matrix->S1 || s1 < 0) {
		ScoreMatrixResult error = {.error_number=1, .success=false, .value=0};
		return error;
	}

	if (s2 >= score_matrix->S2 || s2 < 0) {
		ScoreMatrixResult error = {.error_number=2, .success=false, .value=0};
		return error;
	}

	ScoreMatrixResult result = {.error_number=0, .success=true, .value=*(score_matrix->data + s1 * score_matrix->S1 + s2)};
	return result;
}

void score_matrix_printf(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len) {
	printf("          ");	
	for (int i2=0; i2 < seq2_len; i2++) {
		printf("%c:%02d ", seq2[i2], i2); 
	}
	printf("\n"); \
	for (int i1=0; i1 < score_matrix->S1; i1++) { 
		if (i1 > 0) {
			printf("%c:%02d ", seq1[i1 - 1], i1-1); 
		} else {
			printf("     ");
		}
		for(int i2=0; i2 < score_matrix->S2; i2++) { 
			ScoreMatrixResult result = score_matrix_get(score_matrix, i1, i2);
			if (result.success) {
				printf("%04d ", result.value); 
			}
		} 
		printf("\n"); 
	}
}

void score_matrix_free(ScoreMatrix ** score_matrix) {
	free((*(*score_matrix)).data);
	(*(*score_matrix)).data = NULL;
	free(*score_matrix);
}
