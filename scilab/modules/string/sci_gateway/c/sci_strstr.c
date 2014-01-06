/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include "gw_string.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*----------------------------------------------------------------------------*/
int sci_strstr(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iErr = 0;

    int* piAddr1 = NULL;
    char** pStrVarOne = NULL;
    int m1 = 0;
    int n1 = 0;

    int* piAddr2 = NULL;
    char** pStrVarTwo = NULL;
    int m2 = 0;
    int n2 = 0;

    int iRhs = nbInputArgument(pvApiCtx);
    int i = 0;
    BOOL is_strstr = (strcmp(fname, "strstr") == 0);

    char** pstOutput_Strings = NULL;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, &pStrVarOne))
    {
        freeAllocatedMatrixOfString(m1, n1, pStrVarOne);
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

    if (getAllocatedMatrixOfString(pvApiCtx, piAddr2, &m2, &n2, &pStrVarTwo))
    {
        freeAllocatedMatrixOfString(m1, n1, pStrVarOne);
        freeAllocatedMatrixOfString(m2, n2, pStrVarTwo);
        Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
        return 0;
    }

    //check size of var1 and var2
    if (m2 * n2 != 1 && m2 * n2 != m1 * n1)
    {
        freeAllocatedMatrixOfString(m1, n1, pStrVarOne);
        freeAllocatedMatrixOfString(m2, n2, pStrVarTwo);
        Scierror(999, _("%s: Wrong size for input argument #%d: Same size expected.\n"), fname, 2);
        return 0;
    }

    //only for strchr, must be a single character
    if (is_strstr == FALSE)
    {
        for (i = 0; i < m2 * n2; i++)
        {
            if (strlen(pStrVarTwo[i]) != 1)
            {
                wchar_t* pwst = to_wide_string(pStrVarTwo[i]);
                if (wcslen(pwst) != 1)
                {
                    FREE(pwst);
                    freeAllocatedMatrixOfString(m1, n1, pStrVarOne);
                    freeAllocatedMatrixOfString(m2, n2, pStrVarTwo);
                    Scierror(999, _("%s: Wrong size for input argument #%d: A character expected.\n"), fname, 2);
                    return 0;
                }

                FREE(pwst);
            }
        }
    }

    //alloc output variable
    pstOutput_Strings = (char**)MALLOC(sizeof(char**) * n1 * m1);

    //two cases, var2 is scalar or not
    if (m2 * n2 == 1)
    {
        int i = 0;
        for (i = 0 ; i < m1 * n1 ; i++)
        {
            char* pst = strstr(pStrVarOne[i], pStrVarTwo[0]);
            if (pst)
            {
                pstOutput_Strings[i] = strdup(pst);
            }
            else
            {
                pstOutput_Strings[i] = strdup("");
            }
        }
    }
    else
    {
        int i = 0;
        for (i = 0 ; i < m1 * n1 ; i++)
        {
            char* pst = strstr(pStrVarOne[i], pStrVarTwo[i]);
            if (pst)
            {
                pstOutput_Strings[i] = strdup(pst);
            }
            else
            {
                pstOutput_Strings[i] = strdup("");
            }
        }
    }

    sciErr = createMatrixOfString(pvApiCtx, iRhs + 1 , m1, n1, pstOutput_Strings);
    freeAllocatedMatrixOfString(m1, n1, pstOutput_Strings);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1 ;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/

