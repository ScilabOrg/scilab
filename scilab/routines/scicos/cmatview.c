/**
   \file cmatview.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CMATVIEW is a scope that connects a matrix to a grayplot. The values of the matrix are the values at the nodes
  \see CMATVIEW.sci in macros/scicos_blocks/Sinks/
*/

#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"
#include <math.h>
#include <stdlib.h>

/** \fn cmatview_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void cmatview_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  int i; //As usual
  int * ipar; //Integer Parameters
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int dimension = 2;
  double * rpar; //Reals parameters
  double  ymin, ymax; //Ymin and Ymax are vectors here
  double  xmin, xmax;
  int number_of_curves_by_subwin;
  int number_of_subwin;
  double * mat;
  int size_mat;

  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);
  number_of_subwin = 1;
  win_pos[0] = -1;
  win_pos[1] = -1;
  win_dim[0] = -1;
  win_dim[1] = -1;

  size_mat = ipar[2];
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
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
    }

  /*Creating the Scope*/
  scoInitOfWindow(*pScopeMemory, dimension, -1, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  sciSetColormap(scoGetPointerScopeWindow(*pScopeMemory), mat , size_mat/3, 3);
  scoAddTitlesScope(*pScopeMemory,"x","y",NULL);
  scoAddGrayplotForShortDraw(*pScopeMemory,0,0,GetInPortSize(block,1,1),GetInPortSize(block,1,2));
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
	cmatview_draw(block,&pScopeMemory,1);
	break;
      }
    case StateUpdate:
      {
	/*Retreiving Scope in the block->work*/
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	/* If window has been destroyed we recreate it */
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    cmatview_draw(block,&pScopeMemory,0);
	  }

	pShortDraw = scoGetPointerShortDraw(pScopeMemory,0,0);
	rpar = GetRparPtrs(block);
	alpha = rpar[0];
	beta = rpar[1];
	u1 = GetInPortPtrs(block,1);

	dim_i = GetInPortRows(block,1);
	dim_j = GetInPortCols(block,1);

	for(i = 0 ; i < dim_i ; i++)
	  {
	    for(j = 0; j < dim_j ; j++)
	      {
		pGRAYPLOT_FEATURE(pShortDraw)->pvecz[i*dim_j+j] = floor(alpha*u1[j+i*dim_j]+beta);
	      }
	  }
	sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	if(pFIGURE_FEATURE(scoGetPointerScopeWindow(pScopeMemory))->pixmap == 1)
	  {
	    C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  }
	sciDrawObj(scoGetPointerShortDraw(pScopeMemory,0,0));

	break;
      }//End of stateupdate
    case Ending:
      {
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break;
      }
    }
}
