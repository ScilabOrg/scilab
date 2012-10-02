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

#include "lsodar.h"
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

void * LSodarCreate (CVRhsFnLSodar f, int *neq, N_Vector y, int itol, realtype reltol, realtype abstol, int itask, CVRootFn grblk, int ng, int *jroot)
{

    LSodarMem cvode_mem;
    cvode_mem = NULL;
    cvode_mem = (LSodarMem) malloc(sizeof(struct LSodarMemRec));
    if (cvode_mem == NULL)
    {
        printf("LSodarCreate\n");
        return(NULL);
    }


    int i = 0;
    realtype * yVec = NV_DATA_S(y);
    int lrw = 20 + 16 * (*neq) + 3 * ng;
    int liw = 20 + (*neq);
    realtype * rwork = NULL;
    int * iwork = NULL;
    rwork = malloc(sizeof(realtype) * lrw);
    iwork = malloc(sizeof(int) * liw);
    for (i = 0; i < lrw; ++i)
    {
        rwork[i] = 0;
    }
    for (i = 0; i < liw; ++i)
    {
        iwork[i] = 0;
    }
    realtype t0 = 0.;
    realtype t0PlusEps = 0.1;

    cvode_mem->func = f;
    cvode_mem->nEquations = neq;
    cvode_mem->yVector = yVec;
    cvode_mem->tStart = t0;
    cvode_mem->tEnd = t0PlusEps;
    cvode_mem->iTol = itol;
    cvode_mem->relTol = reltol;
    cvode_mem->absTol = abstol;
    cvode_mem->iTask = itask;
    cvode_mem->iState = 1;
    cvode_mem->iOpt = 0;
    cvode_mem->rWork = rwork;
    cvode_mem->Lrw = lrw;
    cvode_mem->iWork = iwork;
    cvode_mem->Liw = liw;
    cvode_mem->Jac = 0;
    cvode_mem->Jactype = 2;
    cvode_mem->g_fun = grblk;
    cvode_mem->ng_fun = ng;
    cvode_mem->jRoot = jroot;

    C2F(lsodar) (cvode_mem->func, cvode_mem->nEquations, cvode_mem->yVector, &(cvode_mem->tStart), &cvode_mem->tEnd, &cvode_mem->iTol, &cvode_mem->relTol, &cvode_mem->absTol, &cvode_mem->iTask, &(cvode_mem->iState), &cvode_mem->iOpt, cvode_mem->rWork, &cvode_mem->Lrw, cvode_mem->iWork, &cvode_mem->Liw, &cvode_mem->Jac, &cvode_mem->Jactype, cvode_mem->g_fun, &cvode_mem->ng_fun, cvode_mem->jRoot);

    return ((void *) cvode_mem);
}

/* =============================
 *
 * 			lsodarSolve
 *
 * =============================
 *
 * Solving the problem
 */

# define func    cv_mem->func
# define iTol	   cv_mem->iTol
# define relTol  cv_mem->relTol
# define absTol  cv_mem->absTol
# define iState  cv_mem->iState
# define iOpt    cv_mem->iOpt
# define rWork   cv_mem->rWork
# define Lrw     cv_mem->Lrw
# define iWork   cv_mem->iWork
# define Liw     cv_mem->Liw
# define Jac     cv_mem->Jac
# define Jactype cv_mem->Jactype
# define g_fun   cv_mem->g_fun
# define ng_fun  cv_mem->ng_fun

int LSodarSolve (void * cvode_mem, int *neq, N_Vector y, realtype t, realtype tout, int itask, int * jroot)
{

    LSodarMem cv_mem;
    cv_mem = (LSodarMem) cvode_mem;

    realtype * yVec = NV_DATA_S(y);

    C2F(lsodar) (func, neq, yVec, &t, &tout, &iTol, &relTol, &absTol, &itask, &iState, &iOpt, rWork, &Lrw, iWork, &Liw, &Jac, &Jactype, g_fun, &ng_fun, jroot);

    NV_DATA_S(y) = yVec;

    return (iState);
}
