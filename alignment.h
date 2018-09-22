#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <stdbool.h>

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

PairAlignment* pair_alignment_create(int length);

PairAlignmentError  pair_alignment_prepend(PairAlignment* pa, char c1, char c2);

void pair_alignment_sprint(PairAlignment* pa);

void pair_alignment_free(PairAlignment** pa);
void fake_malloc(void);
#endif /* ALIGNMENT_H */
