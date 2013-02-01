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

#include "ddasrt.h"

#define NO_FPRINTF_OUTPUT 1

/*
 * Control constant for tolerances
 *
 * Scicos only uses scalar tolerances, so we only need the scalar-scalar (SS) value for info[1].
 * --------------------------------
 */

#define IDA_SS  0

/* =============================
 *
 *            ddasrt
 *
 * =============================
 *
 * Actual solving function, from 'ODEPACK' in 'differential_equations' module
 */

extern void C2F(ddasrt) (DDASResFn res, int *neq, realtype *t, realtype *y, realtype *yp, realtype *tout, int *info, realtype *reltol, realtype *abstol, int *istate, struct DDrWork_t *rwork, int *lrw, int *iwork, int *liw, double *dummy1, int *dummy2, int *jacobian, DDASRootFn grblk, int *ng, int *jroot);

/* =============================
 *
 *         DDasrtCreate
 *
 * =============================
 *
 * DDasrtCreate creates an internal memory block for a problem to be solved by DDASRT.
 * If successful, DDasrtCreate returns a pointer to the problem memory.
 * This pointer should be passed to DDasrtInit.
 * If an initialization error occurs,
 * DDasrtCreate prints an error message to standard err and returns NULL.
 */

void * DDasrtCreate (int * neq, int ng)
{
    int lRn, lRs, lIw, lRw;
    DDasrtMem ddasrt_mem;

    /* Allocate the problem memory space */
    ddasrt_mem = NULL;
    ddasrt_mem = (DDasrtMem) malloc(sizeof(struct DDasrtMemRec));
    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, 0, "DDASRT", "DDasrtCreate", MSG_MEM_FAIL);
        return (NULL);
    }

    /* Zero out ddas_mem */
    memset(ddasrt_mem, 0, sizeof(struct DDasrtMemRec));

    /* Set the 'rwork' and 'iwork' workspaces lengths by default */
    lRw = 50 + (*neq) * (MAXORD_DEFAULT + 4 * (*neq)) + 3 * ng;
    lIw = 20 + (*neq);

    /* Copy the variables into the problem memory space */
    ddasrt_mem->nEquations = neq;
    ddasrt_mem->user_data  = NULL;
    ddasrt_mem->iState     = 1;
    ddasrt_mem->info       = NULL;
    ddasrt_mem->rwork      = NULL;
    ddasrt_mem->lrw        = lRw;
    ddasrt_mem->iwork      = NULL;
    ddasrt_mem->liw        = lIw;
    ddasrt_mem->jacobian   = NULL;
    ddasrt_mem->maxord     = MAXORD_DEFAULT;
    ddasrt_mem->ehfun      = NULL;
    ddasrt_mem->g_fun      = NULL;
    ddasrt_mem->ng_fun     = ng;
    ddasrt_mem->jroot      = NULL;

    return ((void *) ddasrt_mem);
}

/* Shortcuts to problem memory space parameters */
# define res        ddas_mem->res
# define nEq        ddas_mem->nEquations
# define yVec       ddas_mem->yVector
# define ypVec      ddas_mem->yPrimeVector
# define tStart     ddas_mem->tStart
# define tEnd       ddas_mem->tEnd
# define info       ddas_mem->info
# define relTol     ddas_mem->relTol
# define absTol     ddas_mem->absTol
# define iState     ddas_mem->iState
# define iOpt       ddas_mem->iOpt
# define rwork      ddas_mem->rwork
# define lrw        ddas_mem->lrw
# define iwork      ddas_mem->iwork
# define liw        ddas_mem->liw
# define jac        ddas_mem->jacobian
# define g_fun      ddas_mem->g_fun
# define ng_fun     ddas_mem->ng_fun
# define jroot      ddas_mem->jroot
# define user_data  ddas_mem->user_data

/* =============================
 *
 *          DDasrtInit
 *
 * =============================
 *
 * DDasrtInit allocates and initializes memory for a problem.
 * All problem inputs are checked for errors. If any error occurs during initialization,
 * it is reported to the file whose file pointer is errfp and an error flag is returned.
 * Otherwise, it returns IDA_SUCCESS.
 */

int DDasrtInit (void * ddasrt_mem, DDASResFn Res, realtype t0, N_Vector yy0, N_Vector yp0)
{
    DDasrtMem ddas_mem;

    /* Check the input arguments */

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtInit", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    if (yy0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtInit", MSG_Y0_NULL);
        return (IDA_ILL_INPUT);
    }

    if (yp0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtInit", MSG_YP0_NULL);
        return (IDA_ILL_INPUT);
    }

    if (Res == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtInit", MSG_RES_NULL);
        return (IDA_ILL_INPUT);
    }

    /* Copy the arguments into the problem memory space */
    res    = Res;
    yVec   = NV_DATA_S(yy0);
    ypVec  = NV_DATA_S(yp0);
    tStart = t0;

    /* Allocate the info[15] tab, used to store parameters */
    info   = calloc(15, sizeof(int));
    //info[10] = 1; // If we don't use IDA's CalcIC & GetConsistentIC, then set yp to 0 too ?

    /* Allocate rwork and iwork workspaces and set them to zero.
       Their size is lrw and liw, respectively */
    rwork = (struct DDrWork_t *) calloc(lrw, sizeof(realtype));
    iwork = calloc(liw, sizeof(int));

    return (IDA_SUCCESS);
}

