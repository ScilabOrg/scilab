/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "gw_linear_algebra2.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
extern double normString  (double *, int, int, char *);
extern double normStringC (doublecomplex *, int, int, char *);
extern double normP       (double *, int, int, double);
extern double normPC      (doublecomplex *, int, int, double);
/*--------------------------------------------------------------------------*/
int C2F(intnorm)(char *fname, unsigned long fname_len)
{
    // Arguments' addresses
    int *pAAddr, *pflagAddr = NULL;
    // Arguments' values
    double *pA        = NULL;
    char **pflagChar  = NULL;
    double *pflagVal  = NULL;
    doublecomplex *pAC = NULL;
    // Arguments' properties (type, dimensions, length)
    int *pflagLen;
    int iType, iRows = 0, iCols = 0, iRowsFlag = 0, iColsFlag = 0;
    // Return value
    double ret;

    double RowsColsTemp;
    int i = 0, isMat = 0, isComplex = 0;

    SciErr sciErr;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // Checking A.
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &pAAddr); // Retrieving A address.
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, pAAddr, &iType); // Retrieving A type.
    if (iType != sci_matrix)
    {
        Scierror(52, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 1);
        return 0;
    }
    if (isVarComplex(pvApiCtx, pAAddr))
    {
        sciErr = getComplexZMatrixOfDouble(pvApiCtx, pAAddr, &iRows, &iCols, &pAC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        isComplex = 1;
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, pAAddr, &iRows, &iCols, &pA);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
    }
    if (iRows == 0) // A = [] => returning 0.
    {
        ret = 0;
        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
        AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
        return 0;
    }
    if (iRows > 1 && iCols > 1) // If A is a matrix, only 1, 2 and %inf are allowed as second argument.
    {
        isMat = 1;
    }
    if (iRows == 1) // If iRows == 1, then transpose A to consider it like a vector.
    {
        RowsColsTemp = iRows;
        iRows = iCols;
        iCols = RowsColsTemp;
    }

    if (Rhs == 1) // One argument => returning norm 2.
    {
        // Call normP() or normPC().
        if (isComplex == 0)
        {
            ret = normP(pA, iRows, iCols, 2);
        }
        else
        {
            ret = normPC(pAC, iRows, iCols, 2); // if A is a complex matrix, call the complex function.
        }
        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
        AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
        return 0;
    }
    else
    {
        // Checking flag.
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &pflagAddr); // Retrieving flag address.
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        sciErr = getVarType(pvApiCtx, pflagAddr, &iType); // Retrieving flag type.
        if (iType == sci_strings)
        {
            sciErr = getMatrixOfString(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, NULL, NULL);  // Retrieving flag dimensions.
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }
            if (iRowsFlag > 1 || iColsFlag > 1)
            {
                Scierror(205, _("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"), fname, 2, 1, 1);
                return sciErr.iErr;
            }
            pflagLen = (int*) malloc(sizeof(int) * iRowsFlag * iColsFlag);
            sciErr = getMatrixOfString(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, pflagLen, NULL); // Retrieving flag length.
            if (sciErr.iErr)
            {
                free(pflagLen);
                printError(&sciErr, 0);
                return 0;
            }
            if (*pflagLen != 3 && *pflagLen != 1)
            {
                Scierror(116, _("%s: Wrong value for input argument #%d: %s, %s, %s, or %s expected.\n"), fname, 2, "i", "inf", "f", "fro");
                free(pflagLen);
                return 0;
            }
            pflagChar = (char**) malloc(sizeof(char*) * iRowsFlag * iColsFlag);
            for (i = 0 ; i < iRowsFlag * iColsFlag; ++i)
            {
                pflagChar[i] = (char*) malloc(sizeof(char) * (pflagLen[i] + 1));//+ 1 for null termination.
            }
            sciErr = getMatrixOfString(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, pflagLen, pflagChar); // Retrieving flag value.
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                free(pflagLen);
                for (i = 0 ; i < iRowsFlag * iColsFlag; ++i)
                {
                    free(pflagChar[i]);
                }
                free(pflagChar);
                return sciErr.iErr;
            }
            if (strncmp(*pflagChar, "inf", 3) != 0 && strncmp(*pflagChar, "i", 1) != 0 &&
                    strncmp(*pflagChar, "fro", 3) != 0 && strncmp(*pflagChar, "f", 1) != 0) // flag must be = "inf", "i", "fro" or "f".
            {
                Scierror(116, _("%s: Wrong value for input argument #%d: %s, %s, %s or %s expected.\n"), fname, 2, "i", "inf", "f", "fro");
                free(pflagLen);
                for (i = 0 ; i < iRowsFlag * iColsFlag; ++i)
                {
                    free(pflagChar[i]);
                }
                free(pflagChar);
                return 0;
            }
            if (isComplex == 0)
            {
                ret = normString(pA, iRows, iCols, *pflagChar); // flag is a string => returning the corresponding norm.
            }
            else
            {
                ret = normStringC(pAC, iRows, iCols, *pflagChar); // if A is a complex matrix, call the complex function.
            }
            createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
            AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
            free(pflagLen);
            for (i = 0 ; i < iRowsFlag * iColsFlag; ++i)
            {
                free(pflagChar[i]);
            }
            free(pflagChar);
            return 0;
        }
        else
        {
            if (iType == sci_matrix)
            {
                sciErr = getMatrixOfDouble(pvApiCtx, pflagAddr, &iRowsFlag, &iColsFlag, &pflagVal); // Retrieving flag value & dimensions as a double.
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                if (iRowsFlag != 1 || iColsFlag != 1) // flag size must be (1,1).
                {
                    Scierror(205, _("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"), fname, 2, 1, 1);
                    return 0;
                }
                // Call the norm functions.
                if (isinf(*pflagVal) == 1) // flag = %inf
                {
                    if (isComplex == 0)
                    {
                        ret = normString(pA, iRows, iCols, "inf"); // The infinite norm is computed by normString().
                    }
                    else
                    {
                        ret = normStringC(pAC, iRows, iCols, "inf"); // if A is a complex matrix, call the complex function.
                    }
                    createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
                    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
                    return 0;
                }
                else
                {
                    if (isMat == 1 && *pflagVal != 1 && *pflagVal != 2 && isinf(*pflagVal) != 1)
                    {
                        Scierror(116, _("%s: Wrong value for input argument #%d: %d, %d or %s expected.\n"), fname, 2, 1, 2, "inf", "-inf");
                        return 0;
                    }
                    if (isComplex == 0)
                    {
                        ret = normP(pA, iRows, iCols, *pflagVal); // flag is an integer => returning the corresponding norm.
                    }
                    else
                    {
                        ret = normPC(pAC, iRows, iCols, *pflagVal); // if A is a complex matrix, call the complex function.
                    }
                    createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &ret);
                    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String or integer expected.\n"), fname, 2);
                return 0;
            }
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
