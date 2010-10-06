/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library 
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 ------------------------------------------------------------------------/-*/

#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "PloEch.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "math_graphics.h"
#include "Format.h"
#include "HandleManagement.h"


#include "BuildDrawingObserver.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

//#include "../../../tclsci/includes/GedManagement.h"


#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)

/* Specifies the ticks labels buffer size */
#define LABEL_BUFFER_LENGTH    128


static void computeCoordinatesFromLogExponents(double* coordinates, int numCoordinates);
static void printLabels(char** stringVector, double* ticksLocations, int numTicks, BOOL logFlag);

static BOOL subwinNeedsDisplay(sciPointObj * pSubwin);



void sciRedrawFigure( void )
{
  sciDrawObj(sciGetCurrentFigure ());
}


void sciClearFigure(sciPointObj * pFigure)
{
  destroyGraphicsSons(pFigure);

	/* recreate the subwindow */
	createFirstSubwin(pFigure);

  forceHierarchyRedraw(pFigure);    
}

void sciXbasc()
{
  sciClearFigure(sciGetCurrentFigure());  
}

void sciXclear()
{
  static sciPointObj *masousfen, *tmpsousfen;
  sciSons *psonstmp; 
     
  tmpsousfen= sciGetCurrentSubWin();
  psonstmp = sciGetSons (sciGetCurrentFigure());
  
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	{
	  masousfen= (sciPointObj *)psonstmp->pointobj;
	  sciSetSelectedSubWin (masousfen);
	  sciSetdrawmode (FALSE); 
	}
      psonstmp = psonstmp->pnext;
    }
  sciSetSelectedSubWin (tmpsousfen);
  notifyObservers(sciGetCurrentFigure ());      
}


