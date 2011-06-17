/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
/**
   \file cmatview.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CMATVIEW is a scope that connects a matrix to a grayplot. The values of the matrix are the values at the nodes
  \see CMATVIEW.sci in macros/scicos_blocks/Sinks/
*/
/*--------------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>
#include "DrawingBridge.h"
#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
/** \fn cmatview_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
SCICOS_BLOCKS_IMPEXP void cmatview_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  int i = 0; //As usual
  int * ipar = NULL; //Integer Parameters
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int dimension = 2;
  double * rpar = NULL; //Reals parameters
  double  ymin = 0., ymax = 0.; //Ymin and Ymax are vectors here
  double  xmin = 0., xmax = 0.;
  int number_of_curves_by_subwin = 0;
  int number_of_subwin = 0;
  double * mat = NULL;
  int size_mat = 0;
  char *label = NULL;

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
  label = GetLabelPtrs(block);

  /*Allocating memory*/

  if(firstdraw == 1)
    {
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
    }

  /*Creating the Scope*/
  scoInitOfWindow(*pScopeMemory, dimension, -1, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      sciSetColormap(scoGetPointerScopeWindow(*pScopeMemory), mat , size_mat/3, 3);
      scoAddTitlesScope(*pScopeMemory,label,"x","y",NULL);
      scoAddGrayplotForShortDraw(*pScopeMemory,0,0,GetInPortSize(block,1,1),GetInPortSize(block,1,2));
    }
  scicos_free(mat);

}
/*--------------------------------------------------------------------------*/
/** \fn void cmatview(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cmatview(scicos_block * block, int flag)
{
  /* Declarations */
  ScopeMemory * pScopeMemory = NULL;
  scoGraphicalObject pShortDraw;
  double * u1 = NULL;
  double alpha = 0.,beta = 0.;
  int i = 0, j = 0;
  double * rpar = NULL;
  int dim_i = 0, dim_j = 0;
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
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
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
	    if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
	      {
			  /* TODO : not implemented */
			  /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
	      }
	    sciDrawObj(scoGetPointerShortDraw(pScopeMemory,0,0));
	  }
	break;
      }//End of stateupdate
    case Ending:
      {
				scoRetrieveScopeMemory(block->work, &pScopeMemory);
				if(scoGetScopeActivation(pScopeMemory) == 1)
				{
					/* sciSetUsedWindow(scoGetWindowID(pScopeMemory)); */
					/*pShortDraw = sciGetCurrentFigure(); */
					/*pFIGURE_FEATURE(pShortDraw)->user_data = NULL; */
					/*pFIGURE_FEATURE(pShortDraw)->size_of_user_data = 0; */
					/* Check if figure is still opened, otherwise, don't try to destroy it again. */
					scoGraphicalObject figure = scoGetPointerScopeWindow(pScopeMemory);
					if (figure != NULL)
					{
						/*pShortDraw = scoGetPointerScopeWindow(pScopeMemory);*/
						clearUserData(figure);
					}
				}
				scoFreeScopeMemory(block->work, &pScopeMemory);
				break;
      }
    }
}
/*--------------------------------------------------------------------------*/
