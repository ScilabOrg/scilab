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

/* =============================
 *
 * 			lsodarCreate
 *
 * =============================
 *
 * Creating the problem
 */

extern int C2F(lsodar) (CVRhsFnlsodar f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, int *itask, int *istate, int *iopt, realtype *rwork, int *lrw, int *iwork, int *liw, int *jac, int *jtype, CVRootFn *grblk, int *ng, int *jroot);

void lsodarCreate (CVRhsFnlsodar f, int *neq, realtype *yVec, realtype t0, realtype t0PlusEps, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int lrw, int *iwork, int liw, int Jacobians, int jtype, CVRootFn grblk, int ng, int *jroot)
{
    C2F(lsodar) (f, neq, yVec, &t0, &t0PlusEps, &itol, &reltol, &abstol, &itask, &istate, &iopt, rwork, &lrw, iwork, &liw, &Jacobians, &jtype, &grblk, &ng, jroot);
    printf("istate ap create %d\n", istate);
    printf("opt ap create %d\n", iopt);
}

/* =============================
 *
 * 			lsodarSolve
 *
 * =============================
 *
 * Solving the problem
 */

void lsodarSolve (CVRhsFnlsodar f, int *neq, realtype *yVec, realtype t, realtype tout, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int Lrw, int *iwork, int Liw, int Jacobians, int jtype, CVRootFn grblk, int ng, int *jroot)
{
    printf("istate Solve %d\n", istate);
    C2F(lsodar) (f, neq, yVec, &tout, &t, &itol, &reltol, &abstol, &itask, &istate, &iopt, rwork, &Lrw, iwork, &Liw, &Jacobians, &jtype, &grblk, &ng, jroot);
    printf("istate ap Solve %d\n", istate);
}
