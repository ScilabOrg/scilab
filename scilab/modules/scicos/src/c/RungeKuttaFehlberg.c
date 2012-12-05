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
#include <string.h>
#include <machine.h>

#include "RungeKuttaFehlberg.h"

/* =============================
 *
 *            rkf45
 *
 * =============================
 *
 * Actual solving function, from 'ODEPACK' in 'differential_equations' module
 */

extern void C2F(rkf45) (RKFRhsFn *f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, int *itask, int *istate, int *iopt, realtype *rwork, int *lrw, int *iwork, int *liw, int *bjac, int *mf);

/* =============================
 *
 *         RKFCreate
 *
 * =============================
 *
 * RKFCreate creates an internal memory block for a problem to be solved by RKF45.
 * If successful, RKFCreate returns a pointer to the problem memory.
 * This pointer should be passed to RKFMalloc.
 * If an initialization error occurs,
 * RKFCreate prints an error message to standard err and returns NULL.
 */

void * RKFCreate (int * neq, int ng)
{
    int lRn, lRs, lIw, lRw;

    /* Allocate the problem memory space */
    RKFMem rkf_mem = NULL;
    rkf_mem = (RKFMem) malloc(sizeof(struct RKFMemRec));
    if (rkf_mem == NULL)
    {
        RKFProcessError(NULL, 0, "RKF", "RKFCreate", MSGCV_CVMEM_FAIL);
        return (NULL);
    }

    /* Set the 'rwork' and 'iwork' workspaces lengths */
    lRn = 20 + 16 * (*neq) + 3 * ng;
    lRs = 22 + ((*neq) * (9 + (*neq))) + 3 * ng;
    lIw = 20 + (*neq);
    lRw = max(Lrn, Lrs);

    /* Copy the variables into the problem memory space */
    rkf_mem->nEquations = neq;
    rkf_mem->iState = 1;
    rkf_mem->iOpt = 0;
    rkf_mem->rwork = NULL;
    rkf_mem->lrw = lRw;
    rkf_mem->iwork = NULL;
    rkf_mem->liw = lIw;
    rkf_mem->jacobian = 0;
    rkf_mem->jacType = 2;
    rkf_mem->g_fun = NULL;
    rkf_mem->ng_fun = ng;
    rkf_mem->jroot = NULL;

    return ((void *) rkf_mem);
}

/* Shortcuts to problem memory space parameters */
# define func    rkf_mem->func
# define nEq     rkf_mem->nEquations
# define yVec    rkf_mem->yVector
# define tStart  rkf_mem->tStart
# define tEnd    rkf_mem->tEnd
# define iTol    rkf_mem->iTol
# define relTol  rkf_mem->relTol
# define absTol  rkf_mem->absTol
# define iState  rkf_mem->iState
# define iOpt    rkf_mem->iOpt
# define rwork   rkf_mem->rwork
# define lrw     rkf_mem->lrw
# define iwork   rkf_mem->iwork
# define liw     rkf_mem->liw
# define jac     rkf_mem->jacobian
# define jacType rkf_mem->jacType
# define g_fun   rkf_mem->g_fun
# define ng_fun  rkf_mem->ng_fun
# define jroot   rkf_mem->jroot

/* =============================
 *
 *         RKFMalloc
 *
 * =============================
 *
 * RKFMalloc allocates and initializes memory for a problem.
 * All problem inputs are checked for errors. If any error occurs during initialization,
 * it is reported to the file whose file pointer is errfp and an error flag is returned.
 * Otherwise, it returns CV_SUCCESS.
 */

int RKFMalloc (void * rkfel_mem, RKFRhsFn f, realtype t0, N_Vector y, int itol, realtype reltol, void * abstol)
{
    /* Check the input arguments */

    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKFMalloc", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    if (f == NULL)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFMalloc", MSGCV_NULL_F);
        return (CV_ILL_INPUT);
    }
    if (y == NULL)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFMalloc", MSGCV_NULL_Y0);
        return (CV_ILL_INPUT);
    }
    if (reltol < 0.)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFMalloc", MSGCV_BAD_RELTOL);
        return (CV_ILL_INPUT);
    }
    if (*((realtype *) abstol) < 0.)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFMalloc", MSGCV_BAD_ABSTOL);
        return (CV_ILL_INPUT);
    }

    /* Copy the arguments into the problem memory space */
    func   =  f;
    yVec   =  NV_DATA_S(y);
    tStart =  t0;
    iTol   =  itol;
    relTol =  reltol;
    absTol =  *((realtype *) abstol);

    /* Allocate rwork and iwork workspaces and set them to zero.
       Their size is lrw and liw, respectively */
    rwork = (struct rWork_t *) calloc(lrw, sizeof(realtype));
    iwork = calloc(liw, sizeof(int));

    return (CV_SUCCESS);
}

