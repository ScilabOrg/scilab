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

#include "RungeKutta45.h"
#include <sundials/sundials_types.h> // Definition of type realtype, booleantype

#if 1

#define NLS_MAXCOR  3

#define CORTES  0.1

#define ADDON   0.000001
#define BIAS2   6.0

#define MXNCF        10
#define MXNEF         7
#define MXNEF1        3
#define SMALL_NEF     2
#define LONG_WAIT    10

#define CRDOWN   0.3
#define RDIV     2.

#define errfp    (cv_mem->cv_errfp)
#define eh_data  (cv_mem->cv_eh_data)
#define ehfun    (cv_mem->cv_ehfun)

#define RTFOUND          +1
#define INITROOT         +2
#define CLOSERT          +3
#define DO_ERROR_TEST    +2
#define PREDICT_AGAIN    +3
#define CONV_FAIL        +4
#define TRY_AGAIN        +5
#define FIRST_CALL       +6
#define PREV_CONV_FAIL   +7
#define PREV_ERR_FAIL    +8
#define RHSFUNC_RECVR    +9
#define RTFOUND          +1
#define INITROOT         +2
#define CLOSERT          +3
#define ZERODETACHING    +4
#define MASKED           55

#endif

/* =============================
 * 
 * 			RKCreate
 * 
 * =============================
 *
 * Creating the problem
 */

void *RKCreate (int iter)
{
  // Step 1 : checking the arguments

  if ((iter != CV_FUNCTIONAL) && (iter != CV_NEWTON)) {
    RKProcessError (NULL, 0., "Runge-Kutta", "RKCreate", MSGCV_BAD_ITER);
    return (NULL);
  }
  
  // Step 2 : function body
  
  // Setting the workspace memory
  CVodeMem cv_mem;
  cv_mem = NULL;
  cv_mem = (CVodeMem) malloc(sizeof (struct CVodeMemRec));
  if (cv_mem == NULL) {
    RKProcessError (NULL, 0., "Runge-Kutta", "RKCreate", MSGCV_CVMEM_FAIL);
    return (NULL);
  }

  int i, j, maxord;
  maxord = 1;
  
  // Copying input parameters into cv_mem
  cv_mem->cv_iter = iter;
  
  // Setting unit roundoff
  cv_mem->cv_uround = UNIT_ROUNDOFF;
  
  // Setting default values
  cv_mem->cv_f        = NULL;
  cv_mem->cv_f_data   = NULL;
  cv_mem->cv_efun     = NULL;
  cv_mem->cv_e_data   = NULL;
  cv_mem->cv_ehfun    = CVErrHandler;
  cv_mem->cv_eh_data  = (void *) cv_mem;
  cv_mem->cv_errfp    = stderr;
  cv_mem->cv_qmax     = maxord;
  cv_mem->cv_mxstep   = MXSTEP_DEFAULT;
  cv_mem->cv_mxhnil   = MXHNIL_DEFAULT;
  cv_mem->cv_sldeton  = FALSE;
  cv_mem->cv_hin      = 0.;
  cv_mem->cv_hmin     = HMIN_DEFAULT;
  cv_mem->cv_hmax_inv = HMAX_INV_DEFAULT;
  cv_mem->cv_tstopset = FALSE;
  cv_mem->cv_maxcor   = NLS_MAXCOR;
  cv_mem->cv_maxnef   = MXNEF;
  cv_mem->cv_maxncf   = MXNCF;
  cv_mem->cv_nlscoef  = CORTES;

  // Initializing root finding variables
  cv_mem->cv_glo    = NULL;
  cv_mem->cv_ghi    = NULL;
  cv_mem->cv_grout  = NULL;
  cv_mem->cv_iroots = NULL;
  cv_mem->cv_gfun   = NULL;
  cv_mem->cv_g_data = NULL;
  cv_mem->cv_nrtfn  = 0;

  // Setting the integ-dependent maximum order
  cv_mem->cv_qmax_alloc = maxord;
  
  // Initializing lrw and liw
  cv_mem->cv_lrw = 58 + 2*L_MAX + NUM_TESTS;
  cv_mem->cv_liw = 40;

  // No mallocs have been done yet
  cv_mem->cv_VabstolMallocDone = FALSE;
  cv_mem->cv_MallocDone        = FALSE;

  // Returning a pointer to the RK memory block */
  return ((void *)cv_mem);
}

#if 1
#define iter  (cv_mem->cv_iter)
#define lrw  	(cv_mem->cv_lrw)
#define liw  	(cv_mem->cv_liw)
#endif

/* =============================
 * 
 * 			RKMalloc
 * 
 * =============================
 *
 * Allocating the problem
 */

