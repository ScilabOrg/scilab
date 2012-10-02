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

#if 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "../scicos_sundials/src/cvode/cvode_impl.h"
#include "../scicos_sundials/include/cvode/cvode.h"

#include <sundials_extension.h>
#include <sundials/sundials_types.h> // Definition of type realtype
#include <sundials/sundials_math.h>  // Various operations (ABS, MIN, RPowerI,...)

#include <nvector/nvector_serial.h>  // Definition of N_Vectors

#endif

// Creating the problem
void* RKCreate (int mode_integ, int mode_resol);

// Allocating the problem
int RKMalloc (void* rk_mem, CVRhsFn f, realtype t0, N_Vector y0, int itol, realtype reltol, void *abstol);

// Reinitializing the problem
int RKReInit (void* rk_mem, CVRhsFn f, realtype t0, N_Vector y0, int itol, realtype reltol, void *abstol);

// Initializing the root-finding problem
int RKRootInit (void* rk_mem, int n_root_functions, CVRootFn g, void *gdata);

// Solving the problem
int RK (void* rk_mem, realtype t, N_Vector yout, realtype *told, int pass_tF_or_back);

// Freeing the problem memory allocated by RKMalloc
void RKFree (void** rkutta_mem);

// Freeing the RK vectors allocated in RKAllocVectors
static void RKFreeVectors (CVodeMem rk_mem);

// Allocating the CVODE vectors ewt, acor, tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
static booleantype RKAllocVectors (CVodeMem rk_mem, N_Vector tmpl, int tol);

/* Setting the error weight vector ewt according to tol_type as follows:
 *
 *  (1) ewt[i] = 1 / (reltol * ABS(ycur[i]) + *abstol), i=0,...,neq-1
 *     if tol_type = CV_SS
 *  (2) ewt[i] = 1 / (reltol * ABS(ycur[i]) + abstol[i]), i=0,...,neq-1
 *     if tol_type = CV_SV */
int RKEwtSet (N_Vector ycur, N_Vector weight, void* data);

static int RKEwtSetSS (CVodeMem rk_mem, N_Vector ycur, N_Vector weight);

static int RKEwtSetSV (CVodeMem rk_mem, N_Vector ycur, N_Vector weight);

// Specifying the time beyond which the integration is not to proceed
int RKSetStopTime (CVodeMem rkutta_mem, realtype itstop);

// Checking input consistency
static int RKInitialSetup (CVodeMem rk_mem);

/* Performing various update operations when the solution to the nonlinear system has passed the local error test. 
 * Incrementing the step counter nSolCalls, recording the values hu and qu, updating the tau array, and applying the corrections to the zn array.
 * The tau[i] are the last q values of h, with tau[1] the most recent.
 * Decrementing qwait, and if qwait == 1 (and q < qmax), saving acor and tq[5] for a possible order increase.
 */
static void RKCompleteStep (CVodeMem rk_mem);

/* Rescaling the Nordsieck array by multiplying the jth column zn[j] by eta^j, j = 1, ..., q.
 * Then rescaling h by eta, and resetting hscale to h.
 */
static void RKRescale (CVodeMem rk_mem);

// Adjusting the history array on a change of order q by deltaq, in the case that mode_integ == RK_ADAMS.
static void RKAdjustAdams (CVodeMem rk_mem, int deltaq);

/* Handling adjustments to the history array on a change of order by deltaq when mode_integ == CV_BDF.
 * Calling RKIncreaseBDF if deltaq = +1 and RKDecreaseBDF if deltaq = -1 to do the actual work.
 */
static void RKAdjustBDF (CVodeMem rk_mem, int deltaq);

/* Adjusting the history array on an increase in the order q in the case that mode_integ == RK_BDF.  
 * Setting a new column zn[q+1] equal to a multiple of the saved vector (= acor) in zn[indx_acor].
 * Then adjusting each zn[j] by a multiple of zn[q+1].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h.
 */
static void RKIncreaseBDF (CVodeMem rk_mem);

/* Adjusting the history array on a decrease in the order q when mode_integ == RK_BDF.  
 * Adjusting each zn[j] by a multiple of zn[q].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h.
 */
static void RKDecreaseBDF (CVodeMem rk_mem);

