/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises
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

#include <stdlib.h>

//TODO Complexes, WideString, Lists

int sci_createNamedVariable(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet = 0;
    int* piAddr1 = NULL;
    char* pstVarName = NULL;
    int* piAddr2 = NULL;
    int iVarType = 0;
    int iRows = 0;
    int iCols = 0;

    CheckRhs(2, 2);
    CheckLhs(1, 1);

    // variable name
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr1, &pstVarName))
    {
        Scierror(999, _("%s: Unable to get argument #%d (variable name).\n"), fname, 1);
        return 1;
    }

    if (isNamedVarExist(pvApiCtx, pstVarName))
    {
        Scierror(999, _("%s: Cannot create named variable %s: the variable already exists.\n"), fname, pstVarName);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // variable type
    sciErr = getVarType(pvApiCtx, piAddr2, &iVarType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    switch (iVarType)
    {
        case sci_matrix:
        {
            double *pdReal = NULL;
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &pdReal);
            if (sciErr.iErr)
                break;

            if ((iCols > 1) || (iRows > 1))
                sciErr = createNamedMatrixOfDouble(pvApiCtx, pstVarName, iRows, iCols, pdReal);
            else
                iRet = createNamedScalarDouble(pvApiCtx, pstVarName, *pdReal);
            break;
        }
        case sci_poly:
        {
            double **pdReal = NULL;
            int iPolyNameLen = 0;
            char *pstPolyName = NULL;
            int *piNbCoef = NULL;
            int i;

            sciErr = getPolyVariableName(pvApiCtx, piAddr2, NULL, &iPolyNameLen);
            if (sciErr.iErr)
                break;

            pstPolyName = (char*)malloc(sizeof(char) * (iPolyNameLen + 1));

            sciErr = getMatrixOfPoly(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
            if (sciErr.iErr)
                break;

            piNbCoef = (int*)malloc(sizeof(int) * iRows * iCols);

            sciErr = getMatrixOfPoly(pvApiCtx, piAddr2, &iRows, &iCols, piNbCoef, NULL);
            if (sciErr.iErr)
                break;

            pdReal = (double **)malloc(sizeof(double*) * iRows * iCols);

            for (i = 0 ; i < iRows * iCols ; i++)
                pdReal[i] = (double*)malloc(sizeof(double) * piNbCoef[i]);

            sciErr = getMatrixOfPoly(pvApiCtx, piAddr2, &iRows, &iCols, piNbCoef, pdReal);
            if (sciErr.iErr)
                break;

            sciErr = createNamedMatrixOfPoly(pvApiCtx, pstVarName, pstPolyName, iRows, iCols, piNbCoef, pdReal);
            break;
        }
        case sci_boolean:
        {
            int *piBool = NULL;
            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr2, &iRows, &iCols, &piBool);
            if (sciErr.iErr)
                break;

            if (iCols * iRows > 1)
                sciErr = createNamedMatrixOfBoolean(pvApiCtx, pstVarName, iRows, iCols, piBool);
            else
                iRet = createNamedScalarBoolean(pvApiCtx, pstVarName, *piBool);
            break;
        }
        case sci_sparse:
        {
            double *pdReal = NULL;
            int iNbItem = 0;
            int *piNbItemRow = NULL;
            int *piColPos = NULL;

            sciErr = getSparseMatrix(pvApiCtx, piAddr2, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdReal);
            if (sciErr.iErr)
                break;

            sciErr = createNamedSparseMatrix(pvApiCtx, pstVarName, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdReal);
            break;
        }
        case sci_boolean_sparse:
        {
            int iNbItem = 0;
            int *piNbItemRow = NULL;
            int *piColPos = NULL;

            sciErr = getBooleanSparseMatrix(pvApiCtx, piAddr2, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
            if (sciErr.iErr)
                break;

            sciErr = createNamedBooleanSparseMatrix(pvApiCtx, pstVarName, iRows, iCols, iNbItem, piNbItemRow, piColPos);
            break;
        }

        case sci_ints:
        {
            int iPrec = 0;
            sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr2, &iPrec);
            if (sciErr.iErr)
                break;

            switch (iPrec)
            {
                case SCI_INT8:
                {
                    char *pcInts = NULL;
                    sciErr = getMatrixOfInteger8(pvApiCtx, piAddr2, &iRows, &iCols, &pcInts);
                    if (sciErr.iErr)
                        break;

                    if (iCols * iRows > 1)
                        sciErr = createNamedMatrixOfInteger8(pvApiCtx, pstVarName, iRows, iCols, pcInts);
                    else
                        iRet = createNamedScalarInteger8(pvApiCtx, pstVarName, pcInts[0]);
                    break;
                }
                case SCI_INT16:
                {
                    short *psInts = NULL;
                    sciErr = getMatrixOfInteger16(pvApiCtx, piAddr2, &iRows, &iCols, &psInts);
                    if (sciErr.iErr)
                        break;
                    if (iCols * iRows > 1)
                        sciErr = createNamedMatrixOfInteger16(pvApiCtx, pstVarName, iRows, iCols, psInts);
                    else
                        iRet = createNamedScalarInteger16(pvApiCtx, pstVarName, psInts[0]);
                    break;
                }
                case SCI_INT32:
                {
                    int *piInts = NULL;
                    sciErr = getMatrixOfInteger32(pvApiCtx, piAddr2, &iRows, &iCols, &piInts);
                    if (sciErr.iErr)
                        break;

                    if (iCols * iRows > 1)
                        sciErr = createNamedMatrixOfInteger32(pvApiCtx, pstVarName, iRows, iCols, piInts);
                    else
                        iRet = createNamedScalarInteger32(pvApiCtx, pstVarName, piInts[0]);
                    break;
                }
                case SCI_UINT8:
                {
                    unsigned char *pucInts = NULL;
                    sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddr2, &iRows, &iCols, &pucInts);
                    if (sciErr.iErr)
                        break;

                    if (iCols * iRows > 1)
                        sciErr = createNamedMatrixOfUnsignedInteger8(pvApiCtx, pstVarName, iRows, iCols, pucInts);
                    else
                        iRet = createNamedScalarUnsignedInteger8(pvApiCtx, pstVarName, pucInts[0]);
                    break;
                }
                case SCI_UINT16:
                {
                    unsigned short *pusInts = NULL;
                    sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddr2, &iRows, &iCols, &pusInts);
                    if (sciErr.iErr)
                        break;

                    if (iCols * iRows > 1)
                        sciErr = createNamedMatrixOfUnsignedInteger16(pvApiCtx, pstVarName, iRows, iCols, pusInts);
                    else
                        iRet = createNamedScalarUnsignedInteger16(pvApiCtx, pstVarName, pusInts[0]);
                    break;
                }
                case SCI_UINT32:
                {
                    unsigned int *puiInts = NULL;
                    sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddr2, &iRows, &iCols, &puiInts);
                    if (sciErr.iErr)
                        break;

                    if (iCols * iRows > 1)
                        sciErr = createNamedMatrixOfUnsignedInteger32(pvApiCtx, pstVarName, iRows, iCols, puiInts);
                    else
                        iRet = createNamedScalarUnsignedInteger32(pvApiCtx, pstVarName, puiInts[0]);
                    break;
                }
            }
            break;
        }
        case sci_strings:
        {
            char **pstString = NULL;
            int *piLen = NULL;
            int i;

            sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
            if(sciErr.iErr)
                break;

            piLen = (int*)malloc(sizeof(int) * iRows * iCols);
            sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
            if(sciErr.iErr)
                break;

            pstString = (char**)malloc(sizeof(char*) * iRows * iCols);
            for (i = 0 ; i < iRows * iCols ; i++)
                pstString[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));

            sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, pstString);
            if(sciErr.iErr)
                break;

            sciErr = createNamedMatrixOfString(pvApiCtx, pstVarName, iRows, iCols, pstString);

            break;
        }
        case sci_pointer:
        {
            void *pvPtr = NULL;
            sciErr = getPointer(pvApiCtx, piAddr2, &pvPtr);
            if(sciErr.iErr)
                break;

            sciErr = createNamedPointer(pvApiCtx, pstVarName, pvPtr);
        }
    }

    if (sciErr.iErr || iRet)
    {
        if (sciErr.iErr)
            printError(&sciErr, 0);
        return 1;
    }

    if (sciErr.iErr || iRet)
        createScalarBoolean(pvApiCtx, Rhs + 1, 0);
    else
        createScalarBoolean(pvApiCtx, Rhs + 1, 1);
    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;

    return 0;
}

int sci_ptr2dbl(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet;
    int* piAddr = NULL;
    void *pvPtr = NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    iRet = createScalarDouble(pvApiCtx, Rhs + 1, (double)(int)pvPtr);
    if (iRet)
        return 1;

    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
    return 0;
}

int sci_dbl2ptr(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet;
    int* piAddr = NULL;
    double dReal = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    iRet = getScalarDouble(pvApiCtx, piAddr, &dReal);
    if (iRet)
        return 1;

    sciErr = createPointer(pvApiCtx, Rhs + 1, (void *)(int)dReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
    return 0;
}