int RKMalloc (void* cvode_mem, CVRhsFn f, realtype t0, N_Vector y0, int itol, realtype reltol, void *abstol)
{
  // Step 1 : checking the arguments
  
  if (cvode_mem==NULL) {
    RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKMalloc", MSGCV_NO_MEM);
    return (CV_MEM_NULL);
  }
  
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) cvode_mem;
  
  if (y0==NULL) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_NULL_Y0);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_NULL_Y0);
	else
	  return (CV_ILL_INPUT);
  }
  
  if ((itol != CV_SS) && (itol != CV_SV) && (itol != CV_WF)) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_BAD_ITOL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_BAD_ITOL);
	else
	  return (CV_ILL_INPUT);

  }

  if (f == NULL) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_NULL_F);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_NULL_F);
	else
	  return (CV_ILL_INPUT);
  }
  
  // Test if all required vector operations are implemented
  booleantype nvectorOK;
  nvectorOK = RKCheckNvector (y0);
  if (!nvectorOK) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_BAD_NVECTOR);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_BAD_NVECTOR);
	else
	  return (CV_ILL_INPUT);
  }
  
  // Testing tolerances
  if (itol != CV_WF) {
    if (abstol == NULL) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_NULL_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (CV_NULL_ABSTOL);
	  else
		return (CV_ILL_INPUT);
    }

    if (reltol < 0.) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_BAD_RELTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (CV_BAD_RELTOL);
	  else
		return (CV_ILL_INPUT);
    }
	
	booleantype neg_abstol;
    if (itol == CV_SS)
      neg_abstol = (*((realtype *)abstol) < 0.);
    else
      neg_abstol = (N_VMin((N_Vector)abstol) < 0.);

    if (neg_abstol) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_BAD_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (CV_BAD_ABSTOL);
	  else
		return (CV_ILL_INPUT);
    }
  }
  
  long int lrw1, liw1;
  if (y0->ops->nvspace != NULL)
    N_VSpace (y0, &lrw1, &liw1);
  else {
    lrw1 = 0;
    liw1 = 0;
  }
  cv_mem->cv_lrw1 = lrw1;
  cv_mem->cv_liw1 = liw1;
  
  // Step 2 : function body
   
  // Allocating the vectors, taking y0 as a template
  booleantype allocOK;
  allocOK = RKAllocVectors(cv_mem, y0, itol);
  if (!allocOK) {
    RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKMalloc", MSGCV_MEM_FAIL);
    return (CV_MEM_FAIL);
  }
  
  // Copying tolerances into memory
  cv_mem->cv_itol   = itol;
  cv_mem->cv_reltol = reltol;      

  if (itol == CV_SS)
    cv_mem->cv_Sabstol = *((realtype *)abstol);
  else if (itol == CV_SV)
    N_VScale(1., (N_Vector)abstol, cv_mem->cv_Vabstol);

  // Copying the input parameters into RK memory block

  cv_mem->cv_f  = f;
  cv_mem->cv_tn = t0;
  cv_mem->cv_tlo = t0;

  // Setting step parameters

  cv_mem->cv_q      = 1;
  cv_mem->cv_L      = 2;
  cv_mem->cv_qwait  = cv_mem->cv_L;

  cv_mem->cv_qu    = 0;
  cv_mem->cv_hu    = 0.;
  cv_mem->cv_tolsf = 1.;

  // Setting the linear solver addresses to NULL. We check != NULL later, in RK(), if using CV_NEWTON.)
  cv_mem->cv_linit  = NULL;
  cv_mem->cv_lsetup = NULL;
  cv_mem->cv_lsolve = NULL;
  cv_mem->cv_lfree  = NULL;
  cv_mem->cv_lmem   = NULL;

  // Initializing zn[0] in the history array
  N_VScale(1., y0, cv_mem->cv_zn[0]);

  // Initializing all the counters
  cv_mem->cv_nst     = 0;
  cv_mem->cv_nfe     = 0;
  cv_mem->cv_ncfn    = 0;
  cv_mem->cv_netf    = 0;
  cv_mem->cv_nni     = 0;
  cv_mem->cv_nsetups = 0;
  cv_mem->cv_nhnil   = 0;
  cv_mem->cv_nstlp   = 0;
  cv_mem->cv_nscon   = 0;
  cv_mem->cv_nge     = 0;

  // Initializing other integrator optional outputs
  cv_mem->cv_h0u      = 0.;
  cv_mem->cv_next_h   = 0.;
  cv_mem->cv_next_q   = 0;

  // Initializing Stablilty Limit Detection data
  /* NOTE: We do this even if stab lim det was not turned on yet.
   * This way, the user can turn it on at any time */
  cv_mem->cv_nor = 0;
  int i, k;
  for (i = 1; i <= 5; ++i)
    for (k = 1; k <= 3; ++k) 
      cv_mem->cv_ssdat[i-1][k-1] = 0.;

  // The problem has been successfully initialized
  cv_mem->cv_MallocDone = TRUE;
  return (CV_SUCCESS); 
}

#if 1
#define lrw1 (cv_mem->cv_lrw1)
#define liw1 (cv_mem->cv_liw1)
#endif

/* =============================
 * 
 * 			RKReInit
 * 
 * =============================
 *
 * Reinitializing the problem
 */

