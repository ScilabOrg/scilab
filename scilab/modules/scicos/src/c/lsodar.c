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

extern int C2F(lsodar) (int f, int *neq, realtype *y, realtype t, realtype tout, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int lrw, realtype *iwork, int liw, int jac, int jtype, int gfun, int ng, int *jroot);

void lsodarCreate (int simblk, int *neq, realtype *yVec, realtype t0PlusEps, realtype t0, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int lrw, realtype *iwork, int liw, int Jacobians, int jtype, int grblk, int ng, int *jroot)
{
    C2F(lsodar) (simblk, neq, yVec, t0PlusEps, t0, itol, reltol, abstol, itask, istate, iopt, rwork, lrw, iwork, liw, Jacobians, jtype, grblk, ng, jroot);
}

/* =============================
 *
 * 			lsodarSetMaxStep
 *
 * =============================
 *
 * Specifies the maximum step size
 */

int lsodarSetMaxStep (void *cvode_mem, realtype hmax)
{
    realtype hmax_inv;
    CVodeMem cv_mem;

    if (cvode_mem == NULL)
    {
        CVProcessError(NULL, CV_MEM_NULL, "CVODE", "CVodeSetMaxStep", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }

    cv_mem = (CVodeMem) cvode_mem;

    if (hmax <= 0)
    {
        lsodarProcessError(cv_mem, CV_ILL_INPUT, "lsodar", "lsodarSetMaxStep", MSGCV_NEG_HMAX);
        return(CV_ILL_INPUT);
    }

    hmax_inv = 1. / hmax;
    cv_mem->cv_hmax_inv = hmax_inv;

    return(CV_SUCCESS);
}

#define hmax_inv (cv_mem->cv_hmax_inv)
#define lrw  	   (cv_mem->cv_lrw)
#define liw  	   (cv_mem->cv_liw)

/* =============================
 *
 * 			lsodarMalloc
 *
 * =============================
 *
 * Allocating the problem
 */

int lsodarMalloc (void* cvode_mem, CVRhsFn f, realtype t0, N_Vector y0)
{
    // Step 1 : checking the arguments

    if (cvode_mem == NULL)
    {
        lsodarProcessError (NULL, CV_MEM_NULL, "lsodar", "lsodarMalloc", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }

    CVodeMem cv_mem;
    cv_mem = (CVodeMem) cvode_mem;

    if (y0 == NULL)
    {
        lsodarProcessError (cv_mem, CV_ILL_INPUT, "lsodar", "lsodarMalloc", MSGCV_NULL_Y0);
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
        lsodarProcessError (cv_mem, CV_ILL_INPUT, "lsodar", "lsodarMalloc", MSGCV_NULL_F);
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
    booleantype nvectorOK;
    nvectorOK = lsodarCheckNvector (y0);
    if (!nvectorOK)
    {
        lsodarProcessError (cv_mem, CV_ILL_INPUT, "lsodar", "lsodarMalloc", MSGCV_BAD_NVECTOR);
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

    long int lrw1, liw1;
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
    booleantype allocOK;
    allocOK = lsodarAllocVectors (cv_mem, y0);
    if (!allocOK)
    {
        lsodarProcessError(cv_mem, CV_MEM_FAIL, "lsodar", "lsodarMalloc", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    // Copying the input parameters into lsodar memory block
    cv_mem->cv_f  = f;
    cv_mem->cv_tn = t0;
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

#define lrw1 (cv_mem->cv_lrw1)
#define liw1 (cv_mem->cv_liw1)

/* =============================
 *
 * 			lsodarReInit
 *
 * =============================
 *
 * Reinitializing the problem
 */

int lsodarReInit (void* cvode_mem, CVRhsFn f, realtype t0, N_Vector y0)
{
    // Step 1 : checking the arguments

    // Checking cvode_mem
    if (cvode_mem == NULL)
    {
        lsodarProcessError (NULL, CV_MEM_NULL, "lsodar", "lsodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }

    CVodeMem cv_mem;
    cv_mem = (CVodeMem) cvode_mem;

    // Checking if cv_mem was allocated
    if (cv_mem->cv_MallocDone == FALSE)
    {
        lsodarProcessError (cv_mem, CV_NO_MALLOC, "Runga-Kutta", "lsodarReInit", MSGCV_NO_MALLOC);
        return (CV_NO_MALLOC);
    }

    // Checking the other arguments
    if (y0 == NULL)
    {
        lsodarProcessError (cv_mem, CV_ILL_INPUT, "lsodar", "lsodarReInit", MSGCV_NULL_Y0);
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
        lsodarProcessError (cv_mem, CV_ILL_INPUT, "lsodar", "lsodarReInit", MSGCV_NULL_F);
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

    // Recopying the input parameters into lsodar memory block
    cv_mem->cv_f = f;
    cv_mem->cv_tn = t0;

    // Reinitializing zn[0] in the history array
    N_VScale(1., y0, cv_mem->cv_zn[0]);

    // Reinitializing all the counters
    cv_mem->cv_nst     = 0;
    cv_mem->cv_nge     = 0;

    // The problem has been successfully re-initialized
    return (CV_SUCCESS);
}

#define gfun   (cv_mem->cv_gfun)
#define g_data (cv_mem->cv_g_data)
#define glo    (cv_mem->cv_glo)
#define ghi    (cv_mem->cv_ghi)
#define iroots (cv_mem->cv_iroots)
#define tlo    (cv_mem->cv_tlo)
#define zn     (cv_mem->cv_zn)

/* =============================
 *
 * 			lsodarRootInit
 *
 * =============================
 *
 * Initializing the Root Finding problem
 */

int lsodarRootInit (void* cvode_mem, int nrtfn, CVRootFn g, void *gdata)
{
    // Checking cvode_mem
    if (cvode_mem == NULL)
    {
        lsodarProcessError (NULL, CV_MEM_NULL, "lsodar", "lsodarRootInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }

    CVodeMem cv_mem;
    cv_mem = (CVodeMem) cvode_mem;

    int nrt;
    nrt = (nrtfn < 0) ? 0 : nrtfn;

    /* If rerunning lsodarRootInit() with a different number of root functions (changing number of gfun components),
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

    /* If lsodarRootInit() was called with nrtfn == 0, then setting cv_nrtfn to
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

    /* If rerunning lsodarRootInit() with the same number of root functions (not changing number of gfun components),
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
                lsodarProcessError (cv_mem, CV_ILL_INPUT, "lsodar", "lsodarRootInit", MSGCV_NULL_G);
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

    // Setting variables in lsodar memory block
    cv_mem->cv_nrtfn = nrt;
    if (g == NULL)
    {
        lsodarProcessError(cv_mem, CV_ILL_INPUT, "lsodar", "lsodarRootInit", MSGCV_NULL_G);
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
        lsodarProcessError(cv_mem, CV_MEM_FAIL, "lsodar", "lsodarRootInit", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    // Initializing glo
    int retval;
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
        lsodarProcessError(cv_mem, CV_MEM_FAIL, "lsodar", "lsodarRootInit", MSGCV_MEM_FAIL);
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
        lsodarProcessError(cv_mem, CV_MEM_FAIL, "lsodar", "lsodarRootInit", MSGCV_MEM_FAIL);
        return (CV_MEM_FAIL);
    }

    lrw += 3 * nrt;
    liw += nrt;

    return (CV_SUCCESS);
}

/* =============================
 * Readibility Constants
 * =============================
 */
#define f          (cv_mem->cv_f)
#define f_data     (cv_mem->cv_f_data)
#define qmax       (cv_mem->cv_qmax)
#define mxstep     (cv_mem->cv_mxstep)
#define istop      (cv_mem->cv_istop)
#define tstop      (cv_mem->cv_tstop)
#define tstopset   (cv_mem->cv_tstopset)

#define y          (cv_mem->cv_y)
#define tempv      (cv_mem->cv_tempv)
#define ftemp      (cv_mem->cv_ftemp)
#define h          (cv_mem->cv_h)
#define tn         (cv_mem->cv_tn)
#define nst        (cv_mem->cv_nst)

#define nrtfn      (cv_mem->cv_nrtfn)
#define thi        (cv_mem->cv_thi)
#define tretlast   (cv_mem->cv_tretlast)
#define toutc      (cv_mem->cv_toutc)
#define taskc      (cv_mem->cv_taskc)
#define irfnd      (cv_mem->cv_irfnd)
#define nge        (cv_mem->cv_nge)

/* =============================
 *
 * 				lsodar
 *
 * =============================
 *
 * Solving the problem
 */

int lsodarSolve (int simblk, int *neq, realtype *yVec, realtype t, realtype told, int itol, realtype reltol, realtype abstol, int itask, int istate, int iopt, realtype *rwork, int Lrw, realtype *iwork, int Liw, int Jacobians, int jtype, int grblk, int ng, int *jroot)
{
    C2F(lsodar) (simblk, neq, yVec, t, told, CV_SS, reltol, abstol, CV_NORMAL_TSTOP, istate, iopt, rwork, Lrw, iwork, Liw, Jacobians, jtype, grblk, ng, jroot);
    return (istate);
}

// Freeing the problem memory allocated by lsodarMalloc
void lsodarFree (void** cvode_mem)
{
    CVodeMem cv_mem;
    if (*cvode_mem == NULL)
    {
        return;
    }

    cv_mem = (CVodeMem) (*cvode_mem);

    lsodarFreeVectors (cv_mem);

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

// Freeing the lsodar vectors allocated in lsodarAllocVectors
void lsodarFreeVectors (CVodeMem cv_mem)
{
    int j;

    N_VDestroy(tempv);
    N_VDestroy(ftemp);
    for (j = 0; j <= qmax; ++j)
    {
        N_VDestroy(zn[j]);
    }

    lrw -= (qmax + 5) * lrw1;
    liw -= (qmax + 5) * liw1;
}

// Allocating the lsodar vectors tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
booleantype lsodarAllocVectors (CVodeMem cv_mem, N_Vector tmpl)
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

    // Allocating zn[0] ... zn[1]. lsodar only solves 1-order equations
    for (j = 0; j <= qmax; ++j)
    {
        zn[j] = NULL;
        zn[j] = N_VClone (tmpl);
        if (zn[j] == NULL)
        {
            N_VDestroy(tempv);
            N_VDestroy(ftemp);
            for (i = 0; i < j; ++i)
            {
                N_VDestroy(zn[i]);
            }
            return (FALSE);
        }
    }

    // Updating solver workspace lengths
    lrw += (qmax + 5) * lrw1;
    liw += (qmax + 5) * liw1;

    return (TRUE);
}

// Specifying the time beyond which the integration is not to proceed
int lsodarSetStopTime (CVodeMem cvode_mem, realtype ttstop)
{
    if (cvode_mem == NULL)
    {
        lsodarProcessError (NULL, CV_MEM_NULL, "lsodar", "lsodarSetStopTime", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    CVodeMem cv_mem;
    cv_mem = (CVodeMem) cvode_mem;
    cv_mem->cv_tstop = ttstop;
    cv_mem->cv_tstopset = TRUE;

    return (CV_SUCCESS);
}

// Error handling function
void lsodarProcessError (CVodeMem cv_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    // Initializing the argument pointer variable (msgfmt is the last required argument to lsodarProcessError)
    va_start (ap, msgfmt);

    if (cv_mem == NULL)
    {
        // Writing to stderr
#ifndef NO_FPRINTF_OUTPUT
        fprintf (stderr, "\n[%s ERROR]  %s\n  ", module, fname);
        fprintf (stderr, msgfmt);
        fprintf (stderr, "\n\n");
#endif
    }
    else
    {
        // Composing the message
        vsprintf(msg, msgfmt, ap);

        // Calling ehfun
        lsodarErrHandler (error_code, module, fname, msg, cv_mem);
    }
    // Finalizing argument process
    va_end(ap);
    return;
}

// Default error handling function
void lsodarErrHandler (int error_code, const char *module, const char *function, char *msg, void *data)
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
        fprintf(errfp, "\n[%s %s]  %s\n", module, err_type, function);
        fprintf(errfp, "  %s\n\n", msg);
    }
#endif
    return;
}

// Checking if all required vector operations are present
booleantype lsodarCheckNvector (N_Vector tmpl)
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
 *            which components g_i have a root at tn.
 * 			  For i = 0, ..., nrtfn-1, iroots[i] = 1 if g_i has a root
 *            and iroots[i] = 0 otherwise.
 *
 * Returning an int equal to:
 *      RTFOUND = 1 if a root of g was found, or
 *      CV_SUCCESS = 0 otherwise.
 */
int lsodarRootFind (CVodeMem cv_mem)
{
    int i, retval = CV_SUCCESS;

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

