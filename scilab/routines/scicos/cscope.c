#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"

/** \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cscope(scicos_block * block,int flag)
{
  ScopeMemory * pScopeMemory;
  scoGraphicalObject pShortDraw;
  double *rpar;
  int *ipar, nipar;
  int current_period_counter;   
  double t;
  double period;
  int i;
  int dimension;
  double * u1;
  double ymin, ymax, xmin, xmax;
  int buffer_size , NbrPtsShort;
  int win_pos[2];
  int win_dim[2];
  int win;
  int number_of_subwin;
  int number_of_curves_by_subwin[1];
  int * colors;

  switch(flag) 
    {
    case Initialization:
      {
	/*Retrieving Parameters*/
	rpar = GetRparPtrs(block);
	ipar = GetIparPtrs(block);
	nipar = GetNipar(block);
	buffer_size = ipar[2];
	win = ipar[0];
	if (win == -1)
	  {
	    win = 20000 + get_block_number() ; 
	  }
	period  = rpar[3];
	win_pos[0] = ipar[(nipar-1) - 3];
	win_pos[1] = ipar[(nipar-1) - 2];
	win_dim[0] = ipar[(nipar-1) - 1];
	win_dim[1] = ipar[nipar-1];
	dimension = 2;
	number_of_curves_by_subwin[0] = block->insz[0];

	number_of_subwin = 1;
	ymin = rpar[1];
	ymax = rpar[2];
	xmin = 0;
	xmax = period;

	colors = (int*)scicos_malloc(number_of_curves_by_subwin[0]*sizeof(int));
	for(i = 0 ; i < number_of_curves_by_subwin[0] ; i++)
	  {
	    colors[i] = ipar[3+i];
	  }

	/*Allocating memory*/
	scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, number_of_curves_by_subwin);
	/*Creating the Scope*/
	scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	scoAddTitlesScope(pScopeMemory,"t","y",NULL);

	/*Must be placed before adding polyline or other elements*/
	scoSetLongDrawSize(pScopeMemory, 0, 50);
	scoSetShortDrawSize(pScopeMemory,0,buffer_size);
	scoSetPeriod(pScopeMemory,0,period);
	/*Add a couple of polyline : one for the shortdraw and one for the longdraw*/
	scoAddCoupleOfPolylines(pScopeMemory,colors);
	/* scoAddPolylineLineStyle(pScopeMemory,colors); */
	scicos_free(colors);
	break;
      }
    case StateUpdate:
      {
	t = get_scicos_time();
	/*Retreiving Scope in the block->work*/
	scoRetrieveScopeMemory(block,&pScopeMemory);
	/*If window has been destroyed we recreate it*/
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    rpar = GetRparPtrs(block);
	    ipar = GetIparPtrs(block);
	    nipar = GetNipar(block);
	    buffer_size = ipar[2];
	    win = ipar[0];
	    if (win == -1)
	      {
		win = 20000 + get_block_number();
	      }
	    period  = rpar[3];
	    win_pos[0] = ipar[(nipar-1) - 3];
	    win_pos[1] = ipar[(nipar-1) - 2];
	    win_dim[0] = ipar[(nipar-1) - 1];
	    win_dim[1] = ipar[nipar-1];
	    dimension = 2;
	    ymin = rpar[1];
	    ymax = rpar[2];
	    current_period_counter = (int)(t/period);
	    xmin = period*(current_period_counter);
	    xmax = period*(current_period_counter+1);
	    colors = (int*)scicos_malloc(scoGetNumberOfCurvesBySubwin(pScopeMemory,0)*sizeof(int));
	    for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	      {
		colors[i] = ipar[3+i];
	      }
	    /*Recreating the Window*/
	    scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	    scoAddTitlesScope(pScopeMemory,"t","y",NULL);
	    scoAddCoupleOfPolylines(pScopeMemory,colors);
	    scicos_free(colors);
	  }
	/*Maybe we are in the end of axes so we have to draw new ones */
	scoRefreshDataBoundsX(pScopeMemory,t);

	//Cannot be factorized depends of the scope
	u1 = GetRealInPortPtrs(block,1);
	for (i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    pShortDraw  = scoGetPointerShortDraw(pScopeMemory,0,i);
	    NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
	    pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort] = t;
	    pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort] = u1[i];
	    pPOLYLINE_FEATURE(pShortDraw)->n1++;
	  }
	//End of Cannot

	//Draw the Scope
	scoDrawScopeAmplitudeTimeStyle(pScopeMemory, t);
	break;
      }
    case Ending:
      {
	scoRetrieveScopeMemory(block, &pScopeMemory);
	scoDelCoupleOfPolylines(pScopeMemory);
	scoFreeScopeMemory(block, &pScopeMemory);
	break;  
      }
    }
}