/* get the displayed bounds of an axis */
void sciGetDisplayedBounds( sciPointObj * pSubWin,
                            double      * xmin   ,
                            double      * xmax   ,
                            double      * ymin   ,
                            double      * ymax   ,
                            double      * zmin   ,
                            double      * zmax    )
{
    /*****************************************************************
     * get initial bounds
   *****************************************************************/

  int* tmp;
  int* zoomEnabled;
  double* bounds;
  int logFlag;

  zoomEnabled = (int*) getGraphicObjectProperty(pSubWin->UID, __GO_ZOOM_ENABLED__, jni_bool);

  if (*zoomEnabled)
  {
    bounds = (double*) getGraphicObjectProperty(pSubWin->UID, __GO_ZOOM_BOX__, jni_double_vector);
  }
  else
  {
    bounds = (double*) getGraphicObjectProperty(pSubWin->UID, __GO_DATA_BOUNDS__, jni_double_vector);
  }

  *xmin = bounds[0];
  *xmax = bounds[1];
  *ymin = bounds[2];
  *ymax = bounds[3];
  *zmin = bounds[4];
  *zmax = bounds[5];

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling X axis
   *****************************************************************/

  tmp = (int*) getGraphicObjectProperty(pSubWin->UID, __GO_X_AXIS_LOG_FLAG__, jni_bool);
  logFlag = *tmp;

  if (logFlag == 1)
  {
    if ( sciGetLogExponent( *xmin, *xmax, xmin, xmax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on X-axis xmin is negative.\n"));
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/

  tmp = (int*) getGraphicObjectProperty(pSubWin->UID, __GO_Y_AXIS_LOG_FLAG__, jni_bool);
  logFlag = *tmp;

  if (logFlag == 1)
  {
    if ( sciGetLogExponent( *ymin, *ymax, ymin, ymax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on Y-axis ymin is negative.\n"));
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Z axis
   *****************************************************************/

  tmp = (int*) getGraphicObjectProperty(pSubWin->UID, __GO_Z_AXIS_LOG_FLAG__, jni_bool);
  logFlag = *tmp;

  if (logFlag == 1)
  {
    if ( sciGetLogExponent( *zmin, *zmax, zmin, zmax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on Z-axis zmin is negative.\n"));
    }
  }

}

/* F.Leray au 13.10.04 completly review for new axes graduations */
/*** F.Leray 02.04.04 */
/* FUNCTION FOR 2D UPDATE ONLY !!!!! <=> beginning of axis_3ddraw (in 2d HERE of course! ) */
/* Copy on update_frame_bounds */
/*
 * This function has been adapted to the MVC framework.
 */
BOOL sci_update_frame_bounds_2d(sciPointObj *pobj)
{
  double xmax, xmin, ymin, ymax, zmin, zmax ;
  double hx,hy,hx1,hy1;
  int i;

  double FRect[4],WRect[4],ARect[4]; 
  char logscale[2];
  
  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */

  double* previousDataBounds;
  double updatedDataBounds[6];
  int logFlags[2];

  int tightLimits;
  int isoview;

  /* in order to determine whether or not the bounds have changed... */
  int nbsubtics[2];
  int nbgrads[2];
  int updatedNbsubtics[2];

  /* Temporary variables for ticks computation */
  double xGrads[20];
  double yGrads[20];

  int updatedNxgrads;
  int updatedNygrads;

  int nbxsubticks;
  int nbysubticks;

  /* Used to print labels */
  char** stringVector;

  int* tmp;
  /* End of Temp variables */

  /* Get the initial data bounds, number of ticks and number of subticks */

  /* Formerly FRect */
  previousDataBounds = (double*) getGraphicObjectProperty(pobj->UID, __GO_REAL_DATA_BOUNDS__, jni_double_vector);

  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int);
  nbsubtics[0] = *tmp;
  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int);
  nbsubtics[1] = *tmp;

  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_NUMBER_TICKS__, jni_int);
  nbgrads[0] = *tmp;
  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int);
  nbgrads[1] = *tmp;

  /* Sets the z-axis number of ticks to 0 */
  setGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_TICKS_LOCATIONS__, NULL, jni_double_vector, 0);

  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;

  /* _grad Init. to 0. */
  for(i = 0; i < 20; i++)
  {
    xGrads[i] = 0.0;
    yGrads[i] = 0.0;
  }

  /*
   * The code generating labels should be moved inside the Java Model.
   * Ticks labels were previously generated in the C++ Renderer module
   * (see AutomaticTicksComputer and AutoLogTicksComputer)
   * using the C-computed locations.
   * This is temporarily done here in order to get updated labels according
   * to the data bounds.
   */

  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_LOG_FLAG__, jni_bool);
  logFlags[0] = *tmp;

  /* x-axis */
  if (logFlags[0] == 0)
  {
    tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int);
    nbxsubticks = *tmp;

    TheTicks(&xmin, &xmax, &xGrads[0], &updatedNxgrads, FALSE);

    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    nbxsubticks = ComputeNbSubTics(pobj,updatedNxgrads,'n',NULL, nbxsubticks);

    stringVector = createStringArray(updatedNxgrads);
    printLabels(stringVector, xGrads, updatedNxgrads, 0);
  }
  /* log x-axis: to be implemented */
  else
  {
    tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int);
    nbxsubticks = *tmp;

    GradLog(xmin, xmax, &xGrads[0], &updatedNxgrads, FALSE);

    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    nbxsubticks = ComputeNbSubTics(pobj, updatedNxgrads, 'l', xGrads, 0);

    stringVector = createStringArray(updatedNxgrads);
    printLabels(stringVector, xGrads, updatedNxgrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(xGrads, updatedNxgrads);
  }

  setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_TICKS_LOCATIONS__, xGrads, jni_double_vector, updatedNxgrads);
  setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNxgrads);

  setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, &nbxsubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNxgrads);


  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_LOG_FLAG__, jni_bool);
  logFlags[1] = *tmp;

  /* y-axis */
  if (logFlags[1] == 0)
  {
    tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int);
    nbysubticks = *tmp;

    TheTicks(&ymin, &ymax, &yGrads[0], &updatedNygrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbysubticks = ComputeNbSubTics(pobj,updatedNygrads,'n',NULL, nbysubticks);

    stringVector = createStringArray(updatedNygrads);
    printLabels(stringVector, yGrads, updatedNygrads, 0);
  }
  else
  {
    tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int);
    nbysubticks = *tmp;

    GradLog(ymin, ymax, &yGrads[0], &updatedNygrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbysubticks = ComputeNbSubTics(pobj, updatedNygrads, 'l', yGrads, 0);

    stringVector = createStringArray(updatedNygrads);
    printLabels(stringVector, yGrads, updatedNygrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(yGrads, updatedNygrads);
  }

  setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LOCATIONS__, yGrads, jni_double_vector, updatedNygrads);
  setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNygrads);

  setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, &nbysubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNygrads);


  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_TIGHT_LIMITS__, jni_bool);
  tightLimits = *tmp;

  if (tightLimits == FALSE)
  {
    xmin = xGrads[0];
    xmax = xGrads[updatedNxgrads - 1];
    ymin = yGrads[0];
    ymax = yGrads[updatedNygrads - 1];
  }

  /*****************************************************************
   * modify  bounds if  isoview requested 
   *****************************************************************/

  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_ISOVIEW__, jni_bool);
  isoview = *tmp;

  /*
   * Depending on the high-level function calling update_specification_bounds, this section
   * may not be useful. Considering for example Plo2dn, the isoview property is initialized within
   * the strflag2properties function, called from Plo2dn after update_specification_bounds has returned.
   * Furthermore, in the previous version of the renderer, the C++ part recomputes the "real" data bounds
   * and sets them again.
   */
  if (isoview)
  {
    int wdim[2];
    char* parentId;
    int* figureDimensions;

    parentId = (char*) getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string);

    /*
     * Window width and height are currently 0 (default Figure values)
     * To be implemented within the MVC (by copying the Figure model's values)
     */
    figureDimensions = (int*) getGraphicObjectProperty(parentId, __GO_SIZE__, jni_int_vector);
    wdim[0] = figureDimensions[0];
    wdim[1] = figureDimensions[1];

