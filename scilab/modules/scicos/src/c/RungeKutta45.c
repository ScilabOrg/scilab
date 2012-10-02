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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "RungeKutta45.h"

#ifndef RTFOUND
#define RTFOUND        1
#endif
#ifndef INITROOT
#define INITROOT       2
#endif
#ifndef ZERODETACHING
#define ZERODETACHING  4
#endif

/* =============================
 *
 *           RKCreate
 *
 * =============================
 *
 * Creates the problem
 */

void * RKCreate ()
{
    int maxord;

    // Setting the workspace memory
    CVodeMem cv_mem;
    cv_mem = NULL;
    cv_mem = (CVodeMem) malloc(sizeof (struct CVodeMemRec));
    if (cv_mem == NULL)
    {
        RKProcessError (NULL, 0., "Runge-Kutta", "RKCreate", MSGCV_CVMEM_FAIL);
        return (NULL);
    }

    maxord = 1;

    // Setting default values
    cv_mem->cv_f        = NULL;
    cv_mem->cv_f_data   = NULL;
    cv_mem->cv_qmax     = maxord;
    cv_mem->cv_mxstep   = 1;
    cv_mem->cv_tstopset = FALSE;

    // Initializing root-finding variables
    cv_mem->cv_glo    = NULL;
    cv_mem->cv_ghi    = NULL;
    cv_mem->cv_iroots = NULL;
    cv_mem->cv_gfun   = NULL;
    cv_mem->cv_g_data = NULL;
    cv_mem->cv_nrtfn  = 0;

    // Initializing lrw and liw
    cv_mem->cv_lrw = 58 + 2 * L_MAX + NUM_TESTS;
    cv_mem->cv_liw = 40;

    // No mallocs have been done yet
    cv_mem->cv_MallocDone = FALSE;

    // Returning a pointer to the RK memory block */
    return ((void *) cv_mem);
}

#define lrw       cv_mem->cv_lrw
#define liw       cv_mem->cv_liw

/* =============================
 *
 *           RKMalloc
 *
 * =============================
 *
 * Allocates the problem
 */

int RKMalloc (void * cvode_mem, CVRhsFn f, realtype t0, N_Vector y0)
{
    booleantype nvectorOK, allocOK;
    long int lrw1, liw1;

    // Step 1 : checking the arguments
    CVodeMem cv_mem;
    if (cvode_mem == NULL)
    {
        RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKMalloc", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    cv_mem = (CVodeMem) cvode_mem;

    if (y0 == NULL)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_NULL_Y0);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_NULL_Y0);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    if (f == NULL)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_NULL_F);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_NULL_F);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    // Test if all required vector operations are implemented
    nvectorOK = RKCheckNvector (y0);
    if (!nvectorOK)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_BAD_NVECTOR);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_BAD_NVECTOR);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    if (y0->ops->nvspace != NULL)
    {
        N_VSpace (y0, &lrw1, &liw1);
    }
    else
    {
        lrw1 = 0;
        liw1 = 0;
    }
    cv_mem->cv_lrw1 = lrw1;
    cv_mem->cv_liw1 = liw1;

    // Step 2 : function body

    // Allocating the vectors, taking y0 as a template
    allocOK = RKAllocVectors (cv_mem, y0);
    if (!allocOK)
    {
        RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKMalloc", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    // Copying the input parameters into RK memory block
    cv_mem->cv_f   = f;
    cv_mem->cv_tn  = t0;
    cv_mem->cv_tlo = t0;

    // Initializing zn[0] in the history array
    N_VScale(1., y0, cv_mem->cv_zn[0]);

    // Initializing all the counters
    cv_mem->cv_nst     = 0;
    cv_mem->cv_nge     = 0;

    // The problem has been successfully initialized
    cv_mem->cv_MallocDone = TRUE;
    return (CV_SUCCESS);
}

#define lrw1      cv_mem->cv_lrw1
#define liw1      cv_mem->cv_liw1

/* =============================
 *
 *          RKReInit
 *
 * =============================
 *
 * Reinitializes the problem
 */