/* =============================
 *
 *         DDasrtReInit
 *
 * =============================
 *
 * DDasrtReInit re-initializes DDASRT's memory for a problem,
 * assuming it has already been allocated in a prior DDasrtInit call.
 * All problem specification inputs are checked for errors.
 * If any error occurs during initialization, it is reported to the file whose file pointer is errfp.
 * The return value is IDA_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int DDasrtReInit (void * ddasrt_mem, realtype tOld, N_Vector yy0, N_Vector yp0)
{
    DDasrtMem ddas_mem;
    double rwork0, rwork2;

    /* Check the input arguments */

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtReInit", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    if (yy0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtInit", MSG_Y0_NULL);
        return (IDA_ILL_INPUT);
    }

    if (yp0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtInit", MSG_YP0_NULL);
        return (IDA_ILL_INPUT);
    }

    /* Reset the problem memory space variables to the arguments */
    *nEq    = NV_LENGTH_S(yy0);
    yVec    = NV_DATA_S(yy0);
    ypVec   = NV_DATA_S(yp0);
    tStart  = tOld;
    info[0] = 0;

    /* Reinitialize rwork and iwork, leave rwork->tcrit and rwork->hmax unchanged, containing tcrit and hmax */
    rwork0 = rwork->tcrit;
    rwork2 = rwork->hmax;
    memset(rwork, 0, lrw);
    memset(iwork, 0, liw);
    rwork->tcrit = rwork0;
    rwork->hmax  = rwork2;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDasrtSStolerances
 *
 * =============================
 *
 * This function specifies the scalar integration tolerances.
 * It MUST be called before the first call to DDasrt.
 */

int DDasrtSStolerances(void * ddasrt_mem, realtype reltol, realtype abstol)
{
    DDasrtMem ddas_mem;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDasrt", "DDasrtSStolerances", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    /* Check inputs */

    if (reltol < 0.)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSStolerances", MSG_BAD_RTOL);
        return (IDA_ILL_INPUT);
    }

    if (abstol < 0.)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSStolerances", MSG_BAD_ATOL);
        return (IDA_ILL_INPUT);
    }

    /* Copy tolerances into memory */

    relTol  = reltol;
    absTol  = abstol;
    info[1] = IDA_SS;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *         DDasrtRootInit
 *
 * =============================
 *
 * DDasrtRootInit initializes a rootfinding problem to be solved during the integration of the ODE system.
 * It loads the root function pointer and the number of root functions, and allocates workspace memory.
 * The return value is IDA_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int DDasrtRootInit (void * ddasrt_mem, int ng, DDASRootFn g)
{
    DDasrtMem ddas_mem;
    int nrt;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtRootInit", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    if (g == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtRootInit", MSG_ROOT_FUNC_NULL);
        return (IDA_ILL_INPUT);
    }

    g_fun  = g;
    nrt    = (ng < 0) ? 0 : ng;
    ng_fun = nrt;

    /* Allocate jroot and set it to zero */
    if (ng > 0)
    {
        jroot = calloc(ng, sizeof(int));
    }

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDasrtSetUserData
 *
 * =============================
 *
 * Sets a pointer to user_data that will be passed to the user's res function
 * every time a user-supplied function is called.
 */

