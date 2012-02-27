/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 * desc : Interface functions between between GetProperty functions and the C++/Java part of module
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetJavaProperty.h"
#include "getHandleDrawer.h"
#include "BasicAlgos.hxx"

extern "C"
{
#include "BasicAlgos.h"
#include "getScilabJavaVM.h"
}

#include "CallRenderer.hxx"

using namespace sciGraphics;
using namespace org_scilab_modules_renderer;

/*---------------------------------------------------------------------------------*/
void sciGetJavaColormap(sciPointObj * pFigure, double rgbMat[])
{
    //getFigureDrawer(pFigure)->getColorMap(rgbMat);
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaFigureSize(sciPointObj * pFigure, int size[2])
{
    //getFigureDrawer(pFigure)->getSize(size);
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaWindowSize(sciPointObj * pFigure, int size[2])
{
    //getFigureDrawer(pFigure)->getWindowSize(size);
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaWindowPosition(sciPointObj * pFigure, int pos[2])
{
    //getFigureDrawer(pFigure)->getWindowPosition(pos);
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaInfoMessage(sciPointObj * pFigure, char *infoMessage)
{
    //getFigureDrawer(pFigure)->getInfoMessage(infoMessage);
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaInfoMessageLength(sciPointObj * pFigure)
{
    //return getFigureDrawer(pFigure)->getInfoMessageLength();
    return 0;
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaAntialiasingQuality(sciPointObj * pFigure)
{
    //return getFigureDrawer(pFigure)->getAntialiasingQuality();
    return 0;
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2])
{
    getSubwinDrawer(pSubwin)->getCamera()->getPixelCoordinates(userCoord, pixCoord);
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewPixelCoordinates(char * pSubwinUID, const double userCoords[3], int pixCoords[2])
{
    double *tmp = NULL;
    double coords[3];

    coords[0] = userCoords[0];
    coords[1] = userCoords[1];
    coords[2] = userCoords[2];

    tmp = CallRenderer::getPixelFrom2dViewCoordinates(getScilabJavaVM(), pSubwinUID, coords, 3);

    pixCoords[0] = (int) tmp[0];
    pixCoords[1] = (int) tmp[1];
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordinates(char *pSubwinUID, const double userCoords3D[3], double userCoords2D[2])
{
    double *tmp = NULL;
    double coords[3];

    coords[0] = userCoords3D[0];
    coords[1] = userCoords3D[1];
    coords[2] = userCoords3D[2];

    tmp = CallRenderer::get2dViewCoordinates(getScilabJavaVM(), pSubwinUID, coords, 3);

    userCoords2D[0] = tmp[0];
    userCoords2D[1] = tmp[1];
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordFromPixel(char * pSubwinUID, const int pixCoords[2], double userCoords2D[2])
{
    double *tmp = NULL;
    double coords[2];

    coords[0] = (double) pixCoords[0];
    coords[1] = (double) pixCoords[1];

    tmp = CallRenderer::get2dViewFromPixelCoordinates(getScilabJavaVM(), pSubwinUID, coords, 2);

    userCoords2D[0] = tmp[0];
    userCoords2D[1] = tmp[1];
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaBoundingBox(sciPointObj * pText, double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaPixelBoundingBox(sciPointObj * pText, int corner1[2], int corner2[2], int corner3[2], int corner4[2])
{
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaViewingArea(char * pSubwinUID, int *xPos, int *yPos, int *width, int *height)
{
    double *tmp = NULL;

    tmp = CallRenderer::getViewingArea(getScilabJavaVM(), pSubwinUID);

    *xPos = (int) tmp[0];
    *yPos = (int) tmp[1];
    *width = (int) tmp[2];
    *height = (int) tmp[3];
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaSegsBoundingBox(sciPointObj * pSegs, double bounds[6])
{

}

/*---------------------------------------------------------------------------------*/
BOOL sciGetJavaAutoResizeMode(sciPointObj * pFigure)
{
    //if (getFigureDrawer(pFigure)->getAutoResizeMode())
    {
        return TRUE;
    }
    //else
    {
        return FALSE;
    }
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaViewport(sciPointObj * pFigure, int viewport[4])
{
    //getFigureDrawer(pFigure)->getViewport(viewport);
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaNbSubticks(sciPointObj * pSubwin, double nbsubtics[3])
{
    getSubwinDrawer(pSubwin)->getNbSubticksPerGrad(nbsubtics);
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaNbXTicks(sciPointObj * pSubwin)
{
    return getSubwinDrawer(pSubwin)->getNbXTicks();
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaXTicksPos(sciPointObj * pSubwin, double ticksPos[], char **ticksLabels)
{
    // ticksLabels comes from graphics so is allocated with MALLOC
    // however getTicksPos will use new to allocate
    int nbTicks = sciGetJavaNbXTicks(pSubwin);
    char **javaLabels = BasicAlgos::createStringArray(nbTicks);

    getSubwinDrawer(pSubwin)->getXTicksPos(ticksPos, javaLabels);

    // copy it into ticksLabels
    stringArrayCopy(ticksLabels, javaLabels, nbTicks);

    BasicAlgos::destroyStringArray(javaLabels, nbTicks);
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaNbYTicks(sciPointObj * pSubwin)
{
    return getSubwinDrawer(pSubwin)->getNbYTicks();
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaYTicksPos(sciPointObj * pSubwin, double ticksPos[], char **ticksLabels)
{
    // ticksLabels comes from graphics so is allocated with MALLOC
    // however getTicksPos will use new to allocate
    int nbTicks = sciGetJavaNbYTicks(pSubwin);
    char **javaLabels = BasicAlgos::createStringArray(nbTicks);

    getSubwinDrawer(pSubwin)->getYTicksPos(ticksPos, javaLabels);

    // copy it into ticksLabels
    stringArrayCopy(ticksLabels, javaLabels, nbTicks);

    BasicAlgos::destroyStringArray(javaLabels, nbTicks);
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaNbZTicks(sciPointObj * pSubwin)
{
    return getSubwinDrawer(pSubwin)->getNbZTicks();
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaZTicksPos(sciPointObj * pSubwin, double ticksPos[], char **ticksLabels)
{
    // ticksLabels comes from graphics so is allocated with MALLOC
    // however getTicksPos will use new to allocate
    int nbTicks = sciGetJavaNbZTicks(pSubwin);
    char **javaLabels = BasicAlgos::createStringArray(nbTicks);

    getSubwinDrawer(pSubwin)->getZTicksPos(ticksPos, javaLabels);

    // copy it into ticksLabels
    stringArrayCopy(ticksLabels, javaLabels, nbTicks);

    BasicAlgos::destroyStringArray(javaLabels, nbTicks);
}

/*---------------------------------------------------------------------------------*/
BOOL sciGetJavaIsAbleToCreateWindow(void)
{
    //if (DrawableFigure::isAbleToCreateFigure())
    {
        return booltoBOOL(true);
    }
    //else
    {
        return booltoBOOL(false);
    }
}

/*---------------------------------------------------------------------------------*/
