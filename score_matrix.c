#include <stdlib.h>
#include <stdio.h>
#include "score_matrix.h"

ScoreMatrix score_matrix_create(int S1, int S2) {
	ScoreMatrix score_matrix;

	score_matrix.data = calloc(S1 * S2, sizeof(Score));
	if (score_matrix.data == NULL) {
		score_matrix.success = false;
		score_matrix.S1 = 0;
		score_matrix.S2 = 0;
	} else {
		score_matrix.S1 = S1;
		score_matrix.S2 = S2;
		score_matrix.success = true;
	}

	return score_matrix;
}


ScoreMatrixError score_matrix_add(ScoreMatrix * score_matrix, int s1, int s2, Score value) {
	if (s1 > score_matrix->S1) {
		ScoreMatrixError error = {.error_number=1, .success=false};
		return error;
	}

	if (s2 > score_matrix->S2) {
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
	if (s1 >= score_matrix->S1) {
		ScoreMatrixResult error = {.error_number=1, .success=false, .value=0};
		return error;
	}

	if (s2 >= score_matrix->S2) {
		ScoreMatrixResult error = {.error_number=2, .success=false, .value=0};
		return error;
	}

	ScoreMatrixResult result = {.error_number=0, .success=true, .value=*(score_matrix->data + s1 * score_matrix->S1 + s2)};
	return result;
}

void score_matrix_free(ScoreMatrix * score_matrix) {
	free(score_matrix->data);
	score_matrix->data = NULL;
}