int RKReInit (void* cvode_mem, CVRhsFn f, realtype t0, N_Vector y0, int itol, realtype reltol, void *abstol)
{
  // Step 1 : checking the arguments
  
  // Checking cvode_mem
  if (cvode_mem==NULL) {
    RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKReInit", MSGCV_NO_MEM);
    return (CV_MEM_NULL);
  }
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) cvode_mem;
  
  // Checking if cv_mem was allocated
  if (cv_mem->cv_MallocDone == FALSE) {
    RKProcessError (cv_mem, CV_NO_MALLOC, "Runga-Kutta", "RKReInit", MSGCV_NO_MALLOC);
    return (CV_NO_MALLOC);
  }
  
  // Checking the other arguments
  if (y0 == NULL) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_NULL_Y0);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_NULL_Y0);
	else
	  return (CV_ILL_INPUT);
  }
  
  if ((itol != CV_SS) && (itol != CV_SV) && (itol != CV_WF)) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_BAD_ITOL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_BAD_ITOL);
	else
	  return (CV_ILL_INPUT);
  }

  if (f == NULL) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_NULL_F);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_NULL_F);
	else
	  return (CV_ILL_INPUT);
  }

  // Testing tolerances
  if (itol != CV_WF) {

    if (abstol == NULL) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_NULL_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (CV_NULL_ABSTOL);
	  else
	    return (CV_ILL_INPUT);
    }

    if (reltol < 0.) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_BAD_RELTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (CV_BAD_RELTOL);
	  else
	    return (CV_ILL_INPUT);
    }
    
    booleantype neg_abstol;
    if (itol == CV_SS)
      neg_abstol = (*((realtype *)abstol) < 0.);
    else
      neg_abstol = (N_VMin((N_Vector)abstol) < 0.);
    
    if (neg_abstol) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKodeReInit", MSGCV_BAD_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (CV_BAD_ABSTOL);
	  else
        return (CV_ILL_INPUT);
    }
  }

  // Step 2 : function body
  
  // Recopying tolerances into memory
  if ( (itol != CV_SV) && (cv_mem->cv_VabstolMallocDone) ) {
    N_VDestroy(cv_mem->cv_Vabstol);
    lrw -= lrw1;
    liw -= liw1;
    cv_mem->cv_VabstolMallocDone = FALSE;
  }

  if ( (itol == CV_SV) && !(cv_mem->cv_VabstolMallocDone) ) {
    cv_mem->cv_Vabstol = NULL;
    cv_mem->cv_Vabstol = N_VClone(y0);
    lrw += lrw1;
    liw += liw1;
    cv_mem->cv_VabstolMallocDone = TRUE;
  }

  cv_mem->cv_itol   = itol;
  cv_mem->cv_reltol = reltol;    
  
  if (itol == CV_SS)
    cv_mem->cv_Sabstol = *((realtype *)abstol);
  else if (itol == CV_SV)
    N_VScale(1., (N_Vector)abstol, cv_mem->cv_Vabstol);
  
  // Recopying the input parameters into RK memory block
  cv_mem->cv_f = f;
  cv_mem->cv_tn = t0;
  
  // Resetting step parameters
  cv_mem->cv_q      = 1;
  cv_mem->cv_L      = 2;
  cv_mem->cv_qwait  = cv_mem->cv_L;

  cv_mem->cv_qu    = 0;
  cv_mem->cv_hu    = 0.;
  cv_mem->cv_tolsf = 1.;

  // Reinitializing zn[0] in the history array
  N_VScale(1., y0, cv_mem->cv_zn[0]);
 
  // Reinitializing all the counters
  cv_mem->cv_nst     = 0;
  cv_mem->cv_nfe     = 0;
  cv_mem->cv_ncfn    = 0;
  cv_mem->cv_netf    = 0;
  cv_mem->cv_nni     = 0;
  cv_mem->cv_nsetups = 0;
  cv_mem->cv_nhnil   = 0;
  cv_mem->cv_nstlp   = 0;
  cv_mem->cv_nscon   = 0;
  cv_mem->cv_nge     = 0;

  // Reinitializing other integrator optional outputs
  cv_mem->cv_h0u      = 0.;
  cv_mem->cv_next_h   = 0.;
  cv_mem->cv_next_q   = 0;

  // Initialize Stablilty Limit Detection data
  cv_mem->cv_nor = 0;
  int i, k;
  for (i = 1; i <= 5; ++i)
    for (k = 1; k <= 3; ++k) 
      cv_mem->cv_ssdat[i-1][k-1] = 0.;
  
  // The problem has been successfully re-initialized
  return (CV_SUCCESS);
}

#if 1
#define gfun   (cv_mem->cv_gfun)
#define g_data (cv_mem->cv_g_data)
#define glo    (cv_mem->cv_glo)
#define ghi    (cv_mem->cv_ghi)
#define grout  (cv_mem->cv_grout)
#define iroots (cv_mem->cv_iroots)
#define tlo    (cv_mem->cv_tlo)
#define zn     (cv_mem->cv_zn)
#endif

/* =============================
 * 
 * 			RKRootInit
 * 
 * =============================
 *
 * Initializing the Root Finding problem
 */

