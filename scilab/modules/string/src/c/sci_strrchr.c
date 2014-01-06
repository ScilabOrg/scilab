/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "stringsstrrchr.h"
/*----------------------------------------------------------------------------*/
int sci_strrchr(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iErr = 0;

    int* piAddr1 = NULL;
    wchar_t **pStrVarOne = NULL;
    int m1 = 0;
    int n1 = 0;

    int* piAddr2 = NULL;
    wchar_t **pStrVarTwo = NULL;
    int m2 = 0;
    int n2 = 0;

    int i = 0;
    wchar_t **wcOutput_Strings = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddr1, &m1, &n1, &pStrVarOne))
    {
        freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne);
        Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddr2, &m2, &n2, &pStrVarTwo))
    {
        freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne);
        freeAllocatedMatrixOfWideString(m2, n2, pStrVarTwo);
        Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
        return 0;
    }

    for (i = 0; i < m2 * n2; i++)
    {
        if (wcslen(pStrVarTwo[i]) != 1)
        {
            freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne);
            freeAllocatedMatrixOfWideString(m2, n2, pStrVarTwo);
            Scierror(999, _("%s: Wrong size for input argument #%d: A character expected.\n"), fname, 2);
            return 0;
        }
    }

    wcOutput_Strings = strings_wcsrchr((const wchar_t**)pStrVarOne, m1 * n1, (const wchar_t**)pStrVarTwo, m2 * n2, FALSE);
    freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne);
    freeAllocatedMatrixOfWideString(m2, n2, pStrVarTwo);

    if (wcOutput_Strings == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1 , m1, n1, wcOutput_Strings);
    freeAllocatedMatrixOfWideString(m1, n1, wcOutput_Strings);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1 ;
    PutLhsVar();
    return 0;
}
