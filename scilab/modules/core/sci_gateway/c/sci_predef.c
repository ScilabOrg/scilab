/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_core.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "predef.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_predef)(char *fname, unsigned long fname_len)
{
    int iRet = 0;
    int nout = 0 , mout = 0;
    int nbElements = 0;
    int * piAddr = NULL;
    double l1 = 0;
    double previous_n_var_protected = 0;
    double new_n_var_protected = 0;
    double * out_values = NULL;
    char * protectMode = NULL;
    char ** variablesPredef = NULL;
    SciErr sciErr;

    Rhs = Max(0, Rhs);

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    previous_n_var_protected = getNumberPredefVariablesProtected();

    if (Rhs == 0)
    {
        iRet = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, previous_n_var_protected);
        if (iRet)
        {
            return iRet;
        }
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else /* Rhs == 1 */
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isDoubleType(pvApiCtx, piAddr))
        {
            if (isScalar(pvApiCtx, piAddr))
            {
                iRet = getScalarDouble(pvApiCtx, piAddr, &l1);
				if (iRet)
				{
					return iRet;
				}
                if (l1 != (int) l1)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer expected.\n"), fname, 1);
                    return 0;
                }

                setNumberPredefVariablesProtected((int) l1);
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
                return 0;
            }
        }
        else if (isStringType(pvApiCtx, piAddr))
        {
            if (isScalar(pvApiCtx, piAddr))
            {
                iRet = getAllocatedSingleString(pvApiCtx, piAddr, &protectMode);
                if (iRet)
                {
                    freeAllocatedSingleString(protectMode);
                    return iRet;
                }
                if ( ((strlen(protectMode) == 1 ) && (protectMode[0] == 'c')) ||
                        (strcmp(protectMode, "clear") == 0) )
                {
                    clearPredef();
                }
                else if ( ((strlen(protectMode) == 1 ) && (protectMode[0] == 'a')) ||
                          (strcmp(protectMode, "all") == 0) )
                {
                    predefAll();
                }
                else if (strcmp(protectMode, "names") == 0)
                {
                    variablesPredef = getPredefinedVariablesName(&nbElements);
                    if (variablesPredef && (nbElements > 0))
                    {
                        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, nbElements, 1, variablesPredef);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            return 0;
                        }
                        freeArrayOfString(variablesPredef, nbElements);
                        variablesPredef = NULL;
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        }
                        else
                        {
                            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                            ReturnArguments(pvApiCtx);
                        }
                        return 0;
                    }
                    else
                    {
                        createEmptyMatrix(pvApiCtx, Rhs + 1);
                        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 1, "clear", "all", "names");
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"), fname, 1);
            return 0;
        }

        new_n_var_protected = getNumberPredefVariablesProtected();

        out_values = (double*) MALLOC(sizeof(double) * 2);
        out_values[0] = previous_n_var_protected;
        out_values[1] = new_n_var_protected;

        nout = 1;
        mout = 2;
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nout, mout, out_values);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        if (out_values)
        {
            FREE(out_values);
            out_values = NULL;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
