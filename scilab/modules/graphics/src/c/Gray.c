/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2005 - INRIA - Fabrice Leray
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
 --------------------------------------------------------------------------*/

#include "math_graphics.h"
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "Axes.h"
#include "Scierror.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "GrayPlot.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Plot2d.h"
#include "get_ticks_utils.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

int C2F(xgray)(double *x, double *y, double *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, long int l1)
{
    double xx[2],yy[2];
    int nn1=1,nn2=2;
    sciPointObj  *psubwin = NULL;
    sciPointObj* pgrayplot = NULL;
    double drect[6];
    BOOL bounds_changed = FALSE;
    BOOL isRedrawn;
    BOOL axes_properties_changed = FALSE;

    char textLogFlags[3];
    double rotationAngles[2];
    int clipState;
    int autoScale;
    int firstPlot;
    int logFlags[3];
    int autoSubticks;
    int* tmp;
  
    xx[0]=Mini(x,*n1);xx[1]=Maxi(x,*n1);
    yy[0]=Mini(y,*n2);yy[1]=Maxi(y,*n2);

    /* Adding F.Leray 22.04.04 */
    psubwin = sciGetCurrentSubWin();

    isRedrawn = checkRedrawing();

    /*
     * Deactivated for now
     * Searches the object hierarchy until a Surface object is found
     * in order to specify the view type (2D or 3D)
     * To be implemented
     */
    /* Force psubwin->is3d to FALSE: we are in 2D mode */
#if 0
    if (sciGetSurface(psubwin) == (sciPointObj *) NULL)
    {
        pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
        pSUBWIN_FEATURE (psubwin)->project[2]= 0;
    }
    else
    {
        pSUBWIN_FEATURE (psubwin)->theta_kp=pSUBWIN_FEATURE (psubwin)->theta;
        pSUBWIN_FEATURE (psubwin)->alpha_kp=pSUBWIN_FEATURE (psubwin)->alpha;  
    }
#endif

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(psubwin->UID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* Force psubwin->axes.aaint to those given by argument aaint*/
    /*****TO CHANGE F.Leray 10.09.04  for (i=0;i<4;i++) pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i]; */

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(psubwin->UID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    tmp = (int*) getGraphicObjectProperty(psubwin->UID, __GO_FIRST_PLOT__, jni_bool);
    firstPlot = *tmp;

    tmp = (int*) getGraphicObjectProperty(psubwin->UID, __GO_AUTO_SCALE__, jni_bool);
    autoScale = *tmp;

    if (autoScale)
    {
        /* compute and merge new specified bounds with the data bounds */
        switch (strflag[1])  {
          case '0': 
            /* do not change data bounds */
            break;
          case '1' : case '3' : case '5' : case '7':
            /* Force data bounds=brect */
            re_index_brect(brect, drect);
            break;
          case '2' : case '4' : case '6' : case '8': case '9':

            tmp = getGraphicObjectProperty(psubwin->UID, __GO_X_AXIS_LOG_FLAG__, jni_bool);
            logFlags[0] = *tmp;
            tmp = getGraphicObjectProperty(psubwin->UID, __GO_Y_AXIS_LOG_FLAG__, jni_bool);
            logFlags[1] = *tmp;
            tmp = getGraphicObjectProperty(psubwin->UID, __GO_Z_AXIS_LOG_FLAG__, jni_bool);
            logFlags[2] = *tmp;

            /* Conversion required by compute_data_bounds2 */
            textLogFlags[0] = getTextLogFlag(logFlags[0]);
            textLogFlags[1] = getTextLogFlag(logFlags[1]);
            textLogFlags[2] = getTextLogFlag(logFlags[2]);

            /* Force data bounds to the x and y bounds */
            compute_data_bounds2(0,'g',textLogFlags,xx,yy,nn1,nn2,drect);
            break;
        }

        /* merge data bounds and drect */
        if (!firstPlot &&(strflag[1] == '7' || strflag[1] == '8'))
        {
            double* dataBounds = (double*) getGraphicObjectProperty(psubwin->UID, __GO_DATA_BOUNDS__, jni_double_vector);

            drect[0] = Min(dataBounds[0],drect[0]); /*xmin*/
            drect[2] = Min(dataBounds[2],drect[2]); /*ymin*/
            drect[1] = Max(dataBounds[1],drect[1]); /*xmax*/
            drect[3] = Max(dataBounds[3],drect[3]); /*ymax*/
        }

        if (strflag[1] != '0') 
        {
            bounds_changed = update_specification_bounds(psubwin, drect,2);
        }
    }

    if (firstPlot)
    {
        bounds_changed = TRUE;
    }

    axes_properties_changed = strflag2axes_properties(psubwin, strflag);

    firstPlot = 0;
    setGraphicObjectProperty(psubwin->UID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and
    u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

    /* The MVC AUTO_SUBTICKS property corresponds to !flagNax */
    autoSubticks = !flagNax;
    setGraphicObjectProperty(psubwin->UID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

    if (flagNax == TRUE)
    {
        if (logFlags[0] == 0 && logFlags[1] == 0)
        {
            int autoTicks;

            autoTicks = 0;
            setGraphicObjectProperty(psubwin->UID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            setGraphicObjectProperty(psubwin->UID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);

            /*
             * Creates user-defined ticks using the Nax values
             * The MVC does not distinguish yet between automatically computed ticks
             * and user-defined ones.
             * To be implemented using the MVC framework
             */
#if 0
            CreatePrettyGradsFromNax(psubwin,aaint);
#endif
        }
        else
        {
            sciprint(_("Warning: Nax does not work with logarithmic scaling.\n"));
        }
    }

    if( bounds_changed || axes_properties_changed )
    {
      /*
       * Deactivated since it tells the renderer module that the object has changed
       * To be implemented
       */
#if 0
        forceRedraw(psubwin);
#endif
    }

    /* Constructs the object */
    pgrayplot = ConstructGrayplot((sciPointObj *) sciGetCurrentSubWin(), x,y,z,*n1,*n2,0);

    /* Failed allocation */
    if (pgrayplot == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "grayplot");
        return -1;
    }

    /* Sets the grayplot as current */
    sciSetCurrentObj(pgrayplot);

    /*
     * Deactivated as it performs redrawing.
     * To be implemented
     */
#if 0
    /* if the auto_clear is on we must redraw everything */
    if ( isRedrawn )
    {
        sciDrawObj( sciGetCurrentFigure() );
    }
    else
    {
        sciDrawObj( sciGetCurrentObj() );
        DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing */
    }
#endif
  
    return(0);
}





int C2F(xgray1)(double *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, long int l1)
{
  double xx[2],yy[2];
  static int nn1=1,nn2=2;
  sciPointObj * psubwin = NULL;
  sciPointObj * pGrayplot = NULL;
  double drect[6];
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;
  
  xx[0]=0.5;xx[1]= *n2+0.5;
  yy[0]=0.5;yy[1]= *n1+0.5;
  
  /* Adding F.Leray 22.04.04 */
  psubwin = sciGetCurrentSubWin();

  checkRedrawing() ;

  /* Force psubwin->is3d to FALSE: we are in 2D mode */
  if (sciGetSurface(psubwin) == (sciPointObj *) NULL)
  {
    pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
    pSUBWIN_FEATURE (psubwin)->project[2]= 0;
  }
  else
  {
    pSUBWIN_FEATURE (psubwin)->theta_kp=pSUBWIN_FEATURE (psubwin)->theta;
    pSUBWIN_FEATURE (psubwin)->alpha_kp=pSUBWIN_FEATURE (psubwin)->alpha;  
  }

  pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
  pSUBWIN_FEATURE (psubwin)->theta  = 270.0;

  /*****TO CHANGE F.Leray 10.09.04    for (i=0;i<4;i++)     pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i]; */

  /* Force "cligrf" clipping */
  sciSetIsClipping (psubwin,0); 

  /*---- Boundaries of the frame ----*/
  if (sciGetGraphicMode (psubwin)->autoscaling){
    /* compute and merge new specified bounds with psubwin->Srect */
    switch (strflag[1])  {
    case '0': 
      /* do not change psubwin->Srect */
      break;
    case '1' : case '3' : case '5' : case '7':
      /* Force psubwin->Srect=brect */
      re_index_brect(brect, drect);
      break;
    case '2' : case '4' : case '6' : case '8': case '9':
      compute_data_bounds2(0,'g',pSUBWIN_FEATURE(psubwin)->logflags,xx,yy,nn1,nn2,drect);
      break;
    }
    if (!pSUBWIN_FEATURE(psubwin)->FirstPlot && 
      (strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9')) { /* merge psubwin->Srect and drect */
        drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
        drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
        drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
        drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
    }
    if (strflag[1] != '0') 
      bounds_changed = update_specification_bounds(psubwin, drect,2);
  } 

  if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;

  axes_properties_changed = strflag2axes_properties(psubwin, strflag);

  pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */

  /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and 
  u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

  pSUBWIN_FEATURE(psubwin)->flagNax = flagNax; /* store new value for flagNax */

  if(pSUBWIN_FEATURE(psubwin)->flagNax == TRUE){
    if(pSUBWIN_FEATURE(psubwin)->logflags[0] == 'n' && pSUBWIN_FEATURE(psubwin)->logflags[1] == 'n')
    {
      BOOL autoTicks[3];
      sciGetAutoTicks(psubwin, autoTicks);
      /* x and y graduations are imposed by Nax */
      sciSetAutoTicks(psubwin, FALSE, FALSE, autoTicks[2]);

      CreatePrettyGradsFromNax(psubwin,aaint);
    }
    else{
      sciprint(_("Warning: Nax does not work with logarithmic scaling.\n"));
    }
  }



  if( bounds_changed || axes_properties_changed )
  {
    /* subwin has been modified by the above code */
    forceRedraw(psubwin);
  }

  /* Construct the grayplot object */
  pGrayplot = ConstructGrayplot(psubwin,NULL,NULL,z,*n1 + 1,*n2 + 1,1);

  if (pGrayplot == NULL)
  {
    // allocation error
    Scierror(999, _("%s: No more memory.\n"), "grayplot");
    return -1;
  }

  sciSetCurrentObj(pGrayplot);
  /* if the auto_clear is on we must redraw everything */

  sciDrawObj(pGrayplot);

  return 0;
}
  
 
/*-------------------------------------------------------
 * like xgray1 : 
 * but xrect here give the rectangle in which the 
 * grayplot is to be drawn using the current scale
 -------------------------------------------------------*/

int C2F(xgray2)(double *z, int *n1, int *n2, double *xrect)
{
  BOOL isRedrawn ;
  double y; /* void for ConstructGrayplot */ 
  sciPointObj *psubwin = NULL;

  isRedrawn = checkRedrawing() ;

  /*---- Boundaries of the frame ----*/
  psubwin = sciGetCurrentSubWin(); 
  sciSetIsClipping (psubwin,0); 

  sciDrawObj(psubwin); 
  sciSetCurrentObj (ConstructGrayplot 
    ((sciPointObj *)
    sciGetCurrentSubWin(),
    xrect,&y,z,*n1+1,*n2+1,2));
  /* if the auto_clear is on we must redraw everything */
  if ( isRedrawn )
  {
    sciDrawObj( sciGetCurrentFigure() ) ;
  }
  else
  {
    sciDrawObj(sciGetCurrentObj ());
    DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing */
  }

 
  return (0);
}
