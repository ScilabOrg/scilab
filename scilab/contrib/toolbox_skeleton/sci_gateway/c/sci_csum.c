/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "csum.h"
/* ==================================================================== */
int sci_csum(char *fname)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    double dVarOne = 0.0;

    int *piAddressVarTwo = NULL;
    double dVarTwo = 0.0;

    double dOut = 0.0;

    /* --> result = csum(3,8)
    /* check that we have only 2 input arguments */
    /* check that we have only 1 output argument */
    CheckInputArgument(pvApiCtx, 2, 2) ;
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* check input type */
    if ( !isDoubleType(pvApiCtx, piAddressVarOne) )
    {
        Scierror(999, "%s: Wrong type for input argument #%d: A scalar expected.\n", fname, 1);
        return 0;
    }

    if ( !isDoubleType(pvApiCtx, piAddressVarTwo) )
    {
        Scierror(999, "%s: Wrong type for input argument #%d: A scalar expected.\n", fname, 2);
        return 0;
    }

    if ( getScalarDouble(pvApiCtx, piAddressVarOne, &dVarOne) )
    {
        Scierror(999, "%s: Wrong size for input argument #%d: A scalar expected.\n", fname, 1);
        return 0;
    }

    if ( getScalarDouble(pvApiCtx, piAddressVarTwo, &dVarTwo) )
    {
        Scierror(999, "%s: Wrong size for input argument #%d: A scalar expected.\n", fname, 2);
        return 0;
    }

    /* call c function csum */
    csum(&dVarOne, &dVarTwo, &dOut);

    /* create result on stack */
    createScalarDouble(pvApiCtx, InputArgument + 1, dOut);

    AssignOutputVariable(1) = InputArgument + 1;

    /* This function put on scilab stack, the lhs variable
    which are at the position lhs(i) on calling stack */
    /* You need to add UpdateStack here because WITHOUT_ADD_PUTLHSVAR
    was defined and equal to %t */
    /* without this, you do not need to add UpdateStack here */
    UpdateStack();

    return 0;
}
/* ==================================================================== */

