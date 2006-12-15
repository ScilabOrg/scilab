#include "scoBase.h"
#include "scoMisc.h"
#include "scoMemoryScope.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "stdio.h"

void scoInitScopeMemory(scicos_block * block, ScopeMemory ** pScopeMemory, int number_of_subwin, int * number_of_curves_by_subwin)
{
  int i,j;
  *block->work = (ScopeMemory*)scicos_malloc(sizeof(ScopeMemory));
  if(*block->work == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  *pScopeMemory = (ScopeMemory*) *block->work;

  scoSetHandleScopeWindow((*pScopeMemory),0);

  scoSetNumberOfSubwin((*pScopeMemory),number_of_subwin);

  (*pScopeMemory)->number_of_curves_by_subwin = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->number_of_curves_by_subwin == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  for(i=0 ; i < number_of_subwin ; i++)
    {
      scoSetNumberOfCurvesBySubwin((*pScopeMemory),i,number_of_curves_by_subwin[i]);
    }

  (*pScopeMemory)->new_draw = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->new_draw == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetNewDraw(*pScopeMemory,i,0);
    }
 
  (*pScopeMemory)->shortdraw_size = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->shortdraw_size == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetShortDrawSize(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->period = (scoLineOfDouble)scicos_malloc(number_of_subwin*sizeof(scoDouble));
  if((*pScopeMemory)->period == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetPeriod(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->period_counter = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->period_counter == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetPeriodCounter(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->longdraw_size = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->longdraw_size == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetLongDrawSize(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->hAxes = (scoLineOfLongInteger)scicos_malloc(number_of_subwin*sizeof(scoLongInteger));
  if((*pScopeMemory)->hAxes == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetHandleAxes(*pScopeMemory,i,0);
    }


  (*pScopeMemory)->hShortDraw = (scoMatrixOfLongInteger)scicos_malloc(number_of_subwin*sizeof(scoLineOfLongInteger));
  if((*pScopeMemory)->hShortDraw == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }

  (*pScopeMemory)->hLongDraw = (scoMatrixOfLongInteger)scicos_malloc(number_of_subwin*sizeof(scoLineOfLongInteger));
  if((*pScopeMemory)->hLongDraw == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }

  for(i = 0 ; i < number_of_subwin ; i++)
    {
      (*pScopeMemory)->hShortDraw[i] = (scoLineOfLongInteger)scicos_malloc(number_of_curves_by_subwin[i]*sizeof(scoLongInteger));
      if((*pScopeMemory)->hShortDraw[i] == NULL)
	{
	  scoScopeError(*pScopeMemory, 1);
	}
      for(j=0; j < number_of_curves_by_subwin[i] ; j++)
	{
	  scoSetHandleShortDraw(*pScopeMemory,i,j,0);
	}
  
      (*pScopeMemory)->hLongDraw[i]  = (scoLineOfLongInteger)scicos_malloc(number_of_curves_by_subwin[i]*sizeof(scoLongInteger));
      if((*pScopeMemory)->hLongDraw[i] == NULL)
	{
	  scoScopeError(*pScopeMemory, 1);
	}
      for(j=0; j < number_of_curves_by_subwin[i] ; j++)
	{
	  scoSetHandleLongDraw(*pScopeMemory,i,j,0);
	}
    }
}


void scoFreeScopeMemory(scicos_block * block, ScopeMemory ** pScopeMemory)
{
  int i;
  scicos_free((*pScopeMemory)->new_draw);
  scicos_free((*pScopeMemory)->period_counter);
  scicos_free((*pScopeMemory)->longdraw_size);
  for(i = 0; i < (*pScopeMemory)->number_of_subwin ; i++)
    {
      scicos_free((*pScopeMemory)->hShortDraw[i]);
      scicos_free((*pScopeMemory)->hLongDraw[i]);
    }
  scicos_free((*pScopeMemory)->hShortDraw);
  scicos_free((*pScopeMemory)->hLongDraw);
  scicos_free((*pScopeMemory)->hAxes);
  scicos_free(*(block->work));

}

void scoRetrieveScopeMemory(scicos_block * block, ScopeMemory ** pScopeMemory)
{
  *pScopeMemory = (ScopeMemory*)*block->work;
}


void scoReallocLongDraw(scoGraphicalObject pLongDraw, int NbrPtsLong, int shortdraw_size, int plus)
{
  switch(sciGetEntityType(pLongDraw))
    {
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE(pLongDraw)->pvx = REALLOC((pPOLYLINE_FEATURE(pLongDraw)->pvx),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      pPOLYLINE_FEATURE(pLongDraw)->pvy = REALLOC((pPOLYLINE_FEATURE(pLongDraw)->pvy),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      if(pPOLYLINE_FEATURE(pLongDraw)->pvz != NULL)
	{
	  pPOLYLINE_FEATURE(pLongDraw)->pvz = REALLOC((pPOLYLINE_FEATURE(pLongDraw)->pvz),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
	}
      break;
    case SCI_SEGS:
      pSEGS_FEATURE(pLongDraw)->vx = REALLOC((pSEGS_FEATURE(pLongDraw)->vx),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      pSEGS_FEATURE(pLongDraw)->vy = REALLOC((pSEGS_FEATURE(pLongDraw)->vy),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      if(pSEGS_FEATURE(pLongDraw)->vz != NULL)
	{
	  pSEGS_FEATURE(pLongDraw)->vz = REALLOC((pSEGS_FEATURE(pLongDraw)->vz),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
	}
      break;
    default:
      sciprint("SCOPE ERROR : Error in scoReallocLongDraw()");
      fprintf(stderr,"SCOPE ERROR : Error in scoReallocLongDraw()");
      break;
    }
}
