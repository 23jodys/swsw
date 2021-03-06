#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#include "swsw.h"

ScoreMatrix* score_matrix_create(size_t S1, size_t S2) {
	ScoreMatrix* score_matrix;

	/* Call alloc with standard malloc and calloc */
	score_matrix = score_matrix_alloc(S1, S2, malloc, calloc);

	return score_matrix;
}

ScoreMatrix* score_matrix_alloc(size_t S1, size_t S2, void*(malloc_ptr)(size_t size), void*(calloc_ptr)(size_t count, size_t size)) {
	DEBUGLOG("malloc_ptr: %p\n", malloc_ptr);
	DEBUGLOG("calloc_ptr: %p\n", calloc_ptr);
	ScoreMatrix* score_matrix;

	score_matrix = (malloc_ptr)(sizeof(ScoreMatrix));
	DEBUGLOG("score_matrix: %p\n", score_matrix);
	if (score_matrix == NULL) {
		DEBUGLOG("NULL score_matrix\n");
		return NULL;
	}

	size_t matrix_size = S1 * S2;
	DEBUGLOG("Creating new matrix total size %zu\n", matrix_size );
	score_matrix->data = (calloc_ptr)(matrix_size, sizeof(Score));
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

ScoreMatrixError score_matrix_add(ScoreMatrix * score_matrix, size_t s1, size_t s2, Score score) {
	//DEBUGLOG("s1: %zu, s2: %zu\n", s1, s2);
	DEBUGLOG("score.value: %lld, score.direction: %d\n", score.value, score.direction );
	if (score_matrix == NULL) {
		ScoreMatrixError error = {.error_number=3, .success=false};
		return error;
	}
	if (score_matrix->data == NULL) {
		ScoreMatrixError error = {.error_number=3, .success=false};
		return error;
	}
	if (s1 >= score_matrix->S1) {
		ScoreMatrixError error = {.error_number=1, .success=false};
		return error;
	}

	if (s2 >= score_matrix->S2) {
		ScoreMatrixError error = {.error_number=2, .success=false};
		return error;
	}
	size_t offset = (s1 * (score_matrix->S2)) + s2; 

	*(score_matrix->data + offset) = score;

	DEBUGLOG("S1 %d, S2 %d, offset %zu, value %lld, direction %d\n", score_matrix->S1, score_matrix->S2, offset, (*(score_matrix->data + offset)).value,  (*(score_matrix->data + offset)).direction); 


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
	//DEBUGLOG("s1: %zu, s2: %zu\n", s1, s2);
	if (score_matrix == NULL) {
		Score empty_score = {.value=0, .direction=DirNone};
		ScoreMatrixResult error = {.error_number=3, .success=false, .score=empty_score};
		return error;
	}
	if (score_matrix->data == NULL) {
		Score empty_score = {.value=0, .direction=DirNone};
		ScoreMatrixResult error = {.error_number=3, .success=false, .score=empty_score};
		return error;
	}
	if (s1 >= score_matrix->S1) {
		Score empty_score = {.value=0, .direction=DirNone};
		ScoreMatrixResult error = {.error_number=1, .success=false, .score=empty_score};
		return error;
	}

	if (s2 >= score_matrix->S2) {
		Score empty_score = {.value=0, .direction=DirNone};
		ScoreMatrixResult error = {.error_number=2, .success=false, .score=empty_score};
		return error;
	}

	size_t offset = (s1 * (score_matrix->S2)) + s2; 
	//DEBUGLOG("S1 %d, S2 %d, offset %zu\n", score_matrix->S1, score_matrix->S2, offset); 

	ScoreMatrixResult result = {
		.error_number=0,
		.success=true,
		.score=*(score_matrix->data + offset)
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
				char dir;
				if (result.score.direction == 0) {
					dir = '*';
				} else if (result.score.direction == 1) {
					dir = '\\';
				} else if (result.score.direction == 2) {
					dir = '-';
				} else if (result.score.direction == 3) {
					dir = '|';
				}

				printf("%c%03" PRIi64, dir, result.score.value); 
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
