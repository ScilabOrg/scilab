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
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/

#define MAXORD_DEFAULT  5

typedef double R;

typedef int (*DDASResFn) (R *tOld, R *y, R *yp, R *res, int *flag, R *dummy1, int *dummy2);

typedef int (*DDASRootFn) (int *neq, R *tOld, R *y, int *ng, R *groot, R *dummy1, int *dummy2);

typedef void (*DDASErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *user_data);

// R workspace
struct DDrWork_t
{
    R tcrit;
    R hmax;
    R hnext;
    R tfarthest;
    R rwork5;
    R rwork6;
    R hlast;
    R rwork[1];
};

enum DDiTask_t
{
    DDAS_NORMAL = 0,
    DDAS_ONE_STEP = 1,
};

extern void C2F(ddaskr) (DDASResFn res, int neq, R *t, R *y, R *yp, R *tout, int *info, R *reltol, R *abstol, int *istate, struct DDrWork_t *rwork, int *lrw, int *iwork, int *liw, R *dummy1, int *dummy2, int *jacobian, int *psol, DDASRootFn grblk, int *ng, int *jroot);

/*--------------------------------------------------------------------------*/
int sci_daskr(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int i, iflag, neq;
    int iType			=  0;
    int iRows			=  0;
    int iCols			=  0;
    int * piAddr		=  NULL;
    R ** pInitCond		=  NULL;
    R * py0				=  NULL;
    R * pyp0			=  NULL;
    R * ptStart			=  NULL;
    R * ptEnd			=  NULL;
    R abstol			=  1.0e-06;
    R reltol			=  1.0e-06;
    struct DDrWork_t * rwork	=  NULL;
    int lrw				=  0;
    int * iwork			=  0;
    int liw				=  0;
    DDASResFn res		=  NULL;
    int ng				=  0;
    DDASRootFn g		=  NULL;
    int * jroot			=  NULL;
    int * info			=  NULL;
    double dummy1		=  0.;
    int dummy2			=  0;
    int * istate		=  0;
    int * jac			=  NULL;
    int * psol			=  NULL;

    CheckInputArgument(pvApiCtx, 7, 11);
    CheckOutputArgument(pvApiCtx, 1, 3);
    int nbInputArg  = nbInputArgument(pvApiCtx);
    int nbOutputArg = nbOutputArgument(pvApiCtx);

    if (nbInputArg == 7) // daskr (x0, t0, t, res, ng, g, jroot)
    {
        // Checking first argument: x0
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isDoubleType(pvApiCtx, piAddr) && !isScalar(pvApiCtx, piAddr))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, pInitCond);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        py0 = (R *) MALLOC(iRows * sizeof(R));
        neq = iRows;
        if (iCols == 1) // User only provided y0
        {
            for (i = 0; i < iRows; ++i)
            {
                py0  = pInitCond[i];
            }
        }
        else // User provided y0 and yp0
        {
            pyp0 = (R *) MALLOC(iRows * sizeof(R));
            for (i = 0; i < iRows; ++i)
            {
                py0  = pInitCond[i, 1];
                pyp0 = pInitCond[i, 2];
            }
        }

        // Checking second argument: t0
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isDoubleType(pvApiCtx, piAddr) && isScalar(pvApiCtx, piAddr))
        {
            iflag = getScalarDouble(pvApiCtx, piAddr, ptStart);
            if (iflag)
            {
                return iflag;
            }
        }

        // Checking third argument: t
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isDoubleType(pvApiCtx, piAddr) && isScalar(pvApiCtx, piAddr))
        {
            iflag = getScalarDouble(pvApiCtx, piAddr, ptStart);
            if (iflag)
            {
                return iflag;
            }
        }

        // Checking fourth argument: res
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isPointerType(pvApiCtx, piAddr))
        {
            sciErr = getPointer(pvApiCtx, piAddr, res);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        // Checking fifth argument: ng
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isIntegerType(pvApiCtx, piAddr) && isScalar(pvApiCtx, piAddr))
        {
            iflag = getScalarInteger32(pvApiCtx, piAddr, &ng);
            if (iflag)
            {
                return iflag;
            }
        }

        // Checking sixth argument: g
        sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isPointerType(pvApiCtx, piAddr))
        {
            sciErr = getPointer(pvApiCtx, piAddr, g);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        // Checking seventh argument: jroot
        sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (isIntegerType(pvApiCtx, piAddr) && !isScalar(pvApiCtx, piAddr))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, pInitCond);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        lrw   =  50 + neq * (MAXORD_DEFAULT + 4 * neq) + 3 * ng;
        liw   =  20 + neq;
        rwork =  (struct DDrWork_t *) CALLOC(lrw, sizeof(R));
        iwork =  CALLOC(liw, sizeof(int));
        info  =  CALLOC(20, sizeof(int));

        C2F(ddaskr) (res, neq, ptStart, py0, pyp0, ptEnd, info, &reltol, &abstol, istate, rwork, &lrw, iwork, &liw, &dummy1, &dummy2, jac, psol, g, &ng, jroot);
    }

    //C2F(ddaskr) (fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