int DDasrtSetUserData(void * ddasrt_mem, void * User_data)
{
    DDasrtMem ddas_mem;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtSetUserData", MSG_NO_MEM);
        return(IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    user_data = User_data;

    return(IDA_SUCCESS);
}

/* =============================
 *
 *       DDasrtSetMaxStep
 *
 * =============================
 *
 * Sets the maximum step size, stocked in rwork->hmax.
 * Sets iOpt to 1 for rwork->hmax to be taken in consideration by ddasrt().
 */

int DDasrtSetMaxStep (void * ddasrt_mem, realtype hMax)
{
    DDasrtMem ddas_mem;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtSetMaxStep", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    if (info[6] == 0)
    {
        info[6] = 1;
    }
    rwork->hmax = hMax;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDasrtSetStopTime
 *
 * =============================
 *
 * Specifies the time beyond which the integration is not to proceed, stocked in rwork->tcrit.
 * Sets iOpt to 1 for rwork->tcrit to be taken in consideration by ddasrt().
 */

int DDasrtSetStopTime (void * ddasrt_mem, realtype tCrit)
{
    DDasrtMem ddas_mem;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtSetStopTime", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    if (info[3] == 0)
    {
        info[3] = 1;
    }

    rwork->tcrit = tCrit;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *    DDasrtDlsSetDenseJacFn
 *
 * =============================
 *
 * Specifies the dense Jacobian function.
 */
/*
int DDasrtDlsSetDenseJacFn(void * ddasrt_mem, DDasrtDlsDenseJacFn Jac)
{
  DDasrtMem ddas_mem;
  DDasrtDlsMem ddasrtdls_mem;
*/
/* Return immediately if ddasrt_mem is NULL */
/*  if (DDasrt_mem == NULL) {
    DDASProcessError(NULL, IDADLS_MEM_NULL, "DDASRT", "DDasrtDlsSetDenseJacFn", MSGD_IDAMEM_NULL);
    return(IDADLS_MEM_NULL);
  }
  ddas_mem = (DDasrtMem) ddasrt_mem;
*//*
  if (lmem == NULL) {
    DDASProcessError(ddasrt_mem, IDADLS_LMEM_NULL, "DDASRT", "DDasrtDlsSetDenseJacFn", MSGD_LMEM_NULL);
    return(IDADLS_LMEM_NULL);
  }
  ddasrtdls_mem = (DDasrtDlsMem) lmem;
*/
/*  if (jac != NULL) {
    //jacDQ = FALSE;
    jac = Jac;
    info[4] = 1;
  } else {
    jacDQ = TRUE;
  }

  return(IDADLS_SUCCESS);
}
*/
/* =============================
 *
 *            DDasrt
 *
 * =============================
 *
 * This routine is the main driver of DDASRT.
 *
 * It integrates and looks for roots over a time interval defined by the user.
 *
 * The first time that DDasrt is called for a successfully initialized problem,
 * it computes a tentative initial step size h.
 *
 * DDasrt supports five modes, specified by itask: DDAS_NORMAL, DDAS_ONE_STEP, DDAS_MESH_STEP, DDAS_NORMAL_TSTOP, and DDAS_ONE_STEP_TSTOP.
 *
 * In the DDAS_NORMAL mode, the solver steps until it reaches or passes tout and then interpolates to obtain y(tOut).
 * In the DDAS_ONE_STEP mode, it takes one internal step and returns.
 * DDAS_MESH_STEP means stop at the first internal mesh point at or beyond t = tout and return.
 * DDAS_NORMAL_TSTOP and DDAS_ONE_STEP_TSTOP are similar to DDAS_NORMAL and DDAS_ONE_STEP, respectively,
 * but the integration never proceeds past tcrit (which must have been defined through a call to DDasrtSetStopTime).
 *
 * It returns IDA_ROOT_RETURN if a root was detected, IDA_SUCCESS if the integration went correctly,
 * or a corresponding error flag.
 */

int DDasrtSolve (void * ddasrt_mem, realtype tOut, realtype * tOld, N_Vector yOut, N_Vector ypOut, enum DDiTask_t itask)
{
    DDasrtMem ddas_mem;

    /* Check the input arguments */

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtSolve", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    if (yOut == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_YRET_NULL);
        return (IDA_ILL_INPUT);
    }

    if (ypOut == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_YPRET_NULL);
        return (IDA_ILL_INPUT);
    }

    if (tOld == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_TRET_NULL);
        return(IDA_ILL_INPUT);
    }

    if ((itask != DDAS_NORMAL) && (itask != DDAS_ONE_STEP))
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_BAD_ITASK);
        return (IDA_ILL_INPUT);
    }

    /* Retrieve nEq if it has changed, use a copy of the solution vector and stock the simulation times */
    *nEq   = NV_LENGTH_S(yOut);
    yVec   = NV_DATA_S(yOut);
    ypVec  = NV_DATA_S(ypOut);
    tStart = *tOld;
    tEnd   = tOut;

    /* Save the task mode in info[2] */
    info[2] = itask;

    /* Dummy arguments (unused) */
    double dummy1 = 0.;
    int dummy2 = 0;

    /* Launch the simulation with the memory space parameters.
       ddasrt() will update yVec, iState, rwork, iwork and jroot */
    C2F(ddasrt) (res, nEq, &tStart, yVec, ypVec, &tEnd, info, &relTol, &absTol, &iState, rwork, &lrw, iwork, &liw, &dummy1, &dummy2, &jac, g_fun, &ng_fun, jroot);

    /* Increment the start times */
    *tOld  = tStart;
    //tStart = tEnd;

    /* ddasrt() stocked the completion status in iState; return accordingly  */
    switch (iState)
    {
        case 4:
            return (IDA_ROOT_RETURN);
        case -1:
            DDASProcessError(ddas_mem, IDA_TOO_MUCH_WORK, "DDASRT", "DDasrtSolve", MSG_MAX_STEPS, tStart);
            return (IDA_TOO_MUCH_WORK);
        case -2:
            DDASProcessError(ddas_mem, IDA_TOO_MUCH_ACC, "DDASRT", "DDasrtSolve", MSG_TOO_MUCH_ACC, tStart);
            return (IDA_TOO_MUCH_ACC);
        case -3:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_BAD_ATOL, tStart);
            return (IDA_ILL_INPUT);
        case -6:
            DDASProcessError(ddas_mem, IDA_ERR_FAIL, "DDASRT", "DDasrtSolve", MSG_ERR_FAILS, tStart);
            return (IDA_ERR_FAIL);
        case -7:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASRT", "DDasrtSolve", MSG_CONV_FAILS, tStart);
            return (IDA_CONV_FAIL);
        case -8:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_SINGULAR);
            return (IDA_ILL_INPUT);
        case -9:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASRT", "DDasrtSolve", MSG_CONV_FAILS, tStart);
            return (IDA_CONV_FAIL);
        case -10:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASRT", "DDasrtSolve", MSG_CONV_FAILS, tStart);
            return (IDA_CONV_FAIL);
        case -11:
            DDASProcessError(ddas_mem, IDA_RES_FAIL, "DDASRT", "DDasrtSolve", MSG_RES_NONRECOV, tStart);
            return (IDA_RES_FAIL);
        case -12:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_IC_FAIL_CONSTR);
            return (IDA_ILL_INPUT);
        case -33:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASRT", "DDasrtSolve", MSG_BAD_INPUT);
            return (IDA_ILL_INPUT);
        default:
            return (IDA_SUCCESS);
    }
}

