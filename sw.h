#include "alignment.h"

#ifndef SW_H_
#define SW_H_
/** @file */

/**
 * @brief define the smith waterman scoring matrix
 */
typedef struct SwswScoreConfig {
	int gap /**< @param the gap score */;
	int match; /**< @param the match score */;
	int mismatch; /**< @param the mismatch score*/;
} SwswScoreConfig;

/**
 * @brief container to include the alignment and the status
 */
typedef struct SwswAlignment {
	bool success /**< @param whether or not there were any errors */;
	int error_number /**< @param error number */;
	PairAlignment* alignment /**< @param the actual alignment */;
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
 * @param [in] seq1 sequence to align
 * @param [in] seq1_len length of sequence to align
 * @param [in] seq2 sequence to align
 * @param [in] seq2_len length of sequence to align
 */
PairAlignment* swsw_sw_traceback(ScoreMatrix * score_matrix, char * seq1, int seq1_len, char * seq2, int seq2_len);

/**
 * @brief free swsw objects
 * @param [in] sa object to free
 */
void swsw_alignment_free(SwswAlignment** sa);
#endif
