
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "api_scilab.h"
#include "string.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "gw_linear_algebra2.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double normString (double *, int, int, char *);
extern double normP      (double *, int, int, int);
/*--------------------------------------------------------------------------*/
int C2F(intnorm)(char *fname,unsigned long fname_len)
{
    int *pAAddr, *pflagAddr = NULL;

    double *pA = NULL;
    char **pflagChar = NULL;
    int *pflagLen;
    double *pflagVal = NULL;
    int iType, iRows = 0, iCols = 0, iRowsFlag = 0, iColsFlag = 0;
    int i = 0;
    double ret;

    SciErr sciErr;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // Checking A
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &pAAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, pAAddr, &iType);
    if (iType != sci_matrix)
    {
        Scierror(52, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 1);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, pAAddr, &iRows, &iCols, &pA);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if (iRows == 0)
    {
        ret = 0;
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
        AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
        return 0;
    }

    if (Rhs == 1)
    {
        //C2F(normP) (pA, iRows, iCols, 2);
        ret = normP(pA, iRows, iCols, 2);
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
        AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
        return 0;
    }
    else
    {
    // Checking flag
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &pflagAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, pflagAddr, &iType);
    if (iType == sci_strings)
    {
        sciErr = getMatrixOfString(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (iRowsFlag > 1 || iColsFlag > 1)
        {
            Scierror(205, _("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"), fname, 2, 1, 1);
            return 0;
        }
        pflagLen = (int*)malloc(sizeof(int) * iRowsFlag * iColsFlag);
        sciErr = getMatrixOfString(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, pflagLen, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (*pflagLen != 3)
        {
            Scierror(116, _("%s: Wrong value for input argument #%d: %s or %s expected.\n"), fname, 2, "inf", "fro");
        }

        pflagChar = (char**)malloc(sizeof(char*) * iRowsFlag * iColsFlag);
		for(i = 0 ; i < iRowsFlag * iColsFlag; i++)
		{
			pflagChar[i] = (char*)malloc(sizeof(char) * (pflagLen[i] + 1));//+ 1 for null termination
		}
        sciErr = getMatrixOfString(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, pflagLen, pflagChar);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (strncmp(*pflagChar, "inf", 3) != 0 && strncmp(*pflagChar,"fro", 3) != 0)
        {
            Scierror(116, _("%s: Wrong value for input argument #%d: %s or %s expected.\n"), fname, 2, "inf", "fro");
            return 0;
        }

		// Call normString
        //C2F(normString) (pA, iRows, iCols, *pflagChar);
        ret = normString(pA, iRows, iCols, *pflagChar);
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
        AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
        return 0;
    }
    else
    {
        if (iType == sci_matrix)
        {
            sciErr = getMatrixOfDouble(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, &pflagVal);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            if (iRowsFlag != 1 || iColsFlag != 1)
            {
                Scierror(205, _("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"), fname, 2, 1, 1);
                return 0;
            }
            if (*pflagVal != 1 && *pflagVal != 2)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                return 0;
            }
            //C2F(normP) (pA, iRows, iCols, *pflagVal);
            ret = normP(pA, iRows, iCols, *pflagVal);
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
            AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
            return 0;
        }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String or integer expected.\n"), fname, 2);
        return 0;
    }
}
}
    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;

    return 0;
}
/*--------------------------------------------------------------------------*/
