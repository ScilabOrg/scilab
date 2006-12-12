#ifndef __SCO_MEMORYSCOPE_H__
#define __SCO_MEMORYSCOPE_H__

#include"scoBase.h"

/**
   \brief Initialization with malloc on the ScopeMemory and put in ScopeMemory the number_of_subwin and the number_of_curves_by_subwin
   \param block a pointer on a scicos_block, typically the bloc in agument of the computational function
   \param pScopeMemory a pointer on a pointer of a ScopeMemory, typically &pScopeMemory where pScopeMemory is a ScopeMemory *
   \param number_of_subwin Number of Subwin in the whole Scope
   \param number_of_curves_by_subwin Number of Curves for the subwin[i]
*/
void scoInitScopeMemory(scicos_block * block, ScopeMemory ** pScopeMemory, int number_of_subwin, int * number_of_curves_by_subwin);

/**
   \brief Retrieve the whole ScopeMemory and put in the pScopeMemory
   \param block a pointer on a scicos_block, typically the bloc in agument of the computational function
   \param pScopeMemory a pointer on a pointer of a ScopeMemory, typically &pScopeMemory where pScopeMemory is a ScopeMemory *
*/
void scoRetrieveScopeMemory(scicos_block * block, ScopeMemory ** pScopeMemory);

/**
   \brief Free the ScopeMemory which has been allocated by malloc in the scoInitScopeMemory
   \param block a pointer on a scicos_block, typically the bloc in agument of the computational function
   \param pScopeMemory a pointer on a pointer of a ScopeMemory, typically &pScopeMemory where pScopeMemory is a ScopeMemory *
*/
void scoFreeScopeMemory(scicos_block * block, ScopeMemory ** pScopeMemory);

/**
   \brief Realloc to extend the size of the longdraw in the memory
*/
void scoReallocLongDraw(scoGraphicalObject pLongDraw, int NbrPtsLong, int buffer_size, int plus);

#endif
