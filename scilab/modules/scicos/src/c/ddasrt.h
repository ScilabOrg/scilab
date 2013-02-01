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

/* By default, we set the maximum order to 5 */
#define MAXORD_DEFAULT 5

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

// Derivative computation and Root functions
typedef int (*DDASResFn) (realtype *tOld, realtype *y, realtype *yp, realtype *res, int *flag, double *dummy1, int *dummy2);
typedef int (*DDASRootFn) (int *neq, realtype *tOld, realtype *y, int *ng, realtype *groot, double *dummy1, int *dummy2);
typedef void (*DDASErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *user_data);

enum DDiTask_t
{
    DDAS_NORMAL = 0,
    DDAS_ONE_STEP = 1,
};

// DDasrt problem memory structure
typedef struct DDasrtMemRec
{
    DDASResFn res;
    int * nEquations;
    void * user_data;
    realtype tStart;
    realtype relTol;
    realtype absTol;
    realtype * yVector;
    realtype * yPrimeVector;
    int iState;
    int * info;
    struct DDrWork_t * rwork;
    int lrw;
    int * iwork;
    int liw;
    int * jacobian;
    int maxord;
    DDASErrHandlerFn ehfun;
    DDASRootFn g_fun;
    int ng_fun;
    int * jroot;
} *DDasrtMem;

// Creating the problem
void * DDasrtCreate (int * neq, int ng);

// Allocating the problem
int DDasrtInit (void * ddasrt_mem, DDASResFn Res, realtype t0, N_Vector yy0, N_Vector yp0);

// Reinitializing the problem
int DDasrtReInit (void * ddasrt_mem, realtype tOld, N_Vector yy0, N_Vector yp0);

// Specifying the tolerances
int DDasrtSStolerances (void *cvode_mem, realtype reltol, realtype abstol);

// Initializing the root-finding problem
int DDasrtRootInit (void * ddasrt_mem, int ng, DDASRootFn g);

// Setting a pointer to user_data that will be passed to the user's res function every time a user-supplied function is called
int DDasrtSetUserData (void * ddasrt_mem, void * User_data);

// Specifying the maximum step size
int DDasrtSetMaxStep (void * ddasrt_mem, realtype hmax);

// Specifying the time beyond which the integration is not to proceed
int DDasrtSetStopTime (void * ddasrt_mem, realtype tcrit);

// Specifying the Jacobian functions
//int DDasrtSetDenseJacFn (void * ddasrt_mem, DDasrtDlsDenseJacFn Jacobians);

// Solving the problem
int DDasrtSolve (void * ddasrt_mem, realtype tOut, realtype * tOld, N_Vector yOut, N_Vector ypOut, enum DDiTask_t itask);

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

