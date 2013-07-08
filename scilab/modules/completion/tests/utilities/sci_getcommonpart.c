/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "MALLOC.h"
#include "getCommonPart.h"
/*--------------------------------------------------------------------------*/
int sci_getcommonpart(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int* piAddr         = NULL;
    char** pstrInput    = NULL;
    char* pcOutput      = NULL;
    int iRows           = 0;
    int iCols           = 0;
    int iSize           = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstrInput))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    iSize = iRows * iCols;

    if (((iRows == 1) && (iCols != 1) ) || ((iRows != 1) && (iCols == 1)))
    {
        pcOutput = getCommonPart(pstrInput, iSize);
        freeAllocatedMatrixOfString(iRows, iCols, pstrInput);

        if (pcOutput == NULL)
        {
            createSingleString(pvApiCtx, *getNbInputArgument(pvApiCtx) + 1, "");
        }
        else
        {
            createSingleString(pvApiCtx, *getNbInputArgument(pvApiCtx) + 1, pcOutput);
            FREE(pcOutput);
            pcOutput = NULL;
        }

        AssignOutputVariable(pvApiCtx, 1) = 2; // rhs + 1
        returnArguments(pvApiCtx);
    }
    else
    {
        freeAllocatedMatrixOfString(iRows, iCols, pstrInput);
        Scierror(999, _("%s: Wrong size for input argument %d.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/

