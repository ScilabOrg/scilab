#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"

/** \fn void canimxy3d(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void canimxy3d(scicos_block * block, int flag)
{
  //REVOIR LES CONDITIONS  SUR CERTAINES VARIABLES
  /* Declarations*/
  int i; //As usual
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
  double *u1,*u2,*u3;
  double xmin, xmax, ymin, ymax, zmin, zmax,alpha,theta; //Ymin and Ymax are vectors here
  ScopeMemory * pScopeMemory;
  scoGraphicalObject Pinceau; //Pointer to each polyline of each axes
  scoGraphicalObject Gomme; //Pointer to each polyline of each axes
  scoGraphicalObject Trait; //Pointer to each trache of each axes
  int number_of_subwin;
  int number_of_curves_by_subwin;
  int dimension = 3;
  /* Initializations and Allocations*/
  //Allocations are done here because there are dependent of some values presents below
  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {	
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
	xmin = rpar[0];
	xmax = rpar[1];
	ymin = rpar[2];
	ymax = rpar[3]; 
	zmin = rpar[4];
	zmax = rpar[5];
        alpha = rpar[6];
        theta = rpar[7];
	number_of_subwin = 1;

	/* If only one element to draw*/
	if (buffer_size == 1)
	  {
	    number_of_curves_by_subwin = 1;

	    scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	    scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
	    pFIGURE_FEATURE(scoGetPointerScopeWindow(pScopeMemory))->pixmap = 1;
	    pFIGURE_FEATURE(scoGetPointerScopeWindow(pScopeMemory))->wshow = 1;

	    scoSetShortDrawSize(pScopeMemory,0,1);

	    scoAddPolylineForShortDraw(pScopeMemory,0,0,color[0]);

	    Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);

	    pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
	  }
	/*else if 2 or more elements*/
	else
	  {
	    number_of_curves_by_subwin = 2; //it is a trick to recognize the type of scope, not sure it is a good way because normally a curve is the combination of a short and a longdraw

	    scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	    scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
	    //sciSetIsBoxed(scoGetPointerAxes(pScopeMemory,0),FALSE);
	    /*if mark style*/
	    if(color[0] < 0)
	      {
		scoSetShortDrawSize(pScopeMemory,0,1);
		scoSetLongDrawSize(pScopeMemory,0,buffer_size-1);
		scoAddPolylineForShortDraw(pScopeMemory,0,0,color[0]);
		scoAddPolylineForShortDraw(pScopeMemory,0,1,color[0]);
		scoAddPolylineForLongDraw(pScopeMemory,0,0,color[0]);

		Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
		Gomme = scoGetPointerShortDraw(pScopeMemory,0,1);
		Trait = scoGetPointerLongDraw(pScopeMemory,0,0);
	    
		pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
		pPOLYLINE_FEATURE(Gomme)->n1 = 1;
		pPOLYLINE_FEATURE(Trait)->n1 = buffer_size-1;
	      }
	    /*if line style*/
	    else
	      {
		scoSetShortDrawSize(pScopeMemory,0,2);
		scoSetLongDrawSize(pScopeMemory,0,buffer_size-1);
		scoAddPolylineForShortDraw(pScopeMemory,0,0,color[0]);
		scoAddPolylineForShortDraw(pScopeMemory,0,1,color[0]);
		scoAddPolylineForLongDraw(pScopeMemory,0,0,color[0]);

		Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
		Gomme = scoGetPointerShortDraw(pScopeMemory,0,1);
		Trait = scoGetPointerLongDraw(pScopeMemory,0,0);
	    
		pPOLYLINE_FEATURE(Pinceau)->n1 = 2;
		pPOLYLINE_FEATURE(Gomme)->n1 = 2;
		pPOLYLINE_FEATURE(Trait)->n1 = buffer_size-1;
	      }

	    /*Xor Mode = 6 // Copy Mode = 3 */ 
	    sciSetXorMode(scoGetPointerScopeWindow(pScopeMemory), 6);
	  }
	pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,0))->alpha = alpha;
	pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,0))->theta = theta;
	scoAddTitlesScope(pScopeMemory,"x","y","z");
	break; //Break of the switch condition don t forget it
      } //End of Initialization

    case StateUpdate:
      {
	scoRetrieveScopeMemory(block,&pScopeMemory);
	/* Charging Elements */
	if (scoGetPointerScopeWindow(pScopeMemory) == NULL) // If the window has been destroyed we recreate it
	  {
	  }

	/*Retrieve Elements*/
	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);
	u3 = GetRealInPortPtrs(block,3);

	/*If only one element to draw*/
	if(scoGetNumberOfCurvesBySubwin(pScopeMemory,0) == 1)
	  {
	    Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
	    pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[0];
	    pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[0];
	    pPOLYLINE_FEATURE(Pinceau)->pvz[0] = u3[0];

	    sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	    C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    sciDrawObj(scoGetPointerAxes(pScopeMemory,0));
	  }
	else
	  {
	    Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
	    Gomme = scoGetPointerShortDraw(pScopeMemory,0,1);
	    Trait = scoGetPointerLongDraw(pScopeMemory,0,0);
	    /* Calculating and Drawing */

	    /*if it is a mark style scope*/
	    if(scoGetShortDrawSize(pScopeMemory,0) == 1)
	      {
		pPOLYLINE_FEATURE(Gomme)->pvx[0] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-1];
		pPOLYLINE_FEATURE(Gomme)->pvy[0] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-1];
		pPOLYLINE_FEATURE(Gomme)->pvz[0] = pPOLYLINE_FEATURE(Trait)->pvz[scoGetLongDrawSize(pScopeMemory,0)-1];

		for (i = scoGetLongDrawSize(pScopeMemory,0)-1 ; i > 0 ; i--)
		  {
		    pPOLYLINE_FEATURE(Trait)->pvx[i] = pPOLYLINE_FEATURE(Trait)->pvx[i-1];
		    pPOLYLINE_FEATURE(Trait)->pvy[i] = pPOLYLINE_FEATURE(Trait)->pvy[i-1];
		    pPOLYLINE_FEATURE(Trait)->pvz[i] = pPOLYLINE_FEATURE(Trait)->pvz[i-1];
		  }

		pPOLYLINE_FEATURE(Trait)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
		pPOLYLINE_FEATURE(Trait)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
		pPOLYLINE_FEATURE(Trait)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[0];
		
		pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[0];
		pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[0];
		pPOLYLINE_FEATURE(Pinceau)->pvz[0] = u3[0];		

		sciSetUsedWindow(scoGetWindowID(pScopeMemory));
		sciDrawObj(Pinceau);
		sciSetMarkForeground(Gomme,1);
		sciDrawObj(Gomme);
		sciSetMarkForeground(Gomme,8);
	      }
	    /*if it is a line style scope*/
	    else
	      {
		pPOLYLINE_FEATURE(Gomme)->pvx[0] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-1];
		pPOLYLINE_FEATURE(Gomme)->pvy[0] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-1];
		pPOLYLINE_FEATURE(Gomme)->pvz[0] = pPOLYLINE_FEATURE(Trait)->pvz[scoGetLongDrawSize(pScopeMemory,0)-1];

		pPOLYLINE_FEATURE(Gomme)->pvx[1] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-2];
		pPOLYLINE_FEATURE(Gomme)->pvy[1] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-2];
		pPOLYLINE_FEATURE(Gomme)->pvz[1] = pPOLYLINE_FEATURE(Trait)->pvz[scoGetLongDrawSize(pScopeMemory,0)-2];

		for (i = scoGetLongDrawSize(pScopeMemory,0)-1 ; i > 0 ; i--)
		  {
		    pPOLYLINE_FEATURE(Trait)->pvx[i] = pPOLYLINE_FEATURE(Trait)->pvx[i-1];
		    pPOLYLINE_FEATURE(Trait)->pvy[i] = pPOLYLINE_FEATURE(Trait)->pvy[i-1];
		    pPOLYLINE_FEATURE(Trait)->pvz[i] = pPOLYLINE_FEATURE(Trait)->pvz[i-1];
		  }

		pPOLYLINE_FEATURE(Trait)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
		pPOLYLINE_FEATURE(Trait)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
		pPOLYLINE_FEATURE(Trait)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[0];
		
		pPOLYLINE_FEATURE(Pinceau)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[1];
 		pPOLYLINE_FEATURE(Pinceau)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[1];
		pPOLYLINE_FEATURE(Pinceau)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[1];
		pPOLYLINE_FEATURE(Pinceau)->pvx[1] = u1[0];
		pPOLYLINE_FEATURE(Pinceau)->pvy[1] = u2[0];
		pPOLYLINE_FEATURE(Pinceau)->pvz[1] = u3[0];

		sciSetUsedWindow(scoGetWindowID(pScopeMemory));
		sciDrawObj(Pinceau);
		sciSetMarkForeground(Gomme,1);
		sciDrawObj(Gomme);
		sciSetMarkForeground(Gomme,8);
	      }
	  }
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
