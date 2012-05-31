/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan Cornet
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "graphicModuleLoad.h"
#include "TerminateGraphics.h"
#include "getDictionaryGetProperties.h"
#include "getDictionarySetProperties.h"
#include "DrawObjects.h"
#include "ObjectSelection.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "math_graphics.h"
#include "DestroyObjects.h"
#include "axesScale.h"
#include "BuildObjects.h"
#include "BasicAlgos.h"
#include "pixel_mode.h"
#include "Axes.h"
#include "getPropertyAssignedValue.h"
#include "SetHashTable.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"
#include "MALLOC.h"
#include "Interaction.h"
#include "InitObjects.h"
#include "ObjectStructure.h"

/*--------------------------------------------------------------------------*/

/*
  We need to add "manually" the functions set, get and delete because
  if the module graphics is not loaded and because set, get and delete
  are essentially functions from the graphics module, they will not be
  available in NWNI mode and if Scilab has been compiled with the flag
  --without-gui.
*/

#define DELETE_INDEX 62
#define GET_INDEX    63
#define SET_INDEX    64

int gw_graphics(void)
{
    Rhs = Max(0, Rhs);

    if ((Fin == DELETE_INDEX || Fin == GET_INDEX || Fin == SET_INDEX) && (VarType(1) == sci_tlist || VarType(1) == sci_mlist))
    {
        int lw = 1 + Top - Rhs;

        if (Fin == DELETE_INDEX)
            C2F(overload) (&lw, "delete", 6);
        if (Fin == GET_INDEX)
            C2F(overload) (&lw, "get", 3);
        if (Fin == SET_INDEX)
            C2F(overload) (&lw, "set", 3);
    }
    else
    {
        Scierror(999, _("Scilab Graphics module not installed.\n"));
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
void loadGraphicModule(void)
{

}

/*--------------------------------------------------------------------------*/
BOOL TerminateGraphics(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
int sciSetViewport(char *pObj, const int viewport[4])
{
    return 0;
}

/*--------------------------------------------------------------------------*/
void sciGetViewport(sciPointObj * pObj, int viewport[4])
{

}

/*--------------------------------------------------------------------------*/
double Maxi(const double vect[], int n)
{
    return 0.0;
}

/*--------------------------------------------------------------------------*/
double Mini(const double vect[], int n)
{
    return 0.0;
}

/*--------------------------------------------------------------------------*/
BOOL sciHasFigures(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
char **getDictionaryGetProperties(int *sizearray)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
char **getDictionarySetProperties(int *sizearray)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciGetHeight(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetNum(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
BOOL sciAddThisToItsParent(sciPointObj * pthis, sciPointObj * pparent)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
sciPointObj *sciGetCurrentFigure(void)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciAddNewHandle(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void sciInitSelectedSons(sciPointObj * pObj)
{

}

/*--------------------------------------------------------------------------*/
sciPointObj *sciGetParent(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
sciPointObj *sciGetPointerFromHandle(long handle)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
unsigned long getHandleFromStack(size_t stackPointer)
{
    return (unsigned long)(-1);
}

/*--------------------------------------------------------------------------*/
int callSetProperty(char *pObj, size_t stackPointer, int valueType, int nbRow, int nbCol, char *propertyName)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
sciEntityType sciGetEntityType(sciPointObj * pobj)
{
    return (sciEntityType) 0;
}

/*--------------------------------------------------------------------------*/
sciPointObj *sciGetParentFigure(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciGetWidth(sciPointObj * pobj)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
int sciInitScreenPosition(char *pobj, int pposx, int pposy)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
BOOL sciDelThisToItsParent(sciPointObj * pthis, sciPointObj * pparent)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
char *getStringFromStack(size_t stackPointer)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
double *getDoubleMatrixFromStack(size_t stackPointer)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
double getDoubleFromStack(size_t stackPointer)
{
    return 0.;
}

/*--------------------------------------------------------------------------*/
char *getFigureFromIndex(int figNum)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
char **getStringMatrixFromStack(size_t stackPointer)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
long sciGetHandle(sciPointObj * pobj)
{
    return (long)0;
}

/*--------------------------------------------------------------------------*/
int sciInitFontSize(char *pobj, double fontSize)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
char *allocateText(char *pparentsubwinUID,
                   char **text,
                   int nbRow,
                   int nbCol,
                   double x,
                   double y,
                   BOOL autoSize,
                   double userSize[2],
                   BOOL centerPos, int *foreground, int *background, BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciInitIsFilled(char *pobj, BOOL isfilled)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitLineStyle(char *pobj, int linestyle)
{
    return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitBackground(char *pobj, int colorindex)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitMarkStyle(char *pobj, int markstyle)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitIsMark(char *pobj, BOOL ismark)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitMarkBackground(char *pobj, int colorindex)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitMarkForeground(char *pobj, int colorindex)
{
    return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitMarkSizeUnit(char *pobj, int marksizeunit)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetNbTypedObjects(sciPointObj * pObj, sciEntityType type)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitArrowSize(char *pobj, double size)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitPolylineStyle(char *pobj, int plot)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetNumColors(char *pobjUID)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int deallocateText(sciPointObj * pthis)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void sciGetLegendPos(sciPointObj * pobj, double position[2])
{

}

/*--------------------------------------------------------------------------*/
char sciGetxLocation(sciPointObj * pObj)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
char sciGetyLocation(sciPointObj * pObj)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
sciLegendPlace sciGetLegendPlace(sciPointObj * pobj)
{
    return (sciLegendPlace) 0;
}

/*--------------------------------------------------------------------------*/
void sciGetDataBounds(sciPointObj * pObj, double bounds[6])
{

}

/*--------------------------------------------------------------------------*/
void sciGetViewingArea(char * pObjUID, int *xPos, int *yPos, int *width, int *height)
{

}

/*--------------------------------------------------------------------------*/
int ComputeXIntervals(char *pobjUID, char xy_type, double **vector, int *N, int checkdim)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void deleteMatrix(sciMatrix * mat)
{

}

/*--------------------------------------------------------------------------*/
char *getStrMatElement(const StringMatrix * mat, int row, int col)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
StringMatrix *computeDefaultTicsLabels(char *pobjUID)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
void rotate2D(double from[2], double center[2], double angle, double dest[2])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetAutoRotation(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsAutoDrawable(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void stringArrayCopy(char *dest[], char *src[], int nbElement)
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetVisibility(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void sciGetLogFlags(char *pObjUID, char flags[3])
{

}

/*--------------------------------------------------------------------------*/
int sciSetFontOrientation(char *pobj, double textorientation)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetAutoPosition(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int sciSetFontStyle(char *pobj, int iAttributes)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitIsClipping(char *pobj, int value)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciSetLegendPos(char *pobj, double position[])
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciSetClipping(char *pobj, double pclip[4])
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciSetTextPos(char *pObj, double posX, double posY, double posZ)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void sciGetUserSize(sciPointObj * pObj, double *width, double *height)
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsBoxed(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsLine(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetAutoSize(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetCenterPos(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciisTextEmpty(char *identifier)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
double sciGetFontOrientation(sciPointObj * pobj)
{
    return 0.;
}

/*--------------------------------------------------------------------------*/
sciTextAlignment sciGetAlignment(sciPointObj * pObj)
{
    return (sciTextAlignment) 0;
}

/*--------------------------------------------------------------------------*/
StringMatrix *sciGetText(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int getMatNbCol(const sciMatrix * mat)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int getMatNbRow(const sciMatrix * mat)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
char **getStrMatData(const StringMatrix * mat)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
BOOL isFigureModel(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
sciFont *sciGetFontContext(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciGetFontStyle(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3])
{

}

/*--------------------------------------------------------------------------*/
void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetAutoSubticks(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIs3d(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int sciGetSubwinIndex(sciPointObj * pSubwin)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
EAxesBoxType sciGetBoxType(sciPointObj * pobj)
{
    return (EAxesBoxType) 0;
}

/*--------------------------------------------------------------------------*/
void sciGetTextPos(sciPointObj * pObj, double position[3])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetRealVisibility(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int getPixelModeIndex(const char *modeName)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciSetXorMode(char *pobj, int value)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciSetVisibility(char *pobj, BOOL value)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetZooming(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetTightLimitsOn(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int sciSetRealDataBounds(char *pObj, const double bounds[6])
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void mat4DMult(const double mat4D[4][4], const double vect3D[3], double res[3])
{

}

/*--------------------------------------------------------------------------*/
void setToIdentity(double mat4D[4][4])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsIsoView(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void vectAdd3D(const double v1[3], const double v2[3], double res[3])
{

}

/*--------------------------------------------------------------------------*/
char *ConstructPolyline(char *pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
                        int closed, int n1, int plot, int *foreground, int *background,
                        int *mark_style, int *mark_foreground, int *mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciSetIsClipping(char *pobj, int value)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
BOOL isPointInTriangle(const double point[2], const double a[2], const double b[2], const double c[2])
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void vectSubstract2D(const double vect1[2], const double vect2[], double res[2])
{

}

/*--------------------------------------------------------------------------*/
void scalarMult2D(const double v[2], const double scalar, double res[2])
{

}

/*--------------------------------------------------------------------------*/
void vectAdd2D(const double v1[2], const double v2[2], double res[2])
{

}

/*--------------------------------------------------------------------------*/
int sciSetZoomBox(char *pObj, const double zoomBox[6])
{
    return -1;
}

/*--------------------------------------------------------------------------*/
double *sciGetMargins(sciPointObj * pObj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
double *sciGetAxesBounds(sciPointObj * pObj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
void sciGetViewingAngles(sciPointObj * pObj, double *alpha, double *theta)
{

}

/*--------------------------------------------------------------------------*/
void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsCubeScaled(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void vectSubstract3D(const double v1[3], const double v2[3], double res[3])
{

}

/*--------------------------------------------------------------------------*/
void normalize3D(double vect[3])
{

}

/*--------------------------------------------------------------------------*/
void scalarMult3D(const double v[3], double scalar, double res[3])
{

}

/*--------------------------------------------------------------------------*/
int TheTicks(double *xminv, double *xmaxv, double *grads, int *ngrads, int compNgrads)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void ChoixFormatE(char *fmt, double xmin, double xmax, double xpas)
{

}

/*--------------------------------------------------------------------------*/
int GradLog(double _min, double _max, double *_grads, int *n_grads, int compNgrads)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetMarkStyle(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetMarkSize(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetMarkSizeUnit(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetLineStyle(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
double sciGetLineWidth(sciPointObj * pobj)
{
    return 0.;
}

/*--------------------------------------------------------------------------*/
double sciGetArrowSize(sciPointObj * pobj)
{
    return 0.;
}

/*--------------------------------------------------------------------------*/
sciGraphicContext *sciGetGraphicContext(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int *sciGetInterpVector(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
void intArrayCopy(int dest[], const int src[], int nbElement)
{

}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsClosed(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsFilled(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsColorInterpolated(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsDisplayingLines(sciPointObj * pObj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL sciGetIsMark(sciPointObj * pobj)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int sciGetNbPoints(sciPointObj * pObj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void doubleArrayCopy(double dest[], const double src[], int nbElement)
{

}

/*--------------------------------------------------------------------------*/
void setDoubleArraySingleValue(double dest[], double value, int nbElement)
{

}

/*--------------------------------------------------------------------------*/
int sciGetPolylineStyle(sciPointObj * pObj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void unzoomSubwin(sciPointObj * pSubwin)
{

}

/*--------------------------------------------------------------------------*/
void sciZoomObject(sciPointObj * pObj, int x1, int y1, int x2, int y2)
{

}

/*--------------------------------------------------------------------------*/
sciPointObj *getClickedSubwin(sciPointObj * pFigure, int xCoord, int yCoord)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciGetXorMode(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
sciSons *sciGetLastSons(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
int sciGetIsClipping(sciPointObj * pobj)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
sciPointObj *sciGetParentSubwin(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
void sciGetRealDataBounds(sciPointObj * pObj, double bounds[6])
{

}

/*--------------------------------------------------------------------------*/
int sciInitMarkSize(char *pobj, int marksize)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
double *sciGetClipping(sciPointObj * pobj)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
BOOL sciIsExistingFigure(int figNum)
{
    return FALSE;
}
