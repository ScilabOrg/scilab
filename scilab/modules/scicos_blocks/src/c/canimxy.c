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
   \file canimxy.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CANIMXY is a scope in 2D which draw its input as a XY scope, there is animation.
   \see CANIMXY.sci in macros/scicos_blocks/Sinks/
*/
/*--------------------------------------------------------------------------*/ 
#include "CurrentObjectsManagement.h"
#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"
#include "DrawingBridge.h"
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
/** \fn canimxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
SCICOS_BLOCKS_IMPEXP void canimxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  /* Declarations*/
  int i = 0;
  int gomme_color = 0; //As usual
  int * ipar = NULL; //Integer Parameters
  int color_flag = 0; //Flag on Color
  int color[2];
  int line_size = 0;
  int animed = 0;
  int win = 0; //Windows ID : To give a name to the window
  int buffer_size = 0; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int nipar = 0;
  double * rpar = NULL; //Reals parameters
  double xmin = 0., xmax = 0., ymin = 0., ymax = 0.; //Ymin and Ymax are vectors here
  scoGraphicalObject Pinceau; //Pointer to each polyline of each axes
  scoGraphicalObject Gomme; //Pointer to each polyline of each axes
  scoGraphicalObject Trait; //Pointer to each trache of each axes
  int number_of_subwin = 0;
  int number_of_curves_by_subwin = 0;
  int dimension = 2;
  int nbr_curves = 0;
  char *label = NULL;

  /*Retrieving Parameters*/
  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  rpar = GetRparPtrs(block);
  win = ipar[0];
  color_flag = ipar[1];
  buffer_size = ipar[2];
  color[0] = ipar[3];
  color[1] = ipar[3];
  line_size = ipar[4];
  animed = ipar[5];
  win_pos[0] = ipar[6];
  win_pos[1] = ipar[7];
  win_dim[0] = ipar[8];
  win_dim[1] = ipar[9];
  nbr_curves = ipar[10];
  xmin = rpar[0];
  xmax = rpar[1];
  ymin = rpar[2];
  ymax = rpar[3]; 
  label = GetLabelPtrs(block);
  number_of_subwin = 1;
  /* If only one element to draw*/
  if (buffer_size == 1)
    {
      number_of_curves_by_subwin = nbr_curves;
      if(firstdraw == 1)
	{
	  scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	  scoSetShortDrawSize(*pScopeMemory,0,1);
	  scoSetLongDrawSize(*pScopeMemory,0,0);
	}
      scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
      if(scoGetScopeActivation(*pScopeMemory) == 1)
	{

	  //sciSetPixmapMode(scoGetPointerScopeWindow(*pScopeMemory),TRUE);

	  for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(*pScopeMemory, 0) ; i++)
	    {
	      scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[0]);
	      Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);
	      pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
	      sciSetMarkSize(Pinceau, line_size);

	    }
	}
    }
  /*else if 2 or more elements*/
  else
    {
      number_of_curves_by_subwin = 2*nbr_curves; //it is a trick to recognize the type of scope, not sure it is a good way because normally a curve is the combination of a short and a longdraw
      if(firstdraw == 1)
	{
	  scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	}

      scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
      if(scoGetScopeActivation(*pScopeMemory) == 1)
	{
	  gomme_color = sciGetBackground(scoGetPointerAxes(*pScopeMemory,0));

	 
	  if(color[0] <= 0)  /*if mark style*/
	    {
	      if(firstdraw == 1)
		{
		  scoSetShortDrawSize(*pScopeMemory,0,1);
		  scoSetLongDrawSize(*pScopeMemory,0,buffer_size-1);
		}
	      for(i = 0 ; i < nbr_curves ; i++)
		{
		  //because of color[0] is negative it will add a black mark with style number color[0]
		  scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[0]);
		  scoAddPolylineForShortDraw(*pScopeMemory,0,i+nbr_curves,color[0]); //same type of mark and black for the rubber
		  scoAddPolylineForLongDraw(*pScopeMemory,0,i,color[0]);
		    
		  Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);
		  Gomme = scoGetPointerShortDraw(*pScopeMemory,0,i+nbr_curves);
		  Trait = scoGetPointerLongDraw(*pScopeMemory,0,i);
		    
		  pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
		  pPOLYLINE_FEATURE(Gomme)->n1 = 1;
		  sciSetMarkForeground(Gomme, gomme_color); //here the rubber becomes colored like the background of the axes
		  pPOLYLINE_FEATURE(Trait)->n1 = buffer_size-1;

		  sciSetMarkSize(Pinceau, line_size);
		  sciSetMarkSize(Gomme, line_size);
		  sciSetMarkSize(Trait, line_size);
		}
	    }
	  /*if line style*/
	  else
	    {
	      if(firstdraw == 1)
		{
		  scoSetShortDrawSize(*pScopeMemory,0,2);
		  scoSetLongDrawSize(*pScopeMemory,0,buffer_size-1);
		}		
	      for(i = 0 ; i < nbr_curves ; i++)
		{
		  scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[0]);
		  scoAddPolylineForShortDraw(*pScopeMemory,0,i+nbr_curves,gomme_color);
		  scoAddPolylineForLongDraw(*pScopeMemory,0,i,color[0]);
		    
		  Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);
		  Gomme = scoGetPointerShortDraw(*pScopeMemory,0,i+nbr_curves);
		  Trait = scoGetPointerLongDraw(*pScopeMemory,0,i);
		    
		  pPOLYLINE_FEATURE(Pinceau)->n1 = 2;
		  pPOLYLINE_FEATURE(Gomme)->n1 = 2;
		  pPOLYLINE_FEATURE(Trait)->n1 = buffer_size-1;

		  sciSetLineWidth(Pinceau, line_size);
		  sciSetLineWidth(Gomme, line_size);
		  sciSetLineWidth(Trait, line_size);
		}
	    }
	}
    }
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      scoAddTitlesScope(*pScopeMemory,label,"x","y",NULL);
    }
}
/*--------------------------------------------------------------------------*/ 
/** \fn void canimxy(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void canimxy(scicos_block * block, int flag)
{
  /*Declarations*/
  ScopeMemory * pScopeMemory = NULL;
  double *u1 = NULL,*u2 = NULL;
  scoGraphicalObject pLongDraw;
  int i = 0;
  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {	
	canimxy_draw(block,&pScopeMemory,1);
	break; //Break of the switch condition don t forget it
      } //End of Initialization

    case StateUpdate:
      {
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {

	    /* Charging Elements */
	    if (scoGetPointerScopeWindow(pScopeMemory) == NULL) // If the window has been destroyed we recreate it
	      {
		canimxy_draw(block,&pScopeMemory,0);
	      }

	    /*Retrieve Elements*/
	    u1 = GetRealInPortPtrs(block,1);
	    u2 = GetRealInPortPtrs(block,2);

	    scoDrawScopeAnimXYStyle(pScopeMemory,u1,u2,NULL);
	  }
	break; //Break of the switch don t forget it !
      }//End of stateupdate
      
      //This case is activated when the simulation is done or when we close scicos
    case Ending:
      {
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    /* Check if figure is still opened, otherwise, don't try to destroy it again. */
	    scoGraphicalObject figure = scoGetPointerScopeWindow(pScopeMemory);
	    if (figure != NULL)
	      {
		if(scoGetLongDrawSize(pScopeMemory,0) == 0)
		  {
		    for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
		      {
			pLongDraw = scoGetPointerLongDraw(pScopeMemory,0,i);
			forceRedraw(pLongDraw);
		      }
		  }
		else
		  {
		    for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2 ; i++)
		      {
			pLongDraw = scoGetPointerLongDraw(pScopeMemory,0,i);
			forceRedraw(pLongDraw);
		      }
		  }
		clearUserData(figure);
	      }
	  }
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break; //Break of the switch
      }
      //free the memory which is allocated at each turn by some variables
 
    }
}
/*--------------------------------------------------------------------------*/ 
