/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xfpolys.c                                                    */
/* desc : interface for xfpolys routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "localization.h"

#include "CurrentObject.h"
#include "HandleManagement.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xfpolys(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddr3 = NULL;
    int* l3 = NULL;

    int m1 = 0, n1 = 0;
    int m2 = 0, n2 = 0;
    int m3 = 0, n3 = 0;
    int mn2 = 0;

    int v1 = 0;                 /* v1 is the flag used for flat (v1==1) or interpolated (v1==2) shading */
    int i = 0;
    long hdl = 0;

    char *pstSubWinUID = NULL;
    char *pstFigureUID = NULL;
    char *pstCompoundUID = NULL;
    int iSubWinForeground = 0;
    int *piSubWinForeground = &iSubWinForeground;

    int iImmediateDrawing = 0;
    int *piImmediateDrawing = &iImmediateDrawing;
    int iFalse = 0;

    int iColorMapSize = 0;
    int* piColorMapSize = &iColorMapSize;
    int iForeGround = 0;
    int* piForeGround = &iForeGround;

    int iVisible = 0;
    int *piVisible = &iVisible;

    CheckInputArgument(pvApiCtx, 2, 3);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckSameDims
    if (m1 != m2 || n1 != n2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
        return 1;
    }

    mn2 = m2 * n2;
    if (mn2 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr3, &m3, &n3, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
            return 1;
        }


        if (m3 * n3 == m1 * n1)
        {
            //CheckSameDims
            if (m1 != m3 || n1 != n3)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
                return 1;
            }

            v1 = 2;             /* interpolated shading */

            if (m3 != 3 && m3 != 4)
            {
                Scierror(999, _("%s: Interpolated shading only works for polygons of size %d or %d\n"), fname, 3, 4);
                return 0;
            }
        }
        else
        {
            //CheckVector
            if (m3 != 1 && n3 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 3);
                return 1;
            }

            //CheckDimProp
            if (m3 * n3 != n2)
            {
                Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
                return 1;
            }

            v1 = 1;             /* flat shading */
        }
    }
    else
    {
        int un = 1, ix = 0;

        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 3, un, n2, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        for (ix = 0; ix < n2; ++ix)
        {
            *(int*)(l3 + ix) = 0;
        }
        m3 = n3 = 1;
    }

    pstSubWinUID = (char*)getOrCreateDefaultSubwin();
    getGraphicObjectProperty(pstSubWinUID, __GO_PARENT__, jni_string, (void**)&pstFigureUID);
    getGraphicObjectProperty(pstFigureUID, __GO_IMMEDIATE_DRAWING__, jni_bool, (void **)&piImmediateDrawing);
    setGraphicObjectProperty(pstFigureUID, __GO_IMMEDIATE_DRAWING__, &iFalse, jni_bool, 1);

    //get color map size
    getGraphicObjectProperty(pstFigureUID, __GO_COLORMAP_SIZE__, jni_int, (void**)&piColorMapSize);

    //get current foreground color
    getGraphicObjectProperty(pstSubWinUID, __GO_LINE_COLOR__, jni_int, (void**)&piForeGround);

    // Create compound.
    pstCompoundUID = createGraphicObject(__GO_COMPOUND__);
    setGraphicObjectProperty(pstCompoundUID, __GO_VISIBLE__, &iFalse, jni_bool, 1);
    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(pstSubWinUID, pstCompoundUID);

    for (i = 0; i < n1; ++i)
    {
        if (m3 == 1 || n3 == 1) /* color vector specified */
        {
            if (*(int*)(l3 + i) == 0)
            {
                if (iForeGround == -1)
                {
                    iSubWinForeground = iColorMapSize + 1;
                }
                else if (iForeGround == -2)
                {
                    iSubWinForeground = iColorMapSize + 2;
                }
                else
                {
                    iSubWinForeground = iForeGround;
                }

                Objpoly((l1 + (i * m1)), (l2 + (i * m1)), m1, 1, iSubWinForeground, &hdl);
            }
            else
            {
                Objfpoly((l1 + (i * m1)), (l2 + (i * m1)), m1, (int*)(l3 + i), &hdl, v1);
            }
        }
        else                    /* we have a color matrix used for interpolated shading : one color per vertex */
        {
            Objfpoly((l1 + (i * m1)), (l2 + (i * m1)), m1, (int*)(l3 + i * m3), &hdl, v1);
        }

        // Add newly created object to Compound
        setGraphicObjectRelationship(pstCompoundUID, getObjectFromHandle(hdl));
    }


    setCurrentObject(pstCompoundUID);

    setGraphicObjectProperty(pstFigureUID, __GO_IMMEDIATE_DRAWING__, &piImmediateDrawing, jni_bool, 1);
    getGraphicObjectProperty(pstFigureUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pstCompoundUID, __GO_VISIBLE__, &iVisible, jni_bool, 1);


    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
