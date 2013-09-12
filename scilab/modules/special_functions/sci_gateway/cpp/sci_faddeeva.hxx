/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Steven G. Johnson <stevenj@alum.mit.edu>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* #define SCI_FADDEEVA and FADDEEVA_FUNC before #including this header
   file.  This gives us a simple way to define several similar "gateway"
   functions for the Faddeeva::erf, erfc, erfcx, erfi, and Dawson functions. */

int SCI_FADDEEVA(char *fname, unsigned long fname_len)
{
    int* zAddr = NULL;
    double* zr = NULL;
    double* zi = NULL;
    int m, n, N;

    double* wr = NULL;
    double* wi = NULL;
    int iType = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 1);


    /* get z */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &zAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // check type
    sciErr = getVarType(pvApiCtx, zAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getComplexMatrixOfDouble(pvApiCtx, zAddr, &m, &n, &zr, &zi);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    N = m * n;

    if (N == 0)    /* w([]) */
    {
        AssignOutputVariable(pvApiCtx, 1) = 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (zi)   // complex inputs and outputs
    {
        allocComplexMatrixOfDouble(pvApiCtx, 2, m, n, &wr, &wi);
        for (int i = 0; i < N; ++i)
        {
            std::complex<double> w =
                FADDEEVA_FUNC(std::complex<double>(zr[i], zi[i]));
            wr[i] = std::real(w);
            wi[i] = std::imag(w);
        }
    }
    else   // real inputs and outputs
    {
        allocMatrixOfDouble(pvApiCtx, 2, m, n, &wr);
        for (int i = 0; i < N; ++i)
        {
            wr[i] = FADDEEVA_FUNC(zr[i]);
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 2;
    ReturnArguments(pvApiCtx);

    return 0;
}
