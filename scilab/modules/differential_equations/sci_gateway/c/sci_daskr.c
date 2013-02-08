/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2013 - Paul Bignier
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
#include "MALLOC.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/

typedef double realtype;

typedef int (*DDASResFn) (realtype *tOld, realtype *y, realtype *yp, realtype *res, int *flag, double *dummy1, int *dummy2);

typedef int (*DDASRootFn) (int *neq, realtype *tOld, realtype *y, int *ng, realtype *groot, double *dummy1, int *dummy2);

typedef void (*DDASErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *user_data);

// realtype workspace
struct DDrWork_t
{
    realtype tcrit;
    realtype hmax;
    realtype hnext;
    realtype tfarthest;
    realtype rwork5;
    realtype rwork6;
    realtype hlast;
    realtype rwork[1];
};

enum DDiTask_t
{
    DDAS_NORMAL = 0,
    DDAS_ONE_STEP = 1,
};

extern void C2F(daskr)(DDASResFn res, int *neq, realtype *t, realtype *y, realtype *yp, realtype *tout, int *info, realtype *reltol, realtype *abstol, int *istate, struct DDrWork_t *rwork, int *lrw, int *iwork, int *liw, double *dummy1, int *dummy2, int *jacobian, DDASRootFn grblk, int *ng, int *jroot);
/*--------------------------------------------------------------------------*/
int sci_daskr(char *fname, unsigned long fname_len)
{

    CheckInputArgument(pvApiCtx, 10, 11);
    CheckOutputArgument(1, 4);

    C2F(daskr)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
