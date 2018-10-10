#include "alignment.h"

#ifndef SW_H_
#define SW_H_
/** @file */

typedef struct SwswScoreConfig {
	int gap;
	int match;
	int mismatch;
} SwswScoreConfig;

typedef struct SwswAlignment {
	bool success;
	int error_number;
	PairAlignment* alignment;
} SwswAlignment;
/**
 * @brief Perform a full smith waterman alignment after allocating space for the final alignment object
 */
SwswAlignment* swsw_sw_align(SwswScoreConfig, char * seq1, int seq1_len, char * seq2, int seq2_len); 


/**
 * @brief Perform the actual scoring operation
 */
ScoreMatrixError swsw_sw_score(ScoreMatrix * score_matrix, SwswScoreConfig score_config, char * seq1, int seq1_len, char * seq2, int seq2_len); 

/**
 * @brief perform traceback
 * @param [in] score_matrix
 */
PairAlignment* swsw_sw_traceback(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len);

void swsw_alignment_free(SwswAlignment** sa);
#endif
