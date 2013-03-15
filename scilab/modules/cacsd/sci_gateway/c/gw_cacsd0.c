
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
<<<<<<< HEAD
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
=======
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
>>>>>>> 630de55... rewrite mex fortran gateway of cacsd with API Scilab
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_cacsd0.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_gschur, "gschur"},
    {sci_gspec, "gspec"},
    {sci_ereduc, "ereduc"},
    {sci_fstair, "fstair"},
    {sci_sorder, "sorder"},
    {sci_findbd, "findbd"},
    {sci_sident, "sident"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd0(void)
{
    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
int checkParam(void* _pvCtx, int _iPos, char* fname)
{
    SciErr sciErr;
    int* piAddr = NULL;

    //get var address
    sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, _iPos);
        return 1;
    }

    //check is real scalar double
    if ( isScalar(_pvCtx, piAddr) == 0 ||
            isDoubleType(_pvCtx, piAddr) == 0 ||
            isVarComplex(pvApiCtx, piAddr) == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, _iPos);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
char getIntegerValue(void* _pvCtx, int _iPos)
{
    SciErr sciErr;
    int* piAddr = NULL;
    double dblVal;

    //get var address
    sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "sorder", _iPos);
        return 1;
    }

    getScalarDouble(_pvCtx, piAddr, &dblVal);
    return (char)dblVal;
}
