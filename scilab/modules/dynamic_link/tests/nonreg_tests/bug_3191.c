/* ============================================== */
#define __USE_DEPRECATED_STACK_FUNCTIONS__
#include "api_scilab.h"
#include "sciprint.h"
/* ============================================== */
/*  Interface for c_fun fonction */
/* ============================================== */
void c_fun(double *b, double *c, double *a);
/* ============================================== */
int c_intfun(char *fname, void * pvApiCtx)
{
    int l1 = 0, m1 = 0, n1 = 0, l2 = 0, m2 = 0, n2 = 0, l3 = 0, m3 = 1, n3 = 1;
    int minlhs = 1, maxlhs = 1, minrhs = 2, maxrhs = 2;

    double *pdbl1 = NULL, *pdbl2 = NULL, *pdbl3 = NULL;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;

    SciErr sciErr;


    /* Check number of inputs (rhs=2) and outputs (lhs=1) */
    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    sciErr = getVarDimension(pvApiCtx, piAddr1, &m1, &n1);
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &pdbl1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    sciErr = getVarDimension(pvApiCtx, piAddr2, &m2, &n2);
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &pdbl2);



    sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, m3, n3, &pdbl3);

    /* Get Qdata (#1) and Param (#2) and create C (#3) as double ("d") matrices */
    /* GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);   /* b */
    /* GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);   /* c */

    /* CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3); /* Result : a = b+c */

    /* Check dimensions  */
    if (!(m1 == 1) | !(n1 == 1))
    {
        sciprint("%s: Wrong inputs \r\n", "c_fun");
        SciError(999);
        return 0;
    }
    if (!(m2 == 1) | !(n2 == 1))
    {
        sciprint("%s: Wrong inputs \r\n", "c_fun");
        SciError(999);
        return 0;
    }

    /* Call c_fun */
    c_fun(pdbl1, pdbl2, pdbl3);

    /*  Return result  */
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/* ============================================== */
void c_fun(double *b, double *c, double *a)
{
    *a = *b + *c;
}
/* ============================================== */
