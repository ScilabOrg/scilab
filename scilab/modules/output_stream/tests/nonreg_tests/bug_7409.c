/* ========================================================================== */
/* bug 8587 */
/* Allan CORNET - 2010 - DIGITEO */
/* ========================================================================== */
#include <api_scilab.h>
#include <BOOL.h>
#include <stack-c.h>
#include <Scierror.h>
#include <MALLOC.h>
/* ========================================================================== */
int sci_empty_string_test(char *fname)
{
    SciErr sciErr;

    // this function does not take input arguments
    CheckRhs(0, 0);

    // the number of output arguments must be zero or one
    CheckLhs(0, 1);

    // allocate memory for values
    char* cOut = "zero";

    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 0, 0, &cOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    return 0;
}
/* ========================================================================== */