int RKReInit (void * cvode_mem, CVRhsFn f, realtype t0, N_Vector y0)
{
    // Step 1 : checking the arguments

    // Checking cvode_mem
    CVodeMem cv_mem;
    if (cvode_mem == NULL)
    {
        RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    cv_mem = (CVodeMem) cvode_mem;

    // Checking if cv_mem was allocated
    if (cv_mem->cv_MallocDone == FALSE)
    {
        RKProcessError (cv_mem, CV_NO_MALLOC, "Runga-Kutta", "RKReInit", MSGCV_NO_MALLOC);
        return (CV_NO_MALLOC);
    }

    // Checking the other arguments
    if (y0 == NULL)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_NULL_Y0);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_NULL_Y0);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    if (f == NULL)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_NULL_F);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_NULL_F);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    // Step 2 : function body

    // Recopying the input parameters into RK memory block
    cv_mem->cv_f  = f;
    cv_mem->cv_tn = t0;

    // Reinitializing zn[0] in the history array
    N_VScale(1., y0, cv_mem->cv_zn[0]);

    // Reinitializing all the counters
    cv_mem->cv_nst = 0;
    cv_mem->cv_nge = 0;

    // The problem has been successfully re-initialized
    return (CV_SUCCESS);
}

#define gfun      cv_mem->cv_gfun
#define g_data    cv_mem->cv_g_data
#define glo       cv_mem->cv_glo
#define ghi       cv_mem->cv_ghi
#define iroots    cv_mem->cv_iroots
#define tlo       cv_mem->cv_tlo
#define zn        cv_mem->cv_zn

/* =============================
 *
 *          RKRootInit
 *
 * =============================
 *
 * Initializes the Root Finding problem
 */

