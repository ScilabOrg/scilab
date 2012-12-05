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

#ifndef _RUNGEKUTTAFEHLBERG_H
#define _RUNGEKUTTAFEHLBERG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <machine.h>

#include <sundials_extension.h>
#include <sundials/sundials_types.h> // Definition of types 'realtype' and 'booleantype'
//#include <sundials/sundials_math.h>  // Various operations (ABS, MIN, RPowerI,...)
#include <nvector/nvector_serial.h>  // Type 'N_Vector'
#include "../scicos_sundials/src/cvode/cvode_impl.h" // Error handling

#define max(A,B) ((A>B) ? A:B)  // 'max()' function

typedef int (*RKFRhsFn) (int * neq, realtype * t, realtype * y, realtype * rwork);
typedef int (*RKFRootFn) (int * neq, realtype * t, realtype * y, int * ng, realtype * rwork);

// Potential tasks
enum iTask_t
{
    RKF_NORMAL = 1,
    RKF_ONE_STEP = 2
};

// realtype workspace
struct rWork_t
{
    realtype work[neq];
    realtype h_attempt;
};

// RKF problem memory structure
typedef struct RKFMemRec
{
    RKFRhsFn func;
    int * nEquations;
    realtype * yVector;
    realtype tStart;
    realtype tEnd;
    int iTol;
    realtype relTol;
    realtype absTol;
    int iState;
    int iOpt;
    struct rWork_t * rwork;
    int lrw;
    int * iwork;
    int liw;
    int jacobian;
    int jacType;
    RKFRootFn g_fun;
    int ng_fun;
    int * jroot;
} *RKFMem;

// Creating the problem
void * RKFCreate (int * neq, int ng);

// Allocating the problem
int RKFMalloc (void * rkf_mem, RKFRhsFn f, realtype t0, N_Vector y, int itol, realtype reltol, void * abstol);

// Reinitializing the problem
int RKFReInit (void * rkf_mem, RKFRhsFn f, realtype tOld, N_Vector y, int itol, realtype reltol, void * abstol);

// Initializing the root-finding problem
int RKFRootInit (void * rkf_mem, int ng, RKFRootFn g, void *gdata);

// Specifying the maximum step size
int RKFSetMaxStep (void * rkf_mem, realtype hmax);

// Specifying the time beyond which the integration is not to proceed
int RKFSetStopTime (void * rkf_mem, realtype tcrit);

// Solving the problem
int RKF (void * rkf_mem, realtype tOut, N_Vector yVec, realtype * tOld, enum iTask_t itask);

// Update rootsfound to the computed jroots
int RKFGetRootInfo (void * rkf_mem, int * rootsfound);

// Freeing the problem memory allocated by RKFMalloc
void RKFFree (void ** rkf_mem);

// Freeing the rkf vectors allocated in RKFAllocVectors
void RKFFreeVectors (RKFMem rkf_mem);

// Error handling function
void RKFProcessError (RKFMem rkf_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

// Default error handling function
void RKFErrHandler (int error_code, const char *module, const char *function, char *msg, void *data);

#endif