/* =============================
 *
 *         RKFReInit
 *
 * =============================
 *
 * RKFReInit re-initializes RKF's memory for a problem,
 * assuming it has already been allocated in a prior RKFMalloc call.
 * All problem specification inputs are checked for errors.
 * If any error occurs during initialization, it is reported to the file whose file pointer is errfp.
 * The return value is CV_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int RKFReInit (void * rkfel_mem, RKFRhsFn f, realtype tOld, N_Vector y, int itol, realtype reltol, void * abstol)
{
    realtype rwork0, rwork5;

    /* Check the input arguments */

    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKFReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    if (y == NULL)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFReInit", MSGCV_NULL_Y0);
        return (CV_ILL_INPUT);
    }
    if (f == NULL)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFReInit", MSGCV_NULL_F);
        return (CV_ILL_INPUT);
    }
    if (reltol < 0.)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFReInit", MSGCV_BAD_RELTOL);
        return (CV_ILL_INPUT);
    }
    if (*((realtype *) abstol) < 0.)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFReInit", MSGCV_BAD_ABSTOL);
        return (CV_ILL_INPUT);
    }

    /* Reset the problem memory space variables to the arguments */
    func   =  f;
    *nEq   =  NV_LENGTH_S(y);
    yVec   =  NV_DATA_S(y);
    tStart =  tOld;
    iTol   =  itol;
    relTol =  reltol;
    absTol =  *((realtype *) abstol);
    iState =  1;

    /* Reinitialize rwork and iwork, leave rwork->tcrit and rwork->hmax unchanged, containing tcrit and hmax */
    rwork0 = rwork->tcrit;
    rwork5 = rwork->hmax;
    memset(rwork, 0., lrw);
    memset(iwork, 0., liw);
    rwork->tcrit = rwork0;
    rwork->hmax = rwork5;

    return (CV_SUCCESS);
}

/* =============================
 *
 *         RKFRootInit
 *
 * =============================
 *
 * RKFRootInit initializes a rootfinding problem to be solved during the integration of the ODE system.
 * It loads the root function pointer and the number of root functions, and allocates workspace memory.
 * The return value is CV_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int RKFRootInit (void * rkfel_mem, int ng, RKFRootFn g, void *gdata)
{
    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKFRootInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    if (g == NULL)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKFRootInit", MSGCV_NULL_G);
        return (CV_MEM_NULL);
    }

    g_fun = g;
    ng_fun = ng;

    /* Allocate jroot and set it to zero */
    if (ng > 0)
    {
        jroot = calloc(ng, sizeof(int));
    }

    return (CV_SUCCESS);
}


/* =============================
 *
 *       RKFSetMaxStep
 *
 * =============================
 *
 * Sets the maximum step size, stocked in rwork->hmax.
 * Sets iOpt to 1 for rwork->hmax to be taken in consideration by rkf45().
 */

int RKFSetMaxStep (void * rkfel_mem, realtype hMax)
{
    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKFSetMaxStep", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    if (iOpt == 0)
    {
        iOpt = 1;
    }
    rwork->hmax = hMax;

    return (CV_SUCCESS);
}

/* =============================
 *
 *       RKFSetStopTime
 *
 * =============================
 *
 * Specifies the time beyond which the integration is not to proceed, stocked in rwork->tcrit.
 * Sets iOpt to 1 for rwork->tcrit to be taken in consideration by rkf45().
 */

int RKFSetStopTime (void * rkfel_mem, realtype tCrit)
{
    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKFSetStopTime", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    if (iOpt == 0)
    {
        iOpt = 1;
    }
    rwork->tcrit = tCrit;

    return (CV_SUCCESS);
}

/* =============================
 *
 *            RKF
 *
 * =============================
 *
 * This routine is the main driver of RKF.
 *
 * It integrates and looks for roots over a time interval defined by the user.
 *
 * The first time that RKF is called for a successfully initialized problem,
 * it computes a tentative initial step size h.
 *
 * RKF supports two modes, specified by itask: RKF_NORMAL, RKF_ONE_STEP.
 *
 * In the RKF_NORMAL mode, the solver steps until it reaches or passes tout and then interpolates to obtain y(tOut).
 * In the RKF_ONE_STEP mode, it takes one internal step and returns.
 *
 * It returns CV_ROOT_RETURN if a root was detected, CV_SUCCESS if the integration went correctly,
 * or a corresponding error flag.
 */

