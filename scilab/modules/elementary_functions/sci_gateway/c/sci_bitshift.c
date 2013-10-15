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
/*--------------------------------------------------------------------------*/
#include <math.h>
#include "gw_elementary_functions.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_bitshift(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    double *dbl1 = NULL;
    int m1 = 0, n1 = 0;
    int *piAddressVarTwo = NULL;
    double *dbl2 = NULL;
    int m2 = 0, n2 = 0;

    CheckInputArgument(2, 2);
    CheckOutputArgument(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
        return 0;
    }

    if (getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &dbl1) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < m1; ++i)
    {
        for (j = 0; j < n1; ++j)
        {
            if (dbl1(i + m1 * j) != (int) dbl1(i + m1 * j))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                return 0;
            }
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 2);
        return 0;
    }

    if (getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &dbl2) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < m2; ++i)
    {
        for (j = 0; j < n2; ++j)
        {
            if (dbl1(i + m2 * j) != (int) dbl1(i + m2 * j))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                return 0;
            }
        }
    }

    &dbl1 = &dbl1 << &dbl2;

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, dbl1);
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
