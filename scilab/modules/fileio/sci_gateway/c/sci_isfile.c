/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "FileExist.h"
#include "isdir.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int sci_isfile(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    int iType = 0;
    int *lenStVarOne = NULL;
    int m1 = 0, n1 = 0;

    BOOL *results = NULL;
    int i = 0;

    /* Check Input & Output parameters */
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType != sci_strings)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

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

    results = (BOOL*)MALLOC(sizeof(BOOL) * (m1 * n1));
    if (results == NULL)
    {
        if (lenStVarOne)
        {
            FREE(lenStVarOne);
            lenStVarOne = NULL;
        }
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(results);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    pStVarOne = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
    if (pStVarOne == NULL)
    {
        if (lenStVarOne)
        {
            FREE(lenStVarOne);
            lenStVarOne = NULL;
        }
        FREE(results);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (i = 0; i < m1 * n1; i++)
    {
        pStVarOne[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
        if (pStVarOne[i] == NULL)
        {
            if (i == 0)
            {
                FREE(pStVarOne);
            }
            else
            {
                freeArrayOfWideString(pStVarOne, i - 1);
            }
            if (lenStVarOne)
            {
                FREE(lenStVarOne);
                lenStVarOne = NULL;
            }
            FREE(results);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }

    sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        freeArrayOfWideString(pStVarOne, m1*n1);
        FREE(results);
        return 0;
    }

    for (i = 0; i < m1 * n1; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pStVarOne[i]);
        if (expandedPath)
        {
            results[i] = !isdirW(expandedPath) && FileExistW(expandedPath);
            FREE(expandedPath);
            expandedPath = NULL;
        }
        else
        {
            results[i] = FALSE;
        }
    }

    if (lenStVarOne)
    {
        FREE(lenStVarOne);
        lenStVarOne = NULL;
    }
    freeArrayOfWideString(pStVarOne, m1 * n1);

    sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, results);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        free(results);
        return 0;
    }

    LhsVar(1) = Rhs + 1;

    if (results)
    {
        FREE(results);
        results = NULL;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
