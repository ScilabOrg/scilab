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
/**
   \file cevscpe.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CEVSCPE is a scope that indicates when the clocks is activated
   \see CEVENTSCOPE.sci in macros/scicos_blocks/Sinks/
*/

#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"

/** \fn cscopxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void cevscpe_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  /* Declarations */

  int nipar; //Number of elements in ipar vector
  int i; //As usual
  int * ipar;
  double * rpar; //Integer Parameter
  int nbr_colors; //Number of colors and lines IS ALSO number of channels
  int win; //To give a name to the window
  int color_flag; //0/1 color flag -- NOT USED
  int  * colors; //Begin at ipar[2] and has a measure of 8 max
  int dimension = 2;
  double period; //Refresh Period of the scope is a vector here
  int number_of_subwin;
  int number_of_curves_by_subwin;
  double xmin, xmax, ymin, ymax;
  int win_pos[2], win_dim[2];

  /* Initialization */
  ipar =  GetIparPtrs(block);
  win = ipar[0];
  color_flag = ipar[1]; /*not used*/
  rpar = GetRparPtrs(block);
  period = rpar[0];
  nipar = GetNipar(block);
  nbr_colors = nipar-6;
  colors=(int*)scicos_malloc(nbr_colors*sizeof(int));
  for( i = 2 ; i < nbr_colors+2 ; i++)
    {
      colors[i-2] = ipar[i];
    }

  number_of_subwin = 1;
  number_of_curves_by_subwin = nbr_colors;

  ymin = 0;
  ymax = 1;

  win_pos[0] = ipar[(nipar-1) - 3];
  win_pos[1] = ipar[(nipar-1) - 2];
  win_dim[0] = ipar[(nipar-1) - 1];
  win_dim[1] = ipar[nipar-1];

  if(firstdraw == 1)
    {
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
      scoSetLongDrawSize(*pScopeMemory,0,5000);
      scoSetShortDrawSize(*pScopeMemory,0,1);
      scoSetPeriod(*pScopeMemory,0,period);
    }

  xmin = period*scoGetPeriodCounter(*pScopeMemory,0);
  xmax = period*(scoGetPeriodCounter(*pScopeMemory,0)+1);

  scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      scoAddTitlesScope(*pScopeMemory,"t","y",NULL);
      scoAddCoupleOfSegments(*pScopeMemory,colors);
    }
  scicos_free(colors);
}

/** \fn void cevscpe(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cevscpe(scicos_block * block, int flag)
{
  ScopeMemory * pScopeMemory;
  int nbseg = 0;
  int tab[20];
  scoGraphicalObject pShortDraw;
  int i;
  double t;

  switch(flag)
    {
    case Initialization:
      {
	cevscpe_draw(block,&pScopeMemory,1);
	break;
      }

    case StateUpdate:
      {

	/* Charging elements */
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    t = get_scicos_time();
	    if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	      {
		cevscpe_draw(block,&pScopeMemory,0);
	      }

	    scoRefreshDataBoundsX(pScopeMemory,t);
	
	    /*Not Factorize*/

	    for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	      {
		if((GetNevIn(block)&(1<<i))==(1<<i))
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
	  }
	break;
      }

    case Ending:
      {
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	    pShortDraw = sciGetCurrentFigure();
	    pFIGURE_FEATURE(pShortDraw)->user_data = NULL;
	    pFIGURE_FEATURE(pShortDraw)->size_of_user_data = 0;
	    scoDelCoupleOfSegments(pScopeMemory);
	  }
	scoFreeScopeMemory(block->work,&pScopeMemory);
	break;
      }
    }
}
