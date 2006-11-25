#include "scicos_block.h"

#define NULLL 0
#define FREE(x) if (x  != NULLL) free((char *) x);

void scicos_free(void *p)
{
  FREE(p);
}