/* Handling an order change by a deltaq (= +1 or -1) amount.
 * If a decrease in order is requested and q==2, then returning the routine immediately.
 * Otherwise, calling RKAdjustAdams or RKAdjustBDF to handle the order change (depending on the value of mode_integ).
 */
static void RKAdjustOrder (CVodeMem rk_mem, int deltaq);

/* Performing the local error test. 
 * Loading the weighted local error norm dsm into *dsmPtr, and testing dsm ?<= 1.
 *
 * If the test passes, returning RK_SUCCESS. 
 * If the test fails, undoing the step just taken (calling RKRestore) and 
 *   - if maxnef error test failures have occurred or if ABS(h) = hmin,
 *     returning RK_ERR_FAILURE.
 *   - if more than MXNEF1 error test failures have occurred, forcing an order reduction.
 * 	   If already at order 1, restarting by reloading zn from scratch.
 *     If f() fails, returning either RK_RHSFUNC_FAIL or RK_UNREC_RHSFUNC_ERR (no recovery is possible at this stage).
 *   - otherwise, setting *nflagPtr to PREV_ERR_FAIL, and returning TRY_AGAIN. 
 */
static booleantype RKDoErrorTest (CVodeMem rk_mem, int *nflagPtr, realtype saved_t, int *nefPtr, realtype *dsmPtr);

/* Restoring the value of tn to saved_t and undoing the prediction.
 * After execution of RKRestore, the Nordsieck array zn has the same values as before the call to RKPredict.
 */
static void RKRestore (CVodeMem rk_mem, realtype saved_t);

/* Calling RKSetAdams or RKSetBDF to set the polynomial l, the test quantity array tq, 
 * and the related variables rl1, gamma and gamrat.
 */
static void RKSet (CVodeMem rk_mem);

/* Performing one internal RK step, from tn to tn + h, calling other routines to do the work.
 *
 * The main operations done here are as follows:
 * - preliminary adjustments if a new step size was chosen;
 * - prediction of the Nordsieck history array zn at tn + h;
 * - setting of multistep method coefficients and test quantities;
 * - solution of the nonlinear system;
 * - testing the local error;
 * - updating zn and other state data if successful;
 * - resetting stepsize and order for the next step.
 * - if SLDET is on, check for stability, reduce order if necessary.
 * On a failure in the nonlinear system solution or error test, the
 * step may be reattempted, depending on the nature of the failure.
 */
static int RKStep (CVodeMem rk_mem);

/* Handling the computation of l and tq when mode_integ == RK_ADAMS.
 * The components of the array l are the coefficients of a polynomial
 *   Lambda(x) = l_0 + l_1 x + ... + l_q x^q, given by q-1
 *   (d/dx) Lambda(x) = c * PRODUCT (1 + x / xi_i) , where i=1
 *   Lambda(-1) = 0, Lambda(0) = 1, and c is a normalization factor.
 * Here, xi_i = [t_n - t_(n-i)] / h.
 *
 * Setting the array tq to test quantities used in the convergence test, the error test,
 * and the selection of h at a new order.
 */
static void RKSetAdams (CVodeMem rk_mem);

// Generating in m[] the coefficients of the product polynomial needed for the Adams l and tq coefficients for q > 1.
static realtype RKAdamsStart (CVodeMem rk_mem, realtype m[]);

/* Returning the value of the alternating sum (i= 0 ... iend) [ (-1)^i * (a[i] / (i + k)) ].
 * If iend < 0 then returning 0.
 * This operation is needed to compute the integral, from -1 to 0,
 * of a polynomial x^(k-1) M(x) given the coefficients of M(x).
 */
static realtype RKAltSum (int iend, realtype a[], int k);

// Completing the calculation of the Adams l and tq.
static void RKAdamsFinish (CVodeMem rk_mem, realtype m[], realtype M[], realtype hsum);

/* Computing the coefficients l and tq when mode_integ == RK_BDF.
 * 
 * Calling RKSetTqBDF to set the test quantity array tq. 
 * 
 * The components of the array l are the coefficients of a
 * polynomial Lambda(x) = l_0 + l_1 x + ... + l_q x^q, given by q-1
 *   Lambda(x) = (1 + x / xi*_q) * PRODUCT (1 + x / xi_i) , where i=1
 *   xi_i = [t_n - t_(n-i)] / h.
 *
 * Setting the array tq to test quantities used in the convergence test, the error test,
 * and the selection of h at a new order.
 */
