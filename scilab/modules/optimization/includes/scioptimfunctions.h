/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "dynlib_optimization.h"

// optim
typedef void (*costf_t)(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
typedef void (*fuclid_t)(int *n, double *x, double *y, double *ps, int *ti, float *tr, double *td);
typedef void (*ctonb_t)(int *n, double *u, double *v, int *ti, float *tr, double *td);
typedef void (*ctcab_t)(int *n, double *u, double *v, int *ti, float *tr, double *td);
extern void C2F(n1qn1)(costf_t, int*, double*, double*, double*, double*, double*, int*, int*, int*, int*, int*, double*, int*, float*, double*);
extern void C2F(n1qn3)(costf_t, fuclid_t, ctonb_t, ctcab_t, int*, double*, double*, double*, double*, double*, double*, int*, int*, int*, int*, int*, double*, int*, int*, float*, double*);
extern void C2F(n1fc1)(costf_t, fuclid_t, int*, double*, double*, double*, double*, double*, double*, double*, int*, int*, int*, int*, int*, int*, int*, double*, double*, int*, float*, double*);
extern void C2F(qnbd)(int*, costf_t, int*, double*, double*, double*, int*, int*, double*, int*, int*, double*, double*, double*, double*, double*, double*, int*, double*, int*, int*, int*, int*, float*, double*);
extern void C2F(gcbd)(int*, costf_t, char*, int*, double*, double*, double*, int*, int*, double*, int*, int*, double*, double*, double*, double*, double*, double*, int*, double*, int*, int*, int*, int*, float*, double*);
OPTIMIZATION_IMPEXP void costf(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
OPTIMIZATION_IMPEXP void C2F(fuclid)(int *n, double *x, double *y, double *ps, int *ti, float *tr, double *td);
OPTIMIZATION_IMPEXP void C2F(ctonb)(int *n, double *u, double *v, int *ti, float *tr, double *td);
OPTIMIZATION_IMPEXP void C2F(ctcab)(int *n, double *u, double *v, int *ti, float *tr, double *td);
