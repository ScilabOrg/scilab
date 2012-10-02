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
 * 			lsodar
 *
 * =============================
 *
 * Actual solving function
 */

extern void C2F(lsodar) (CVRhsFnLSodar *f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, int *itask, int *istate, int *iopt, realtype *rwork, int *lrw, int *iwork, int *liw,  int *Jac, int *Jactype, CVRootFnLSodar *grblk, int *ng, int *jroot);

/* =============================
 *
 * 			LSodarCreate
 *
 * =============================
 *
 * Create the problem
 */

void * LSodarCreate (CVRhsFnLSodar f, int *neq, N_Vector y, int itol, realtype reltol, realtype abstol, int itask, CVRootFnLSodar gfun, int ng, int *jroot)
{

    LSodarMem cvode_mem;
    cvode_mem = NULL;
    cvode_mem = (LSodarMem) malloc(sizeof(struct LSodarMemRec));
    if (cvode_mem == NULL)
    {
        LSProcessError(NULL, 0, "LSODAR", "LSodarCreate", MSGCV_CVMEM_FAIL);
        return(NULL);
    }


    int i = 0;
    realtype * yVec = NV_DATA_S(y);
    int lrw = 20 + 16 * (*neq) + 3 * ng;
    int liw = 20 + (*neq);
    realtype * rwork = NULL;
    int * iwork = NULL;
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
    realtype t0 = 0.;
    realtype t0PlusEps = 0.1;

    cvode_mem->func = f;
    cvode_mem->nEquations = neq;
    cvode_mem->yVector = yVec;
    cvode_mem->tStart = t0;
    cvode_mem->tEnd = t0PlusEps;
    cvode_mem->iTol = itol;
    cvode_mem->relTol = reltol;
    cvode_mem->absTol = abstol;
    cvode_mem->iTask = itask;
    cvode_mem->iState = 1;
    cvode_mem->iOpt = 0;
    cvode_mem->rWork = rwork;
    cvode_mem->Lrw = lrw;
    cvode_mem->iWork = iwork;
    cvode_mem->Liw = liw;
    cvode_mem->Jac = 0;
    cvode_mem->Jactype = 2;
    cvode_mem->g_fun = gfun;
    cvode_mem->ng_fun = ng;
    cvode_mem->jRoot = jroot;

    C2F(lsodar) (cvode_mem->func, cvode_mem->nEquations, cvode_mem->yVector, &(cvode_mem->tStart), &cvode_mem->tEnd, &cvode_mem->iTol, &cvode_mem->relTol, &cvode_mem->absTol, &cvode_mem->iTask, &(cvode_mem->iState), &cvode_mem->iOpt, cvode_mem->rWork, &cvode_mem->Lrw, cvode_mem->iWork, &cvode_mem->Liw, &cvode_mem->Jac, &cvode_mem->Jactype, cvode_mem->g_fun, &cvode_mem->ng_fun, cvode_mem->jRoot);

    return ((void *) cvode_mem);
}

/* =============================
 *
 * 			LSodar
 *
 * =============================
 *
 * Solve the problem
 */

# define func    cv_mem->func
# define yVec    cv_mem->yVector
# define iTol    cv_mem->iTol
# define relTol  cv_mem->relTol
# define absTol  cv_mem->absTol
# define iState  cv_mem->iState
# define iOpt    cv_mem->iOpt
# define rWork   cv_mem->rWork
# define Lrw     cv_mem->Lrw
# define iWork   cv_mem->iWork
# define Liw     cv_mem->Liw
# define Jac     cv_mem->Jac
# define Jactype cv_mem->Jactype
# define g_fun   cv_mem->g_fun
# define ng_fun  cv_mem->ng_fun
# define jroot  cv_mem->jRoot

int LSodar (void * cvode_mem, realtype tOut, N_Vector y, realtype * tOld, int itask, int * neq, int * jroots)
{

    LSodarMem cv_mem;
    cv_mem = (LSodarMem) cvode_mem;

    yVec = NV_DATA_S(y);
    realtype told = *tOld;
    jroot = jroots;

    C2F(lsodar) (func, neq, yVec, &told, &tOut, &iTol, &relTol, &absTol, &itask, &iState, &iOpt, rWork, &Lrw, iWork, &Liw, &Jac, &Jactype, g_fun, &ng_fun, jroot);

    NV_DATA_S(y) = yVec;
    *tOld = tOut;

    if (iState == 2)
    {
        return (CV_SUCCESS);
    }
    if (iState == 3)
    {
        return (CV_ROOT_RETURN);
    }
}

/* =============================
 *
 * 			LSFree
 *
 * =============================
 *
 * Free the problem memory space
 */

void LSodarFree (void **cvode_mem)
{
    LSodarMem cv_mem;

    if (*cvode_mem == NULL)
    {
        return;
    }

    cv_mem = (LSodarMem) (*cvode_mem);

    LSFreeVectors(cv_mem);

    free (*cvode_mem);
    *cvode_mem = NULL;
}

/* =============================
 *
 * 			LSFreeVectors
 *
 * =============================
 *
 * Free the problem memory space vectors
 */

static void LSFreeVectors (LSodarMem cv_mem)
{
    //free (yVec);
    free (rWork);
    free (iWork);
    //free (jroot);
}

/* =============================
 *
 * 			LSProcessError
 *
 * =============================
 *
 * Error handling function
 */

#define ehfun    LSErrHandler
#define eh_data  (void *) cvode_mem

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

/* =============================
 *
 * 			LSErrHandler
 *
 * =============================
 *
 * Default error handling function
 */

#define errfp    stderr

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
