#include <stdlib.h>
#include <stdio.h>
#include "swsw.h"


void score_matrix_align(char * seq1, int seq1_len, char * seq2, int seq2_len) {
	ScoreMatrix * score_matrix = score_matrix_create(seq1_len + 1, seq2_len + 1);
	if (score_matrix->success != true) {
		return;
	} 

	score_matrix_printf(score_matrix, seq1, seq1_len, seq2, seq2_len);
	score_matrix_free(&score_matrix);
}

ScoreMatrixError score_matrix_score(
		ScoreMatrix * score_matrix,
		ScoreConfig score_config,
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


PairAlignment* score_matrix_traceback(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len) {
	int current_score = 0;
	int s1_index = score_matrix->highest_s1;
	int s2_index = score_matrix->highest_s2;
	PairAlignment* pa = pair_alignment_create(seq1_len + seq2_len);
	if (NULL == pa) {
		return NULL;
	}
	do {
		printf("Looking at %d, %d\n", s1_index, s2_index);
		Score nw_score = 0;
		score_matrix_geta(score_matrix, s1_index - 1, s2_index - 1, nw_score);

		Score w_score = 0;
		score_matrix_geta(score_matrix, s1_index, s2_index - 1, w_score);

		Score n_score = 0;
		score_matrix_geta(score_matrix, s1_index - 1, s2_index, n_score);

		if (nw_score >= w_score && nw_score >= n_score) {
			printf(" nw_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", nw_score, seq1[s1_index - 1], seq2[s2_index - 1], s1_index -1, s2_index -1);
			current_score = nw_score;
			pair_alignment_prepend(pa, seq1[s1_index - 1], seq2[s2_index - 1]);
			s1_index--;
			s2_index--;
		} else if (w_score >= nw_score && w_score >= n_score) {
			printf(" w_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", w_score, ' ', seq2[s2_index], s1_index, s2_index -1);
			pair_alignment_prepend(pa, ' ', seq2[s2_index - 1]);
			current_score = w_score;
			s2_index--;
		} else {
			printf(" n_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", n_score, seq1[s1_index], ' ', s1_index -1 , s2_index);
			pair_alignment_prepend(pa, seq1[s1_index], ' ');
			current_score = n_score;
			s1_index--;
		}
	} while (current_score > 0); 
	return pa;
}


ScoreMatrix* score_matrix_create(size_t S1, size_t S2) {
	ScoreMatrix* score_matrix;

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


ScoreMatrixError score_matrix_add(ScoreMatrix * score_matrix, size_t s1, size_t s2, Score value) {
	if (score_matrix == NULL) {
		ScoreMatrixError error = {.error_number=3, .success=false};
		return error;
	}
	if (score_matrix->data == NULL) {
		ScoreMatrixError error = {.error_number=3, .success=false};
		return error;
	}
	if (s1 > score_matrix->S1) {
		ScoreMatrixError error = {.error_number=1, .success=false};
		return error;
	}

	if (s2 > score_matrix->S2) {
		ScoreMatrixError error = {.error_number=2, .success=false};
		return error;
	}
	size_t offset = (s1 * score_matrix->S1) + s2; 

	*(score_matrix->data + offset) = value;

	ScoreMatrixError error = {.error_number=0, .success=true};
	return error;
}

ScoreMatrixError score_matrix_adds(ScoreMatrix * score_matrix, int i1, int i2, Score value) {
	ScoreMatrixError result = score_matrix_add(score_matrix, i1 + 1, i2 + 1, value);
	return result;
}

ScoreMatrixResult score_matrix_get(ScoreMatrix * score_matrix, size_t s1, size_t s2) {
	/*
	 *
	 *               Seq 2/ s2         
	 *      +--+--+--+--+--+--+--+--+  
	 *      | 0| 1| 2| 3| 4| 5| 6| 7|  
	 *   +--+--+--+--+--+--+--+--+--+  
	 * S | 0| 0| 1| 2| 3| 4| 5| 6| 7|  
	 * e |  |  |  |  |  |  |  |  |  |  
	 * q | 1| 8| 9|10|11|12|13|14|15|  
	 *   |  |  |  |  |  |  |  |  |  |  
	 * 1 | 2|16|17|18|19|20|21|22|23|  
	 *   |  |  |  |  |  |  |  |  |  |  
	 * / | 3|24|25|26|27|28|29|30|31|  
	 *   |  |  |  |  |  |  |  |  |  |  
	 * s | 4|32|33|34|35|36|37|38|39|  
	 * 1 +--+--l--v--v--v--v--v--v--+  
	 *                                 
	 *        +-------------------+    
	 *        |   (s1 * 8) + s2   |    
	 *        +-------------------+    
	 *  
	 */
	printf("getting s1 '%zu', s2'%zu', S1 '%d', S2'%d'\n", s1, s2, score_matrix->S1, score_matrix->S2);
	if (score_matrix == NULL) {
		ScoreMatrixResult error = {.error_number=3, .success=false, .value=0};
		return error;
	}
	if (score_matrix->data == NULL) {
		ScoreMatrixResult error = {.error_number=3, .success=false, .value=0};
		return error;
	}
	if (s1 > score_matrix->S1) {
		ScoreMatrixResult error = {.error_number=1, .success=false, .value=0};
		return error;
	}

	if (s2 > score_matrix->S2) {
		ScoreMatrixResult error = {.error_number=2, .success=false, .value=0};
		return error;
	}

	size_t offset = (s1 * score_matrix->S1) + s2; 
	printf("offset: %lu\n", offset);

	ScoreMatrixResult result = {
		.error_number=0,
		.success=true,
		.value=*(score_matrix->data + offset)
	};
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
	*score_matrix = NULL;
}