/* =============================
 *
 *       DDasrtGetRootInfo
 *
 * =============================
 *
 * Updates rootsfound[] to the computed roots stocked in jroot[].
 */

int DDasrtGetRootInfo (void * ddasrt_mem, int * rootsfound)
{
    DDasrtMem ddas_mem;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtGetRootInfo", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDasrtMem) ddasrt_mem;

    /* Copy jroot to rootsfound */
    memcpy(rootsfound, jroot, ng_fun * sizeof(int));

    return (IDA_SUCCESS);
}

/* =============================
 *
 *            DDASFree
 *
 * =============================
 *
 * This routine frees the problem memory allocated by DDasrtInit.
 */

void DDasrtFree (void ** ddasrt_mem)
{
    DDasrtMem ddas_mem;

    if (*ddasrt_mem == NULL)
    {
        return;
    }
    ddas_mem = (DDasrtMem) (*ddasrt_mem);

    /* Free the inner vectors */
    DDASFreeVectors (ddas_mem);

    free (*ddasrt_mem);
    *ddasrt_mem = NULL;
}

/* =============================
 *
 *         DDASFreeVectors
 *
 * =============================
 *
 * Frees the problem memory space vectors.
 */

void DDASFreeVectors (DDasrtMem ddas_mem)
{
    /* info, rwork, iwork and jroot have been allocated; free them */
    free (info);
    free (rwork);
    free (iwork);
    free (jroot);
}

/* =============================
 *
 *     DDasrtSetErrHandlerFn
 *
 * =============================
 *
 * Specifies the error handler function.
 */

int DDasrtSetErrHandlerFn (void * ddasrt_mem, DDASErrHandlerFn ehfun, void * eh_data)
{
    DDasrtMem ddas_mem;

    if (ddasrt_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASRT", "DDasrtSetErrHandlerFn", MSG_NO_MEM);
        return(IDA_MEM_NULL);
    }

    ddas_mem = (DDasrtMem) ddasrt_mem;

    ddas_mem->ehfun = ehfun;

    return(IDA_SUCCESS);
}

#define ehfun    ddas_mem->ehfun

/* =============================
 *
 *         DDASProcessError
 *
 * =============================
 *
 * Error handling function.
 */

void DDASProcessError (DDasrtMem ddas_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    /* Initialize the argument pointer variable
       (msgfmt is the last required argument to DDASProcessError) */
    va_start(ap, msgfmt);

    if (ddas_mem == NULL)      /* We write to stderr */
    {
#ifndef NO_FPRINTF_OUTPUT
        fprintf(stderr, "\n[%s ERROR]  %s\n  ", module, fname);
        fprintf(stderr, msgfmt);
        fprintf(stderr, "\n\n");
#endif
    }
    else                     /* We can call ehfun */
    {
        /* Compose the message */
        vsprintf(msg, msgfmt, ap);

        /* Call ehfun */
        ehfun(error_code, module, fname, msg, NULL);
    }

    /* Finalize argument processing */
    va_end(ap);

    return;
}
