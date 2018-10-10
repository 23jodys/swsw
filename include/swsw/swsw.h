#include "alignment.h"
#include "score_matrix.h"
#include "sw.h"

#ifdef DEBUG
#define DEBUGLOG(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt,  __FILE__, __LINE__, __func__, ##args)
#else 
#define DEBUGLOG( ...) 
#endif

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

/**
 * @brief Retrieve the reference sequence from a given alignment as a sds string
 */
sds pair_alignment_get_reference(PairAlignment* pa);

/**
 * @brief Retrieve the query sequence from a given alignment as a sds string
 */
sds pair_alignment_get_query(PairAlignment* pa);

void pair_alignment_free(PairAlignment** pa);

CigarString* cigar_string_create(PairAlignment* pa);

void cigar_string_free(CigarString**);
