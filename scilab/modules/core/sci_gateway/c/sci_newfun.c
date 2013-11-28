/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - 2008 - INRIA - Allan CORNET
* Copyright (C) 2008 - INRIA - Bruno JOFRET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include <string.h>
#include <ctype.h>
#include "gw_core.h"
#include "BOOL.h"
#include "api_scilab.h"
#include "Funtab.h"
#include "IsAScalar.h"
#include "hashtable_core.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static BOOL Is_a_correct_function_name(char *functionname);
/*--------------------------------------------------------------------------*/
int C2F(sci_newfun) (char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    int iType1						= 0;
    int *piAddressVarOne	= NULL;
    char *pStVarOne				= NULL;
    int lenStVarOne				= 0;

    int m2 = 0, n2 = 0;
    int iType2						= 0;
    int *piAddressVarTwo	= NULL;
    double *pdVarTwo			= NULL;

    int ifptr = 0;

    CheckRhs(2, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if ( iType1 != sci_strings )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if ( iType2 != sci_matrix )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ( (m1 != 1) || (n1 != 1) )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &pdVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if ( (m2 != 1) || (n2 != 1) )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    ifptr = (int) * pdVarTwo;
    if (*pdVarTwo != (double)ifptr)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer expected.\n"), fname, 2);
        return 0;
    }

    pStVarOne = (char*)MALLOC(sizeof(char) * (lenStVarOne + 1));
    if (pStVarOne)
    {
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (ifptr > 0)
    {
        if (Is_a_correct_function_name(pStVarOne))
        {
            int id[nsiz];
            int i = 0;
            int zero = 0;
            int fptr = ifptr;
            int job = 0 ;

            /* initialize id */
            for (i = 0; i < nsiz; i++)
            {
                id[i] = 0;
            }

            C2F(cvname)(id, pStVarOne, &zero, (unsigned long)strlen(pStVarOne));
            job = SCI_HFUNCTIONS_ENTER;
            C2F(funtab)(id, &fptr, &job, pStVarOne, (unsigned long)strlen(pStVarOne));
            if (pStVarOne)
            {
                FREE(pStVarOne);
                pStVarOne = NULL;
            };

            LhsVar(1) = 0;
            PutLhsVar();
        }
        else
        {
            if (pStVarOne)
            {
                FREE(pStVarOne);
                pStVarOne = NULL;
            };
            Scierror(999, _("%s: function-name is incorrect.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be %s.\n"), fname, 2, "> 0");
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static BOOL Is_a_correct_function_name(char *functionname)
{
    if (functionname)
    {
        if ( (strlen(functionname) > 0) && (strlen(functionname) <  nlgh) )
        {
            int i = 0;
            int len_functionname = (int)strlen(functionname);

            if (isdigit(functionname[0]))
            {
                return FALSE;
            }

            for (i = 1; i < len_functionname; i++)
            {
                /*
                ???? = 3
                a___ = 3
                c!!! = 3
                */
                if ( (functionname[i] != '_') && (functionname[i] != '?') && (functionname[i] != '!') )
                {
                    if (!isalnum(functionname[i]))
                    {
                        return FALSE;
                    }
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
