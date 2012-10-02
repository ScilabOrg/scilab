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

typedef int (*CVRhsFnLSodar) (int * neq, realtype * t, realtype * y, realtype * rwork);
typedef int (*CVRootFnLSodar) (int * neq, realtype * t, realtype * y, int * ng, realtype * rwork);


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
    int iState;
    int iOpt;
    realtype * rwork;
    int lrw;
    int * iwork;
    int liw;
    int Jac;
    int Jactype;
    CVRootFn g_fun;
    int ng_fun;
    int * jroot;
} *LSodarMem;

// Creating the problem
void * LSodarCreate (int neq, int ng);

// Allocating the problem
int LSodarMalloc (void * cvode_mem, CVRhsFnLSodar f, realtype t0, N_Vector y0, int itol, realtype reltol, void *abstol);

// Specifying the maximum step size
int LSodarSetMaxStep (void * cvode_mem, realtype hmax);

// Reinitializing the problem
int LSodarReInit (void * cvode_mem, CVRhsFnLSodar f, realtype t0, N_Vector y, int itol, realtype reltol, void * abstol);

// Initializing the root-finding problem
int LSodarRootInit (void * cvode_mem, int ng, CVRootFnLSodar g, void *gdata);

// Solving the problem
int LSodar (void * cvode_mem, realtype tOut, N_Vector yVec, realtype * tOld, int itask);

// Update rootsfound to the computed jroots
int LSodarGetRootInfo (void * cvode_mem, int * rootsfound);

// Freeing the problem memory allocated by lsodarMalloc
void LSodarFree (void ** cvode_mem);

// Freeing the lsodar vectors allocated in lsodarAllocVectors
void LSFreeVectors (LSodarMem cvode_mem);

// Specifying the time beyond which the integration is not to proceed
int lsodarSetStopTime (LSodarMem cvode_mem, realtype itstop);

// Error handling function
void LSProcessError (LSodarMem cvode_mem, int error_code, const char * module, const char * fname, const char * msgfmt, ...);

// Default error handling function
void LSErrHandler (int error_code, const char * module, const char *function, char * msg, void * data);

#endif
