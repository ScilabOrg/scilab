/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "msgs.h"
#include "setieee.h"

/*--------------------------------------------------------------------------*/
int sci_log10(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int i = 0;
    int call_overload = 0;
    int check_ieee = 0;

    int* piAddr = NULL;
    int iType = 0;
    int iRows = 0;
    int iCols = 0;
    double* pIn = NULL;
    double* pOut = NULL;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType != sci_matrix || isVarComplex(pvApiCtx, piAddr))
    {
        //manage complex log10 via %s_log macro
        OverLoad(1);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pIn);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check values
    for (i = 0 ; i < iRows * iCols ; ++i)
    {
        double d = pIn[i];
        if (d < 0)
        {
            call_overload = 1;
            return 0;
        }

        if (d == 0)
        {
            check_ieee = 1;
        }
    }

    if (call_overload)
    {
        OverLoad(1);
        return 0;
    }

    if (check_ieee)
    {
        if (getieee() == 0)
        {
            SciError(32);
            return 0;
        }

        if (getieee() == 1)
        {
            Msgs(64, 0);
        }
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, iRhs + 1, iRows, iCols, &pOut);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (i = 0 ; i < iRows * iCols ; ++i)
    {
        pOut[i] = log10(pIn[i]);
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}