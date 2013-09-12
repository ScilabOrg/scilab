/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include <stdlib.h> /* _MAX_ENV */
#include "gw_io.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "getenvc.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int sci_getenv(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int ierr = 0;
    char *default_env_value = NULL;
    char *env_value = NULL;
    int length_env = 0;
    char *env_name = NULL;

    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    int iType1	= 0;
    char *pStVarOne = NULL;
    int lenStVarOne = 0;

    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;
    int iType2	= 0;
    char *pStVarTwo = NULL;
    int lenStVarTwo = 0;

    int iflag = 0;
    int m_out = 1, n_out = 1;

    Rhs = Max(Rhs, 0);

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (iType2  != sci_strings )
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if ( (m2 != n2) && (n2 != 1) )
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        pStVarTwo = (char*)MALLOC(sizeof(char) * (lenStVarTwo + 1));
        if (pStVarTwo)
        {
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }

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

    if (iType1  != sci_strings )
    {
        if (pStVarTwo)
        {
            FREE(pStVarTwo);
            pStVarTwo = NULL;
        }
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ( (m1 != n1) && (n1 != 1) )
    {
        if (pStVarTwo)
        {
            FREE(pStVarTwo);
            pStVarTwo = NULL;
        }
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
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
        if (pStVarTwo)
        {
            FREE(pStVarTwo);
            pStVarTwo = NULL;
        }
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    default_env_value =  pStVarTwo;
    env_name = pStVarOne;

    C2F(getenvc)(&ierr, env_name, NULL, &length_env, &iflag);

    if (ierr == 0)
    {
        env_value = (char*)MALLOC( (length_env + 1) * sizeof(char) );
        if (env_value == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
        }
        else
        {
            C2F(getenvc)(&ierr, env_name, env_value, &length_env, &iflag);

            //create variable on stack and return it.
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, (const char**)&env_value);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
    }
    else //ierr == 1 -> env var does not exist.
    {
        if (default_env_value)
        {
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, &default_env_value);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
            }
            else
            {
                LhsVar(1) = Rhs + 1;
                PutLhsVar();
            }
        }
        else
        {
            Scierror(999, _("%s: Undefined environment variable %s.\n"), fname, env_name);
        }
    }



    if (default_env_value)
    {
        FREE(default_env_value);
        default_env_value = NULL;
    }

    if (env_name)
    {
        FREE(env_name);
        env_name = NULL;
    }

    if (env_value)
    {
        FREE(env_value);
        env_value = NULL;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
