/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_graphics.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "name2rgb.h"
#include "CurrentFigure.h"
#include "FigureModel.h"
#include "addColor.h"

int sci_color(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRows = 0;
    int iCols = 0;
    double pdblColor[3];

    int iCurrentFigure = 0;
    int iColor = 0;

    int iRhs = nbInputArgument(pvApiCtx);

    if (iRhs != 1 && iRhs != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), fname, 1, 3);
        return 1;
    }

    if (iRhs == 1)
    {
        int* piAddr = NULL;
        char* pstColor = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (isStringType(pvApiCtx, piAddr) == FALSE || isScalar(pvApiCtx, piAddr) == FALSE)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
            return 1;
        }

        getAllocatedSingleString(pvApiCtx, piAddr, &pstColor);
        name2rgb(pstColor, pdblColor);
        freeAllocatedSingleString(pstColor);
        if (pdblColor[0] == -1 || pdblColor[1] == -1 || pdblColor[2] == -1)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A known color expected.\n"), fname, 1);
            return 1;
        }

        pdblColor[0] /= 255;
        pdblColor[1] /= 255;
        pdblColor[2] /= 255;
    }
    else //iRhs == 3
    {
        int i = 0;
        int count = 3;
        double dbl = 0;
        for (i = 0 ; i < count ; i++)
        {
            int* piAddr = NULL;

            sciErr = getVarAddressFromPosition(pvApiCtx, i + 1, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if (isDoubleType(pvApiCtx, piAddr) == FALSE || isScalar(pvApiCtx, piAddr) == FALSE)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, i + 1);
                return 1;
            }

            getScalarDouble(pvApiCtx, piAddr, &dbl);
            if (dbl < 0 || dbl > 255)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, i + 1, "0", "255");
                return 1;
            }

            pdblColor[i] = dbl / 255;
        }
    }

    iCurrentFigure = getCurrentFigure();
    if (iCurrentFigure == 0)
    {
        //use gdf
        iCurrentFigure = getFigureModel();
    }

    iColor = addColor(iCurrentFigure, pdblColor);

    createScalarDouble(pvApiCtx, iRhs + 1, (double)iColor);

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}