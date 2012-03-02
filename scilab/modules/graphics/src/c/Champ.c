/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philipp Chancelier
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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


#include "BuildObjects.h"
#include "Axes.h"
#include "Champ.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Plot2d.h"
#include "Scierror.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"

/*-----------------------------------------------------------------
 *  int C2F(champ)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
 *  int C2F(champ1)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
 *
 * Used for Drawing 2 dimensional vector fields
 * (fx[i+(*n1)*j], fy[i+(*n1)*j]) is the value of the vector field
 * at point X=(x[i],y[j]);
 *
 * - fx and fy are (*n1)*(*n2) matrix of double
 * - arfact : a factor by which to multiply the default arrow size
 *          usr 1.0 by defaut
 * - strflag : a string of length 3 (see plot2d)
 * - brect=[xmin,ymin,xmax,ymax]    (see plot2d)
 *
 * - lstr : (used when called from Fortran code)
 -------------------------------------------------------------------*/

void champg(char *name, int colored, double *x, double *y, double *fx, double *fy, int *n1,
	    int *n2, char *strflag, double *brect, double *arfact, int lstr)
{
    char* psubwinUID = NULL;
    char* newSegsUID = NULL;

    int clipState;
    char textLogFlags[3];
    double xx[2];
    double yy[2];
    double* boundingBox;
    double rotationAngles[2];
    int nn1=1;
    int nn2=2;
    int autoScale = 0;
    int* piAutoScale = &autoScale;
    int firstPlot = 0;
    int* piFirstPlot = &firstPlot;
    int logFlags[3];
    int flag,type =1;
    double arsize1;
    int *style;

    int iTmp = 0;
    int* piTmp = &iTmp;

    int i;
    double drect[6];
    BOOL bounds_changed = FALSE;
    BOOL axes_properties_changed = FALSE;

    /* champ with color inherited from subwin */
    /* or champ1 (normed vector + color) is enabled */
    int typeofchamp = ( colored == 0 ? 0 : 1 );

    /* First create champ object */
    /* F.Leray Allocation de style[dim = Nbr1] */
    if ((style = MALLOC ((*n1) * sizeof (int) )) == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"),"champg");
        return;
    }

    flag = 1; /* je le mets � 1 pour voir F.Leray 19.02.04*/
    arsize1 = *arfact;

    psubwinUID = getCurrentSubWin();

    /* then modify subwindow if needed */
    checkRedrawing();

    /* Force clipping to CLIPGRF (1) */
    clipState = 1;
    setGraphicObjectProperty(psubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    for(i=0;i<(*n1);i++)
    {
        style[i] = i;
    }

    newSegsUID = ConstructSegs(psubwinUID,type,x,y, NULL,*n1,*n2,0,fx,fy,flag,
                            style,arsize1,colored,typeofchamp);

    if (newSegsUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "champg");
        if (style != NULL)
        {
            FREE(style);
        }

        return;
    }

    setCurrentObject(newSegsUID);

    if( style != NULL )
    {
        FREE( style );
        style = NULL;
    }

    /* Force clipping to CLIPGRF (1) */
    clipState = 1;
    setGraphicObjectProperty(newSegsUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

  /* Deactivated since it tells the renderer module that the object has changed */
#if 0
  forceRedraw(newSegs); /* update drawer */
#endif

    /* Get segs bounding box */
    getGraphicObjectProperty(newSegsUID, __GO_BOUNDING_BOX__, jni_double_vector, &boundingBox);

    xx[0] = boundingBox[0];
    xx[1] = boundingBox[1];
    yy[0] = boundingBox[2];
    yy[1] = boundingBox[3];

    /* To be implemented */
#if 0
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
#endif

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(psubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    getGraphicObjectProperty(psubwinUID, __GO_AUTO_SCALE__, jni_bool, &piAutoScale);

    getGraphicObjectProperty(psubwinUID, __GO_FIRST_PLOT__, jni_bool, &piFirstPlot);

    if (autoScale)
    {
        /* compute and merge new specified bounds with the data bounds */
        switch (strflag[1])
        {
        case '0':
          /* do not change data bounds */
          break;
        case '1' : case '3' : case '5' : case '7':
          /* Force data bounds=brect */
          re_index_brect(brect,drect);
          break;
        case '2' : case '4' : case '6' : case '8': case '9':

          getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, &piTmp);
          logFlags[0] = iTmp;
          getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, &piTmp);
          logFlags[1] = iTmp;
          getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, &piTmp);
          logFlags[2] = iTmp;

          /* Conversion required by compute_data_bounds2 */
          textLogFlags[0] = getTextLogFlag(logFlags[0]);
          textLogFlags[1] = getTextLogFlag(logFlags[1]);
          textLogFlags[2] = getTextLogFlag(logFlags[2]);

          /* Force data bounds to the x and y bounds */
          compute_data_bounds2(0,'g',textLogFlags,xx,yy,nn1,nn2,drect);
          break;
        }

        /* merge data bounds and drect */
        if (!firstPlot &&
          (strflag[1] == '7' || strflag[1] == '8'|| strflag[1] == '9'))
        {
            double* dataBounds;

            getGraphicObjectProperty(psubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, &dataBounds);

            drect[0] = Min(dataBounds[0],drect[0]); /*xmin*/
            drect[2] = Min(dataBounds[2],drect[2]); /*ymin*/
            drect[1] = Max(dataBounds[1],drect[1]); /*xmax*/
            drect[3] = Max(dataBounds[3],drect[3]); /*ymax*/
        }

        if (strflag[1] != '0')
        {
            bounds_changed = update_specification_bounds(psubwinUID, drect,2);
        }

    }

    if (firstPlot)
    {
        bounds_changed = TRUE;
    }

    axes_properties_changed = strflag2axes_properties(psubwinUID, strflag);

    /* just after strflag2axes_properties */
    firstPlot = 0;
    setGraphicObjectProperty(psubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

  /*
   * Deactivated since it tells the renderer module that the object has changed
   * To be implemented
   */
#if 0
    if( bounds_changed || axes_properties_changed )
    {
        forceRedraw(psubwin);
    }
#endif

  /*
   * Deactivated since it involves drawing via the renderer module
   */
#if 0
    sciDrawObj(sciGetCurrentFigure());
#endif

}

int C2F(champ)(double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr)
{
  champg("champ",0,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0);
}

int C2F(champ1)(double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr)
{
  champg("champ1",1,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0);
}
/*----------------------------------------------------------------------------------*/

