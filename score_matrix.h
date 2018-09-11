#include <stdbool.h>

typedef int Score;

typedef struct ScoreMatrix {
	Score * data;
	int S1;
	int S2;
	bool success;
} ScoreMatrix;

typedef struct ScoreMatrixError {
	int error_number;
	bool success;
} ScoreMatrixError;

typedef struct ScoreMatrixResult {
	int error_number;
	bool success;
	Score value;
} ScoreMatrixResult;

/**
 * @brief create a new ScoreMatrix
 * @param [in] S1 dimension of the sequence 1 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @param [in] S2 dimension of the sequence 2 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @returns a new ScoreMatrix with the given dimensions
 */
ScoreMatrix score_matrix_create(int S1, int S2);


/**
 * @brief add a single value to the matrix
 *
 * This works with the raw indexing of the matrix, which includes an extra
 * row and column per the alogrithm. See score_matrix_adds() for convience function
 * which adds using the sequence index. 
 *
 * @param [in,out] score_matrix is modified with the requested value
 * @param [in] s1, the location on the sequence 1 dimension
 * @param [in] s2, the location on the sequence 2 dimension
 */
ScoreMatrixError score_matrix_add(ScoreMatrix *, int, int, Score);

/**
 * @brief add a single value to the matrix
 *
 * This is a convenience function so that indexes can remain in the "space" of
 * the sequence rather than in the matrix.  See score_matrix_add() for raw
 * access to the matrix which adds using the sequence index. 
 *
 * @param [in,out] score_matrix is modified with the requested value
 * @param [in] s1, the index into sequence 1. 1 is added to this to properly locate it in the matrix.
 * @param [in] s2, the index into sequence 2. 2 is added to this to properly locate it in the matrix.
 */
ScoreMatrixError score_matrix_adds(ScoreMatrix *, int, int, Score);

/**
 * @brief Retrieve a single value from the matrix
 *
 * Retrieve a value from the matrix.
 *
 * @param [in] score_matrix
 * @param [in] s1 the index into the sequence 1 dimension of the matrix.
 * @param [in] s2 the index into the sequence 2 dimension of the matrix.
 *
 * @return The #ScoreMatrixResult structure containing the requested #Score
 */
ScoreMatrixResult score_matrix_get(ScoreMatrix *, int, int);

/**
 * @brief Free the score matrix
 * @param [in,out] score_matrix
 * @detail It is the responsibility to deal with the storage associated with the
 * ScoreMatrix struct itself, e.g relying on automatic storage or free on the
 * ScoreMatrix struct itself _after_ this has been called.
 */
void score_matrix_free(ScoreMatrix *);