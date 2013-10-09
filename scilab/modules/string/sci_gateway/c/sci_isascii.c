/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "api_scilab.h"
#ifndef _MSC_VER
#define iswascii(x) isascii(wctob(x))
#endif
/*----------------------------------------------------------------------------*/
static int isasciiStrings(char *fname, int *piAddressVarOne);
static int isasciiMatrix(char *fname, int *piAddressVarOne);
/*----------------------------------------------------------------------------*/
int sci_isascii(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int iType1 = 0;

    CheckRhs(1, 1);
    CheckLhs(0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarOne))
    {
        return isasciiMatrix(fname, piAddressVarOne);
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        return isasciiStrings(fname, piAddressVarOne);
    }

    Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix or matrix of strings expected.\n"), fname, 1);
    return 0;
}
/*--------------------------------------------------------------------------*/
static int isasciiMatrix(char *fname, int *piAddressVarOne)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    double *pdVarOne = NULL;

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (m1 * n1 > 0)
    {
        int i = 0;
        BOOL *bOutputMatrix = NULL;

        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &pdVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, &bOutputMatrix);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        for (i = 0; i < m1 * n1; i++)
        {
            int iVal = (int)pdVarOne[i];

            if (isascii(iVal))
            {
                bOutputMatrix[i] = (int)TRUE;
            }
            else
            {
                bOutputMatrix[i] = (int)FALSE;
            }
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        /* returns [] */
        m1 = 0;
        n1 = 0;

        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int isasciiStrings(char *fname, int *piAddressVarOne)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    wchar_t **pwcStVarOne = NULL;
    int *lenStVarOne = NULL;
    int mOut = 0;
    int nOut = 0;
    int x = 0;

    BOOL *bOutputMatrix = NULL;
    int lengthAllStrings = 0;

    int i = 0;

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
    if (lenStVarOne == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }


    sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, NULL);
    if (sciErr.iErr)
    {
        if (lenStVarOne)
        {
            FREE(lenStVarOne);
            lenStVarOne = NULL;
        }

        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    pwcStVarOne = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
    for (i = 0; i < (m1 * n1); i++)
    {
        lengthAllStrings = lengthAllStrings + lenStVarOne[i];

        pwcStVarOne[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));

        if (pwcStVarOne[i] == NULL)
        {
            if (lenStVarOne)
            {
                FREE(lenStVarOne);
                lenStVarOne = NULL;
            }

            freeArrayOfWideString(pwcStVarOne, m1 * n1);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }

    sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pwcStVarOne);
    if (sciErr.iErr)
    {
        if (lenStVarOne)
        {
            FREE(lenStVarOne);
            lenStVarOne = NULL;
        }

        freeArrayOfWideString(pwcStVarOne, m1 * n1);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    bOutputMatrix = (BOOL*)MALLOC(sizeof(BOOL) * lengthAllStrings);
    if (bOutputMatrix == NULL)
    {
        freeArrayOfWideString(pwcStVarOne, m1 * n1);
        FREE(lenStVarOne);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (i = 0; i < (m1 * n1); i++)
    {
        int j = 0;
        wchar_t* wcInput = pwcStVarOne[i];
        int len = (int)wcslen(wcInput);

        for (j = 0; j < len; j++)
        {
            if (iswascii(wcInput[j]))
            {
                bOutputMatrix[x] = (int)TRUE;
            }
            else
            {
                bOutputMatrix[x] = (int)FALSE;
            }
            x++;
        }
    }

    freeArrayOfWideString(pwcStVarOne, m1 * n1);
    FREE(lenStVarOne);

    mOut = 1;
    nOut = lengthAllStrings;

    sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, mOut, nOut, bOutputMatrix);
    FREE(bOutputMatrix);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/