#if 0
    wdim[0] = sciGetWindowWidth(sciGetParentFigure(pobj));
    wdim[1] = sciGetWindowHeight(sciGetParentFigure(pobj));
#endif

    hx=xmax-xmin;
    hy=ymax-ymin;

    getscale2d(WRect,FRect,logscale,ARect);
    
    wdim[0]=linint((double)wdim[0] *WRect[2]);
    wdim[1]=linint((double)wdim[1] *WRect[3]);
    
    if ( hx/(double)wdim[0]  <hy/(double) wdim[1] ) {
      hx1=wdim[0]*hy/wdim[1];
      xmin=xmin-(hx1-hx)/2.0;
      xmax=xmax+(hx1-hx)/2.0;
    }
    else {
      hy1=wdim[1]*hx/wdim[0];
      ymin=ymin-(hy1-hy)/2.0;
      ymax=ymax+(hy1-hy)/2.0;
    }
    
    /* F.Leray 28.09.04 */
    /* I need to recompute the correct xgrads and ygrads vector to have a good display */

    /* x-axis */
    if (logFlags[0] == 0)
    {
      TheTicks(&xmin, &xmax, xGrads, &updatedNxgrads, FALSE);
      /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
      nbxsubticks = ComputeNbSubTics(pobj, updatedNxgrads, 'n', NULL, nbxsubticks);

      stringVector = createStringArray(updatedNxgrads);
      printLabels(stringVector, xGrads, updatedNxgrads, FALSE);
    }
    else{ /* log. case */
      GradLog(xmin, xmax, xGrads, &updatedNxgrads, FALSE);
      nbxsubticks = ComputeNbSubTics(pobj, updatedNxgrads, 'l',xGrads,0);

      stringVector = createStringArray(updatedNxgrads);
      printLabels(stringVector, xGrads, updatedNxgrads, TRUE);

      /* Transform back exponents to non-log coordinates to set positions */
      computeCoordinatesFromLogExponents(xGrads, updatedNxgrads);
    }

    setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_TICKS_LOCATIONS__, xGrads, jni_double_vector, updatedNxgrads);
    setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNxgrads);

    setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, &nbxsubticks, jni_int, 1);

    destroyStringArray(stringVector, updatedNxgrads);

    /* y-axis */
    if (logFlags[1] == 0)
    {
      TheTicks(&ymin, &ymax, yGrads, &updatedNygrads, FALSE);
      /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
      nbysubticks = ComputeNbSubTics(pobj, updatedNygrads, 'n', NULL, nbysubticks);

      stringVector = createStringArray(updatedNygrads);
      printLabels(stringVector, yGrads, updatedNygrads, FALSE);
    }
    else{ /* log. case */
      GradLog(ymin, ymax, yGrads, &updatedNygrads, FALSE);
      nbysubticks = ComputeNbSubTics(pobj, updatedNygrads, 'l',yGrads,0);

      stringVector = createStringArray(updatedNygrads);
      printLabels(stringVector, yGrads, updatedNygrads, TRUE);

      /* Transform back exponents to non-log coordinates to set positions */
      computeCoordinatesFromLogExponents(yGrads, updatedNygrads);
    }

    setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LOCATIONS__, yGrads, jni_double_vector, updatedNygrads);
    setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNygrads);

    setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, &nbysubticks, jni_int, 1);

    destroyStringArray(stringVector, updatedNygrads);

    /* END ISO if */
  }


  /*****************************************************************
   * set the actual bounds in the Axes object
   *****************************************************************/

  updatedDataBounds[0] = xmin;
  updatedDataBounds[1] = xmax;
  updatedDataBounds[2] = ymin;
  updatedDataBounds[3] = ymax;
  updatedDataBounds[4] = zmin;
  updatedDataBounds[5] = zmax;

  setGraphicObjectProperty(pobj->UID, __GO_REAL_DATA_BOUNDS__, updatedDataBounds, jni_double_vector, 6);

  for(i=0;i<4;i++)
  {
    if (updatedDataBounds[i] != previousDataBounds[i])
    {
      return TRUE;
      break;
    }
  }

  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int);
  updatedNbsubtics[0] = *tmp;
  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int);
  updatedNbsubtics[1] = *tmp;
   
  for(i=0;i<2;i++) 
  {
    if(nbsubtics[i] != updatedNbsubtics[i])
    {
      return TRUE;
      break;
    }
  }

  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_NUMBER_TICKS__, jni_int);
  updatedNxgrads = *tmp;
  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int);
  updatedNygrads = *tmp;

  if(nbgrads[0] != updatedNxgrads) return TRUE;
  if(nbgrads[1] != updatedNygrads) return TRUE;
   
  return FALSE;
}

