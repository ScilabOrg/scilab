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
#include "addColor.h"
#include "CurrentFigure.h"
#include "createGraphicObject.h"
#include "sciprint.h"

int checkValue(double dblValue)
{
    return (dblValue >= 0.0 && dblValue <= 1.0);
}

int checkValues(double* pdblValues, int iRows)
{
    int i = 0;
    for (i = 0 ; i < iRows ; i++)
    {
        if ((checkValue(pdblValues[i]) && checkValue(pdblValues[i + iRows]) && checkValue(pdblValues[i + iRows * 2])) == 0)
        {
            sciprint("%d %f %f %f\n", i, pdblValues[i], pdblValues[i + iRows], pdblValues[i + iRows * 2]);
            return 0;
        }
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int sci_addcolor(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int i = 0;
    int* piAddr = NULL;
    int iRows = 0;
    int iCols = 0;
    double* pdblColor = NULL;
    double color[3];

    int iCurrentFigure = 0;
    double* pdblReturnColor = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isDoubleType(pvApiCtx, piAddr) == FALSE)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real vector 1x3 expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblColor);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iCols != 3)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real vector nx3 expected.\n"), fname, 1);
        return 1;
    }

    //check values
    if (checkValues(pdblColor, iRows) == 0)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Value must be between 0.0 and 1.0 expected.\n"), fname, 1);
        return 1;
    }

    iCurrentFigure = getCurrentFigure();
    if (iCurrentFigure == 0)
    {
        iCurrentFigure = createNewFigureWithAxes();
    }

    allocMatrixOfDouble(pvApiCtx, 2, 1, iRows, &pdblReturnColor);
    for (i = 0 ; i < iRows ; i++)
    {
        color[0] = pdblColor[i];
        color[1] = pdblColor[i + iRows];
        color[2] = pdblColor[i + iRows * 2];

        pdblReturnColor[i] = addColor(iCurrentFigure, color);
    }

    AssignOutputVariable(pvApiCtx, 1) = 2;
    ReturnArguments(pvApiCtx);
    return 0;
}
