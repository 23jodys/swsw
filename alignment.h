#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <stdbool.h>
#include "sds/sds.h"

/** @brief
 * Represents the alignment of two strings
 */
typedef struct PairAlignment {
	char* s1; /**< @brief Generally, the query string */
	char* s2; /**< @brief Generally, the reference string */
	sds cigar; /**< @brief Contains a sds string with the cigar string as the alignment is built */
	int success; /**< @brief Used when creating new structs to confirm that it was successfully allocated*/
	int length; /**< @brief The maximum length of the alignment */

	int _index; /**< @brief Internal index used when building the alignment string */
	uint_fast64_t _last_cigar_count; /**< @brief Internal accumulator for building the cigar string */
	char _last_cigar_operator; /**< @brief Internal the last cigar string operator */
} PairAlignment;

/** @brief
 * Represents the cigar operator, per the SAM definition */
typedef char CigarOperator;

#define CigarOperatorMatch     'M'
#define CigarOperatorMisMatch  'X'
#define CigarOperatorRefOnly   'I'
#define CigarOperatorQueryOnly 'D'
#define CigarOperatorNull      '@'

/** @brief Used as a return when working with pair_alignment methods to indicate success when nothing is normally returned. */
typedef struct PairAlignmentError {
	/** @brief overall success of the method */
	bool success;
	/** @todo Create error number list */
	/** @brief error number, see xxx */
	int error_number;
} PairAlignmentError;


PairAlignment* pair_alignment_create(int length);

PairAlignmentError pair_alignment_prepend(PairAlignment* pa, char c1, char c2, CigarOperator cigar_operator);

void pair_alignment_prepend_cigar(PairAlignment* pa, CigarOperator cigar_operator);

/**
 * @brief Retrieve the reference sequence from a given alignment as a sds string
 */
sds pair_alignment_get_reference(PairAlignment* pa);

/**
 * @brief Retrieve the query sequence from a given alignment as a sds string
 */
sds pair_alignment_get_query(PairAlignment* pa);

void pair_alignment_sprint(PairAlignment* pa);

void pair_alignment_free(PairAlignment** pa);

#endif /* ALIGNMENT_H */
