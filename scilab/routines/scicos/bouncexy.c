#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"


/** \fn void bouncexy(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void bouncexy(scicos_block * block,int flag)
{
  ScopeMemory * pScopeMemory;

  scoGraphicalObject pAxes;
  scoGraphicalObject pShortDraw;
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pTemp;
  double * z;
  double *rpar;
  int *ipar, nipar;  
  double t;
  int i,j;
  int dimension;
  double * u1, *u2;
  double ymin, ymax, xmin, xmax;
  int win;
  int number_of_subwin;
  int number_of_curves_by_subwin;
  int * colors;
  int imode;
  int * size_balls;
  switch(flag) 
    {
    case Initialization:
      {
	/*Retrieving Parameters*/
	rpar = GetRparPtrs(block);
	ipar = GetIparPtrs(block);
	nipar = GetNipar(block);
	win = ipar[0];
	if (win == -1)
	  {
	    win = 20000 + get_block_number() ; 
	  }
	dimension = 2;
	imode = ipar[1];
	number_of_curves_by_subwin = GetInPortRows(block,1);
	double radius_max;
	radius_max = 0;
	size_balls = (int*)scicos_malloc(number_of_curves_by_subwin*sizeof(int));
	z = GetDstate(block);
	for(i = 0 ; i < number_of_curves_by_subwin ; i++)
	  {
	    size_balls[i] = z[6*i+2];
	    if(radius_max < size_balls[i])
	      {
		radius_max = size_balls[i];
	      }
	  }
	number_of_subwin = 1;
	xmin = rpar[0];
	xmax = rpar[1];
	ymin = rpar[2];
	ymax = rpar[3];
	colors = (int*)scicos_malloc(number_of_curves_by_subwin*sizeof(int));
	for(i = 0 ; i < number_of_curves_by_subwin ; i++)
	  {
	    colors[i] = ipar[i+2];
	  }
	/*Allocating memory*/
	scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	/*Creating the Scope*/
	scoInitOfWindow(pScopeMemory, dimension, win, NULL, NULL, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	pTemp = scoGetPointerScopeWindow(pScopeMemory);
	pAxes = scoGetPointerAxes(pScopeMemory,0);
	pSUBWIN_FEATURE(pAxes)->isoview = TRUE;
	(pSUBWIN_FEATURE(pAxes)->axes).axes_visible[0] = FALSE;
	(pSUBWIN_FEATURE(pAxes)->axes).axes_visible[1] = FALSE;
	sciSetIsBoxed(pAxes, FALSE);
	pFIGURE_FEATURE(pTemp)->pixmap = 1;
	pFIGURE_FEATURE(pTemp)->wshow = 1;
	//	pFIGURE_FEATURE(scoGetPointerScopeWindow(pScopeMemory))->wshow = 1;
	/*Must be placed before adding polyline or other elements*/
	/* 	scoSetTraceLength(pScopeMemory, 0, 50); */
	/*scoSetBufferSize(pScopeMemory,0,buffer_size);*/
	/* scoSetPeriod(pScopeMemory,0,period); */
	/*Add a couple of polyline : one for the shortdraw and one for the longdraw*/
	for(j = 0 ; j < number_of_curves_by_subwin ; j++)
	  {
	    scoAddSphereForShortDraw(pScopeMemory, 0, j, size_balls[j], colors[j]);
	  }
	scoAddRectangleForLongDraw(pScopeMemory,0,0,xmin,(ymax-abs(ymin)),abs(xmax-xmin),abs(ymax-ymin));
	sciDrawObj(scoGetPointerLongDraw(pScopeMemory,0,0));
	scicos_free(colors);
	scicos_free(size_balls);
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
	    /*Retrieving Parameters*/
	    rpar = GetRparPtrs(block);
	    ipar = GetIparPtrs(block);
	    nipar = GetNipar(block);
	    win = ipar[0];
	    if (win == -1)
	      {
		win = 20000 + get_block_number() ; 
	      }
	    dimension = 2;
	    imode = ipar[1];
	    number_of_curves_by_subwin = GetInPortRows(block,1);
	    double radius_max;
	    radius_max = 0;
	    size_balls = (int*)scicos_malloc(number_of_curves_by_subwin*sizeof(int));
	    z = GetDstate(block);
	    for(i = 0 ; i < number_of_curves_by_subwin ; i++)
	      {
		size_balls[i] = z[6*i+2];
		if(radius_max < size_balls[i])
		  {
		    radius_max = size_balls[i];
		  }
	      }
	    number_of_subwin = 1;
	    xmin = rpar[0];
	    xmax = rpar[1];
	    ymin = rpar[2];
	    ymax = rpar[3];
	    colors = (int*)scicos_malloc(number_of_curves_by_subwin*sizeof(int));
	    for(i = 0 ; i < number_of_curves_by_subwin ; i++)
	      {
		colors[i] = ipar[i+2];
	      }
	    /*Allocating memory*/
	    /*Creating the Scope*/
	    scoInitOfWindow(pScopeMemory, dimension, win, NULL, NULL, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	    pTemp = scoGetPointerScopeWindow(pScopeMemory);
	    pAxes = scoGetPointerAxes(pScopeMemory,0);
	    pSUBWIN_FEATURE(pAxes)->isoview = TRUE;
	    (pSUBWIN_FEATURE(pAxes)->axes).axes_visible[0] = FALSE;
	    (pSUBWIN_FEATURE(pAxes)->axes).axes_visible[1] = FALSE;
	    sciSetIsBoxed(pAxes, FALSE);
	    pFIGURE_FEATURE(pTemp)->pixmap = 1;
	    pFIGURE_FEATURE(pTemp)->wshow = 1;

	    for(j = 0 ; j < number_of_curves_by_subwin ; j++)
	      {
		scoAddSphereForShortDraw(pScopeMemory, 0, j, size_balls[j], colors[j]);
	      }
	    scoAddRectangleForLongDraw(pScopeMemory,0,0,xmin,(ymax-abs(ymin)),abs(xmax-xmin),abs(ymax-ymin));
	    sciDrawObj(scoGetPointerLongDraw(pScopeMemory,0,0));
	    scicos_free(colors);
	    scicos_free(size_balls);
	  }
	/*Maybe we are in the end of axes so we have to draw new ones */
	//scoRefreshDataBoundsX(pScopeMemory,t);

	//Cannot be factorized depends of the scope
	size_balls = (int*)scicos_malloc(scoGetNumberOfCurvesBySubwin(pScopeMemory,0)*sizeof(int));
	z = GetDstate(block);
	for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    size_balls[i] = z[6*i+2];
	  }
	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);
	for (i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    pShortDraw  = scoGetPointerShortDraw(pScopeMemory,0,i);
	    pLongDraw  = scoGetPointerLongDraw(pScopeMemory,0,i);
	    pARC_FEATURE(pShortDraw)->x = u1[i]-size_balls[i]/2;
	    pARC_FEATURE(pShortDraw)->y = u2[i]+size_balls[i]/2;

	  }

	/* 	xset("wshow"); */
	sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	//sciDrawObj(sciGetCurrentFigure());

	sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
	//End of Cannot
	scicos_free(size_balls);
	//Draw the Scope
	//scoDrawScopeAmplitudeTimeStyle(pScopeMemory, t);
	break;
      }
    case Ending:
      {
	scoRetrieveScopeMemory(block, &pScopeMemory);
	/* scoDelPolylineLineStyle(pScopeMemory); */
	scoFreeScopeMemory(block, &pScopeMemory);
	break;  
      }
    }
}
