/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: StringBox.c                                                      */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#include "StringBox.h"
#include "GetProperty.h"
#include "axesScale.h"
#include "math_graphics.h"
#include "MALLOC.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "GraphicSynchronizerInterface.h"
#include "SetProperty.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*-------------------------------------------------------------------------------*/
void getTextBoundingBox(char ** text, int nbRow, int nbCol,
                        double xPos, double yPos,
                        double angle, int fontId, double fontSize,
                        double corners[4][2])
{
    /* first step, create a text object */
    char * parentFigureUID = getCurrentFigure();
    char * parentSubwinUID = getCurrentSubWin();
    char * pTextUID = NULL;
    double * textCorners = NULL;
    int defaultColor = 0; /* color does not matter */

    /* Update subwin scale if needed */
    updateSubwinScale(parentSubwinUID);

#if 0
  startFigureDataWriting(parentFigure);
#endif

    pTextUID = allocateText(parentSubwinUID,
                       text, nbRow, nbCol,
                       xPos, yPos,
                       TRUE,
                       NULL,
                       FALSE,
                       &defaultColor, &defaultColor,
                       FALSE, FALSE, FALSE,
                       ALIGN_LEFT);

    /* Make it invisible to be sure */
    int visible = 0;
    setGraphicObjectProperty(pTextUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    double fontAngle = DEG2RAD(angle);
    setGraphicObjectProperty(pTextUID, __GO_FONT_ANGLE__, &fontAngle, jni_double, 1);

    setGraphicObjectProperty(pTextUID, __GO_FONT_SIZE__, &fontSize, jni_double, 1);
    setGraphicObjectProperty(pTextUID, __GO_FONT_STYLE__, &fontId, jni_int, 1);

    setGraphicObjectRelationship(parentSubwinUID, pTextUID);

    /* Update its bounds */
    updateTextBounds(pTextUID);

    /* Then get its bounding box */
    getGraphicObjectProperty(pTextUID, __GO_CORNERS__, jni_double_vector, &textCorners);

    /*
     * To do: performs a projection/unprojection to obtain the bounding box in object coordinates
     * but using a rotation matrix corresponding to the default rotation angles (view == 2d)
     */
#if 0
    sciGet2dViewBoundingBox( pText, corners[0], corners[1], corners[2], corners[3]) ;
#endif

    corners[1][0] = textCorners[0];
    corners[1][1] = textCorners[1];

    corners[0][0] = textCorners[3];
    corners[0][1] = textCorners[4];

    corners[3][0] = textCorners[6];
    corners[3][1] = textCorners[7];

    corners[2][0] = textCorners[9];
    corners[2][1] = textCorners[10];

    deleteGraphicObject(pTextUID);


#if 0
  endFigureDataWriting(parentFigure);
#endif
}
/*-------------------------------------------------------------------------------*/

