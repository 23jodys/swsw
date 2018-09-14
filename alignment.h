#include <stdbool.h>

typedef struct PairAlignment {
	char* s1;
	char* s2;
	int success;
	int length;
	int _index;
} PairAlignment;

PairAlignment * pair_alignment_create(int length);

void pair_alignment_prepend(PairAlignment* pa, char c1, char c2);

void pair_alignment_sprint(PairAlignment* pa);

void pair_alignment_free(PairAlignment** pa);
