#include "alignment.h"
#include "score_matrix.h"
#include "sw.h"

#ifdef DEBUG
#define DEBUGLOG(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt,  __FILE__, __LINE__, __func__, ##args)
#else 
#define DEBUGLOG( ...) 
#endif
