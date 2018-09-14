#include <stdlib.h>
#include <stdio.h>

#include "alignment.h"

PairAlignment * pair_alignment_create(int length) {
	PairAlignment* _alignment = malloc(sizeof(PairAlignment));

	_alignment->test = malloc(sizeof(int));
	_alignment->s1 = malloc(sizeof(char) * length);
	_alignment->s2 = malloc(sizeof(char) * length);
	if (!_alignment || !_alignment->s1 || !_alignment->s2) {
		return NULL;
	}
	_alignment->length = length;
	_alignment->_index = length - 1;
	return _alignment;
}

void pair_alignment_prepend(PairAlignment * pa, char c1, char c2) {
	printf("c1: %c, c2: %c\n", c1, c2);
	pa->s1[pa->_index] = c1;	
	pa->s2[pa->_index] = c2;	
	pa->_index -= 1;
}

void pair_alignment_sprint(PairAlignment * pa) {
	printf("index %d, length %d\n", pa->_index, pa->length);
	for(int i=pa->_index; i <= pa->length; i++) {
		printf("%c %c\n", pa->s1[i], pa->s2[i]);
	}
}

void pair_alignment_free(PairAlignment ** pa) {
	free((*(*pa)).test);
	free((*(*pa)).s1);
	free((*(*pa)).s2);
	(*(*pa)).s1 = NULL;
	(*(*pa)).s2 = NULL;
	free(*pa);

	*pa = NULL;
}