int RKRootInit (void* cvode_mem, int nrtfn, CVRootFn g, void *gdata)
{
  // Checking cvode_mem
  if (cvode_mem == NULL) {
    RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKRootInit", MSGCV_NO_MEM);
    return (CV_MEM_NULL);
  }
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) cvode_mem;
  
  int nrt;
  nrt = (nrtfn < 0) ? 0 : nrtfn;

  /* If rerunning RKRootInit() with a different number of root functions (changing number of gfun components),
   * then freeing currently held memory resources */
  if ((nrt != cv_mem->cv_nrtfn) && (cv_mem->cv_nrtfn > 0)) {
    free(glo); glo = NULL;
    free(ghi); ghi = NULL;
    free(grout); grout = NULL;
    free(iroots); iroots = NULL;

    lrw -= 3* (cv_mem->cv_nrtfn);
    liw -= cv_mem->cv_nrtfn;
  }
  
  /* If RKRootInit() was called with nrtfn == 0, then setting cv_nrtfn to
   * zero and cv_gfun to NULL before returning */
  if (nrt == 0) {
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
  if (nrt == cv_mem->cv_nrtfn) {
    if (g != gfun) {
      if (g == NULL) {
		free(glo); glo = NULL;
		free(ghi); ghi = NULL;
		free(grout); grout = NULL;
		free(iroots); iroots = NULL;
        lrw -= 3*nrt;
        liw -= nrt;
        RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKRootInit", MSGCV_NULL_G);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (CV_NULL_G);
		else
		  return (CV_ILL_INPUT);
      }
      else {
		gfun = g;
		return (CV_SUCCESS);
      }
    }
    else return (CV_SUCCESS);
  }

  // Step 2 : function body
  
  // Setting variables in RK memory block
  cv_mem->cv_nrtfn = nrt;
  if (g == NULL) {
    RKProcessError(cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKRootInit", MSGCV_NULL_G);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_NULL_G);
	else
	  return (CV_ILL_INPUT);
  }
  else gfun = g;

  // Allocating necessary memory and returning
  glo = NULL;
  glo = (realtype *) malloc(nrt*sizeof(realtype));
  if (glo == NULL) {
    RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
    return (CV_MEM_FAIL);
  }
  
  // Initializing glo
  int retval;
  retval = gfun (tlo, zn[0], glo, g_data);
  if (retval != 0) return (CV_RTFUNC_FAIL);

  ghi = NULL;
  ghi = (realtype *) malloc(nrt*sizeof(realtype));
  if (ghi == NULL) {
    free(glo); glo = NULL;
    RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
    return (CV_MEM_FAIL);
  }

  grout = NULL;
  grout = (realtype *) malloc(nrt*sizeof(realtype));
  if (grout == NULL) {
    free(glo); glo = NULL;
    free(ghi); ghi = NULL;
    RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
    return (CV_MEM_FAIL);
  }

  iroots = NULL;
  iroots = (int *) malloc(nrt*sizeof(int));
  if (iroots == NULL) {
    free(glo); glo = NULL; 
    free(ghi); ghi = NULL;
    free(grout); grout = NULL;
    RKProcessError(cv_mem, CV_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGCV_MEM_FAIL);
    return (CV_MEM_FAIL);
  }

  lrw += 3*nrt;
  liw += nrt;

  return (CV_SUCCESS);
}

#if 1
/* =================================================================
 * Readibility Constants
 * =================================================================*/
#define f              (cv_mem->cv_f)      
#define f_data         (cv_mem->cv_f_data) 
#define efun           (cv_mem->cv_efun)
#define e_data         (cv_mem->cv_e_data) 
#define qmax           (cv_mem->cv_qmax)
#define mxstep         (cv_mem->cv_mxstep)
#define mxhnil         (cv_mem->cv_mxhnil)
#define sldeton        (cv_mem->cv_sldeton)
#define hin            (cv_mem->cv_hin)
#define hmin           (cv_mem->cv_hmin)
#define hmax_inv       (cv_mem->cv_hmax_inv)
#define istop          (cv_mem->cv_istop)
#define tstop          (cv_mem->cv_tstop)
#define tstopset       (cv_mem->cv_tstopset)
#define maxnef         (cv_mem->cv_maxnef)
#define maxncf         (cv_mem->cv_maxncf)
#define maxcor         (cv_mem->cv_maxcor)
#define nlscoef        (cv_mem->cv_nlscoef)
#define itol           (cv_mem->cv_itol)         
#define reltol         (cv_mem->cv_reltol)       
#define Sabstol        (cv_mem->cv_Sabstol)
#define Vabstol        (cv_mem->cv_Vabstol)

#define uround         (cv_mem->cv_uround)
#define ewt            (cv_mem->cv_ewt)
#define y              (cv_mem->cv_y)
#define acor           (cv_mem->cv_acor)
#define tempv          (cv_mem->cv_tempv)
#define ftemp          (cv_mem->cv_ftemp) 
#define q              (cv_mem->cv_q)
#define qprime         (cv_mem->cv_qprime)
#define next_q         (cv_mem->cv_next_q)
#define qwait          (cv_mem->cv_qwait)
#define L              (cv_mem->cv_L)
#define h              (cv_mem->cv_h)
#define hprime         (cv_mem->cv_hprime)
#define next_h         (cv_mem->cv_next_h)
#define eta            (cv_mem->cv_eta) 
#define etaqm1         (cv_mem->cv_etaqm1) 
#define etaq           (cv_mem->cv_etaq) 
#define etaqp1         (cv_mem->cv_etaqp1) 
#define nscon          (cv_mem->cv_nscon)
#define hscale         (cv_mem->cv_hscale)
#define tn             (cv_mem->cv_tn)
#define tau            (cv_mem->cv_tau)
#define tq             (cv_mem->cv_tq)
#define l              (cv_mem->cv_l)
#define rl1            (cv_mem->cv_rl1)
#define gamma          (cv_mem->cv_gamma) 
#define gammap         (cv_mem->cv_gammap) 
#define gamrat         (cv_mem->cv_gamrat)
#define crate          (cv_mem->cv_crate)
#define acnrm          (cv_mem->cv_acnrm)
#define mnewt          (cv_mem->cv_mnewt)
#define nst      		  (cv_mem->cv_nst)
#define nfe            (cv_mem->cv_nfe)
#define ncfn           (cv_mem->cv_ncfn)
#define netf           (cv_mem->cv_netf)
#define nni            (cv_mem->cv_nni)
#define nsetups        (cv_mem->cv_nsetups)
#define nhnil          (cv_mem->cv_nhnil)
#define linit          (cv_mem->cv_linit)
#define lsetup         (cv_mem->cv_lsetup)
#define lsolve         (cv_mem->cv_lsolve) 
#define lfree          (cv_mem->cv_lfree) 
#define lmem           (cv_mem->cv_lmem) 
#define qu             (cv_mem->cv_qu)          
#define nstlp          (cv_mem->cv_nstlp)  
#define h0u            (cv_mem->cv_h0u)
#define hu             (cv_mem->cv_hu)         
#define saved_tq5      (cv_mem->cv_saved_tq5)  
#define indx_acor      (cv_mem->cv_indx_acor)
#define jcur           (cv_mem->cv_jcur)         
#define tolsf          (cv_mem->cv_tolsf)      
#define setupNonNull   (cv_mem->cv_setupNonNull) 
#define nor            (cv_mem->cv_nor)
#define ssdat          (cv_mem->cv_ssdat)

#define nrtfn 		  (cv_mem->cv_nrtfn)
#define thi            (cv_mem->cv_thi)
#define tretlast       (cv_mem->cv_tretlast)
#define toutc          (cv_mem->cv_toutc)
#define trout          (cv_mem->cv_trout)
#define ttol           (cv_mem->cv_ttol)
#define taskc          (cv_mem->cv_taskc)
#define irfnd          (cv_mem->cv_irfnd)
#define nge            (cv_mem->cv_nge)

#endif



/* =============================
 * 
 * 				RK
 * 
 * =============================
 *
 * Solving the problem
 */

int RK (void* cvode_mem, realtype t, N_Vector yout, realtype *told, int pass_tF_or_back)
{
  // Step 1 : checking the arguments
  
  // Checking cvode_mem
  if (cvode_mem == NULL) {
    RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RK", MSGCV_NO_MEM);
    return (CV_MEM_NULL);
  }
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) cvode_mem;

  // Checking cv_mem was allocated
  if (cv_mem->cv_MallocDone == FALSE) {
    RKProcessError (cv_mem, CV_NO_MALLOC, "Runge-Kutta", "RK", MSGCV_NO_MALLOC);
    return (CV_NO_MALLOC);
  }
  
  // Checking yout
  if (yout == NULL) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_YOUT_NULL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_YOUT_NULL);
	else
	  return (CV_ILL_INPUT);
  }

  // Checking told
  if (told == NULL) {
    RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_TRET_NULL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (CV_TRET_NULL);
	else
	  return (CV_ILL_INPUT);
  }
  
  // Checking itask
  if ((pass_tF_or_back != CV_NORMAL)       && 
      (pass_tF_or_back != CV_ONE_STEP)     &&
      (pass_tF_or_back != CV_NORMAL_TSTOP) &&
      (pass_tF_or_back != CV_ONE_STEP_TSTOP) ) {
		RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_BAD_ITASK);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (CV_BAD_ITASK);
		else
		  return (CV_ILL_INPUT);
  }

  // Splitting itask into task and istop
  if ((pass_tF_or_back == CV_NORMAL_TSTOP) || (pass_tF_or_back == CV_ONE_STEP_TSTOP)) {
    if (tstopset == FALSE) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RK", MSGCV_NO_TSTOP);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (CV_NO_TSTOP);
	  else
	    return (CV_ILL_INPUT);
    }
    istop = TRUE;
  }
  else istop = FALSE;
  int task;
  if ((pass_tF_or_back == CV_NORMAL) || (pass_tF_or_back == CV_NORMAL_TSTOP)) {
    task = CV_NORMAL; toutc = t;
  }
  else task = CV_ONE_STEP;
  taskc = task;
  
  /* Step 2 : function body
   * nst = 0 means that the solver is called for the first time.
   * Performing initializations :
   *    - initializing Nordsieck history array
   *    - splitting the interval
   *    - check for approach to a root
   */
  
  int retval;   // Error flag
  
  // First time the solver is called
  if (nst == 0) {
    retval = f(tn, zn[0], zn[1], f_data);  // Setting zn[1] = y'(t0) = f(t0, y0), Nordsieck history array
    
    // Checking for errors in the evaluations of f thanks to retval
    if (retval < 0) {
      RKProcessError(cv_mem, CV_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FAILED, tn);
      return (CV_RHSFUNC_FAIL);
	}
	if (retval > 0) {
      RKProcessError(cv_mem, CV_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FIRST);
      return (CV_FIRST_RHSFUNC_ERR);
	}
  }
  
  // Initializing some variables
  long int n_points;
  int i, j;
  realtype interval_size;
  
  tn = *told;  // Tn at 0
  
  // Splitting the interval, h ~ Sabstol
  interval_size = t-*told;
  if (interval_size <= Sabstol) {  // "Small" interval, h is the size of it
	n_points = 2;
	h = interval_size;
  }
  else {
	realtype test_div = interval_size/Sabstol;
	int floor_test = floor(test_div);
    if (test_div-floor_test == 0.) {  // Testing if we can cut the interval into Sabstol-long parts
	  n_points = floor_test+1;
      h = interval_size/(n_points-1);
    }
    else {  // Adding a point and h < Sabstol to fit the interval
      n_points = floor_test+2;
      h = interval_size/(n_points-1);
    }
  }
  
  /* Proceeding to :
   * K1 = F(Tn, Yn),
   * K2 = F(Tn + h/2, Yn + (h/2)*K1),
   * K3 = F(Tn + h/2, Yn + (h/2)*K2),
   * K4 = F(Tn + h, Yn + h*K3),
   * Yn+1 = Yn + (h/6)*(K1 + 2K2 + 2K3 + K4)
   */
  
  N_Vector K[4];				 			// K = [K1, K2, K3, K4], Runge-Kutta coefficients
  int problem_size = NV_LENGTH_S(yout);	// Size of the problem, number of equations
  y = N_VNew_Serial (problem_size);			// Storage vector, temporary
  
  // Allocating K and checking success
  for (i=0; i<=3; ++i) {
    K[i] = NULL;
    K[i] = N_VNew_Serial (problem_size);
    if (K[i] == NULL) {
      N_VDestroy(ewt); N_VDestroy(acor); N_VDestroy(tempv); N_VDestroy(ftemp);
      for (j=0; j<i; ++j) N_VDestroy (K[j]);
      return (FALSE);
    }
  }
  
  // Main loop on time
  i = 0;  // i-th step
  
  //while (ABS(tn - t) > 1.0e-10) {				// As long as tn if strictly inferior to t
  for (i=0; i<=n_points-2; ++i) {			// Fixed number of points, so a 'for' is possible, to bypass 'ABS(tn+(i*h) - t)' rounding error

	retval = f(tn, zn[0], K[0], f_data);				// K[0] = K1,
    N_VLinearSum_Serial (h/2, K[0], 1., zn[0], y);		// y = K1*h/2 + Yn,
    retval = f(tn + h/2, y, K[1], f_data);				// K[1] = K2 = f(Tn+h/2, Yn + (h/2)*K1),
    N_VLinearSum_Serial (1., K[0], 2., K[1], y); 		// y = K1 + 2K2,

    N_VLinearSum_Serial (h/2, K[1], 1., zn[0], K[0]);	// K[0] (reusing) = Yn + K2*h/2,
    retval = f(tn + h/2, K[0], K[2], f_data);			// K[2] = K3 = f(Tn+h/2, Yn + (h/2)*K2),
    N_VLinearSum_Serial (1., y, 2., K[2], y);			// y = K1 + 2K2 + 2K3,

    N_VLinearSum_Serial (h, K[2], 1., zn[0], K[1]); 	// K[1] (reusing) = Yn + K3*h,
    retval = f(tn + h, K[1], K[3], f_data);			// K[3] = K4 = f(Tn+h/2, Yn + h*K3),
    N_VLinearSum_Serial (1., y, 1., K[3], y);			// y = K1 + 2K2 + 2K3 + K4,

    N_VLinearSum_Serial(1., zn[0], h/6, y, yout);		// yout = Yn+1 = Yn + y*h/6

    // Checking for errors in the evaluations of f thanks to retval
	if (retval < 0) {
      RKProcessError(cv_mem, CV_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FAILED, tn);
      return (CV_RHSFUNC_FAIL);
    }
	if (retval > 0) {
	  RKProcessError(cv_mem, CV_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FIRST);
      return (CV_FIRST_RHSFUNC_ERR);
	}

    // Updating the Nordsieck history array to take the new values
    N_VScale (1., yout, zn[0]);			// zn[0] = yout
    retval = f(tn, zn[0], zn[1], f_data);	// zn[1] = y'(tn+h)

	// Incrementing tn => taking a step (increment i if using while loop)
	tn += h;
	//i++;

	// Checking for root in last step taken
    if (nrtfn > 0) {
      retval = RKRootFind (cv_mem);
	  if (retval == RTFOUND) {  // A new root was found
        irfnd = 1;
        tretlast = *told = tlo;
        return (CV_ROOT_RETURN);
      }
      else if (retval == CV_RTFUNC_FAIL) {  // g failed
				RKProcessError(cv_mem, CV_RTFUNC_FAIL, "Runge-Kutta", "RKRootFind", MSGCV_RTFUNC_FAILED, tlo);
				return (CV_RTFUNC_FAIL);
			  }
	  // SUNDIALS EXTENSION
	  if (is_sundials_with_extension())
	    if (retval == ZERODETACHING) {  // Zero detaching
		  irfnd = 1;
          tretlast = *told = tlo;
          return (CV_ZERO_DETACH_RETURN);
		}
    }
	nst++; // Number of loop calls within cold restarts

  } // End of time loop

  // Freeing the N_Vectors K[]
  for (i=0; i<=3; ++i) N_VDestroy(K[i]);

  // Success : start time (*told) := end time (t), and return the success flag
  *told = tn;
  return (CV_SUCCESS);
}



