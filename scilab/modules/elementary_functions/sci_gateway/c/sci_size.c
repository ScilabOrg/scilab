/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 * Copyright (C) 2011 - INRIA - Serge STEER
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_elementary_functions.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsize) (int *id);

/*--------------------------------------------------------------------------*/
typedef enum
{
    I_SIZE_ROW = 1,
    I_SIZE_COL = 2,
    I_SIZE_ROWCOL = 0,
} size_second_input_argument_int;

/*--------------------------------------------------------------------------*/
#define C_SIZE_ROW "r"
#define C_SIZE_COL "c"
#define C_SIZE_ROWCOL "*"
/*--------------------------------------------------------------------------*/
int sci_size(char *fname, unsigned long fname_len)
{
    static int id[6];

    CheckRhs(1, 2);

    /*  bug 8296, we check second input argument, it must be r,c,* or 1,2,0 (compatibility) */
    if (Rhs == 2)
    {
        int iType = 0;
        int *piAddressVarTwo = NULL;
        int *piAddressVarOne = NULL;

        /* get Address of inputs */
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

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

        if (iType == sci_mlist)
        {
            C2F(intsize) (id);
            return 0;
        }

        if (isDoubleType(pvApiCtx, piAddressVarTwo) || isStringType(pvApiCtx, piAddressVarTwo))
        {
            int iValue = -1;

            if (isScalar(pvApiCtx, piAddressVarTwo))
            {
                if (isDoubleType(pvApiCtx, piAddressVarTwo))
                {
                    double dValue = 0.;

                    if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue) == 0)
                    {
                        iValue = (int)(dValue);

                        if ((double)iValue != dValue)
                        {
                            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                            return 0;
                        }

                        if (iValue <= 0)
                        {
                            Scierror(44, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), fname, 2);
                            return 0;
                        }
                    }
                }
                else // string
                {
                    char *pStr = NULL;
                    if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pStr) == 0)
                    {
                        if (pStr)
                        {
                            if (strcmp(pStr, C_SIZE_ROW) == 0)
                            {
                                iValue = I_SIZE_ROW;
                            }

                            if (strcmp(pStr, C_SIZE_COL) == 0)
                            {
                                iValue = I_SIZE_COL;
                            }

                            if (strcmp(pStr, C_SIZE_ROWCOL) == 0)
                            {
                                iValue = I_SIZE_ROWCOL;
                            }

                            freeAllocatedSingleString(pStr);
                            pStr = NULL;
                            if ((iValue != I_SIZE_ROW) && (iValue != I_SIZE_COL) && (iValue != I_SIZE_ROWCOL))
                            {
                                /* compatibility with previous error code 44 */
                                Scierror(44, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "r", "c", "*");
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if (pStr)
                        {
                            freeAllocatedSingleString(pStr);
                        }
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 2);
            return 0;
        }
    }

    C2F(intsize) (id);
    return 0;
}

/*--------------------------------------------------------------------------*/
