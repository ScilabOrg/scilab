#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"

/** \fn void ngmscope(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cmscope(scicos_block * block, int flag)
{
  ScopeMemory * pScopeMemory;
  //REVOIR LES CONDITIONS STATIC SUR CERTAINES VARIABLES
  /* Declarations */
  int i,j; //As usual
  int * ipar; //Integer Parameters
  int * colors; //Colors
  int win; //Windows ID : To give a name to the window
  int buffer_size; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int nu;
  int inherited_events;
  int nipar;
  int dimension = 2;
  double t; //get_scicos_time()
  double * rpar; //Reals parameters
  double dt; //Time++
  double * period; //Refresh Period of the scope is a vector here
  double * ymin,* ymax; //Ymin and Ymax are vectors here
  double * xmin, *xmax;
  int nbr_period;
  int * number_of_curves_by_subwin;
  int number_of_subwin;
  int NbrPtsShort;
  double * u1;
  int nbr_total_curves;
  scoGraphicalObject pShortDraw;

  /* Initializations and Allocations*/
  //Allocations are done here because there are dependent of some values presents below
 
  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {
	rpar = GetRparPtrs(block);
	ipar = GetIparPtrs(block);
	nipar = GetNipar(block);
	win = ipar[0];
	number_of_subwin = ipar[1];
	buffer_size = ipar[2];
	win_pos[0] = ipar[3];
	win_pos[1] = ipar[4];
	win_dim[0] = ipar[5];
	win_dim[1] = ipar[6];
	nbr_total_curves = 0;
	//Don't forget malloc for 'type *'
	number_of_curves_by_subwin = (int*)scicos_malloc(number_of_subwin*sizeof(int));
	for (i = 7; i < 7+number_of_subwin ; i++)
	  {
	    number_of_curves_by_subwin[i-7] = ipar[i];
	    nbr_total_curves = nbr_total_curves + ipar[i];
	  }
	colors = (int*)scicos_malloc(nbr_total_curves*sizeof(int));
	for(i = 0; i < nbr_total_curves ; i++)
	  {
	    colors[i] = ipar[i+7+number_of_subwin];
	  }
	inherited_events = ipar[7+number_of_subwin+number_of_subwin];

	dt = rpar[0];

	nbr_period = 0;
	period = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	for (i = 0 ; i < number_of_subwin ; i++)
	  {
	    period[i] = rpar[i+1];
	    nbr_period++; 
	  }

	ymin = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	ymax = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	for (i = 0 ; i < number_of_subwin ; i++)
	  {
	    ymin[i] = rpar[2*i+nbr_period+1];
	    ymax[i] = rpar[2*i+nbr_period+2];
	  }
	xmin = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	xmax = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	for (i = 0 ; i < number_of_subwin ; i++)
	  {
	    xmin[i] = 0;
	    xmax[i] = period[i];
	  }
	/*Allocating memory*/
	scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, number_of_curves_by_subwin);
	/*Creating the Scope*/
	scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, xmin, xmax, ymin, ymax, NULL, NULL);
	scoAddTitlesScope(pScopeMemory,"t","y",NULL);
	/*Must be placed before adding polyline or other elements*/
	for(i = 0 ; i < number_of_subwin ; i++)
	  {
	    scoSetLongDrawSize(pScopeMemory, i, 5000);
	    scoSetShortDrawSize(pScopeMemory,i,buffer_size);
	    scoSetPeriod(pScopeMemory,i,period[i]);
	  }
	/*Add a couple of polyline : one for the shortdraw and one for the longdraw*/
/* 	scoAddPolylineLineStyle(pScopeMemory,colors); */
	scoAddCoupleOfPolylines(pScopeMemory,colors);
      	scicos_free(number_of_curves_by_subwin);
	scicos_free(colors);
	scicos_free(period);
	scicos_free(ymin);
	scicos_free(ymax);
	scicos_free(xmin);
	scicos_free(xmax);
	break; //Break of the switch condition don t forget it
      } //End of Initialization
  
    case StateUpdate:
      {
	/* Charging Elements */
	t = get_scicos_time();
	/*Retreiving Scope in the block->work*/
	scoRetrieveScopeMemory(block,&pScopeMemory);
	/* If window has been destroyed we recreate it */
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    rpar = GetRparPtrs(block);
	    ipar = GetIparPtrs(block);
	    nipar = GetNipar(block);
	    nu = block->insz[0];
	    win = ipar[0];
	    number_of_subwin = ipar[1];
	    buffer_size = ipar[2];
	    win_pos[0] = ipar[3];
	    win_pos[1] = ipar[4];
	    win_dim[0] = ipar[5];
	    win_dim[1] = ipar[6];

	    //Don't forget scicos_malloc for 'type *'
	    number_of_curves_by_subwin = (int*)scicos_malloc(number_of_subwin*sizeof(int));
	    for (i = 7; i < 7+number_of_subwin ; i++)
	      {
		number_of_curves_by_subwin[i-7] = ipar[i];
	      }
	    colors = (int*)scicos_malloc(number_of_subwin*sizeof(int));
	    for(i = 7+number_of_subwin ; i < 7+number_of_subwin+number_of_subwin ; i++)
	      {
		colors[i-(7+number_of_subwin)] = ipar[i];
	      }
	    inherited_events = ipar[7+number_of_subwin+number_of_subwin];

	    dt = rpar[0];

	    nbr_period = 0;
	    period = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	    for (i = 0 ; i < number_of_subwin ; i++)
	      {
		period[i] = rpar[i+1];
		nbr_period++; 
	      }

	    ymin = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	    ymax = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	    for (i = 0 ; i < number_of_subwin ; i++)
	      {
		ymin[i] = rpar[2*i+nbr_period+1];
		ymax[i] = rpar[2*i+nbr_period+2];
	      }
	    xmin = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	    xmax = (double*)scicos_malloc(number_of_subwin*sizeof(double));
	    for (i = 0 ; i < number_of_subwin ; i++)
	      {
		xmin[i] = 0;
		xmax[i] = period[i];
	      }

	    scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, xmin, xmax, ymin, ymax, NULL, NULL);
	    scoAddTitlesScope(pScopeMemory,"t","y",NULL);
	    /*Readding Polylines*/
	    scoAddCoupleOfPolylines(pScopeMemory,colors);
	    scicos_free(number_of_curves_by_subwin);
	    scicos_free(colors);
	    scicos_free(ymin);
	    scicos_free(period);
	    scicos_free(ymax);
	    scicos_free(xmin);
	    scicos_free(xmax);
	  }


	scoRefreshDataBoundsX(pScopeMemory,t);

	//Here we are calculating the points in the polylines
	for (i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
	  {
	    u1 = GetRealInPortPtrs(block,i+1);
	    pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
	    NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
	    for (j = 0; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	      {
		pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
		pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort] = t;
		pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort] = u1[j];
		pPOLYLINE_FEATURE(pShortDraw)->n1++;
	      }
	  }

	scoDrawScopeAmplitudeTimeStyle(pScopeMemory, t);

	break; 
	//Break of the switch don t forget it !
      }//End of stateupdate
      
      //This case is activated when the simulation is done or when we close scicos
    case Ending:
      {
	scoRetrieveScopeMemory(block, &pScopeMemory);
	scoDelCoupleOfPolylines(pScopeMemory);
	scoFreeScopeMemory(block, &pScopeMemory);
	break;
      }
    }
}
