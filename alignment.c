#include <stdlib.h>
#include <stdio.h>

#include "sds/sds.h"
#include "swsw.h"

PairAlignment* pair_alignment_create(int length) {
	PairAlignment* _alignment;
       _alignment = malloc(sizeof(PairAlignment));
	if(_alignment == NULL) {
		return NULL;
	}
	_alignment->s1 = calloc(sizeof(char), length);
	_alignment->s2 = calloc(sizeof(char), length);
	if (!_alignment || !_alignment->s1 || !_alignment->s2) {
		return NULL;
	}
	_alignment->length = length;
	_alignment->_index = length - 1;
	return _alignment;
}

PairAlignmentError pair_alignment_prepend(PairAlignment * pa, char c1, char c2) {
	DEBUGLOG("Before -- c1: %c, c2: %c, index: %d, length: %d\n", c1, c2, pa->_index, pa->length);
	if (pa->_index < 0) {
		PairAlignmentError error = {.success=false, .error_number=1};
		return error;
	} 
	pa->s1[pa->_index] = c1;	
	pa->s2[pa->_index] = c2;	
	pa->_index -= 1;
	PairAlignmentError error = {.success=true, .error_number=0};
	DEBUGLOG("After -- c1: %c, c2: %c, index: %d, length: %d\n", c1, c2, pa->_index, pa->length);
	return error;
}

void pair_alignment_sprint(PairAlignment* pa) {
	for(int i=pa->_index + 1; i < pa->length; i++) {
		char sym = 'x';
		if (pa->s1[i] == pa->s2[i]) {
			sym = '-';
		} else {
			sym = ' ';
		}
		printf("%c%c%c\n", pa->s1[i], sym, pa->s2[i]);
	}
}

void pair_alignment_free(PairAlignment** pa) {
	free((*(*pa)).s1);
	free((*(*pa)).s2);
	(*(*pa)).s1 = NULL;
	(*(*pa)).s2 = NULL;
	free(*pa);

	*pa = NULL;
}

CigarString* cigar_string_create(PairAlignment* pa) {
	CigarString* result = malloc(sizeof(CigarString));
	result->cigar = sdsempty();
	result->pos = 0;
	return result;
}

void cigar_string_free(CigarString** cs) {
	sdsfree((*cs)->cigar);
	free(*cs);
	*cs = NULL;
}
