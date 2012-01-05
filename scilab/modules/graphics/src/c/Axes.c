/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
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
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots
 *--------------------------------------------------------------------------*/

#include <string.h>
#include "Axes.h"
#include "DestroyObjects.h"
#include "InitObjects.h"
#include "SetProperty.h"
#include "axesScale.h"
#include "CurrentSubwin.h"
#include "DrawingBridge.h"

#include "GetProperty.h"
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "AxesModel.h"
#include "CurrentSubwin.h"

/*--------------------------------------------------------------------------------*/
static int getSqDistanceToCenter(sciPointObj * pSubwin, int xCoord, int yCoord);
static BOOL isSubwinUnderPixel(sciPointObj * pSubwin, int xCoord, int yCoord);

/*--------------------------------------------------------------------------------*/
/* clear a subwindow from all of its children */
void clearSubWin(char * pSubWinUID)
{
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    int i;
    char **pstChildrenUID;

    // Iterate on children.
    getGraphicObjectProperty(pSubWinUID, __GO_CHILDREN_COUNT__, jni_int, &piChildrenCount);

    if (iChildrenCount != 0)
    {
        getGraphicObjectProperty(pSubWinUID, __GO_CHILDREN__, jni_string_vector, &pstChildrenUID);

        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            destroyGraphicHierarchy(pstChildrenUID[i]);
        }
    }
}
/*--------------------------------------------------------------------------------*/
/* reinit a subwindow (but don't change position ) */
void reinitSubWin(char * pSubWinUID)
{
  int visible;
  int firstPlot;
  int axisLocation;

  /* Deletes the Axes' hierarchy */
  clearSubWin(pSubWinUID);

  initSubWinBounds(pSubWinUID);

  /* bottom */
  axisLocation = 0;
  setGraphicObjectProperty(pSubWinUID, __GO_X_AXIS_LOCATION__, &axisLocation, jni_int, 1);
  /* left */
  axisLocation = 4;
  setGraphicObjectProperty(pSubWinUID, __GO_Y_AXIS_LOCATION__, &axisLocation, jni_int, 1);

  visible = 1;
  setGraphicObjectProperty(pSubWinUID, __GO_VISIBLE__, &visible, jni_bool, 1);
  firstPlot = 1;
  setGraphicObjectProperty(pSubWinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

  initSubWinAngles(pSubWinUID);
}
/*--------------------------------------------------------------------------------*/
/* reinit the viewing angles of a subwindow */
void initSubWinAngles(char * pSubWinUID)
{
    int iViewType = 0;
    int* piViewType = &iViewType;
    double* rotationAngles;
    char* axesModelUID = getAxesModel();

    getGraphicObjectProperty(axesModelUID, __GO_VIEW__, jni_int, (void **) &piViewType);
    setGraphicObjectProperty(pSubWinUID, __GO_VIEW__, &piViewType, jni_int, 1);

    getGraphicObjectProperty(axesModelUID, __GO_ROTATION_ANGLES__, jni_double_vector, (void **) &rotationAngles);
    setGraphicObjectProperty(pSubWinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    getGraphicObjectProperty(axesModelUID, __GO_ROTATION_ANGLES_3D__, jni_double_vector, (void **) &rotationAngles);
    setGraphicObjectProperty(pSubWinUID, __GO_ROTATION_ANGLES_3D__, rotationAngles, jni_double_vector, 2);
}

/*--------------------------------------------------------------------------------*/
/* set the size and position of the subwindow to the default */
void initSubWinSize( sciPointObj * pSubWin )
{
// ???
#if 0
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->WRect[0] = ppAxesMdl->WRect[0] ;
  ppSubWin->WRect[1] = ppAxesMdl->WRect[1] ;
  ppSubWin->WRect[2] = ppAxesMdl->WRect[2] ;
  ppSubWin->WRect[3] = ppAxesMdl->WRect[3] ;
#endif
}
/*--------------------------------------------------------------------------------*/
/* set the data_bounds of the axes to the default value */
void initSubWinBounds(char * pSubWinUID)
{
    double* dataBounds;
    double* realDataBounds;   
    char* axesModelUID = getAxesModel();

    getGraphicObjectProperty(axesModelUID, __GO_DATA_BOUNDS__, jni_double_vector, &dataBounds);
    setGraphicObjectProperty(pSubWinUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

    getGraphicObjectProperty(axesModelUID, __GO_REAL_DATA_BOUNDS__, jni_double_vector, &realDataBounds);
    setGraphicObjectProperty(pSubWinUID, __GO_REAL_DATA_BOUNDS__, realDataBounds, jni_double_vector, 6);
}
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void )
{
    int iAutoClear = 0;
    int* piAutoClear = &iAutoClear;

    //  nbCheckRedraw++;
    //  fprintf(stderr, "[DEBUG] checkRedrawing : %d\n", nbCheckRedraw);
    char* pstSubWinID = getCurrentSubWin();

    getGraphicObjectProperty(pstSubWinID, __GO_AUTO_CLEAR__, jni_bool, &piAutoClear);

    if (iAutoClear)
    {
        reinitSubWin(pstSubWinID);
        return TRUE;
    }

    return FALSE;
}
/*--------------------------------------------------------------------------------*/
/**
 * Return the square distance between the center of axes box anf point (xCoord, yCoord);
 */
static int getSqDistanceToCenter(sciPointObj * pSubwin, int xCoord, int yCoord)
{
  int xPos;
  int yPos;
  int width;
  int height;
  /* get area used by the subwindow */
  sciGetViewingArea(pSubwin, &xPos, &yPos, &width, &height);

  /* get coordinate sof middle */
  xPos = xPos + width / 2;
  yPos = yPos + height / 2;

  return (xCoord - xPos) * (xCoord - xPos) + (yCoord - yPos) * (yCoord - yPos);

}
/*--------------------------------------------------------------------------------*/
/**
 * @return TRUE if the point in pixel (xCoord, yCoord) is above the subwin
 */
static BOOL isSubwinUnderPixel(sciPointObj * pSubwin, int xCoord, int yCoord)
{
  int xPos;
  int yPos;
  int width;
  int height;
  /* get area used by the subwindow */
  sciGetViewingArea(pSubwin, &xPos, &yPos, &width, &height);

  return (   xCoord > xPos && xCoord < xPos + width
          && yCoord > yPos && yCoord < yPos + height);
}
/*--------------------------------------------------------------------------------*/
sciPointObj * getClickedSubwin(sciPointObj * pFigure, int xCoord, int yCoord)
{
  int nbItem;
  sciPointObj * res = NULL;

  /* First get the list of subwindow that are under the click */
  /* Might be several if some are hidding others */
  DoublyLinkedList * foundSubwins = DoublyLinkedList_new();
  sciSons * pSons = sciGetSons(pFigure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      updateSubwinScale(curObj);
      if (isSubwinUnderPixel(curObj, xCoord, yCoord))
      {
        foundSubwins = List_push(foundSubwins, curObj);
      }
    }
    pSons = pSons->pnext;
  }

  /* all the subwindows that are under the clicked pixel has been found */

  nbItem = List_nb_item(foundSubwins);
  if (nbItem == 0)
  {
    res = NULL;
  }
  else if (nbItem == 1)
  {
    /* index starts to 1 */
    res = List_item(foundSubwins, 1);
  }
  else
  {
    /* select the one whose middle is closer the point */
    int minDist = 0;
    int i;

    res = (sciPointObj *) List_item(foundSubwins, 1);
    minDist = getSqDistanceToCenter(res, xCoord, yCoord);

    for (i = 2; i <= nbItem; i++)
    {
      sciPointObj * curSubwin = (sciPointObj *) List_item(foundSubwins, i);
      int curDist = getSqDistanceToCenter(curSubwin, xCoord, yCoord);
      if (curDist < minDist)
      {
        res = curSubwin;
        minDist = curDist;
      }
    }
  }

  List_free(foundSubwins);

  return res;

}
/*--------------------------------------------------------------------------------*/
sciLegendPlace propertyNameToLegendPlace(const char * string)
{
	if ( strcmp(string, "in_upper_right" ) == 0 )
	{
		return SCI_LEGEND_IN_UPPER_RIGHT;
	}
	else if ( strcmp(string, "in_upper_left" ) == 0 )
	{
		return SCI_LEGEND_IN_UPPER_LEFT;
	}
	else if ( strcmp(string, "in_lower_right" ) == 0 )
	{
		return SCI_LEGEND_IN_LOWER_RIGHT;
	}
	else if ( strcmp(string, "in_lower_left" ) == 0 )
	{
		return SCI_LEGEND_IN_LOWER_LEFT;
	}
	else if ( strcmp(string, "out_upper_right" ) == 0 )
	{
		return SCI_LEGEND_OUT_UPPER_RIGHT;
	}
	else if ( strcmp(string, "out_upper_left" ) == 0 )
	{
		return SCI_LEGEND_OUT_UPPER_LEFT;
	}
	else if ( strcmp(string, "out_lower_right" ) == 0 )
	{
		return SCI_LEGEND_OUT_LOWER_RIGHT;
	}
	else if ( strcmp(string, "out_lower_left" ) == 0 )
	{
		return SCI_LEGEND_OUT_LOWER_LEFT;
	}
	else if ( strcmp(string, "upper_caption" ) == 0 )
	{
		return SCI_LEGEND_UPPER_CAPTION;
	}
	else if ( strcmp(string, "lower_caption" ) == 0 )
	{
		return SCI_LEGEND_LOWER_CAPTION;
	}
	else if ( strcmp(string, "by_coordinates" ) == 0 )
	{
		return SCI_LEGEND_BY_COORDINATES;
	}
	else {
		return SCI_LEGEND_POSITION_UNSPECIFIED;
	}
}
/*--------------------------------------------------------------------------------*/
/*
 * Converts a boolean log flag to the character format
 */
char getTextLogFlag(int logFlag)
{
    if (logFlag)
    {
        return 'l';
    }
    else
    {
        return 'n';
    }
}
/*--------------------------------------------------------------------------------*/
/*
 * Converts a character log flag to the equivalent boolean
 */
int getBooleanLogFlag(char logFlag)
{
    if (logFlag == 'l')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*--------------------------------------------------------------------------------*/