// Freeing the problem memory allocated by RKMalloc
void RKFree (void** cvode_mem)
{
  CVodeMem cv_mem;
  if (*cvode_mem == NULL) return;

  cv_mem = (CVodeMem) (*cvode_mem);
  
  RKFreeVectors (cv_mem);

  if (iter == CV_NEWTON && lfree != NULL) lfree (cv_mem);

  if (nrtfn > 0) {
    free(glo); glo = NULL;
    free(ghi); ghi = NULL;
    free(grout); grout = NULL;
    free(iroots); iroots = NULL;
  }

  free(*cvode_mem);
  *cvode_mem = NULL;
}

// Freeing the RK vectors allocated in RKAllocVectors
static void RKFreeVectors (CVodeMem cv_mem)
{
  int j, maxord;
  
  maxord = cv_mem->cv_qmax_alloc;

  N_VDestroy(ewt);
  N_VDestroy(acor);
  N_VDestroy(tempv);
  N_VDestroy(ftemp);
  for (j=0; j <= maxord; ++j) N_VDestroy(zn[j]);

  lrw -= (maxord + 5)*lrw1;
  liw -= (maxord + 5)*liw1;

  if (cv_mem->cv_VabstolMallocDone) {
    N_VDestroy(Vabstol);
    lrw -= lrw1;
    liw -= liw1;
  }
}

