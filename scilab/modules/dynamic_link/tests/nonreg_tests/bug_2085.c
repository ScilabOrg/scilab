/* ============================================== */
/* Allan CORNET - bug 2085 */
/* ============================================== */
#include <string.h>
#include <stdio.h>
#define __USE_DEPRECATED_STACK_FUNCTIONS__
#include "api_scilab.h"
/* ============================================== */
static void f99(double *ar, double *ac, int *ita, int *ma, int *na);
/* ============================================== */
int intex2c(char *fname, void * pvApiCtx)
{
    static int lr1, lc1, it1, m1, n1;
    int minlhs = 1, minrhs = 1, maxlhs = 1, maxrhs = 1;
    double *pdblReal = NULL, *pdblImag = NULL;
    int* piAddr1 = NULL;

    SciErr sciErr;

    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    sciErr = getVarDimension(pvApiCtx, piAddr1, &m1, &n1);
    it1 = isVarComplex(pvApiCtx, piAddr1);
    sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &pdblReal, &pdblImag);

    f99(pdblReal, pdblImag, &it1, &m1, &n1);

    LhsVar(1) = 1;
    PutLhsVar();
    return 0;
}
/* ============================================== */
static void f99(double *ar, double *ac, int *ita, int *ma, int *na)
{
    int i = 0;
    for ( i = 0 ; i < (*ma) * (*na) ; i++)
    {
        ar[i] = 2 * ar[i] ;
    }
    if ( *ita == 1)
    {
        for ( i = 0 ; i < (*ma) * (*na) ; i++)
        {
            ac[i] = 3 * ac[i] ;
        }
    }
}
/* ============================================== */
