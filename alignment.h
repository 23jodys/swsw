#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <stdbool.h>
#include "sds/sds.h"

typedef struct PairAlignment {
	char* s1;
	char* s2;
	int success;
	int length;
	int _index;
} PairAlignment;

typedef struct PairAlignmentError {
	bool success;
	int error_number;
} PairAlignmentError;

typedef struct CigarString {
	bool success;
	int error_number;
	size_t pos;
	sds cigar;
} CigarString;

PairAlignment* pair_alignment_create(int length);

PairAlignmentError pair_alignment_prepend(PairAlignment* pa, char c1, char c2);

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

CigarString* cigar_string_create(PairAlignment* pa);

void cigar_string_free(CigarString**);


#endif /* ALIGNMENT_H */
