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

#ifndef _LSODAR_H
#define _LSODAR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include <sundials_extension.h>
#include <sundials/sundials_types.h> // Definition of types 'realtype' and 'booleantype'
#include <sundials/sundials_math.h>  // Various operations (ABS, MIN, RPowerI,...)
#include <nvector/nvector_serial.h>  // Type 'N_Vector'
#include "../scicos_sundials/src/cvode/cvode_impl.h" // Type 'CVodeMem'

typedef int (*CVRhsFnLSodar) (int *neq, realtype * t, realtype * y, realtype * rwork);


typedef struct LSodarMemRec
{
    CVRhsFnLSodar func;
    int * nEquations;
    realtype * yVector;
    realtype tStart;
    realtype tEnd;
    int iTol;
    realtype relTol;
    realtype absTol;
    int iTask;
    int iState;
    int iOpt;
    realtype * rWork;
    int Lrw;
    int * iWork;
    int Liw;
    int Jac;
    int Jactype;
    CVRootFn g_fun;
    int ng_fun;
    int * jRoot;
} *LSodarMem;

// Creating the problem
void * LSodarCreate (CVRhsFnLSodar f, int *neq, N_Vector y, int itol, realtype reltol, realtype abstol, int itask, CVRootFn grblk, int ng, int *jroot);

// Specifying the maximum step size
int lsodarSetMaxStep (void *cvode_mem, realtype hmax);

// Allocating the problem
int lsodarMalloc (void* cvode_mem, CVRhsFn f, realtype t0, N_Vector y0);

// Reinitializing the problem
int lsodarReInit (void* cvode_mem, CVRhsFn f, realtype t0, N_Vector y0);

// Initializing the root-finding problem
int lsodarRootInit (void* cvode_mem, int n_root_functions, CVRootFn g, void *gdata);

// Solving the problem
int LSodarSolve (void * cvode_mem, int *neq, N_Vector yVec, realtype t, realtype tout, int itask, int * jroot);

// Freeing the problem memory allocated by lsodarMalloc
void lsodarFree (void** cvode_mem);

// Freeing the lsodar vectors allocated in lsodarAllocVectors
void lsodarFreeVectors (CVodeMem cvode_mem);

// Allocating the CVODE vectors ewt, acor, tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
booleantype lsodarAllocVectors (CVodeMem cvode_mem, N_Vector tmpl);

// Specifying the time beyond which the integration is not to proceed
int lsodarSetStopTime (CVodeMem cvode_mem, realtype itstop);

// Error handling function
void lsodarProcessError (CVodeMem cvode_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

// Default error handling function
void lsodarErrHandler (int error_code, const char *module, const char *function, char *msg, void *data);

// Checking if all required vector operations are present
booleantype lsodarCheckNvector (N_Vector tmpl);

/* Testing g(t) for a root between tlo and thi, if one exists.
 * Only roots of odd multiplicity (i.e. with a change
 * of sign in one of the g_i), or exact zeros, are found.
 * Here the sign of tlo - thi is arbitrary, but if multiple roots
 * are found, the one closest to tlo is returned.
 *
 * This routine uses the following parameters for communication :
 *
 * nrtfn    = number of functions g_i, or number of components of
 *            the vector-valued function g(t). Input only.
 *
 * gfun     = user-defined function for g(t).
 * 			  Its form is (void) gfun (t, y, gt, g_data)
 *
 * nge      = cumulative counter for gfun calls.
 *
 * tlo, thi = endpoints of the interval in which roots are sought.
 *            On input, and must be distinct.
 * 			  The direction of integration is assumed to be from tlo to thi.
 * 			  On return, tlo and thi are the endpoints of the final relevant interval.
 *
 * glo, ghi = arrays of length nrtfn containing the vectors g(tlo) and g(thi) respectively.
 * 			  Input and output.
 *
 * iroots   = int array of length nrtfn with root information.
 *            Output only. If a root was found, iroots indicates
 *            which components g_i have a root at trout.
 * 			  For i = 0, ..., nrtfn-1, iroots[i] = 1 if g_i has a root
 *            and iroots[i] = 0 otherwise.
 *
 * Returning an int equal to:
 *      RTFOUND = 1 if a root of g was found, or
 *      CV_SUCCESS = 0 otherwise.
 */
int lsodarRootFind (CVodeMem cvode_mem);

#endif
