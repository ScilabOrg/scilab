/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int read_double(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int i;
    //first variable info : real matrix of double
    int iType			= 0;
    int iRows			= 0;
    int iCols			= 0;
    int iComplex		= 0;
    int *piAddr			= NULL;
    double* pdblReal	= NULL;
    double* pdblImg		= NULL;

    //check input and output arguments
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /************************
    *    First variable    *
    ************************/

    //get variable address of the first input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check type
    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr || iType != sci_matrix)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //get complexity
    iComplex	= isVarComplex(pvApiCtx, piAddr);

    //check complexity
    if (iComplex)
    {
        //get size and data from Scilab memory
        sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
    }
    else
    {
        //get size and data from Scilab memory
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Do something with data
    //if variable is complex, switch real part and imaginary part otherwise multiply by -1
    if (iComplex)
    {
        sciErr = createComplexMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pdblImg, pdblReal);
    }
    else
    {
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = pdblReal[i] * -1;
        }
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pdblReal);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