int RKF (void * rkfel_mem, realtype tOut, N_Vector yOut, realtype * tOld, enum iTask_t itask)
{
    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKF", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    if (yOut == NULL)
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKF", MSGCV_YOUT_NULL);
        return (CV_ILL_INPUT);
    }
    if ((itask != RKF_NORMAL) && (itask != RKF_ONE_STEP))
    {
        RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKF", MSGCV_BAD_ITASK);
        return(CV_ILL_INPUT);
    }

    /* Retrieve nEq if it has changed, use a copy of the solution vector and stock the simulation times */
    *nEq = NV_LENGTH_S(yOut);
    yVec = NV_DATA_S(yOut);
    tStart = *tOld;
    tEnd = tOut;

    /* Launch the simulation with the memory space parameters.
       rkf45() will update yVec, iState, rwork, iwork and jroot */
    C2F(rkf45) (func, nEq, yVec, &tStart, &tEnd, &iTol, &relTol, &absTol, &itask, &iState, &iOpt, rwork, &lrw, iwork, &liw, &jac, &jacType, g_fun, &ng_fun, jroot);

    /* Increment the start times */
    *tOld = tOut;
    tStart = tEnd;

    /* rkf45() stocked the completion status in iState; return accordingly  */
    switch (iState)
    {
        case 3:
            return (CV_ROOT_RETURN);
        case -1:
            RKFProcessError(rkf_mem, CV_TOO_MUCH_WORK, "RKF", "RKF", MSGCV_MAX_STEPS, tStart);
            return (CV_TOO_MUCH_WORK);
        case -2:
            RKFProcessError(rkf_mem, CV_TOO_MUCH_ACC, "RKF", "RKF", MSGCV_TOO_MUCH_ACC, tStart);
            return (CV_TOO_MUCH_ACC);
        case -3:
            RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKF", MSGCV_BAD_INPUT, tStart);
            return (CV_ILL_INPUT);
        case -4:
            RKFProcessError(rkf_mem, CV_ERR_FAILURE, "RKF", "RKF", MSGCV_ERR_FAILS, tStart);
            return (CV_ERR_FAILURE);
        case -5:
            RKFProcessError(rkf_mem, CV_CONV_FAILURE, "RKF", "RKF", MSGCV_CONV_FAILS, tStart);
            return (CV_CONV_FAILURE);
        case -6:
            RKFProcessError(rkf_mem, CV_ILL_INPUT, "RKF", "RKF", MSGCV_EWT_NOW_BAD, tStart);
            return (CV_ILL_INPUT);
        default:
            return (CV_SUCCESS);
    }
}

/* =============================
 *
 *       RKFGetRootInfo
 *
 * =============================
 *
 * Updates rootsfound[] to the computed roots stocked in jroot[].
 */

int RKFGetRootInfo (void * rkfel_mem, int * rootsfound)
{
    RKFMem rkf_mem = NULL;
    if (rkfel_mem == NULL)
    {
        RKFProcessError(NULL, CV_MEM_NULL, "RKF", "RKFGetRootInfo", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    rkf_mem = (RKFMem) rkfel_mem;

    /* Copy jroot to rootsfound */
    memcpy(rootsfound, jroot, ng_fun * sizeof(int));

    return (CV_SUCCESS);
}

/* =============================
 *
 *            RKFFree
 *
 * =============================
 *
 * This routine frees the problem memory allocated by RKFMalloc.
 */

void RKFFree (void ** rkfel_mem)
{
    RKFMem rkf_mem = NULL;
    if (*rkfel_mem == NULL)
    {
        return;
    }
    rkf_mem = (RKFMem) (*rkfel_mem);

    /* Free the inner vectors */
    RKFFreeVectors (rkf_mem);

    free (*rkfel_mem);
    *rkfel_mem = NULL;
}

/* =============================
 *
 *         RKFFreeVectors
 *
 * =============================
 *
 * Frees the problem memory space vectors.
 */

void RKFFreeVectors (RKFMem rkf_mem)
{
    /* rwork, iwork and jroot have been allocated; free them */
    free (rwork);
    free (iwork);
    free (jroot);
}

#define ehfun    RKFErrHandler
#define eh_data  (void *) rkfel_mem

/* =============================
 *
 *         RKFProcessError
 *
 * =============================
 *
 * Error handling function.
 */

void RKFProcessError (RKFMem rkfel_mem, int error_code, const char *mod, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    /* Initialize the argument pointer variable
       (msgfmt is the last required argument to RKFProcessError) */
    va_start(ap, msgfmt);

    if (rkfel_mem == NULL)      /* We write to stderr */
    {
#ifndef NO_FPRINTF_OUTPUT
        fprintf(stderr, "\n[%s ERROR]  %s\n  ", mod, fname);
        fprintf(stderr, msgfmt);
        fprintf(stderr, "\n\n");
#endif
    }
    else                     /* We can call ehfun */
    {
        /* Compose the message */
        vsprintf(msg, msgfmt, ap);

        /* Call ehfun */
        ehfun(error_code, mod, fname, msg, eh_data);
    }

    /* Finalize argument processing */
    va_end(ap);

    return;
}

#define errfp    stderr

/* =============================
 *
 *          RKFErrHandler
 *
 * =============================
 *
 * Default error handling function.
 */

void RKFErrHandler (int error_code, const char *mod, const char *func, char *msg, void *data)
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

