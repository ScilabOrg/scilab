#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"

/** \fn void ngevscope2(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cevscpe(scicos_block * block, int flag)
{
  /* Declarations */
  ScopeMemory * pScopeMemory;
  scoGraphicalObject pShortDraw;
  int nipar; //Number of elements in ipar vector
  int i; //As usual
  int * ipar; //Integer Parameter
  int nbr_colors; //Number of colors and lines IS ALSO number of channels
  int win; //To give a name to the window
  int color_flag; //0/1 color flag -- NOT USED
  int  * colors; //Begin at ipar[2] and has a measure of 8 max
  double t; //get_scicos_time()
  int tab[20];
  int dimension = 2;
  double period; //Refresh Period of the scope is a vector here
  int nbseg = 0;
  int number_of_subwin;
  int number_of_curves_by_subwin;
  double xmin, xmax, ymin, ymax;
  int win_pos[2], win_dim[2];
  switch(flag)
    {
    case Initialization:
      {
	/* Initialization */

	ipar =  GetIparPtrs(block);
	win = ipar[0];
	color_flag = ipar[1];
	period = block->rpar[0];
	nipar = GetNipar(block);
	nbr_colors = nipar-6;
	colors=(int*)malloc(nbr_colors*sizeof(int));
	for( i = 2 ; i < nbr_colors+2 ; i++)
	  {
	    colors[i-2] = ipar[i];
	  }

	number_of_subwin = 1;
	number_of_curves_by_subwin = nbr_colors;
	xmin = 0;
	xmax= period;
	ymin = 0;
	ymax = 1;

	win_pos[0] = 100;
	win_pos[1] = 100;
	win_dim[0] = 640;
	win_dim[1] = 400;

	scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	scoAddTitlesScope(pScopeMemory,"t","y",NULL);
	scoSetLongDrawSize(pScopeMemory,0,5000);
	scoSetShortDrawSize(pScopeMemory,0,1);
	scoSetPeriod(pScopeMemory,0,period);
	scoAddCoupleOfSegments(pScopeMemory,colors);
	break;
      }

    case StateUpdate:
      {
	t = get_scicos_time();
	/* Charging elements */
	scoRetrieveScopeMemory(block,&pScopeMemory);
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    ipar =  GetIparPtrs(block);
	    win = ipar[0];
	    color_flag = ipar[1];
	    period = block->rpar[0];
	    nipar = GetNipar(block);
	    nbr_colors = nipar-6;
	    colors=(int*)malloc(nbr_colors*sizeof(int));
	    for( i = 2 ; i < nbr_colors+2 ; i++)
	      {
		colors[i-2] = ipar[i];
	      }
	    xmin = 0;
	    xmax= period;
	    ymin = 0;
	    ymax = 1;
	    win_pos[0] = 100;
	    win_pos[1] = 100;
	    win_dim[0] = 640;
	    win_dim[1] = 400;
	    scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	    scoAddTitlesScope(pScopeMemory,"t","y",NULL);
	    scoAddCoupleOfSegments(pScopeMemory,colors);
	  }

	scoRefreshDataBoundsX(pScopeMemory,t);
	
	/*Not Factorize*/

	for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    if((block->nevprt&(1<<i))==(1<<i))
	      {
		tab[nbseg]=i;
		nbseg++;
	      }
	  }

	for(i = 0 ; i < nbseg ; i++)
	  {
	    pShortDraw = scoGetPointerShortDraw(pScopeMemory,0,tab[i]);
	    pSEGS_FEATURE(pShortDraw)->vx[0] = t;
	    pSEGS_FEATURE(pShortDraw)->vx[1] = t;
	    pSEGS_FEATURE(pShortDraw)->vy[0] = i*0.8/nbseg;
	    pSEGS_FEATURE(pShortDraw)->vy[1] = (i+1)*0.8/nbseg;
	    pSEGS_FEATURE(pShortDraw)->Nbr1 = 2;
	    pSEGS_FEATURE(pShortDraw)->Nbr2 = 2;
	  }
	/*End of Not Factorize*/
	scoDrawScopeAmplitudeTimeStyle(pScopeMemory,t);
	break;
      }

    case Ending:
      {
	scoRetrieveScopeMemory(block, &pScopeMemory);
	scoDelCoupleOfSegments(pScopeMemory);
	scoFreeScopeMemory(block,&pScopeMemory);
	break;
      }
    }
}
