#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <stdbool.h>
#include "sds/sds.h"
/** @file */

/**
 * @brief Represents the alignment of two strings 
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

/**
 * @brief Represents the cigar operator, per the SAM definition
 */
typedef char CigarOperator;

/** 
 * @brief cigar string character representing a match
 */
#define CigarOperatorMatch     'M'

/** 
 * @brief cigar string character representing a mismatch
 */
#define CigarOperatorMisMatch  'X'

/** 
 * @brief cigar string character representing an insertion on reference
 */
#define CigarOperatorRefOnly   'I'

/** 
 * @brief cigar string character representing an insertion on query
 */
#define CigarOperatorQueryOnly 'D'

/**
 * @brief cigar string null
 */
#define CigarOperatorNull      '@'

/**
 * @brief used to return status when methods would not nomrally return anything
 */
typedef struct PairAlignmentError {
	/** @brief overall success of the method */
	bool success;
	/** @todo Create error number list */
	/** @brief error number, see xxx */
	int error_number;
} PairAlignmentError;

/**
 * @brief create PairAlignment object
 * @param [in] length size of alignment to allocate
 */
PairAlignment* pair_alignment_create(int length);

/**
 * @brief prepend an alignment element to the cigar opject contained in PairAlignment
 * @param [in] pa object to modify
 * @param [in] c1 character on s1 half of alignment
 * @param [in] c2 character on s2 half of alignment
 * @param [in] cigar_operator CigarOperator
 */
PairAlignmentError pair_alignment_prepend(PairAlignment* pa, char c1, char c2, CigarOperator cigar_operator);

/**
 * @brief prepend cigar operator to buffer holding cigar string in PairAlignment object
 * @param [in] pa object to modify
 * @param [in] cigar_operator CigarOperator
 */
void pair_alignment_prepend_cigar(PairAlignment* pa, CigarOperator cigar_operator);

/**
 * @brief Retrieve the reference sequence from a given alignment as a sds string
 */
sds pair_alignment_get_reference(PairAlignment* pa);

/**
 * @brief Retrieve the query sequence from a given alignment as a sds string
 */
sds pair_alignment_get_query(PairAlignment* pa);

/**
 * @brief print a debug representation of the alignment
 * @brief pa PairAlignment to print
 */
void pair_alignment_sprint(PairAlignment* pa);

/**
 * @brief release all memory and set pointers in object to NULL and set passed in pointer to NULL
 * @brief pa PairAlignment to free
 */
void pair_alignment_free(PairAlignment** pa);

#endif /* ALIGNMENT_H */
