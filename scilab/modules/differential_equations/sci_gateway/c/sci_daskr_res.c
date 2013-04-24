/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_differential_equations2.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern C2F(bresd)(double*, double*, double*, double*, int*, double*, int*);
/*--------------------------------------------------------------------------*/
// fake gateway of res called by pjac when pjac is a macro
// this gateway will call the fake function of res
/*--------------------------------------------------------------------------*/
int sci_daskr_res(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrT    = NULL;
    int* piAddrY    = NULL;
    int* piAddrYdot = NULL;

    int rowsT = 0, colsT = 0;
    int rowsY = 0, colsY = 0;
    int rowsYdot = 0, colsYdot = 0;

    double* pT      = NULL;
    double* pY      = NULL;
    double* pYdot   = NULL;
    double* pIres   = NULL;
    double* pRes    = NULL;

    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 2, 2);

    // get inputs argument t, y, ydot
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrT, &rowsT, &colsT, &pT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A scalar expected.\n"), fname, 1);
        return 1;
    }

    if (rowsT * colsT != 1)
    {
        Scierror(999, _("%s: Wrong size for argument %d: A scalar expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrY, &rowsY, &colsY, &pY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A matrix expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrYdot);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrT, &rowsYdot, &colsYdot, &pYdot);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A matrix expected.\n"), fname, 3);
        return 1;
    }

    // create output arguments res and ires
    sciErr = allocMatrixOfDouble(pvApiCtx, 4, 1, 1, &pIres);
    sciErr = allocMatrixOfDouble(pvApiCtx, 4, rowsY, colsY, &pRes);

    // call user function res wich fill res and ires
    int ires = 0;
    C2F(bresd)(pT, pY, pYdot, pRes, &ires, NULL, NULL);
    *pIres = (double)ires;

    return 0;
}
/*--------------------------------------------------------------------------*/
