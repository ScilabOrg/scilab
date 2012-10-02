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

/* =============================
 *
 *            lsodar
 *
 * =============================
 *
 * Actual solving function
 */

extern void C2F(lsodar) (CVRhsFnLSodar *f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, int *itask, int *istate, int *iopt, realtype *rwork, int *lrw, int *iwork, int *liw,  int *Jac, int *Jactype, CVRootFnLSodar *grblk, int *ng, int *jroot);

/* =============================
 *
 *         LSodarCreate
 *
 * =============================
 *
 * Creates the problem
 */

void * LSodarCreate (int neq, int ng)
{
    /* Allocate the problem memory space */
    LSodarMem cvode_mem = NULL;
    cvode_mem = (LSodarMem) malloc(sizeof(struct LSodarMemRec));
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, 0, "LSODAR", "LSodarCreate", MSGCV_CVMEM_FAIL);
        return (NULL);
    }

    /* Initialize/Allocate the problem variables */
    int lrn = 20 + 16 * neq + 3 * ng;
    int lrs = 22 + neq * (9 + neq) + 3 * ng;
    int Liw = 20 + neq;
    int Lrw = MAX(lrn, lrs);

    /* Copy the variables into the problem memory space */
    cvode_mem->nEquations = neq;
    cvode_mem->iState = 1;
    cvode_mem->iOpt = 0;
    cvode_mem->rwork = NULL;
    cvode_mem->lrw = Lrw;
    cvode_mem->iwork = NULL;
    cvode_mem->liw = Liw;
    cvode_mem->Jac = 0;
    cvode_mem->Jactype = 2;
    cvode_mem->g_fun = NULL;
    cvode_mem->ng_fun = ng;
    cvode_mem->jroot = NULL;

    return ((void *) cvode_mem);
}

# define rwork   cv_mem->rwork
# define lrw     cv_mem->lrw
# define iwork   cv_mem->iwork
# define liw     cv_mem->liw

/* =============================
 *
 *         LSodarMalloc
 *
 * =============================
 *
 * Allocates the problem
 */

int LSodarMalloc (void * cvode_mem, CVRhsFnLSodar f, realtype t0, N_Vector y0, int itol, realtype reltol, void * abstol)
{
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) cvode_mem;

    if (f == NULL)
    {
        CVProcessError(cv_mem, CV_ILL_INPUT, "LSODAR", "LSodarMalloc", MSGCV_NULL_F);
        return (CV_ILL_INPUT);
    }

    if (y0 == NULL)
    {
        CVProcessError(cv_mem, CV_ILL_INPUT, "LSODAR", "LSodarMalloc", MSGCV_NULL_Y0);
        return (CV_ILL_INPUT);
    }

    int i;

    cv_mem->func = f;
    cv_mem->yVector = NV_DATA_S(y0);
    cv_mem->tStart = t0;
    cv_mem->iTol = itol;
    cv_mem->relTol = reltol;
    cv_mem->absTol = *((realtype *) abstol);
    rwork = malloc(sizeof(realtype) * lrw);
    iwork = malloc(sizeof(int) * liw);
    for (i = 0; i < lrw; ++i)
    {
        rwork[i] = 0;
    }
    for (i = 0; i < liw; ++i)
    {
        iwork[i] = 0;
    }

    return (CV_SUCCESS);
}

# define jroot   cv_mem->jroot

/* =============================
 *
 *         LSodarRootInit
 *
 * =============================
 *
 * Allocates the rootfinding problem
 */

int LSodarRootInit (void * cvode_mem, int ng, CVRootFnLSodar g, void * gdata)
{
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return(CV_MEM_NULL);
    }
    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) cvode_mem;


    cv_mem->g_fun = g;
    cv_mem->ng_fun = ng;

    int i;
    if (ng > 0)
    {
        jroot = malloc(sizeof(int) * ng);
        for (i = 0; i < ng; ++i)
        {
            jroot[i] = 0;
        }
    }

    return (CV_SUCCESS);
}

# define iOpt    cv_mem->iOpt

/* =============================
 *
 *         LSodarSetMaxStep
 *
 * =============================
 *
 * Sets the maximum step size
 */

int LSodarSetMaxStep (void * cvode_mem, realtype hmax)
{
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) cvode_mem;

    if (iOpt == 0)
    {
        iOpt = 1;
    }
    rwork[5] = hmax;
    return (CV_SUCCESS);
}

# define func    cv_mem->func
# define nEq     cv_mem->nEquations
# define yVec    cv_mem->yVector
# define iTol    cv_mem->iTol
# define relTol  cv_mem->relTol
# define absTol  cv_mem->absTol
# define iState  cv_mem->iState
# define Jac     cv_mem->Jac
# define Jactype cv_mem->Jactype
# define g_fun   cv_mem->g_fun
# define ng_fun  cv_mem->ng_fun

/* =============================
 *
 *         LSodarReInit
 *
 * =============================
 *
 * Reinitializes the problem
 */