// Allocating the RK vectors ewt, acor, tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
static booleantype RKAllocVectors (CVodeMem cv_mem, N_Vector tmpl, int tol)
{
  int i, j;

  // Allocating ewt, acor, tempv, ftemp
  ewt = NULL;
  ewt = N_VClone (tmpl);
  if (ewt == NULL) return (FALSE);

  acor = NULL;
  acor = N_VClone (tmpl);
  if (acor == NULL) {
    N_VDestroy(ewt);
    return (FALSE);
  }

  tempv = NULL;
  tempv = N_VClone (tmpl);
  if (tempv == NULL) {
    N_VDestroy(ewt);
    N_VDestroy(acor);
    return (FALSE);
  }

  ftemp = NULL;
  ftemp = N_VClone (tmpl);
  if (ftemp == NULL) {
    N_VDestroy(tempv);
    N_VDestroy(ewt);
    N_VDestroy(acor);
    return (FALSE);
  }

  // Allocating zn[0] ... zn[1]. RK only solves 1-order equations
  for (j=0; j <= qmax; ++j) {
    zn[j] = NULL;
    zn[j] = N_VClone (tmpl);
    if (zn[j] == NULL) {
      N_VDestroy(ewt);
      N_VDestroy(acor);
      N_VDestroy(tempv);
      N_VDestroy(ftemp);
      for (i=0; i < j; ++i) N_VDestroy(zn[i]);
      return (FALSE);
    }
  }

  // Updating solver workspace lengths
  lrw += (qmax + 5)*lrw1;
  liw += (qmax + 5)*liw1;

  if (tol == CV_SV) {
    Vabstol = NULL;
    Vabstol = N_VClone (tmpl);
    if (Vabstol == NULL) {
      N_VDestroy(ewt);
      N_VDestroy(acor);
      N_VDestroy(tempv);
      N_VDestroy(ftemp);
      for (i=0; i <= qmax; ++i) N_VDestroy(zn[i]);
      return (FALSE);
    }
    lrw += lrw1;
    liw += liw1;
    cv_mem->cv_VabstolMallocDone = TRUE;
  }

  // Storing the value of qmax used here
  cv_mem->cv_qmax_alloc = qmax;
  
  return (TRUE);
}

