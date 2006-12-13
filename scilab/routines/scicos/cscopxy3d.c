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
void cscopxy3d(scicos_block * block, int flag)
{
  //REVOIR LES CONDITIONS  SUR CERTAINES VARIABLES
  /* Declarations*/
  int * ipar; //Integer Parameters
  int c__1 = 1;
  int color_flag; //Flag on Color
  int color[2];
  int line_size;
  int animed;
  int win; //Windows ID : To give a name to the window
  int buffer_size; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int nipar;
  int NbrPtsLong;
  int NbrPtsShort;
  double * rpar; //Reals parameters
  double *u1,*u2, *u3;
  double xmin, xmax, ymin, ymax, zmin, zmax,alpha, theta; //Ymin and Ymax are vectors here
  ScopeMemory * pScopeMemory;
  scoGraphicalObject Pinceau; //Pointer to each polyline of each axes
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
	number_of_curves_by_subwin = 2; //it is a trick to recognize the type of scope, not sure it is a good way because normally a curve is the combination of a short and a longdraw

	scoInitScopeMemory(block,&pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
	
	scoAddTitlesScope(pScopeMemory,"x","y","z");
	pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,0))->alpha = alpha;
	pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,0))->theta = theta;

	scoSetShortDrawSize(pScopeMemory,0,buffer_size);
	scoSetLongDrawSize(pScopeMemory,0,5000);
	scoAddPolylineForShortDraw(pScopeMemory,0,0,color[0]);
	scoAddPolylineForLongDraw(pScopeMemory,0,0,color[0]);

	Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
	Trait = scoGetPointerLongDraw(pScopeMemory,0,0);
	   
	break; //Break of the switch condition don t forget it
      } //End of Initialization

    case StateUpdate:
      {
	scoRetrieveScopeMemory(block,&pScopeMemory);
	/* Charging Elements */
	if (scoGetPointerScopeWindow(pScopeMemory) == NULL) // If the window has been destroyed we recreate it
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
	    scoInitOfWindow(pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
	    scoAddTitlesScope(pScopeMemory,"x","y","z");
	    scoAddPolylineForShortDraw(pScopeMemory,0,0,color[0]);
	    scoAddPolylineForLongDraw(pScopeMemory,0,0,color[0]);
	    pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,0))->alpha = alpha;
	    pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,0))->theta = theta;
	  }
	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);
	u3 = GetRealInPortPtrs(block,3);

	Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
	Trait = scoGetPointerLongDraw(pScopeMemory,0,0);

	NbrPtsShort = pPOLYLINE_FEATURE(Pinceau)->n1;

	pPOLYLINE_FEATURE(Pinceau)->pvx[NbrPtsShort] = u1[0];
	pPOLYLINE_FEATURE(Pinceau)->pvy[NbrPtsShort] = u2[0];
	pPOLYLINE_FEATURE(Pinceau)->pvz[NbrPtsShort] = u3[0];

	pPOLYLINE_FEATURE(Pinceau)->n1++;
	
	scoDrawScopeXYStyle(pScopeMemory);

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
