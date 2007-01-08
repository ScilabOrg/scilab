#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"

#include <math.h>

/** \fn cmatview_draw(scicos_block * block, ScopeMemory * pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void cmatview_draw(scicos_block * block, ScopeMemory * pScopeMemory, int firstdraw)
{
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
  double * rpar; //Reals parameters
  double dt; //Time++
  double * period; //Refresh Period of the scope is a vector here
  double  ymin, ymax; //Ymin and Ymax are vectors here
  double  xmin, xmax;
  int nbr_period;
  int number_of_curves_by_subwin;
  int number_of_subwin;
  int nbr_total_curves;
  double * mat;
  int size_mat;
  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  win = ipar[0];
  number_of_subwin = 1;
  win_pos[0] = -1;
  win_pos[1] = -1;
  win_dim[0] = -1;
  win_dim[1] = -1;

  size_mat = ipar[3];
  mat = (double*)scicos_malloc(size_mat*sizeof(double));
  for(i = 0 ; i < size_mat ; i++)
    {
      mat[i] = rpar[i+2];
    }
  xmax = GetInPortSize(block,1,1);
  xmin = 0;
  ymax = GetInPortSize(block,1,2);
  ymin = 0;

  number_of_curves_by_subwin = 1;

  /*Allocating memory*/

  if(firstdraw == 1)
    {
      scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
    }

  /*Creating the Scope*/
  scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  sciSetColormap(scoGetPointerScopeWindow(pScopeMemory), mat , size_mat/3, 3);
  scoAddTitlesScope(pScopeMemory,"x","y",NULL);
  scoAddGrayplotForShortDraw(pScopeMemory,0,0,GetInPortSize(block,1,1),GetInPortSize(block,1,2));
  scicos_free(mat);
}

/** \fn void cmatview(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cmatview(scicos_block * block, int flag)
{
  /* Declarations */
  ScopeMemory * pScopeMemory;
  scoGraphicalObject pShortDraw;
  double * u1;
  double t;
  double alpha,beta;
  int i,j;
  double * rpar;
  int dim_i, dim_j;
  /* Initializations and Allocations*/
  //Allocations are done here because there are dependent of some values presents below
 
  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {
	cmatview_draw(block,pScopeMemory,1);
	break;
      }
    case StateUpdate:
      {
	/*Retreiving Scope in the block->work*/
	scoRetrieveScopeMemory(block,&pScopeMemory);
	/* If window has been destroyed we recreate it */
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    cmatview_draw(block,pScopeMemory,0);
	  }

	pShortDraw = scoGetPointerShortDraw(pScopeMemory,0,0);
        
	rpar = GetRparPtrs(block);
	alpha = rpar[0];
	beta = rpar[1];
	
	u1 = GetInPortPtrs(block,1);
	dim_i = GetInPortRows(block,1);
	dim_j = GetInPortCols(block,1);

	for(i = 1 ; i <= dim_i ; i++)
	  {
	    
	    for(j = 1; j <= dim_j ; j++)
	      {
		pGRAYPLOT_FEATURE(pShortDraw)->pvecz[(i-1)*dim_j+(j-1)] = floor(alpha*u1[(i-1)*dim_j+(j-1)]+beta);
	      }
	  }
	sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	sciDrawObj(scoGetPointerShortDraw(pScopeMemory,0,0));

	break;
      }//End of stateupdate
    case Ending:
      {
	scoRetrieveScopeMemory(block, &pScopeMemory);
	scoFreeScopeMemory(block, &pScopeMemory);
	break;
      }
    }
}