/**update_3dbounds -> renammed sci_update_frame_bounds_3d
 * @author Djalel Abdemouche 10/2003
 * Should be in Plo2dEch.c file
 */
BOOL sci_update_frame_bounds_3d(sciPointObj *pobj)
{
  double xmin,xmax,ymin,ymax,zmin,zmax; 
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj); 

  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */
  double ExistingFRect[6]; /* the Existing FRect at start to be compared at the end of this routine */
  /* in order to determine wheter or not the bounds have changed... */
  int nbsubtics[3];
  int nbgrads[3];
  /* End of Temp variables */

  for(i=0;i<6;i++) ExistingFRect[i] =  ppsubwin->FRect[i]; /* store old initial bounds*/
  
  for(i=0;i<3;i++) nbsubtics[i] = ppsubwin->axes.nbsubtics[i];
  nbgrads[0] = ppsubwin->axes.nxgrads;
  nbgrads[1] = ppsubwin->axes.nygrads;
  nbgrads[2] = ppsubwin->axes.nzgrads;

  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;
  

  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
      ppsubwin->axes.zgrads[i] = 0.;
    }
   
  
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE );
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
  
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
  }
  
  if ( ppsubwin->logflags[2]=='n') { /* z-axis */
    TheTicks(&zmin, &zmax, &(ppsubwin->axes.zgrads[0]), &ppsubwin->axes.nzgrads, FALSE);
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'n',NULL, ppsubwin->axes.nbsubtics[2]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(zmin,zmax,ppsubwin->axes.zgrads,&ppsubwin->axes.nzgrads, FALSE );
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'l',ppsubwin->axes.zgrads,0);
  }
  
  if(ppsubwin->tight_limits == FALSE )
    {
      xmin = ppsubwin->axes.xgrads[0];
      xmax = ppsubwin->axes.xgrads[ ppsubwin->axes.nxgrads - 1];
      ymin = ppsubwin->axes.ygrads[0];
      ymax = ppsubwin->axes.ygrads[ ppsubwin->axes.nygrads - 1];
      zmin = ppsubwin->axes.zgrads[0];
      zmax = ppsubwin->axes.zgrads[ ppsubwin->axes.nzgrads - 1];
    }

     
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
  ppsubwin->FRect[4]=zmin;
  ppsubwin->FRect[5]=zmax;
   
 
    
  for(i=0;i<6;i++)
    if(ppsubwin->FRect[i] != ExistingFRect[i]){
      return TRUE;
      break;
    } 
    
  for(i=0;i<3;i++) 
    if(nbsubtics[i] != ppsubwin->axes.nbsubtics[i]){
      return TRUE;
      break;
    }
  
  if(nbgrads[0] != ppsubwin->axes.nxgrads) return TRUE;
  if(nbgrads[1] != ppsubwin->axes.nygrads) return TRUE;
  if(nbgrads[2] != ppsubwin->axes.nzgrads) return TRUE;
  
  return FALSE;  
}







