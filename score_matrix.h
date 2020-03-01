#ifndef SCORE_MATRIX_H
#define SCORE_MATRIX_H
/** @file */
#include <stdbool.h>

/** 
 * @brief enum describing direction that score came from 
 */
enum ScoreDirection { DirNone=0, DirNw=1, DirW=2, DirN=3 };

/**
 * @brief value of score in a cell
 */
typedef int64_t ScoreValue;

/**
 * @brief represents the value and direction of the score
 */
typedef struct Score {
	ScoreValue value; /**< @brief score value */
	enum ScoreDirection direction; /**< @brief direction that value came from */
} Score;

/**
 * @brief holds a matrix of Score cells
 *
 * This is implemented as a allocated block of memory that is accessed
 * by strides. This holds the dimensions and pointer to start of the block.
 */
typedef struct ScoreMatrix {
	Score* data; /**< @brief point to start of matrix */
	int S1; /**< @brief size of the x dimension */
	int S2; /**< @brief size of the y dimension */
	int highest_s1; /**< @brief highest score value on s1 found */
	int highest_s2; /**< @brief highest score value on s1 found */
	bool success; /**< @brief whether there have been any errors */
} ScoreMatrix;

/** 
 * @brief container describing errors returned from methods
 */
typedef struct ScoreMatrixError {
	int error_number; /**< @brief error number */
	bool success; /**< @brief whether or not the command was successful */
} ScoreMatrixError;

/**
 * @brief container to hold the result of scoring
 */
typedef struct ScoreMatrixResult {
	int error_number; /**< @brief error number */
	bool success; /**< @brief success */
	Score score; /**< @brief actual score object */
} ScoreMatrixResult;


//TODO: this needs to properly exit.

/**
 * @def score_matrix_geta(score_matrix, s1, s2, out) 
 * Appends the value at s1 x s2 and appends it to out
 * @param [in] score_matrix score matrix to retrieve from
 * @param [in] s1 sequence 1 coordinate 
 * @param [in] s2 sequence 2 coordinate 
 * @param [out] out append to this value
 */
#define score_matrix_geta(score_matrix, s1, s2, out) do { \
	ScoreMatrixResult __xyzresult = score_matrix_get(score_matrix, s1, s2); \
	if (__xyzresult.success == true) {out += __xyzresult.score.value;} \
	} while(0);

/**
 * @brief create a new ScoreMatrix. Cell values are set to 0 explicitly on creation.
 * @param [in] S1 dimension of the sequence 1 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @param [in] S2 dimension of the sequence 2 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @returns a new ScoreMatrix with the given dimensions
 */
ScoreMatrix * score_matrix_create(size_t S1, size_t S2);

/**
 * @brief allocates a new ScoreMatrix. Cell values are set to 0 explicitly on creation.
 * @param [in] S1 dimension of the sequence 1 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @param [in] S2 dimension of the sequence 2 side. This includes an extra column that will be the zeroed column so that tracebacks work easily.
 * @param [in] malloc_ptr function pointer to a function implementing the malloc interface, normally malloc itself
 * @param [in] calloc_ptr function pointer to a function implementing calloc interface, normally calloc itself
 * @returns a new ScoreMatrix with the given dimensions
 */
ScoreMatrix * score_matrix_alloc(size_t S1, size_t S2, void* (malloc_ptr)(size_t size), void* (calloc_ptr)(size_t count, size_t size));


/**
 * @brief add a single value to the matrix
 *
 * This works with the raw indexing of the matrix, which includes an extra
 * row and column per the alogrithm. See score_matrix_adds() for convience function
 * which adds using the sequence index. 
 *
 * @param [in,out] score_matrix is modified with the requested value
 * @param [in] s1 the location on the sequence 1 dimension (query)
 * @param [in] s2 the location on the sequence 2 dimension (reference)
 * @param [in] score the value and direction to store in this cell
 */
ScoreMatrixError score_matrix_add(ScoreMatrix* score_matrix, size_t s1, size_t s2, Score score);

/**
 * @brief add a single value to the matrix
 *
 * This is a convenience function so that indexes can remain in the "space" of
 * the sequence rather than in the matrix.  See score_matrix_add() for raw
 * access to the matrix which adds using the sequence index. 
 *
 * @param [in,out] score_matrix is modified with the requested value
 * @param [in] s1 the index into sequence 1. 1 is added to this to properly locate it in the matrix.
 * @param [in] s2 the index into sequence 2. 2 is added to this to properly locate it in the matrix.
 * @param [in] score the score to insert
 */
ScoreMatrixError score_matrix_adds(ScoreMatrix* score_matrix, int s1, int s2, Score score);

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
ScoreMatrixResult score_matrix_get(ScoreMatrix * score_matrix, size_t s1, size_t s2);

/**
 * @brief Print a scoring matrix to stdout for debugging
 */
void score_matrix_printf(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len); 

/**
 * @brief Free the score matrix
 * @param [in,out] score_matrix
 *
 * This frees both the #ScoreMatrix struct and its dynamically sized 
 * score matrix.
 */
void score_matrix_free(ScoreMatrix ** score_matrix);
#endif /*SCORE_MATRIX_H*/
