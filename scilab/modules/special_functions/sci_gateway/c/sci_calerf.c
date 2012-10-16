/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_special_functions.h"
#include "machine.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern void C2F(calerf)();
/*--------------------------------------------------------------------------*/
int sci_calerf(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    double* lX      = NULL;
    double dblFlag  = 0;

    int* piAddrX    = NULL;
    int* piAddrFlag = NULL;
    int iFlag       = 0;

    int NX = 0, MX = 0, i = 0;

    nbInputArgument(pvApiCtx) = Max(0, nbInputArgument(pvApiCtx));

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //     checking variable x (number 1)
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddrX))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrX, &MX, &NX, &lX);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Wrong type for argument %d: A matrix expected.\n"), fname, 1);
    }

    if (MX * NX == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 1;
        returnArguments(pvApiCtx);
        return 0;
    }

    //     checking variable flag (number 2)
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrFlag);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddrFlag))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
        return 1;
    }

    if (getScalarDouble(pvApiCtx, piAddrFlag, &dblFlag))
    {
        Scierror(999, _("%s: Wrong type for argument %d: A scalar expected.\n"), fname, 2);
        return 1;
    }

    iFlag = (int)dblFlag;

    if (iFlag < 0 || iFlag > 2)
    {
        Scierror(999, _("%s: Wrong value for argument %d: 0, 1 or 2 expected.\n"), fname, 2);
        return 1;
    }

    for (i = 0; i < MX * NX; i++)
    {
        C2F(calerf)(lX + i, lX + i, &iFlag);
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    returnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