int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, const double * grads, int nbsubtics_input)
{
  int ticsval[] =    {2 ,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  int subticsval[] = {9,6,4,4,3,3,2,1,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
  int i;

#if 0
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);
#endif

  int* autoSubticks;


  if(logflag =='l'){
    return 8; /* 9 subtics to have a pretty tics/grid in log.*/
  }
  else{

    autoSubticks = (int*) getGraphicObjectProperty(pobj->UID, __GO_AUTO_SUBTICKS__, jni_bool);

#if 0
    if(ppsubwin->flagNax == FALSE) /* if auto subtics mode == ON */
#endif

    /* if auto subtics mode == ON */
    if (*autoSubticks)
      { 
	for(i=0;i<19;i++)
        {
	  if(nbtics == ticsval[i])
	    {
	      return subticsval[i];
	    }
        }
      }
    else /* if auto subtics mode == OFF already computed in Plo2dn.c, Champ.c routines... */
      {  /* or given via a.subtics=[nbsubtics_on_x, nbsubtics_on_y, nbsubtics_on_z] command */
	return nbsubtics_input;
      }
  }
  
  return -1;
}



/**DrawAxesIfRequired
 * Draws Axes (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
void DrawAxesIfRequired(sciPointObj * pobj)
{
  sciPointObj * pfigure = sciGetParentFigure(pobj);
  
  if( sciGetIsAutoDrawable(pfigure) && sciGetVisibility(pfigure) )
  {
    DrawAxes(pobj);
  }
  
}

/* Routine used inside Plo2dn.c, Champ.c, Gray.c... */
/* to force the drawing of the axes after a new object is created */
void DrawAxes(sciPointObj * pobj)
{
  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  sciDrawObj(psubwin);
}

/*---------------------------------------------------------------------------------*/
/**
 * draw the figure number numFigure.
 */
void sciDrawFigure( int numFigure )
{
  int curFigure = sciGetNumFigure( sciGetCurrentFigure() ) ;
  sciSetUsedWindow( numFigure ) ;
  sciDrawObj( sciGetCurrentFigure() ) ;
  sciSetUsedWindow( curFigure ) ;
}
/*---------------------------------------------------------------------------------*/




/**sciDrawObjIfRequired
 * Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciDrawObjIfRequired (sciPointObj * pobj)
{
  /*sciPointObj * pfigure = sciGetParentFigure(pobj);

  if( sciGetIsAutoDrawable(pfigure) && sciGetVisibility(pfigure) )
  {
    sciDrawObj( pobj ) ;
  }*/

  sciDrawObj( pobj ) ;

  return 0;
}
/*---------------------------------------------------------------------------------*/
void showPixmap(sciPointObj * pFigure)
{
  /* Hack here. Pixmap has same action as drawlater() */
  /* So we just draw the figure here to make it appear */
  if (sciGetPixmapMode(pFigure))
  {
    sciSetPixmapMode(pFigure, FALSE);
    sciDrawObj(pFigure);
    sciSetPixmapMode(pFigure, TRUE);
    sciDrawObj(pFigure);
  }
}
/*---------------------------------------------------------------------------------*/
void clearPixmap(sciPointObj * pFigure)
{
  // nothing to do with the hack
}
/*---------------------------------------------------------------------------------*/
BOOL needsDisplay(sciPointObj * pFigure)
{
	/* return false if the figure contains no or one subwindow and the subwindow is not displayed. */
	
	if (!sciGetVisibility(pFigure))
	{
		/* Figure not visible */
		return FALSE;
	}
	else if (sciGetNbTypedObjects(pFigure, SCI_SUBWIN) == 0)
	{
		/* No subwindows, return false */
		return FALSE;
	}
	else if (sciGetNbTypedObjects(pFigure, SCI_SUBWIN) == 1)
	{
		/* One subwindow check if it is visible */
		sciPointObj * onlySubwin = sciGetFirstTypedSelectedSon(pFigure, SCI_SUBWIN);
		return subwinNeedsDisplay(onlySubwin);
	}
	else
	{
		return TRUE;
	}
}
/*---------------------------------------------------------------------------------*/
static BOOL subwinNeedsDisplay(sciPointObj * pSubwin)
{
	/* the subwindow is not displayed if it does not have any children, its box is of and is transparent or */
	/* has the same background as the figure */
	if (!sciGetVisibility(pSubwin))
	{
		/* subwin invisible */
		return FALSE;
	}
	else if (sciGetNbChildren(pSubwin) > 4)
	{
		/* Other children than the labels */
		return TRUE;
	}
	else
	{
		BOOL axesVisible[3];

		if (sciGetBoxType(pSubwin) != BT_OFF)
		{
			/* Box is displayed */
			return TRUE;
		}

		sciGetAxesVisible(pSubwin, axesVisible);
		if (axesVisible[0] || axesVisible[1] || axesVisible[2])
		{
			/* One axis is visible */
			return TRUE;
		}

		if (   sciGetIsFilled(pSubwin)
			  && sciGetBackground(sciGetParentFigure(pSubwin)) != sciGetBackground(pSubwin))
		{
			/* Compare subwin background and figure one */
			return TRUE;
		}

		/* Check that labels texts are empty */
		if (   !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_title)
				|| !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_x_label)
				|| !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_y_label)
				|| !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_z_label))
		{
			return TRUE;
		}

		/* apparently no need to display the axes */
		return FALSE;
	}
}
/*---------------------------------------------------------------------------------*/
/*
 * Utility function which transforms back positions (logarithmic exponents) to non-log coordinates
 * Required to be able to display correct coordinate values in the console.
 */
