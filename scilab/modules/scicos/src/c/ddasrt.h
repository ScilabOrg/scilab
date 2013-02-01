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

#ifndef _DDASRT_H
#define _DDASRT_H

#include "sundials_extension.h"
#include "sundials/sundials_types.h" // Definition of types 'realtype' and 'booleantype'
#include "nvector/nvector_serial.h"  // Type 'N_Vector'
#include "../scicos_sundials/src/ida/ida_impl.h" // Error handling

#ifndef max
#define max(A,B) ((A>B) ? A:B)  // 'max()' function
#endif

#define MAXORD_DEFAULT 5

// realtype workspace
struct DDrWork_t
{
    realtype tcrit;
    realtype hmax;
    realtype rwork3;
    realtype rwork4;
    realtype h0;
    realtype rwork6;
    realtype hmin;
    realtype rwork[1];
};

// Derivative computation and Root functions
typedef int (*DDASResFn) (int *neq, realtype *t, realtype *y, realtype *resval, realtype *rwork, void *rdata);
typedef int (*DDASRootFn) (int *neq, realtype *t, realtype *y, realtype *yp, int *ng, realtype *rwork);
typedef void (*DDASErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *user_data);

enum DDiTask_t
{
    DDAS_NORMAL = 1,
    DDAS_ONE_STEP = 2,
    DDAS_MESH_STEP = 3,
    DDAS_NORMAL_TSTOP = 4,
    DDAS_ONE_STEP_TSTOP = 5
};

// DDasrt problem memory structure
typedef struct DDasrtMemRec
{
    DDASResFn res;
    int * nEquations;
    realtype * yVector;
    realtype * yPrimeVector;
    realtype tStart;
    realtype tEnd;
    realtype relTol;
    realtype absTol;
    int iState;
    int info[15];
    struct DDrWork_t * rwork;
    int lrw;
    int * iwork;
    int liw;
    int * jacobian;
    DDASRootFn g_fun;
    int ng_fun;
    int * jroot;
    DDASErrHandlerFn ehfun;
    void * user_data;
} *DDasrtMem;

// Creating the problem
void * DDasrtCreate (int * neq, int ng);

// Allocating the problem
int DDasrtInit (void * ddasrt_mem, DDASResFn f, realtype t0, N_Vector y, N_Vector yp);

// Reinitializing the problem
int DDasrtReInit (void * ddasrt_mem, realtype tOld, N_Vector y, N_Vector yp);

// Specifying the tolerances
int DDasrtSStolerances(void *cvode_mem, realtype reltol, realtype abstol);

// Initializing the root-finding problem
int DDasrtRootInit (void * ddasrt_mem, int ng, DDASRootFn g);

// Specifying the maximum step size
int DDasrtSetMaxStep (void * ddasrt_mem, realtype hmax);

// Specifying the time beyond which the integration is not to proceed
int DDasrtSetStopTime (void * ddasrt_mem, realtype tcrit);

// Specifying the Jacobian functions
//int DDasrtSetDenseJacFn(void * ddasrt_mem, DDasrtDlsDenseJacFn Jacobians);
// don't forget info[4] = 1 in the .c

// Solving the problem
int DDasrt (void * ddasrt_mem, realtype tOut, N_Vector yVec, realtype * tOld, enum DDiTask_t itask);

// Update rootsfound to the computed jroots
int DDasrtGetRootInfo (void * ddasrt_mem, int * rootsfound);

// Freeing the problem memory allocated by ddasrtMalloc
void DDasrtFree (void ** ddasrt_mem);

// Freeing the ddasrt vectors allocated in ddasrtAllocVectors
void DDASFreeVectors (DDasrtMem ddasrt_mem);

// Specifies the error handler function
int DDasrtSetErrHandlerFn (void * ddasrt_mem, DDASErrHandlerFn ehfun, void * eh_data);

// Error handling function
void DDASProcessError (DDasrtMem ddas_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

#endif

