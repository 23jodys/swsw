#include <stdbool.h>

/** @file */

typedef int Score;

typedef struct ScoreMatrix {
	Score * data;
	int S1;
	int S2;
	int highest_s1;
	int highest_s2;
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

typedef struct ScoreConfig {
	int gap;
	int match;
	int mismatch;
} ScoreConfig;

//TODO: this needs to properly exit.
/**
 * @def score_matrix_geta(score_matrix, s1, s2, out) 
 * Appends the value at s1 x s2 and appends it to out
 * @param [in] score_matrix score matrix to retrieve from
 * @param [in] s1, sequence 1 coordinate 
 * @param [in] s2, sequence 2 coordinate 
 * @param [out] out, append to this value
 */
#define score_matrix_geta(score_matrix, s1, s2, out) do { \
	ScoreMatrixResult __xyzresult = score_matrix_get(score_matrix, s1, s2); \
	if (__xyzresult.success == true) {out += __xyzresult.value;} \
	} while(0);

/**
 * @brief create a new ScoreMatrix
 * @param [in] S1 dimension of the sequence 1 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @param [in] S2 dimension of the sequence 2 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @returns a new ScoreMatrix with the given dimensions
 */
ScoreMatrix * score_matrix_create(int S1, int S2);


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
 * @brief Print a scoring matrix to stdout for debugging
 */
void score_matrix_printf(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len); 

/**
 * @brief Create a score matrix and perform alignment
 */
void score_matrix_align(char * seq1, int seq1_len, char * seq2, int seq2_len); 

/**
 * @brief Perform the actual scoring operation
 */
ScoreMatrixError score_matrix_score(ScoreMatrix * score_matrix, ScoreConfig score_config, char * seq1, int seq1_len, char * seq2, int seq2_len); 

/**
 * @brief perform traceback
 * @param [in] score_matrix
 */
void score_matrix_traceback(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len);

/**
 * @brief Free the score matrix
 * @param [in,out] score_matrix
 * @detail This frees both the #ScoreMatrix struct and its dynamically sized 
 * score matrix.
 */
void score_matrix_free(ScoreMatrix **);