static void computeCoordinatesFromLogExponents(double* coordinates, int numCoordinates)
{
    int i;
    double loginit = 1.0;

    for (i = 0; i < coordinates[0]; i++)
    {
      loginit *= 10.0;
    }

    for (i = 0; i < numCoordinates; i++)
    {
      coordinates[i] = loginit;
      loginit *= 10.0;
    }
}
/*---------------------------------------------------------------------------------*/
/*
 * Utility function which generates label strings
 * Its code is equivalent to that of the renderer module's TicksComputers' getTicksPosition
 * methods. It is temporarily located there, in order to be able to initialize labels strings,
 * and should reside within the Java Model.
 */
static void printLabels(char** stringVector, double* ticksLocations, int numTicks, BOOL logFlag)
{
    char labelBuffer[LABEL_BUFFER_LENGTH];
    char format[5];
    char* logPrefix = "10e";
    int i;
    int prefixLength;

    ChoixFormatE(format, ticksLocations[0], ticksLocations[numTicks-1],
        (ticksLocations[numTicks-1] - ticksLocations[0]) / (numTicks-1));

    if (logFlag == FALSE)
    {
        for (i = 0; i < numTicks; i++)
        {
            sprintf(labelBuffer, format, ticksLocations[i]);
            stringVector[i] = strdup(labelBuffer);
        }
    }
    else
    {
        prefixLength = strlen(logPrefix);

        for (i = 0; i < numTicks; i++)
        {
            strcpy(labelBuffer, logPrefix);

            sprintf(labelBuffer+prefixLength, format, ticksLocations[i]);
            stringVector[i] = strdup(labelBuffer);
        }
    }

}
/*---------------------------------------------------------------------------------*/
#undef round
