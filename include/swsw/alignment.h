#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <stdbool.h>
#include "sds/sds.h"


PairAlignment* pair_alignment_create(int length);

PairAlignmentError pair_alignment_prepend(PairAlignment* pa, char c1, char c2);


void pair_alignment_sprint(PairAlignment* pa);



#endif /* ALIGNMENT_H */