// Specifying the time beyond which the integration is not to proceed
int RKSetStopTime (CVodeMem cvode_mem, realtype itstop)
{
  if (cvode_mem==NULL) {
    RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKSetStopTime", MSGCV_NO_MEM);
    return (CV_MEM_NULL);
  }
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) cvode_mem;
  cv_mem->cv_tstop = itstop;
  cv_mem->cv_tstopset = TRUE;

  return (CV_SUCCESS);
}

// Error handling function
void RKProcessError (CVodeMem cv_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
  va_list ap;
  char msg[256];

  // Initializing the argument pointer variable (msgfmt is the last required argument to RKProcessError)
  va_start (ap, msgfmt);

  if (cv_mem == NULL) {
    // Writing to stderr
	#ifndef NO_FPRINTF_OUTPUT
      fprintf (stderr, "\n[%s ERROR]  %s\n  ", module, fname);
      fprintf (stderr, msgfmt);
      fprintf (stderr, "\n\n");
	#endif
  }
  else {
    // Composing the message
    vsprintf(msg, msgfmt, ap);

    // Calling ehfun
    ehfun (error_code, module, fname, msg, eh_data);
  }
  // Finalizing argument process
  va_end(ap);
  return;
}

// Default error handling function
void RKErrHandler (int error_code, const char *module, const char *function, char *msg, void *data)
{
  char err_type[10];

  if (error_code == CV_WARNING) sprintf(err_type,"WARNING");
  else sprintf(err_type,"ERROR");

  #ifndef NO_FPRINTF_OUTPUT
  if (errfp != NULL) {
    fprintf(errfp,"\n[%s %s]  %s\n",module,err_type,function);
    fprintf(errfp,"  %s\n\n",msg);
  }
  #endif
  return;
}

