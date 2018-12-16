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

	_alignment->s1 = sdsempty();
	_alignment->s1 = sdsgrowzero(_alignment->s1, length);

	_alignment->s2 = sdsempty();
	_alignment->s2 = sdsgrowzero(_alignment->s2, length);

	if (!_alignment || !_alignment->s1 || !_alignment->s2) {
		return NULL;
	}
	_alignment->length = length;
	_alignment->_index = length - 1;

	_alignment->cigar = sdsempty();

	_alignment->_last_cigar_count = 0;
	_alignment->_last_cigar_operator = CigarOperatorNull;

	return _alignment;
}

PairAlignmentError pair_alignment_prepend(PairAlignment * pa, char c1, char c2, CigarOperator cigar_operator) {
	DEBUGLOG("Before -- c1: %c, c2: %c, index: %d, length: %d\n", c1, c2, pa->_index, pa->length);
	if (pa->_index < 0) {
		PairAlignmentError error = {.success=false, .error_number=1};
		return error;
	} 
	pa->s1[pa->_index] = c1;	
	pa->s2[pa->_index] = c2;	

	pair_alignment_prepend_cigar(pa, cigar_operator);

	pa->_index -= 1;
	PairAlignmentError error = {.success=true, .error_number=0};
	DEBUGLOG("After -- c1: %c, c2: %c, index: %d, length: %d\n", c1, c2, pa->_index, pa->length);
	return error;
}

void pair_alignment_prepend_cigar(PairAlignment* pa, CigarOperator cigar_operator) {
	if (pa->_last_cigar_operator != cigar_operator && pa->_last_cigar_operator != CigarOperatorNull) {
		// Prepend the cigar operator and length to the existing cigar string in the pair alignment object
		sds temp_buffer = sdsempty();
		temp_buffer = sdscatprintf(temp_buffer, "%lld%c%s", pa->_last_cigar_count, pa->_last_cigar_operator, pa->cigar);
		sdsfree(pa->cigar);
		pa->cigar = temp_buffer;
		DEBUGLOG("Cigar operators '%c' and '%c' did not match, cigar string is now %s\n",
				pa->_last_cigar_operator, cigar_operator, pa->cigar);

		pa->_last_cigar_count = 1;
	} else {
		pa->_last_cigar_count++;
		DEBUGLOG("Cigar operators '%c' and '%c', _last_cigar_count is now %llu\n",pa->_last_cigar_operator, cigar_operator, pa->_last_cigar_count);
	}
	pa->_last_cigar_operator = cigar_operator;
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

sds pair_alignment_get_reference(PairAlignment* pa) {
	return pa->s2 + pa->_index + 1;
}

sds pair_alignment_get_query(PairAlignment* pa) {
	return pa->s1 + pa->_index + 1;
}

void pair_alignment_free(PairAlignment** pa) {
	sdsfree((*(*pa)).s1);
	sdsfree((*(*pa)).s2);
	sdsfree((*(*pa)).cigar);
	(*(*pa)).s1 = NULL;
	(*(*pa)).s2 = NULL;
	free(*pa);

	*pa = NULL;
}
