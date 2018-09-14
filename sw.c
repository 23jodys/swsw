#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#define PRINT_MATRIX printf("   "); \
	printf("       "); \
	for (int i2=0; i2 < seq2_len; i2++) { printf("%c:%02d ", seq2[i2], i2); } \
	printf("\n"); \
	for (int i1=0; i1 < seq1_dim; i1++) { \
		if (i1 > 0) { printf("%c:%02d ", seq1[i1 - 1], i1-1); } else {printf("     ");}\
		for(int i2=0; i2 < seq2_dim; i2++) { \
			printf("%04d ", scoring_matrix[i1][i2]); \
		} \
		printf("\n"); \
	}
typedef struct Score {
	int gap;
	int match;
	int mismatch;
} Score;

int max(int sc1,int sc2,int sc3) {
	if (sc1 >= sc2 && sc1 >= sc3) {
		return sc1;
	} else if (sc2 >= sc1 && sc2 >= sc3) {
		return sc2;
	} else {
		return sc3;
	}
}

char * align(char * seq1, char * seq2) {
	char *fake;
	size_t seq1_len = strlen(seq1);
	size_t seq2_len = strlen(seq2);
	size_t seq1_dim = seq1_len + 1;
	size_t seq2_dim = seq2_len + 1;
	printf("seq1_len: %zu, seq2_len: %zu, seq1_dim: %zu, seq2_dim: %zu\n", seq1_len, seq2_len, seq1_dim, seq2_dim);
	printf("seq1: %s\n", seq1);
	printf("seq2: %s\n", seq2);

	Score score;
	score.gap = 1;
	score.match=2; 
	score.mismatch=-1;

	int scoring_matrix[seq1_dim][seq2_dim];
	for (int i1=0; i1 < seq1_dim; i1++) {
		scoring_matrix[i1][0] = 0;
		for(int i2=0; i2 < seq2_dim; i2++) {
			scoring_matrix[0][i2] = 0;
		}
	}
	PRINT_MATRIX

	int highest_s1 = 0;
	int highest_s2 = 0;

	/* Scoring matrix block 
	 *
	 * Computes the complete scoring matrix.
	 *
	 * :param scoring_matrix int * [][]:modified with calculated scores when block completes
	 * :param highest_s1 int: index on seq1 side of matrix to highest scoring cell, modified when block completes
	 * :param highest_s2 int: index on seq2 side of matrix to highest scoring cell, modified when block completes
	 * */
	{
	int max = 0;
	// Index into actual sequence, not into the scoring matrix
	int seq1_index = 0;

	// These index into the scoring matrix and start at 1 because we set everything to zero on the
	// north and west edge so that the traceback is simpler.
	for (int s1_index=1; s1_index < seq1_dim; s1_index++) {
		// Index into actual sequence, not into the scoring matrix
		int seq2_index = 0;
		for (int s2_index=1; s2_index < seq2_dim; s2_index++) {
			printf("Comparing %c with %c at %d,%d\n", seq1[seq1_index], seq2[seq2_index], seq1_index, seq2_index);
			int nw_score, w_score, n_score;
			// nw_score
			if (seq1[seq1_index] == seq2[seq2_index]) {
				nw_score = scoring_matrix[s1_index - 1][s2_index - 1] + score.match;
			} else {
				nw_score = scoring_matrix[s1_index - 1][s2_index - 1] + score.mismatch;
			}

			// w_score
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

			scoring_matrix[s1_index][s2_index] = score;

			if (score >= max) {
				printf("Found maximum so far at %d, %d with %d\n", s1_index, s2_index, score);
				max = score;
				highest_s1 = s1_index;
				highest_s2 = s2_index;
			}
			printf("-----\n");
			seq2_index++;
		}
		seq1_index++;
	}
	PRINT_MATRIX
	}

	PairAlignment pa = pair_alignment_create(seq1_len * seq2_len);
	/* Traceback block
	 *
	 * :param int[][] scoring_matrix: unchanged
	 * :param PairAlignment pa: object that will contain the final alignment representation after traceback block completes
	 */
	{
		int current_score = 0;
		int s1_index = highest_s1;
		int s2_index = highest_s2;
		do {
			printf("Looking at %d, %d\n", s1_index, s2_index);
			int nw_score = scoring_matrix[s1_index - 1][s2_index - 1];
			int w_score = scoring_matrix[s1_index][s2_index - 1];
			int n_score = scoring_matrix[s1_index - 1][s2_index];

			if (nw_score >= w_score && nw_score >= n_score) {
				printf(" nw_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", nw_score, seq1[s1_index - 1], seq2[s2_index - 1], s1_index -1, s2_index -1);
				current_score = nw_score;
				s1_index--;
				s2_index--;
			} else if (w_score >= nw_score && w_score >= n_score) {
				printf(" w_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", w_score, ' ', seq2[s2_index], s1_index, s2_index -1);
				current_score = w_score;
				s2_index--;
			} else {
				printf(" n_score (%d) was highest adding '%c':'%c', moving to %d, %d\n", n_score, seq1[s1_index], ' ', s1_index -1 , s2_index);
				current_score = n_score;
				s1_index--;
			}
		} while (current_score > 0); 
	}

	//int global_max = 0;
	//int highest_x = 0;
	//int highest_y = 0;
	//for (int i1=1; i1 < seq1_dim; i1++) {
	//	for(int i2=1; i2 < seq2_dim; i2++) {
	//		printf("cell: %d %d comparing %c (seq1) and %c (seq2)\n", i1, i2, seq1[i1], seq2[i2]);
	//		int max = 0;
	//		int s = 0;

	//		// left score
	//		s = scoring_matrix[i1][i2-1] - score.gap;
	//		if (s > max) {
	//			max = s;
	//		}
	//		// up score
	//		s = scoring_matrix[i1-1][i2] - score.gap;
	//		if (s > max) {
	//			max = s;
	//		}
	//		if (seq1[i1-1] == seq2[i2-1]) {
	//			s = scoring_matrix[i1-1][i2-1] + score.match;
	//		} else {
	//			s = scoring_matrix[i1-1][i2-1] + score.mismatch;
	//		}
	//		if (s > max) {
	//			max = s;
	//		}
	//		if (max >= global_max ) {
	//			printf("%d >= %d, setting new global max at i1=%d and i2=%d\n", max, global_max, i1, i2);

	//			highest_x = i1;
	//			highest_y = i2;
	//			global_max = max;
	//		}
	//		scoring_matrix[i1][i2] = max;
	//	}
	//}

	//PRINT_MATRIX
	//int cell_score = scoring_matrix[highest_x][highest_y];


	//PairAlignment pa = pair_alignment_create(max_len);

	//int s1_index = highest_x;
	//int s2_index = highest_y;

	//printf("Traceback: highest x (%d), highest y (%d), s1 %d, s2 %d\n", highest_x, highest_y, s1_index, s2_index);
	//while(cell_score != 0) {
	//	int nw_score = scoring_matrix[highest_x - 1][highest_y - 1];
	//	int w_score = scoring_matrix[highest_x -1][highest_y];
	//	int n_score = scoring_matrix[highest_x][highest_y - 1];

	//	//printf(
	//	//	"Cell: %d|%d\n",
	//	//	scoring_matrix[highest_x - 1][highest_y - 1],
	//	//	scoring_matrix[highest_x -1 ][highest_y]
	//	//);
	//	//printf(
	//	//	"Cell: %d|%d\n",
	//	//	scoring_matrix[highest_x][highest_y - 1],
	//	//	scoring_matrix[highest_x][highest_y]
	//	//);

	//	if (nw_score >= w_score && nw_score >= n_score) {
	//		//printf("nw score\n");
	//		highest_x = highest_x - 1;
	//		highest_y = highest_y - 1;

	//		pair_alignment_prepend(&pa, seq1[s1_index], seq2[s2_index - 1]);
	//		s1_index--;
	//		s2_index--;

	//		cell_score = nw_score;
	//	} else if (w_score >= nw_score && w_score >= n_score) {
	//		//printf("w score\n");
	//		highest_x = highest_x - 1;

	//		pair_alignment_prepend(&pa, seq1[s1_index], ' ');
	//		s1_index--;

	//		cell_score = w_score;
	//	} else {
	//		//printf("n score\n");
	//		highest_y = highest_y - 1;

	//		pair_alignment_prepend(&pa, seq2[s2_index], ' ');
	//		s2_index--;

	//		cell_score = n_score;
	//	}
	//	//printf("s1_index: %d, s2_index: %d\n", s1_index, s2_index);
	//}

	pair_alignment_sprint(&pa);

	asprintf(&fake, "seq 1 (%zu):\n  %s\nseq 2 (%zu):\n  %s\n", seq1_len, seq1, seq2_len, seq2);
	pair_alignment_free(&pa);
	return fake;
}

int main(void) {
	char *seq1 = "AGCATTTA";
	char *seq2 = "AGCCCCCCCCCCCCCCCTTTACC";
	char *log;
	
	log = align(seq1, seq2);
	printf("%s", log);
	free(log);
}
