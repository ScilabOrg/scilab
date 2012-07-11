/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2002 - INRIA - Serge Steer
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
#include "axesScale.h"
#include "Format.h"
#include "HandleManagement.h"
#include "Axes.h"

#include "MALLOC.h" /* MALLOC */

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentSubwin.h"

/*-------------------------------------------
 * setscale2d
 * uses WRect,ARect,FRect,logscale to update
 * current subwindow
 *  WRect gives the subwindow to use
 *  ARect gives the axis rectangle
 *  FRect gives the bounds
 *  WRect=[<x-upperleft>,<y-upperleft>,largeur,hauteur]
 *    example WRect=[0,0,1.0,1.0] we use all the window
 *            WRect=[0.5,0.5,0.5,0.5] we use the down right
 *            quarter of the window
 *  logscale : gives xy log axis flags
 *  each argument can be a null pointer if they are
 *  not to be changed from their current value
 *
 *
 *-------------------------------------------*/

extern void unzoom()
{
  /** 17/09/2002 ***/
  double fmin = 0., fmax = 0, lmin = 0., lmax = 0.;
  int min = 0, max = 0, puiss = 0, deux = 2, dix = 10;
  sciPointObj *psousfen = NULL;
  sciSons *psonstmp = NULL;

  /***** 02/10/2002 ****/
  psonstmp = sciGetSons(sciGetCurrentFigure());
  while (psonstmp != (sciSons *) NULL)
  {
    if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
    {
      psousfen= (sciPointObj *)psonstmp->pointobj;
      if (sciGetZooming(psousfen))
      {
        sciSetZooming((char*)psousfen, 0);

        pSUBWIN_FEATURE (psousfen)->ZRect[0]   = pSUBWIN_FEATURE (psousfen)->SRect[0];
        pSUBWIN_FEATURE (psousfen)->ZRect[1]   = pSUBWIN_FEATURE (psousfen)->SRect[1];
        pSUBWIN_FEATURE (psousfen)->ZRect[2]   = pSUBWIN_FEATURE (psousfen)->SRect[2];
        pSUBWIN_FEATURE (psousfen)->ZRect[3]   = pSUBWIN_FEATURE (psousfen)->SRect[3];

        /*}  SS: moved below because if sciGetZooming(psousfen)==0
        ZRect is undefined -> code may enter in infinite recursion loop to compute graduation
        and there is no use to regraduate */

        /** regraduation de l'axe des axes ***/
        fmin= pSUBWIN_FEATURE (psousfen)->ZRect[0];
        fmax= pSUBWIN_FEATURE (psousfen)->ZRect[2];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;

        fmin= pSUBWIN_FEATURE (psousfen)->ZRect[1];
        fmax= pSUBWIN_FEATURE (psousfen)->ZRect[3];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
        /*****/
      }
    }
    psonstmp = psonstmp->pnext;
  }

  //sciDrawObj(sciGetCurrentFigure());
}

/*--------------------------------------------------------------------------*/
/**
 * Interface to function xchange "f2i". Convert user 2d coordinates
 * to pixel ones.
 * @param rect [x,y,w,h] of current subwin
 */
void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4])
{
    /* coordinates transformation */
    int i = 0;
    char * selectedSubwinUID = (char*)getOrCreateDefaultSubwin();
    updateSubwinScale(selectedSubwinUID);

    for (i = 0; i < nbCoords; i++)
    {
        // specify a default value for Z
        double curCoords[3] = {xCoords[i], yCoords[i], 0.0};
        int curPixCoords[2];
        sciGet2dViewPixelCoordinates(selectedSubwinUID, curCoords, curPixCoords);
        xPixCoords[i] = curPixCoords[0];
        yPixCoords[i] = curPixCoords[1];
    }

    /* get viewing area */
    sciGetViewingArea(selectedSubwinUID, &rect[0], &rect[1], &rect[2], &rect[3]);

}
/*--------------------------------------------------------------------------*/
/**
* Interface to function xchange "i2f". Convert pixel coordinates
* to user 2d coordinates.
* @param rect [x,y,w,h] of current subwin
*/
void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4])
{
    /* coordinates transformation */
    int i = 0;
    char * selectedSubwinUID = (char*)getOrCreateDefaultSubwin();
    updateSubwinScale(selectedSubwinUID);
    for (i = 0; i < nbCoords; i++)
    {
        // specify a default value for Z
        int curPixCoords[2] = {xPixCoords[i], yPixCoords[i]};
        double curCoords[2];
        sciGet2dViewCoordFromPixel(selectedSubwinUID, curPixCoords, curCoords);
        xUserCoords[i] = curCoords[0];
        yUserCoords[i] = curCoords[1];
    }

    /* get viewing area */
    sciGetViewingArea(selectedSubwinUID, &rect[0], &rect[1], &rect[2], &rect[3]);

}
/*--------------------------------------------------------------------------*/
