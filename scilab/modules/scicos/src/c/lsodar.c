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

extern int C2F(lsodar) (int *f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, int *itask, int *istate, int *iopt, realtype *rwork, int *lrw, int *iwork, int *liw, int *jac, int *jtype, int *gfun, int *ng, int *jroot);

void lsodarCreate (int simblk, int *neq, realtype *yVec, realtype t0PlusEps, realtype t0, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int lrw, int *iwork, int liw, int Jacobians, int jtype, int grblk, int ng, int *jroot)
{
    C2F(lsodar) (&simblk, neq, yVec, &t0PlusEps, &t0, &itol, &reltol, &abstol, &itask, &istate, &iopt, rwork, &lrw, iwork, &liw, &Jacobians, &jtype, &grblk, &ng, jroot);
}

/* =============================
 *
 * 			lsodarSolve
 *
 * =============================
 *
 * Solving the problem
 */

void lsodarSolve (int simblk, int *neq, realtype *yVec, realtype t, realtype told, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int Lrw, int *iwork, int Liw, int Jacobians, int jtype, int grblk, int ng, int *jroot)
{
    C2F(lsodar) (&simblk, neq, yVec, &t, &told, &itol, &reltol, &abstol, &itask, &istate, &iopt, rwork, &Lrw, iwork, &Liw, &Jacobians, &jtype, &grblk, &ng, jroot);
}