int RKRootInit (void * cvode_mem, int nrtfn, CVRootFn g, void *gdata)
{
    int nrt, retval;

    // Checking cvode_mem
    if (cvode_mem == NULL)
    {
        RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKRootInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }

    CVodeMem cv_mem;
    cv_mem = (CVodeMem) cvode_mem;

    nrt = (nrtfn < 0) ? 0 : nrtfn;

    /* If rerunning RKRootInit() with a different number of root functions (changing number of gfun components),
     * then freeing currently held memory resources */
    if ((nrt != cv_mem->cv_nrtfn) && (cv_mem->cv_nrtfn > 0))
    {
        free(glo);
        glo = NULL;
        free(ghi);
        ghi = NULL;
        free(iroots);
        iroots = NULL;

        lrw -= 3 * (cv_mem->cv_nrtfn);
        liw -= cv_mem->cv_nrtfn;
    }

    /* If RKRootInit() was called with nrtfn == 0, then setting cv_nrtfn to
     * zero and cv_gfun to NULL before returning */
    if (nrt == 0)
    {
        cv_mem->cv_nrtfn = nrt;
        gfun = NULL;
        g_data = NULL;
        return (CV_SUCCESS);
    }

    // Storing user's data pointer
    g_data = gdata;

    /* If rerunning RKRootInit() with the same number of root functions (not changing number of gfun components),
     * then checking if the root function argument has changed.
     * If g != NULL then returning as currently reserved memory resources will suffice */
    if (nrt == cv_mem->cv_nrtfn)
    {
        if (g != gfun)
        {
            if (g == NULL)
            {
                free(glo);
                glo = NULL;
                free(ghi);
                ghi = NULL;
                free(iroots);
                iroots = NULL;
                lrw -= 3 * nrt;
                liw -= nrt;
                RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKRootInit", MSGCV_NULL_G);
                /* SUNDIALS EXTENSION */
                if (is_sundials_with_extension())
                {
                    return (CV_NULL_G);
                }
                else
                {
                    return (CV_ILL_INPUT);
                }
            }
            else
            {
                gfun = g;
                return (CV_SUCCESS);
            }
        }
        else
        {
            return (CV_SUCCESS);
        }
    }

    // Step 2 : function body

    // Setting variables in RK memory block
    cv_mem->cv_nrtfn = nrt;
    if (g == NULL)
    {
        RKProcessError(cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKRootInit", MSGCV_NULL_G);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_NULL_G);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }
    else
    {
        gfun = g;
    }

    // Allocating necessary memory and returning
    glo = NULL;
    glo = (realtype *) malloc(nrt * sizeof(realtype));
    if (glo == NULL)
    {
        RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    // Initializing glo
    retval = gfun (tlo, zn[0], glo, g_data);
    if (retval != 0)
    {
        return (CV_RTFUNC_FAIL);
    }

    ghi = NULL;
    ghi = (realtype *) malloc(nrt * sizeof(realtype));
    if (ghi == NULL)
    {
        free(glo);
        glo = NULL;
        RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    iroots = NULL;
    iroots = (int *) malloc(nrt * sizeof(int));
    if (iroots == NULL)
    {
        free(glo);
        glo = NULL;
        free(ghi);
        ghi = NULL;
        RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    lrw += 3 * nrt;
    liw += nrt;

    return (CV_SUCCESS);
}

#define hmax_inv  cv_mem->cv_hmax_inv

/* =============================
 *
 *          RKSetMaxStep
 *
 * =============================
 *
 * Specifies the maximum step size
 */

int RKSetMaxStep (void * cvode_mem, realtype hmax)
{
    CVodeMem cv_mem;
    if (cvode_mem == NULL)
    {
        CVProcessError(NULL, CV_MEM_NULL, "CVODE", "CVodeSetMaxStep", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    cv_mem = (CVodeMem) cvode_mem;

    if (hmax <= 0)
    {
        RKProcessError(cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKSetMaxStep", MSGCV_NEG_HMAX);
        return (CV_ILL_INPUT);
    }

    // Actually stock the inverse of the maximum step size
    hmax_inv = 1. / hmax;

    return (CV_SUCCESS);
}

#define tstop     cv_mem->cv_tstop
#define tstopset  cv_mem->cv_tstopset

// Specifies the time beyond which the integration is not to proceed
int RKSetStopTime (void * cvode_mem, realtype ttstop)
{
    CVodeMem cv_mem;
    if (cvode_mem == NULL)
    {
        RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKSetStopTime", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    cv_mem = (CVodeMem) cvode_mem;

    tstop = ttstop;
    tstopset = TRUE;

    return (CV_SUCCESS);
}

/* =============================
 * Readibility Constants
 * =============================
 */
#define f         cv_mem->cv_f
#define f_data    cv_mem->cv_f_data
#define qmax      cv_mem->cv_qmax
#define mxstep    cv_mem->cv_mxstep
#define istop     cv_mem->cv_istop
#define tstop     cv_mem->cv_tstop
#define tstopset  cv_mem->cv_tstopset

#define y         cv_mem->cv_y
#define tempv     cv_mem->cv_tempv
#define ftemp     cv_mem->cv_ftemp
#define h         cv_mem->cv_h
#define tn        cv_mem->cv_tn
#define nst       cv_mem->cv_nst

#define nrtfn     cv_mem->cv_nrtfn
#define thi       cv_mem->cv_thi
#define tretlast  cv_mem->cv_tretlast
#define toutc     cv_mem->cv_toutc
#define taskc     cv_mem->cv_taskc
#define irfnd     cv_mem->cv_irfnd
#define nge       cv_mem->cv_nge

/* =============================
 *
 *              RK
 *
 * =============================
 *
 * Solves the problem
 */

int RK (void * cvode_mem, realtype t, N_Vector yout, realtype * told, int pass_tF_or_back)
{
    // Initializing some variables
    int i, j, retval, task, floor_test;
    long int n_points;
    realtype interval_size, hmax, test_div;

    // Step 1 : checking the arguments

    // Checking cvode_mem
    CVodeMem cv_mem;
    if (cvode_mem == NULL)
    {
        RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RK", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    cv_mem = (CVodeMem) cvode_mem;

    // Checking cv_mem was allocated
    if (cv_mem->cv_MallocDone == FALSE)
    {
        RKProcessError (cv_mem, CV_NO_MALLOC, "Runge-Kutta", "RK", MSGCV_NO_MALLOC);
        return (CV_NO_MALLOC);
    }

    // Checking yout
    if (yout == NULL)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_YOUT_NULL);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_YOUT_NULL);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    // Checking told
    if (told == NULL)
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_TRET_NULL);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_TRET_NULL);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    // Checking itask
    if ((pass_tF_or_back != CV_NORMAL)       &&
            (pass_tF_or_back != CV_ONE_STEP)     &&
            (pass_tF_or_back != CV_NORMAL_TSTOP) &&
            (pass_tF_or_back != CV_ONE_STEP_TSTOP))
    {
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_BAD_ITASK);
        /* SUNDIALS EXTENSION */
        if (is_sundials_with_extension())
        {
            return (CV_BAD_ITASK);
        }
        else
        {
            return (CV_ILL_INPUT);
        }
    }

    // Splitting itask into task and istop
    if ((pass_tF_or_back == CV_NORMAL_TSTOP) || (pass_tF_or_back == CV_ONE_STEP_TSTOP))
    {
        if (tstopset == FALSE)
        {
            RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_NO_TSTOP);
            /* SUNDIALS EXTENSION */
            if (is_sundials_with_extension())
            {
                return (CV_NO_TSTOP);
            }
            else
            {
                return (CV_ILL_INPUT);
            }
        }
        istop = TRUE;
    }
    else
    {
        istop = FALSE;
    }
    if ((pass_tF_or_back == CV_NORMAL) || (pass_tF_or_back == CV_NORMAL_TSTOP))
    {
        task = CV_NORMAL;
        toutc = t;
    }
    else
    {
        task = CV_ONE_STEP;
    }
    taskc = task;

    /* Step 2 : function body
     * nst = 0 means that the solver is called for the first time.
     * Performing initializations :
     *    - initializing Nordsieck history array
     *    - splitting the interval
     *    - check for approach to a root
     */

    // First time the solver is called
    if (nst == 0)
    {
        retval = f(tn, zn[0], zn[1], f_data);  // Setting zn[1] = y'(t0) = f(t0, y0), Nordsieck history array

        // Checking for errors in the evaluations of f thanks to retval
        if (retval < 0)
        {
            RKProcessError(cv_mem, CV_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FAILED, tn);
            return (CV_RHSFUNC_FAIL);
        }
        if (retval > 0)
        {
            RKProcessError(cv_mem, CV_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FIRST);
            return (CV_FIRST_RHSFUNC_ERR);
        }
    }

    tn = *told;  // Tn at 0

    // Splitting the interval, h ~ hmax (= 1/hmax_inv)
    hmax = 1. / hmax_inv;
    interval_size = t - *told;
    if (interval_size <= hmax)    // "Small" interval, h is the size of it
    {
        n_points = 2;
        h = interval_size;
    }
    else
    {
        test_div = interval_size / hmax;
        floor_test = floor(test_div);
        if (test_div - floor_test == 0.)  // Testing if we can cut the interval into hmax-long parts
        {
            n_points = floor_test + 1;
            h = interval_size / (n_points - 1);
        }
        else    // Adding a point and h < hmax to fit the interval
        {
            n_points = floor_test + 2;
            h = interval_size / (n_points - 1);
        }
    }

    // At maximum, do n_points-1 iterations
    mxstep = n_points - 1;

    /* Proceeding to :
     * K1 = F(Tn, Yn),
     * K2 = F(Tn + h/2, Yn + (h/2)*K1),
     * K3 = F(Tn + h/2, Yn + (h/2)*K2),
     * K4 = F(Tn + h, Yn + h*K3),
     * Yn+1 = Yn + (h/6)*(K1 + 2K2 + 2K3 + K4)
     * incrementing tn
     * In order to temporarily store the results, we use tempv and ftemp, which will represent the Ki in turn
     */

    // Main loop on time
    i = 0;  // i-th step

    //while (ABS(tn - t) > 1.0e-10) {               // As long as tn if strictly inferior to t
    for (i = 0; i <= mxstep - 1; ++i)             // Fixed number of points, so a 'for' is possible, to bypass 'ABS(tn+(i*h) - t)' rounding error
    {

        retval = f(tn, zn[0], ftemp, f_data);                  // ftemp = K1,
        N_VLinearSum_Serial (h / 2., ftemp, 1., zn[0], y);     // y = K1*h/2 + Yn,
        retval = f(tn + h / 2., y, tempv, f_data);             // tempv = K2 = f(Tn+h/2, Yn + (h/2)*K1),
        N_VLinearSum_Serial (1., ftemp, 2., tempv, y);         // y = K1 + 2K2,

        N_VLinearSum_Serial (h / 2., tempv, 1., zn[0], ftemp); // ftemp = Yn + K2*h/2,
        retval = f(tn + h / 2., ftemp, tempv, f_data);         // tempv= K3 = f(Tn+h/2, Yn + (h/2)*K2),
        N_VLinearSum_Serial (1., y, 2., tempv, y);             // y = K1 + 2K2 + 2K3,

        N_VLinearSum_Serial (h, tempv, 1., zn[0], ftemp);      // ftemp = Yn + K3*h,
        retval = f(tn + h, ftemp, tempv, f_data);              // tempv = K4 = f(Tn+h/2, Yn + h*K3),
        N_VLinearSum_Serial (1., y, 1., tempv, y);             // y = K1 + 2K2 + 2K3 + K4,

        N_VLinearSum_Serial(1., zn[0], h / 6, y, yout);        // yout = Yn+1 = Yn + y*h/6

        // Checking for errors in the evaluations of f thanks to retval
        if (retval < 0)
        {
            RKProcessError(cv_mem, CV_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FAILED, tn);
            return (CV_RHSFUNC_FAIL);
        }
        if (retval > 0)
        {
            RKProcessError(cv_mem, CV_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FIRST);
            return (CV_FIRST_RHSFUNC_ERR);
        }

        // Updating the Nordsieck history array to take the new values
        N_VScale (1., yout, zn[0]);            // zn[0] = yout
        retval = f(tn, zn[0], zn[1], f_data);  // zn[1] = y'(tn+h)

        // Incrementing tn => taking a step (increment i if using while loop)
        tn += h;
        //i++;

        // Checking for root in last step taken
        if (nrtfn > 0)
        {
            retval = RKRootFind (cv_mem);
            if (retval == RTFOUND)    // A new root was found
            {
                irfnd = 1;
                tretlast = *told = tlo;
                return (CV_ROOT_RETURN);
            }
            else if (retval == CV_RTFUNC_FAIL)    // g failed
            {
                RKProcessError(cv_mem, CV_RTFUNC_FAIL, "Runge-Kutta", "RKRootFind", MSGCV_RTFUNC_FAILED, tlo);
                return (CV_RTFUNC_FAIL);
            }
            // SUNDIALS EXTENSION
            if (is_sundials_with_extension())
                if (retval == ZERODETACHING)    // Zero detaching
                {
                    irfnd = 1;
                    tretlast = *told = tlo;
                    return (CV_ZERO_DETACH_RETURN);
                }
        }
        nst++; // Number of loop calls within cold restarts

    } // End of time loop

    // Success : start time (*told) := end time (t), and return the success flag
    *told = tn;
    return (CV_SUCCESS);
}

// Frees the problem memory allocated by RKMalloc
void RKFree (void ** cvode_mem)
{
    CVodeMem cv_mem;
    if (*cvode_mem == NULL)
    {
        return;
    }
    cv_mem = (CVodeMem) (*cvode_mem);

    RKFreeVectors (cv_mem);

    if (nrtfn > 0)
    {
        free(glo);
        glo = NULL;
        free(ghi);
        ghi = NULL;
        free(iroots);
        iroots = NULL;
    }

    free(*cvode_mem);
    *cvode_mem = NULL;
}

// Frees the RK vectors allocated in RKAllocVectors
void RKFreeVectors (CVodeMem cv_mem)
{
    int i;

    N_VDestroy(tempv);
    N_VDestroy(ftemp);
    for (i = 0; i <= qmax; ++i)
    {
        N_VDestroy(zn[i]);
    }

    lrw -= (qmax + 5) * lrw1;
    liw -= (qmax + 5) * liw1;
}

// Allocates the RK vectors tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
booleantype RKAllocVectors (CVodeMem cv_mem, N_Vector tmpl)
{
    int i, j;

    // Allocating tempv, ftemp and zn[]

    tempv = NULL;
    tempv = N_VClone (tmpl);
    if (tempv == NULL)
    {
        return (FALSE);
    }

    ftemp = NULL;
    ftemp = N_VClone (tmpl);
    if (ftemp == NULL)
    {
        N_VDestroy(tempv);
        return (FALSE);
    }

    // Allocating zn[0] ... zn[1]. RK only solves 1-order equations
    for (i = 0; i <= qmax; ++i)
    {
        zn[i] = NULL;
        zn[i] = N_VClone (tmpl);
        if (zn[i] == NULL)
        {
            N_VDestroy(tempv);
            N_VDestroy(ftemp);
            for (j = 0; j < i; ++j)
            {
                N_VDestroy(zn[j]);
            }
            return (FALSE);
        }
    }

    // Updating solver workspace lengths
    lrw += (qmax + 5) * lrw1;
    liw += (qmax + 5) * liw1;

    return (TRUE);
}

// Error handling function
void RKProcessError (CVodeMem cv_mem, int error_code, const char *mod, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    // Initializing the argument pointer variable (msgfmt is the last required argument to RKProcessError)
    va_start (ap, msgfmt);

    if (cv_mem == NULL)
    {
        // Writing to stderr
#ifndef NO_FPRINTF_OUTPUT
        fprintf (stderr, "\n[%s ERROR]  %s\n  ", mod, fname);
        fprintf (stderr, msgfmt);
        fprintf (stderr, "\n\n");
#endif
    }
    else
    {
        // Composing the message
        vsprintf(msg, msgfmt, ap);

        // Calling ehfun
        RKErrHandler (error_code, mod, fname, msg, cv_mem);
    }
    // Finalizing argument process
    va_end(ap);
    return;
}

// Default error handling function
void RKErrHandler (int error_code, const char *mod, const char *func, char *msg, void *data)
{
    char err_type[10];

    if (error_code == CV_WARNING)
    {
        sprintf(err_type, "WARNING");
    }
    else
    {
        sprintf(err_type, "ERROR");
    }

#ifndef NO_FPRINTF_OUTPUT
    if (errfp != NULL)
    {
        fprintf(errfp, "\n[%s %s]  %s\n", mod, err_type, func);
        fprintf(errfp, "  %s\n\n", msg);
    }
#endif
    return;
}

// Checks if all required vector operations are present
booleantype RKCheckNvector (N_Vector tmpl)
{
    if ((tmpl->ops->nvclone     == NULL) ||
            (tmpl->ops->nvdestroy   == NULL) ||
            (tmpl->ops->nvlinearsum == NULL) ||
            (tmpl->ops->nvconst     == NULL) ||
            (tmpl->ops->nvprod      == NULL) ||
            (tmpl->ops->nvdiv       == NULL) ||
            (tmpl->ops->nvscale     == NULL) ||
            (tmpl->ops->nvabs       == NULL) ||
            (tmpl->ops->nvinv       == NULL) ||
            (tmpl->ops->nvaddconst  == NULL) ||
            (tmpl->ops->nvmaxnorm   == NULL) ||
            (tmpl->ops->nvwrmsnorm  == NULL) ||
            (tmpl->ops->nvmin       == NULL))
    {
        return (FALSE);
    }
    else
    {
        return (TRUE);
    }
}

/* Tests g(t) for a root between tlo and thi, if one exists.
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
 *            which components g_i have a root at tn.
 *            For i = 0, ..., nrtfn-1, iroots[i] = 1 if g_i has a root
 *            and iroots[i] = 0 otherwise.
 *
 * Returning an int equal to:
 *      RTFOUND = 1 if a root of g was found, or
 *      CV_SUCCESS = 0 otherwise.
 */
int RKRootFind (CVodeMem cv_mem)
{
    int i, retval;
    retval = CV_SUCCESS;

    // Setting y = y(thi) and thi = next t
    N_VScale (1., zn[0], y);
    thi = tn;

    // Setting ghi = g(thi), incrementing gfun counter and searching [tlo,thi] for roots
    retval = gfun (thi, y, ghi, g_data);
    nge++;
    if (retval != 0)
    {
        return (CV_RTFUNC_FAIL);
    }

    // Checking for zeros in ghi or sign change, and update iroots accordingly
    for (i = 0; i < nrtfn; ++i)
    {
        iroots[i] = 0;
        if (ghi[i] == 0.)    // ghi contains a zero
        {
            iroots[i] = (glo[i] > 0) ? -1 : 1;
            retval = RTFOUND;
        }
        else if (glo[i]*ghi[i] < 0)   // Sign change
        {
            iroots[i] = (ghi[i] > glo[i]) ? 1 : -1;
            retval = RTFOUND;
        }
    }

    // No root found : incrementing tlo and glo and returning CV_SUCCESS
    tlo = thi;
    for (i = 0; i < nrtfn; ++i)
    {
        glo[i] = ghi[i];
    }
    return (retval);
}