int LSodarReInit (void * cvode_mem, CVRhsFnLSodar f, realtype tOld, N_Vector y, int itol, realtype reltol, void * abstol)
{
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) cvode_mem;

    iState = 1;
    nEq = NV_LENGTH_S(y);
    realtype * yVector = NV_DATA_S(y);
    func = f;
    realtype tStartReInit = tOld;
    realtype tEndReInit = tStartReInit + 0.001;
    yVec = NV_DATA_S(y);
    iTol = itol;
    relTol = reltol;
    absTol = *((realtype *) abstol);
    int itask = CV_NORMAL;

    C2F(lsodar) (func, &nEq, yVector, &tStartReInit, &tEndReInit, &iTol, &relTol, &absTol, &itask, &iState, &iOpt, rwork, &lrw, iwork, &liw, &Jac, &Jactype, g_fun, &ng_fun, jroot);

    return (CV_SUCCESS);
}

# define tStart  cv_mem->tStart
# define tEnd    cv_mem->tEnd

/* =============================
 *
 *            LSodar
 *
 * =============================
 *
 * Solves the problem
 */

int LSodar (void * cvode_mem, realtype tOut, N_Vector yOut, realtype * tOld, int itask)
{
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) cvode_mem;

    if (yOut == NULL)
    {
        LSProcessError(cv_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_YOUT_NULL);
        return (CV_ILL_INPUT);
    }

    nEq = NV_LENGTH_S(yOut);
    yVec = NV_DATA_S(yOut);
    tStart = *tOld;

    tEnd = tOut;
    C2F(lsodar) (func, &nEq, yVec, &tStart, &tEnd, &iTol, &relTol, &absTol, &itask, &iState, &iOpt, rwork, &lrw, iwork, &liw, &Jac, &Jactype, g_fun, &ng_fun, jroot);

    *tOld = tOut;
    tStart = tEnd;

    switch (iState)
    {
        case 3:
            return (CV_ROOT_RETURN);
        case -1:
            LSProcessError(cv_mem, CV_TOO_MUCH_WORK, "LSODAR", "LSodar", MSGCV_MAX_STEPS, tStart);
            return (CV_TOO_MUCH_WORK);
        case -2:
            LSProcessError(cv_mem, CV_TOO_MUCH_ACC, "LSODAR", "LSodar", MSGCV_TOO_MUCH_ACC, tStart);
            return (CV_TOO_MUCH_ACC);
        case -3:
            LSProcessError(cv_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_BAD_INPUT, tStart);
            return (CV_ILL_INPUT);
        case -4:
            LSProcessError(cv_mem, CV_ERR_FAILURE, "LSODAR", "LSodar", MSGCV_ERR_FAILS, tStart);
            return (CV_ERR_FAILURE);
        case -5:
            LSProcessError(cv_mem, CV_CONV_FAILURE, "LSODAR", "LSodar", MSGCV_CONV_FAILS, tStart);
            return (CV_CONV_FAILURE);
        case -6:
            LSProcessError(cv_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_EWT_NOW_BAD, tStart);
            return (CV_ILL_INPUT);
        default:
            return (CV_SUCCESS);
    }
}

/* =============================
 *
 *       LSodarGetRootInfo
 *
 * =============================
 *
 * Updates rootsfound to the computed joots
 */

int LSodarGetRootInfo (void * cvode_mem, int * rootsfound)
{
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) cvode_mem;
    int i;

    for (i = 0; i < ng_fun; ++i)
    {
        rootsfound[i] = jroot[i];
    }

    return (CV_SUCCESS);
}

/* =============================
 *
 *            LSFree
 *
 * =============================
 *
 * Frees the problem memory space
 */

void LSodarFree (void ** cvode_mem)
{
    if (*cvode_mem == NULL)
    {
        return;
    }

    LSodarMem cv_mem = NULL;
    cv_mem = (LSodarMem) (*cvode_mem);
    LSFreeVectors (cv_mem);
    free (*cvode_mem);
    *cvode_mem = NULL;
}

/* =============================
 *
 *         LSFreeVectors
 *
 * =============================
 *
 * Frees the problem memory space vectors
 */

void LSFreeVectors (LSodarMem cv_mem)
{
    //free (yVec);
    free (rwork);
    free (iwork);
    free (jroot);
}

#define ehfun    LSErrHandler
#define eh_data  (void *) cvode_mem

/* =============================
 *
 *         LSProcessError
 *
 * =============================
 *
 * Error handling function
 */

void LSProcessError (LSodarMem cvode_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    /* Initialize the argument pointer variable
       (msgfmt is the last required argument to CVProcessError) */

    va_start(ap, msgfmt);

    if (cvode_mem == NULL)      /* We write to stderr */
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

        ehfun(error_code, module, fname, msg, eh_data);

    }

    /* Finalize argument processing */

    va_end(ap);

    return;

}

#define errfp    stderr

/* =============================
 *
 *          LSErrHandler
 *
 * =============================
 *
 * Default error handling function
 */

void LSErrHandler (int error_code, const char *module, const char *function, char *msg, void *data)
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

