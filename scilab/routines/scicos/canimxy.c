#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"


/** \fn canimxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void canimxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  /* Declarations*/
  int i;
  int gomme_color; //As usual
  int * ipar; //Integer Parameters
  int color_flag; //Flag on Color
  int color[2];
  int line_size;
  int animed;
  int win; //Windows ID : To give a name to the window
  int buffer_size; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int nipar;
  double * rpar; //Reals parameters
  double xmin, xmax, ymin, ymax; //Ymin and Ymax are vectors here
  scoGraphicalObject Pinceau; //Pointer to each polyline of each axes
  scoGraphicalObject Gomme; //Pointer to each polyline of each axes
  scoGraphicalObject Trait; //Pointer to each trache of each axes
  int number_of_subwin;
  int number_of_curves_by_subwin;
  int dimension = 2;
  int nbr_curves;

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
  number_of_subwin = 1;

  /* If only one element to draw*/
  if (buffer_size == 1)
    {
      number_of_curves_by_subwin = nbr_curves;
      if(firstdraw == 1)
	{
	  scoInitScopeMemory(block,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	  scoSetShortDrawSize(*pScopeMemory,0,1);
	  scoSetLongDrawSize(*pScopeMemory,0,0);
	}
      scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
	    
      pFIGURE_FEATURE(scoGetPointerScopeWindow(*pScopeMemory))->pixmap = 1;
      pFIGURE_FEATURE(scoGetPointerScopeWindow(*pScopeMemory))->wshow = 1;


      for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(*pScopeMemory, 0) ; i++)
	{
	  scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[0]);
	  Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);
	  pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
	}
    }
  /*else if 2 or more elements*/
  else
    {
      number_of_curves_by_subwin = 2*nbr_curves; //it is a trick to recognize the type of scope, not sure it is a good way because normally a curve is the combination of a short and a longdraw
      if(firstdraw == 1)
	{
	  scoInitScopeMemory(block,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	}

      scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
      gomme_color = sciGetBackground(scoGetPointerAxes(*pScopeMemory,0));
      //sciSetIsBoxed(scoGetPointerAxes(*pScopeMemory,0),FALSE);
      /*if mark style*/
      if(color[0] < 0)
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
	    }
	}
    }
  scoAddTitlesScope(*pScopeMemory,"x","y",NULL);
}

/** \fn void canimxy(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void canimxy(scicos_block * block, int flag)
{
  /*Declarations*/
  ScopeMemory * pScopeMemory;
  double *u1,*u2;

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
	scoRetrieveScopeMemory(block,&pScopeMemory);
	/* Charging Elements */
	if (scoGetPointerScopeWindow(pScopeMemory) == NULL) // If the window has been destroyed we recreate it
	  {
	    canimxy_draw(block,&pScopeMemory,0);
	  }

	/*Retrieve Elements*/
	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);

	scoDrawScopeAnimXYStyle(pScopeMemory,u1,u2,NULL);

	break; //Break of the switch don t forget it !
      }//End of stateupdate
      
      //This case is activated when the simulation is done or when we close scicos
    case Ending:
      {
	scoRetrieveScopeMemory(block, &pScopeMemory);
	scoFreeScopeMemory(block, &pScopeMemory);
	break; //Break of the switch
      }
      //free the memory which is allocated at each turn by some variables
 
    }
}
