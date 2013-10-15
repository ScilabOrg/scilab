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

    int i = 0, j = 0;
    int res2 = 0;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

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

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &dbl1);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < m1; ++i)
    {
        for (j = 0; j < n1; ++j)
        {
            if (dbl1[i + m1 * j] != (int) dbl1[i + m1 * j])
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

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &dbl2);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < m2; ++i)
    {
        for (j = 0; j < n2; ++j)
        {
            if (dbl2[i + m2 * j] != (int) dbl2[i + m2 * j])
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                return 0;
            }
        }
    }

    if (m1 == m2 && n1 == n2) // bitshift(mat1, mat2), proceed elementwise
        for (i = 0; i < m1 * n1; ++i)
            if (dbl2[i] >= 0)
            {
                dbl1[i] = ((int) dbl1[i]) << ((int) dbl2[i]); // Multiply dbl1[i] by 2^dbl2[i]
            }
            else
            {
                dbl1[i] = ((int) dbl1[i]) >> ((int) - dbl2[i]); // Divide dbl1[i] by 2^-dbl2[i]
            }
    else
    {
        if (m1 == 1 && n1 == 1) // bitshift(scalar, mat), shift 'scalar' by
        {
            for (i = 0; i < m2 * n2; ++i)
                if (dbl2[i] >= 0)
                {
                    dbl2[i] = ((int) * dbl1) << ((int) dbl2[i]); // Multiply dbl1 by 2^dbl2[i]
                }
                else
                {
                    dbl2[i] = ((int) * dbl1) >> ((int) - dbl2[i]); // Divide dbl1 by 2^-dbl2[i]
                }
            res2 = 1;
        }
        else
        {
            if (m2 == 1 && n2 == 1)
            {
                for (i = 0; i < m1 * n1; ++i)
                    if (*dbl2 >= 0)
                    {
                        dbl1[i] = ((int) dbl1[i]) << ((int) * dbl2); // Multiply dbl1[i] by 2^dbl2
                    }
                    else
                    {
                        dbl1[i] = ((int) dbl1[i]) >> ((int) - *dbl2); // Divide dbl1[i] by 2^-dbl2
                    }
            }
            else
            {
                Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), fname);
                return 0;
            }
        }
    }

    if (res2 == 0)
    {
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, dbl1);
    }
    else
    {
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, m2, n2, dbl2);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1 + res2;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