// Checking if all required vector operations are present
static booleantype RKCheckNvector (N_Vector tmpl)
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
    return (FALSE);
  else
    return (TRUE);
}

/* Completing the initialization of rootfinding memory information,
 * and checking whether g has a zero both at and very near the initial point of the IVP.
 *
 * Returning an int equal to :
 *  INITROOT = -1 if a close pair of zeros was found, and
 *  CV_SUCCESS = 0 otherwise.
 */
static int RKRcheck1 (CVodeMem cv_mem)
{
  int i, retval;
  realtype smallh, hratio;
  booleantype zroot;

  for (i=0; i<nrtfn; ++i) iroots[i] = 0;
  tlo = tn;
  ttol = (ABS(tn) + ABS(h))*uround*100.;

  // Evaluating g at t0 and checking for zeros
  retval = gfun (tlo, zn[0], glo, g_data);
  nge = 1;
  if (retval != 0) return (CV_RTFUNC_FAIL);

  zroot = FALSE;
  for (i=0; i<nrtfn; ++i) {
    if (ABS(glo[i]) == 0.) zroot = TRUE;
  }
  if (!zroot) return (CV_SUCCESS);

  // At least one zero in g at t0; checking g at t0+(small increment)
  hratio = MAX(ttol/ABS(h), 0.1);
  smallh = hratio*h;
  tlo += smallh;
  N_VLinearSum (1., zn[0], hratio, zn[1], y);
  retval = gfun (tlo, y, glo, g_data);
  nge++;
  if (retval != 0) return (CV_RTFUNC_FAIL);

  zroot = FALSE;
  for (i=0; i<nrtfn; i++) {
    if (ABS(glo[i]) == 0.) {
      zroot = TRUE;
      iroots[i] = 1;
    }
  }
  if (zroot) return (INITROOT);
  return (CV_SUCCESS);
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
 *            which components g_i have a root at trout.
 * 			  For i = 0, ..., nrtfn-1, iroots[i] = 1 if g_i has a root
 *            and iroots[i] = 0 otherwise.
 *
 * Returning an int equal to:
 *      RTFOUND = 1 if a root of g was found, or
 *      CV_SUCCESS = 0 otherwise.
 */
static int RKRootFind (CVodeMem cv_mem)
{
	int i, j, retval = CV_SUCCESS;
	
	// Setting y = y(thi) and thi = next t
	N_VScale (1., zn[0], y);
	thi = tn;
	
	// Setting ghi = g(thi), incrementing gfun counter and searching [tlo,thi] for roots
	retval = gfun (thi, y, ghi, g_data);
	nge++;
	if (retval != 0) return (CV_RTFUNC_FAIL);

	//printf("c3e tlo = %E, thi = %E, tn = %E, h = %E\n", tlo, thi, tn, h);

	// Checking for zeros in ghi or sign change, and update iroots accordingly
	for (i=0; i<nrtfn; ++i) {
	  iroots[i] = 0;
	  if (ghi[i] == 0.) {  // ghi contains a zero
		iroots[i] = (glo[i] > 0.) ? -1 : 1;
		retval = RTFOUND;
	  }
	  else 
	    if (glo[i]*ghi[i] < 0) {  // Sign change
		  iroots[i] = (ghi[i]>glo[i]) ? 1 : -1;
		  retval = RTFOUND;
	    }
	}
    
    // No root found : incrementing tlo and glo and returning CV_SUCCESS
	tlo = thi;
	for (i=0; i<nrtfn; ++i) glo[i] = ghi[i];
	return (retval);
}