static void RKSetBDF (CVodeMem rk_mem);

// Setting the test quantity array tq when mode_integ == RK_BDF
static void RKSetTqBDF (CVodeMem rk_mem, realtype hsum, realtype alpha0, realtype alpha0_hat, realtype  xi_inv, realtype xistar_inv);

/* Performing the Newton iteration. If the iteration succeeds, returning RK_SUCCESS.
 * If not, it may signal the RKNlsNewton routine to call lsetup again and reattempt the iteration, by
 * returning the value TRY_AGAIN (in this case, RKNlsNewton must set convfail to CV_FAIL_BAD_J before calling setup again).
 * Otherwise, returning one of the appropriate values CV_LSOLVE_FAIL, CV_RHSFUNC_FAIL, CONV_FAIL,
 * or RHSFUNC_RECVR back to RKNlsNewton.
 */
static int RKNewtonIteration (CVodeMem rk_mem);

/* Computing the k-th derivative of the interpolating polynomial at the time t and stores the result in the vector dky.
 * The formula is:
 * 
 *         q 
 *  dky = SUM c(j,k) * (t - tn)^(j-k) * h^(-j) * zn[j], 
 *        j=k
 * 
 * where c(j,k) = j*(j-1)*...*(j-k+1), q is the current order, and
 * zn[j] is the j-th column of the Nordsieck history array.
 *
 * Called by RK with k = 0 and t = t, but may also be called directly by the user.
 */
int RKGetDky (void *rkutta_mem, realtype t, int k, N_Vector dky);

// Error handling function
void RKProcessError (CVodeMem rk_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

// Default error handling function
void RKErrHandler (int error_code, const char *module, const char *function, char *msg, void *data);

// Checking if all required vector operations are present
static booleantype RKCheckNvector (N_Vector tmpl);

/* Printing error messages for all cases of failure by RKHin and RKStep.
 * Returning to RK the value that RK is to return to the user.
 */
static int RKHandleFailure (CVodeMem rk_mem, int flag);

/* Taking action on the return value nflag = *nflagPtr returned by RKNls, as follows:
 *
 * If RKNls succeeded in solving the nonlinear system, then returning the constant DO_ERROR_TEST, which tells RKStep to perform the error test.
 *
 * If the nonlinear system was not solved successfully, then ncfn and ncf = *ncfPtr are incremented and Nordsieck array zn is restored.
 *
 * If the solution of the nonlinear system failed due to an unrecoverable failure by setup, returning the value RK_LSETUP_FAIL.
 * 
 * If it failed due to an unrecoverable failure in solve, then returning the value RK_LSOLVE_FAIL.
 *
 * If it failed due to an unrecoverable failure in rhs, then returning the value RK_RHSFUNC_FAIL.
 *
 * Otherwise, a recoverable failure occurred when solving the nonlinear system (RKNls returned nflag == CONV_FAIL or RHSFUNC_RECVR).
 * In this case, if ncf is now equal to maxncf or |h| = hmin, returning the value RK_CONV_FAILURE (if nflag=CONV_FAIL) or
 * RK_REPTD_RHSFUNC_ERR (if nflag = RHSFUNC_RECVR).
 * If not, setting *nflagPtr = PREV_CONV_FAIL and returning the value PREDICT_AGAIN, telling RKStep to reattempt the step.
 */
static int RKHandleNFlag (CVodeMem rk_mem, int *nflagPtr, realtype saved_t, int *ncfPtr);

/* Attempting to solve the nonlinear system associated with a single implicit step of the linear multistep method.
 * Depending on mode_iter, it calls RKNlsFunctional or RKNlsNewton to do the work.
 */
static int RKNls (CVodeMem rk_mem, int nflag);

static int RKRcheck1 (CVodeMem cv_mem);

/* Completing the initialization of rootfinding memory information,
 * and checking whether g has a zero both at and very near the initial point of the IVP.
 *
 * Returning an int equal to :
 *  INITROOT = -1 if a close pair of zeros was found, and
 *  CV_SUCCESS = 0 otherwise.
 */
static int RKRcheck1Std (CVodeMem cv_mem);

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
static int RKRootFind (CVodeMem cv_mem);

static int RKRootfindExt (CVodeMem cv_mem);

#endif
