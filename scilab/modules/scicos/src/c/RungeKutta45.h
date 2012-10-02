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

#ifndef _RUNGEKUTTA45_H
#define _RUNGEKUTTA45_H

#include "sundials_extension.h"
#include "sundials/sundials_types.h" // Definition of types 'realtype' and 'booleantype'
#include "sundials/sundials_math.h"  // Various operations (ABS, MIN, RPowerI,...)
#include "nvector/nvector_serial.h"  // Type 'N_Vector'
#include "../scicos_sundials/src/cvode/cvode_impl.h" // Type 'CVodeMem'

// Creating the problem
void * RKCreate ();

// Allocating the problem
int RKMalloc (void * rk_mem, CVRhsFn f, realtype t0, N_Vector y0);

// Reinitializing the problem
int RKReInit (void * rk_mem, CVRhsFn f, realtype t0, N_Vector y0);

// Initializing the root-finding problem
int RKRootInit (void * rk_mem, int n_root_functions, CVRootFn g, void *gdata);

// Specifying the time beyond which the integration is not to proceed
int RKSetStopTime (void * rk_mem, realtype itstop);

// Specifying the maximum step size
int RKSetMaxStep (void * rk_mem, realtype hmax);

// Solving the problem
int RK (void * rk_mem, realtype t, N_Vector yout, realtype * told, int pass_tF_or_back);

// Freeing the problem memory allocated by RKMalloc
void RKFree (void ** rk_mem);

// Freeing the RK vectors allocated in RKAllocVectors
void RKFreeVectors (CVodeMem rk_mem);

// Allocating the CVODE vectors ewt, acor, tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
booleantype RKAllocVectors (CVodeMem rk_mem, N_Vector tmpl);

// Error handling function
void RKProcessError (CVodeMem rk_mem, int error_code, const char *mod, const char *fname, const char *msgfmt, ...);

// Default error handling function
void RKErrHandler (int error_code, const char *mod, const char *func, char *msg, void *data);

// Checking if all required vector operations are present
booleantype RKCheckNvector (N_Vector tmpl);

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
 *            Its form is (void) gfun (t, y, gt, g_data)
 *
 * nge      = cumulative counter for gfun calls.
 *
 * tlo, thi = endpoints of the interval in which roots are sought.
 *            On input, and must be distinct.
 *            The direction of integration is assumed to be from tlo to thi.
 *            On return, tlo and thi are the endpoints of the final relevant interval.
 *
 * glo, ghi = arrays of length nrtfn containing the vectors g(tlo) and g(thi) respectively.
 *            Input and output.
 *
 * iroots   = int array of length nrtfn with root information.
 *            Output only. If a root was found, iroots indicates
 *            which components g_i have a root at trout.
 *            For i = 0, ..., nrtfn-1, iroots[i] = 1 if g_i has a root
 *            and iroots[i] = 0 otherwise.
 *
 * Returning an int equal to:
 *      RTFOUND = 1 if a root of g was found, or
 *      CV_SUCCESS = 0 otherwise.
 */
int RKRootFind (CVodeMem cv_mem);

#endif
