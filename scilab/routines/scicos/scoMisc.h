#ifndef __SCO_MISC_H__
#define __SCO_MISC_H__

#include"scoBase.h"

/**
   \brief Stop simulation and return an indication of the bug
   \param pScopeMemory a pointer on a ScopeMemory
   \param code_error a code error (see in the function to determine the right number for the moment)
*/
void scoScopeError(ScopeMemory * pScopeMemory, int code_error);

/**
 */
extern int C2F(getlabel)();

/**
 */
extern int C2F(deletewin)();

/**
 */
void C2F (oldgraphics) (int *flag);

#endif
