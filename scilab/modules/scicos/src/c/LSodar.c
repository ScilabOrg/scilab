/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2012 - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "LSodar.h"
#include "machine.h"

#define RTFOUND          1
#define INITROOT         2
#define ZERODETACHING    4

extern void C2F(lsodar) (CVRhsFnLSodar *f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, int *itask, int *istate, int *iopt, realtype *rwork, int *lrw, int *iwork, int *liw,  int *Jac, int *Jactype, CVRootFn *grblk, int *ng, int *jroot);

/* =============================
 *
 * 			lsodarCreate
 *
 * =============================
 *
 * Creating the problem
 */

int LSodarCreate (CVRhsFnLSodar f, int *neq, int itol, realtype reltol, realtype abstol, int itask, CVRootFn grblk, int ng, int *jroot)
{

    LSodarMem lsodar_mem;

    int j = 0;
    realtype * yVec = NULL;
    int lrw = 20 + 16 * (*neq) + 3 * ng;
    int liw = 20 + (*neq);
    realtype * rwork = NULL;
    int * iwork = NULL;
    rwork = malloc(sizeof(realtype) * lrw);
    iwork = malloc(sizeof(int) * liw);
    for (j = 0; j < lrw; ++j)
    {
        rwork[j] = 0;
    }
    for (j = 0; j < liw; ++j)
    {
        iwork[j] = 0;
    }
    realtype t0 = 0.;
    realtype t0PlusEps = 0.1;

    lsodar_mem->func = f;
    lsodar_mem->nEquations = neq;
    lsodar_mem->yVector = yVec;
    lsodar_mem->tStart = t0;
    lsodar_mem->tEnd = t0PlusEps;
    lsodar_mem->iTol = itol;
    lsodar_mem->relTol = reltol;
    lsodar_mem->absTol = abstol;
    lsodar_mem->iTask = itask;
    lsodar_mem->iState = 1;
    lsodar_mem->iOpt = 0;
    lsodar_mem->rWork = rwork;
    lsodar_mem->Lrw = lrw;
    lsodar_mem->iWork = iwork;
    lsodar_mem->Liw = liw;
    lsodar_mem->Jac = 0;
    lsodar_mem->Jactype = 2;
    lsodar_mem->g_fun = grblk;
    lsodar_mem->ng_fun = ng;
    lsodar_mem->jRoot = jroot;

    printf("istate\n");
    C2F(lsodar) (lsodar_mem->func, lsodar_mem->nEquations, lsodar_mem->yVector, &(lsodar_mem->tStart), &lsodar_mem->tEnd, &lsodar_mem->iTol, &lsodar_mem->relTol, &lsodar_mem->absTol, &lsodar_mem->iTask, &(lsodar_mem->iState), &lsodar_mem->iOpt, lsodar_mem->rWork, &lsodar_mem->Lrw, lsodar_mem->iWork, &lsodar_mem->Liw, &lsodar_mem->Jac, &lsodar_mem->Jactype, lsodar_mem->g_fun, &lsodar_mem->ng_fun, lsodar_mem->jRoot);
    printf("stop\n");
    return (lsodar_mem->iState);
}

/* =============================
 *
 * 			lsodarSolve
 *
 * =============================
 *
 * Solving the problem
 */

int LSodarSolve (LSodarMem lsodar_mem, int *neq, N_Vector y, realtype t, realtype tout, int itask, int * jroot)
{

    realtype * yVec = NV_DATA_S(y);

    C2F(lsodar) (lsodar_mem->func, neq, yVec, &t, &tout, &lsodar_mem->iTol, &lsodar_mem->relTol, &lsodar_mem->absTol, &itask, &lsodar_mem->iState, &lsodar_mem->iOpt, lsodar_mem->rWork, &lsodar_mem->Lrw, lsodar_mem->iWork, &lsodar_mem->Liw, &lsodar_mem->Jac, &lsodar_mem->Jactype, lsodar_mem->g_fun, &lsodar_mem->ng_fun, jroot);

    NV_DATA_S(y) = yVec;

    return (lsodar_mem->iState);
}
