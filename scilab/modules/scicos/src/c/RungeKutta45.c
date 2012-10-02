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
//#include <sundials/sundials_math.h>


#if 1

#define ZERO    0.0      // Real 0.0
#define TINY    1.0e-10  // Epsilon
#define TENTH   0.1      // Real 0.1
#define HALF    0.5      // Real 0.5
#define ONE     1.0      // Real 1.0
#define TWO     2.0      // Real 2.0
#define FIVE    5.0      // Real 5.0
#define TWELVE  12.0     // Real 12.0
#define HUN     100.0    // Real 100.0
#define ONEPSM  1.000001 // Real 1.000001

#define NLS_MAXCOR  3

#define CORTES  0.1

#define ETAMX1  10000.0
#define ETAMXF  0.2
#define ETAMIN  0.1
#define ETACF   0.25
#define ADDON   0.000001
#define BIAS2   6.0

#define MXNCF        10
#define MXNEF         7
#define MXNEF1        3
#define SMALL_NEF     2
#define LONG_WAIT    10

#define CRDOWN   0.3
#define RDIV     TWO

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

void *RKCreate (int lmm, int iter)
{
  
  // Step 1 : checking the arguments
  
  if ((lmm != CV_ADAMS) && (lmm != CV_BDF)) {
    RKProcessError (NULL, ZERO, "Runge-Kutta", "RKCreate", MSGCV_BAD_LMM);
    return (NULL);
  }
  if ((iter != CV_FUNCTIONAL) && (iter != CV_NEWTON)) {
    RKProcessError (NULL, ZERO, "Runge-Kutta", "RKCreate", MSGCV_BAD_ITER);
    return (NULL);
  }
  
  // Step 2 : function body
  
  // Setting the workspace memory
  CVodeMem cv_mem;
  cv_mem = NULL;
  cv_mem = (CVodeMem) malloc(sizeof (struct CVodeMemRec));
  if (cv_mem == NULL) {
    RKProcessError (NULL, ZERO, "Runge-Kutta", "RKCreate", MSGCV_CVMEM_FAIL);
    return (NULL);
  }

  int maxord;
  maxord = (lmm == CV_ADAMS) ? ADAMS_Q_MAX : BDF_Q_MAX;
  maxord = 1; // Maybe RK, so maxord = 1 ?
  
  // Copying input parameters into cv_mem
  cv_mem->cv_lmm  = lmm;
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
  cv_mem->cv_hin      = ZERO;
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
#define lmm   (cv_mem->cv_lmm) 
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

    if (reltol < ZERO) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGCV_BAD_RELTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (CV_BAD_RELTOL);
	  else
		return (CV_ILL_INPUT);
    }
	
	booleantype neg_abstol;
    if (itol == CV_SS)
      neg_abstol = (*((realtype *)abstol) < ZERO);
    else
      neg_abstol = (N_VMin((N_Vector)abstol) < ZERO);

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
    N_VScale(ONE, (N_Vector)abstol, cv_mem->cv_Vabstol);

  // Copying the input parameters into RK memory block

  cv_mem->cv_f  = f;
  cv_mem->cv_tn = t0;
  cv_mem->cv_tlo = t0;

  // Setting step parameters

  cv_mem->cv_q      = 1;
  cv_mem->cv_L      = 2;
  cv_mem->cv_qwait  = cv_mem->cv_L;
  cv_mem->cv_etamax = ETAMX1;

  cv_mem->cv_qu    = 0;
  cv_mem->cv_hu    = ZERO;
  cv_mem->cv_tolsf = ONE;

  // Setting the linear solver addresses to NULL. We check != NULL later, in RK(), if using CV_NEWTON.)
  cv_mem->cv_linit  = NULL;
  cv_mem->cv_lsetup = NULL;
  cv_mem->cv_lsolve = NULL;
  cv_mem->cv_lfree  = NULL;
  cv_mem->cv_lmem   = NULL;

  // Initializing zn[0] in the history array
  N_VScale(ONE, y0, cv_mem->cv_zn[0]);

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
  cv_mem->cv_h0u      = ZERO;
  cv_mem->cv_next_h   = ZERO;
  cv_mem->cv_next_q   = 0;

  // Initializing Stablilty Limit Detection data
  /* NOTE: We do this even if stab lim det was not turned on yet.
   * This way, the user can turn it on at any time */
  cv_mem->cv_nor = 0;
  int i, k;
  for (i = 1; i <= 5; ++i)
    for (k = 1; k <= 3; ++k) 
      cv_mem->cv_ssdat[i-1][k-1] = ZERO;

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

    if (reltol < ZERO) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGCV_BAD_RELTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (CV_BAD_RELTOL);
	  else
	    return (CV_ILL_INPUT);
    }
    
    booleantype neg_abstol;
    if (itol == CV_SS)
      neg_abstol = (*((realtype *)abstol) < ZERO);
    else
      neg_abstol = (N_VMin((N_Vector)abstol) < ZERO);
    
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
    N_VScale(ONE, (N_Vector)abstol, cv_mem->cv_Vabstol);
  
  // Recopying the input parameters into RK memory block
  cv_mem->cv_f = f;
  cv_mem->cv_tn = t0;
  
  // Resetting step parameters
  cv_mem->cv_q      = 1;
  cv_mem->cv_L      = 2;
  cv_mem->cv_qwait  = cv_mem->cv_L;
  cv_mem->cv_etamax = ETAMX1;

  cv_mem->cv_qu    = 0;
  cv_mem->cv_hu    = ZERO;
  cv_mem->cv_tolsf = ONE;

  // Reinitializing zn[0] in the history array
  N_VScale(ONE, y0, cv_mem->cv_zn[0]);
 
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
  cv_mem->cv_h0u      = ZERO;
  cv_mem->cv_next_h   = ZERO;
  cv_mem->cv_next_q   = 0;

  // Initialize Stablilty Limit Detection data
  cv_mem->cv_nor = 0;
  int i, k;
  for (i = 1; i <= 5; ++i)
    for (k = 1; k <= 3; ++k) 
      cv_mem->cv_ssdat[i-1][k-1] = ZERO;
  
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
	
  // Step 1 : checking the arguments
  
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
     zero and cv_gfun to NULL before returning */
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
#define etamax         (cv_mem->cv_etamax)
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
  
  
  
  
  
  
  
  
  
  /* Step 2 : nst = 0 means that the solver is called for the first time.
   * Performing initializations :
   *    - initial setup
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
	
    if (nrtfn > 0) {printf("ap nst\n");
      retval = RKRcheck1(cv_mem);
      printf("retval c1 = %d\n", retval);
      if (retval == INITROOT) {
        RKProcessError(cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKRcheck1", MSGCV_BAD_INIT_ROOT);
		// SUNDIALS EXTENSION
	    if (is_sundials_with_extension())
		  return(CV_BAD_INIT_ROOT);
	    else
		  return(CV_ILL_INPUT);
      }
      else if (retval == CV_RTFUNC_FAIL) {
		      RKProcessError(cv_mem, CV_RTFUNC_FAIL, "Runge-Kutta", "RKRcheck1", MSGCV_RTFUNC_FAILED, tn);
			  return(CV_RTFUNC_FAIL);
            }
    }
  }
  
  
  
  // At least one Zero-Crossing block
  /*if (nrtfn > 0) {
	tlo = tn;  // Saving t0 for later root finding
    retval = RKRootFind (cv_mem);  // Looking for zero crossings, maybe use check2 because out of main loop ?
    if (retval == INITROOT) {
      CVProcessError(cv_mem, CV_ILL_INPUT, "CVODE", "RKRcheck1", MSGCV_BAD_INIT_ROOT);
	  // SUNDIALS EXTENSION
	  if (is_sundials_with_extension())
	    return (CV_BAD_INIT_ROOT);
	  else
	    return (CV_ILL_INPUT);
    }
    else if (retval == CV_RTFUNC_FAIL) {  // Error with the root evaluation function
		    CVProcessError(cv_mem, CV_RTFUNC_FAIL, "CVODE", "RKRcheck1", MSGCV_RTFUNC_FAILED, tn);
			return (CV_RTFUNC_FAIL);
		  }
    else if (retval == RTFOUND) {  // Root found
			irfnd = 1;
			tretlast = *told = tlo;
			return (CV_ROOT_RETURN);
		}
  }*/
  
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
	int test_div = interval_size/Sabstol, floor_test;
    floor_test = floor(test_div);
    if (test_div-floor_test == ZERO) {  // Testing if we can cut the interval into Sabstol-long parts.
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
  
  N_Vector K[4], Kprime[4];				 	// K = [K1, K2, K3, K4], Runge-Kutta coefficients, Kprime = [K1h/2, K2h/2, K3h, K4]
  int problem_size = NV_LENGTH_S(yout);	// Size of the problem, number of equations
  y = N_VNew_Serial (problem_size);			// Storage vector, temporary
  
  // Allocating K, Kprime and checking success
  for (i=0; i<=3; ++i) {
    K[i] = Kprime[i] = NULL;
    K[i] = N_VNew_Serial (problem_size);
    Kprime[i] = N_VNew_Serial (problem_size);
    if (K[i] == NULL) {
      N_VDestroy(ewt); N_VDestroy(acor); N_VDestroy(tempv); N_VDestroy(ftemp);
      for (j=0; j<i; ++j) N_VDestroy (K[j]);
      return (FALSE);
    }
    if (Kprime[i] == NULL) {
	  N_VDestroy(ewt); N_VDestroy(acor); N_VDestroy(tempv); N_VDestroy(ftemp);
      for (j=0; j<i; ++j) N_VDestroy (Kprime[j]);
      return (FALSE);
    }
  }
//printf("tn = %E, h = %E, n = %ld, tn+h = %E, t = %E\n", tn, h, n_points, tn+h, t);
  
  // Looping on time
  i = 0;  // i-th step
  
  //while (ABS(tn - t) > TINY) {				// As long as tn if strictly inferior to t
  for (i=0; i<=n_points-2; ++i) {			// Fixed number of points, so a 'for' is possible, to bypass 'ABS(tn+(i*h) - t)' rounding error

    // Computing K1, K2, K3, K4 by evaluating f
    retval = f (tn, yout, K[0], f_data);		// K[0] = K1,
    N_VScale_Serial (h/2, K[0], Kprime[0]);	// K1 *= h/2,
    N_VLinearSum_Serial (ONE, Kprime[0], ONE, yout, y);			// y += K1;
    retval = f (tn + h/2, y, K[1], f_data);	// K[1] = K2 = f(Tn+h/2, Yn+ (h/2)*K1),
    N_VScale_Serial (h/2, K[1], Kprime[1]);	// K2 *= h/2,
    N_VLinearSum_Serial (ONE, Kprime[1], ONE, yout, y);			// y += K2,
    retval = f (tn + h/2, y, K[2], f_data);	// K[2] = K3 = f(Tn+h/2, Yn + (h/2)*K2)),
    N_VScale_Serial (h, K[2], Kprime[2]);		// K3 *= h,
    N_VLinearSum_Serial (ONE, Kprime[2], ONE, yout, y);			// y += K3,
    retval = f (tn + h, y, K[3], f_data);		// K[3] = K4 = f(Tn+h, Yn + h*K3)
    // Checking for errors in the evaluations of f thanks to retval
	if (retval < 0) {
      RKProcessError(cv_mem, CV_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FAILED, tn);
      return (CV_RHSFUNC_FAIL);
    }
	if (retval > 0) {
	  RKProcessError(cv_mem, CV_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FIRST);
      return (CV_FIRST_RHSFUNC_ERR);
	}
	
    // Multiplying K2 and K3 by 2 and computing Yn+1
    N_VLinearSum_Serial (ONE, K[0], TWO, K[1], K[1]);	// K2 = K1 + 2K2,
    N_VLinearSum_Serial (TWO, K[2], ONE, K[3], K[3]);	// K4 = 2K3 + K4,
    N_VLinearSum_Serial (ONE, K[1], ONE, K[3], K[3]);	// K4 = K1 + K4, so K4 = K1 + 2K2 + 2K3 + K4 (initial vectors),
    N_VLinearSum_Serial (ONE, yout, h/6, K[3], yout);	// yout = Yn+1 = Yn + K4*h/6
    
    // Updating the Nordsieck history array to take the new values
    N_VScale (ONE, yout, zn[0]);			// zn[0] = yout
    retval = f(tn, zn[0], zn[1], f_data);	// zn[1] = y'(tn)
	
	// Incrementing tn => taking a step (increment i if using while loop)
	tn += h;
	//i++;
	
	// Checking for root in last step taken
    if (nrtfn > 0) {
      retval = RKRootFind (cv_mem);
	  if (retval == RTFOUND) {  // A new root was found
        irfnd = 1;
        tretlast = *told = tlo;
        printf("RTF\n");
        return (CV_ROOT_RETURN);
      }
      else if (retval == CV_RTFUNC_FAIL) { // g failed
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
  
  // Success : start time (*told) := end time (t), and return the success flag
  *told = tn;
  //printf("RK Success\n");
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

/* Setting the error weight vector ewt according to tol_type as follows:
 *
 *  (1) ewt[i] = 1 / (reltol * ABS(ycur[i]) + *abstol), i=0,...,neq-1
 *     if tol_type = CV_SS
 *  (2) ewt[i] = 1 / (reltol * ABS(ycur[i]) + abstol[i]), i=0,...,neq-1
 *     if tol_type = CV_SV
 */
int RKEwtSet (N_Vector ycur, N_Vector weight, void *data)
{
  int flag = 0;

  // Here, data points to cv_mem
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) data;

  switch (itol) {
    case CV_SS:
      flag = RKEwtSetSS (cv_mem, ycur, weight);
      break;
    case CV_SV:
      flag = RKEwtSetSV (cv_mem, ycur, weight);
      break;
  }
  
  return (flag);
}

static int RKEwtSetSS (CVodeMem cv_mem, N_Vector ycur, N_Vector weight)
{
  N_VAbs (ycur, tempv);
  N_VScale (reltol, tempv, tempv);
  N_VAddConst (tempv, Sabstol, tempv);
  if (N_VMin(tempv) <= ZERO) return (-1);
  N_VInv(tempv, weight);
  return (0);
}

static int RKEwtSetSV (CVodeMem cv_mem, N_Vector ycur, N_Vector weight)
{
  N_VAbs (ycur, tempv);
  N_VLinearSum(reltol, tempv, ONE, Vabstol, tempv);
  if (N_VMin(tempv) <= ZERO) return (-1);
  N_VInv(tempv, weight);
  return (0);
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

//Checking input consistency
static int RKInitialSetup (CVodeMem cv_mem)
{
  int ier;
  int ewtsetOK;

  // Checking if efun is provided
  if (itol != CV_WF) {
    efun = RKEwtSet;
    e_data = (void *)cv_mem;
  }
  else {
    if (efun == NULL) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGCV_NO_EFUN);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		  return (CV_NO_EFUN);
	  }
	  else
	  {
		  return (CV_ILL_INPUT);
	  }
    }
  }

  ewtsetOK = efun (zn[0], ewt, e_data);
  if (ewtsetOK != 0) {

    if (itol == CV_WF){ 
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGCV_EWT_FAIL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		return (CV_EWT_FAIL);
	  }
	  else
	  {
		return (CV_ILL_INPUT);
	  }
    }
    else { 
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGCV_BAD_EWT);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		  return (CV_BAD_EWT);
	  }
	  else
	  {
		  return (CV_ILL_INPUT);
	  }
    }
  }
  
  // Checking if lsolve function exists (if needed) and calling linit function (if it exists)
  if (iter == CV_NEWTON) {
    if (lsolve == NULL) {
      RKProcessError (cv_mem, CV_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGCV_LSOLVE_NULL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		return (CV_LSOLVE_NULL);
	  }
	  else
	  {
		return (CV_ILL_INPUT);
	  }
    }
    if (linit != NULL) {
      ier = linit (cv_mem);
      if (ier != 0) {
        RKProcessError (cv_mem, CV_LINIT_FAIL, "Runge-Kutta", "RKInitialSetup", MSGCV_LINIT_FAIL);
        return (CV_LINIT_FAIL);
      }
    }
  }
  
  return (CV_SUCCESS);
}

/* Performing various update operations when the solution to the nonlinear system has passed the local error test. 
 * Incrementing the step counter nst, recording the values hu and qu, updating the tau array, and applying the corrections to the zn array.
 * The tau[i] are the last q values of h, with tau[1] the most recent.
 * Decrementing qwait, and if qwait == 1 (and q < qmax), saving acor and tq[5] for a possible order increase
 */
static void RKCompleteStep (CVodeMem cv_mem)
{
  int i, j;
  
  nst++;
  nscon++;
  hu = h;
  qu = q;

  for (i=q; i >= 2; i--)  tau[i] = tau[i-1];
  if ((q==1) && (nst > 1)) tau[2] = tau[1];
  tau[1] = h;

  for (j=0; j <= q; ++j) 
    N_VLinearSum(l[j], acor, 1, zn[j], zn[j]);
  qwait--;
  if ((qwait == 1) && (q != qmax)) {
    N_VScale (1, acor, zn[qmax]);
    saved_tq5 = tq[5];
    indx_acor = qmax;
  }
}

/* Rescaling the Nordsieck array by multiplying the jth column zn[j] by eta^j, j = 1, ..., q.
 * Then rescaling h by eta, and resetting hscale to h
 */
static void RKRescale (CVodeMem cv_mem)
{
  int j;
  realtype factor;
  
  factor = eta;
  for (j=1; j <= q; ++j) {
    N_VScale (factor, zn[j], zn[j]);
    factor *= eta;
  }
  h = hscale * eta;
  next_h = h;
  hscale = h;
  nscon = 0;
}

// Adjusting the history array on a change of order q by deltaq when lmm == CV_ADAMS
static void RKAdjustAdams (CVodeMem cv_mem, int deltaq)
{
  int i, j;
  realtype xi, hsum;

  // On an order increase, setting new column of zn to zero and returning
  
  if (deltaq==1) {
    N_VConst (ZERO, zn[L]);
    return;
  }

  /* On an order decrease, adjusting each zn[j] by a multiple of zn[q].
   * The coefficients in the adjustment are the ones of the polynomial:
   *        x
   * q * INT { u * ( u + xi_1 ) * ... * ( u + xi_{q-2} ) } du 
   *        0
   * where xi_j = [t_n - t_(n-j)]/h => xi_0 = 0
   */
  for (i=0; i <= qmax; ++i) l[i] = ZERO;
  l[1] = ONE;
  hsum = ZERO;
  for (j=1; j <= q-2; ++j) {
    hsum += tau[j];
    xi = hsum / hscale;
    for (i=j+1; i >= 1; i--) l[i] = l[i]*xi + l[i-1];
  }
  
  for (j=1; j <= q-2; ++j) l[j+1] = q * (l[j] / (j+1));
  
  for (j=2; j < q; ++j)
    N_VLinearSum(-l[j], zn[q], ONE, zn[j], zn[j]);
}

/* Handling adjustments to the history array on a change of order by deltaq when lmm == CV_BDF.
 * Calling RKIncreaseBDF if deltaq = +1 and RKDecreaseBDF if deltaq = -1 to do the actual work
 */
static void RKAdjustBDF (CVodeMem cv_mem, int deltaq)
{
  switch (deltaq) {
    case 1 :
      RKIncreaseBDF (cv_mem);
      return;
    case -1:
      RKDecreaseBDF (cv_mem);
      return;
  }
}

/* Adjusting the history array on an increase in the order q in the case that lmm == CV_BDF.  
 * Setting a new column zn[q+1] equal to a multiple of the saved vector (= acor) in zn[indx_acor].
 * Then adjusting each zn[j] by a multiple of zn[q+1].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h
 */
static void RKIncreaseBDF (CVodeMem cv_mem)
{
  realtype alpha0, alpha1, prod, xi, xiold, hsum, A1;
  int i, j;
  
  for (i=0; i <= qmax; ++i) l[i] = ZERO;
  l[2] = alpha1 = prod = xiold = ONE;
  alpha0 = -ONE;
  hsum = hscale;
  if (q > 1) {
    for (j=1; j < q; ++j) {
      hsum += tau[j+1];
      xi = hsum / hscale;
      prod *= xi;
      alpha0 -= ONE / (j+1);
      alpha1 += ONE / xi;
      for (i=j+2; i >= 2; i--) l[i] = l[i]*xiold + l[i-1];
      xiold = xi;
    }
  }
  A1 = (-alpha0 - alpha1) / prod;
  N_VScale (A1, zn[indx_acor], zn[L]);
  for (j=2; j <= q; ++j) {
    N_VLinearSum(l[j], zn[L], ONE, zn[j], zn[j]);
  }  
}

/* Adjusting the history array on a decrease in the order q when lmm == CV_BDF.  
 * Adjusting each zn[j] by a multiple of zn[q].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h
 */
static void RKDecreaseBDF (CVodeMem cv_mem)
{
  realtype hsum, xi;
  int i, j;
  
  for (i=0; i <= qmax; ++i) l[i] = ZERO;
  l[2] = ONE;
  hsum = ZERO;
  for (j=1; j <= q-2; ++j) {
    hsum += tau[j];
    xi = hsum /hscale;
    for (i=j+2; i >= 2; i--) l[i] = l[i]*xi + l[i-1];
  }
  
  for (j=2; j < q; ++j)
    N_VLinearSum(-l[j], zn[q], ONE, zn[j], zn[j]);
}

/* Handling an order change by a deltaq (= +1 or -1) amount.
 * If a decrease in order is requested and q==2, then returning the routine immediately.
 * Otherwise, calling RKAdjustAdams or RKAdjustBDF to handle the order change (depending on the value of lmm)
 */
static void RKAdjustOrder (CVodeMem cv_mem, int deltaq)
{
  if ((q==2) && (deltaq != 1)) return;
  
  switch (lmm) {
    case CV_ADAMS: 
      RKAdjustAdams (cv_mem, deltaq);
      break;
    case CV_BDF:   
      RKAdjustBDF (cv_mem, deltaq);
      break;
  }
}

/* Performing the local error test. 
 * Loading the weighted local error norm dsm into *dsmPtr, and testing dsm ?<= 1.
 *
 * If the test passes, returning CV_SUCCESS. 
 * If the test fails, undoing the step just taken (calling RKRestore) and 
 *   - if maxnef error test failures have occurred or if ABS(h) = hmin,
 *     returning CV_ERR_FAILURE.
 *   - if more than MXNEF1 error test failures have occurred, forcing an order reduction.
 * 	   If already at order 1, restarting by reloading zn from scratch.
 *     If f() fails, returning either CV_RHSFUNC_FAIL or CV_UNREC_RHSFUNC_ERR (no recovery is possible at this stage).
 *   - otherwise, setting *nflagPtr to PREV_ERR_FAIL, and returning TRY_AGAIN
 */
static booleantype RKDoErrorTest (CVodeMem cv_mem, int *nflagPtr, realtype saved_t, int *nefPtr, realtype *dsmPtr)
{
  realtype dsm;
  int retval;
  
  dsm = acnrm / tq[2];

  // If estimated local error norm dsm passes test, returning CV_SUCCESS  
  *dsmPtr = dsm;
  if (dsm <= 1) return (CV_SUCCESS);
  
  // If test failed, incrementing counters, setting nflag and restoring zn array
  (*nefPtr)++;
  netf++;
  *nflagPtr = PREV_ERR_FAIL;
  RKRestore (cv_mem, saved_t);

  // At maxnef failures or |h| = hmin, returning CV_ERR_FAILURE */
  if ((ABS (h) <= hmin*ONEPSM) || (*nefPtr == maxnef)) return (CV_ERR_FAILURE);

  // Setting etamax = 1 to prevent step size increase at end of this step
  etamax = 1;

  // Setting h ratio eta from dsm, rescaleing, and returning for new try of step 
  if (*nefPtr <= MXNEF1) {
    eta = 1 / (RPowerR(BIAS2*dsm,1/L) + ADDON);
    eta = MAX(ETAMIN, MAX(eta, hmin / ABS (h)));
    if (*nefPtr >= SMALL_NEF) eta = MIN (eta, ETAMXF);
    RKRescale (cv_mem);
    return (TRY_AGAIN);
  }
  
  // After MXNEF1 failures, forcing an order reduction and retrying step
  if (q > 1) {
    eta = MAX(ETAMIN, hmin / ABS (h));
    RKAdjustOrder (cv_mem,-1);
    L = q;
    q--;
    qwait = L;
    RKRescale (cv_mem);
    return (TRY_AGAIN);
  }

  // If already at order 1, restarting by reloading zn from scratch

  eta = MAX(ETAMIN, hmin / ABS (h));
  h *= eta;
  next_h = h;
  hscale = h;
  qwait = LONG_WAIT;
  nscon = 0;

  retval = f (tn, zn[0], tempv, f_data);
  nfe++;
  if (retval < 0)  return (CV_RHSFUNC_FAIL);
  if (retval > 0)  return (CV_UNREC_RHSFUNC_ERR);

  N_VScale (h, tempv, zn[1]);

  return (TRY_AGAIN);
}

/* Restoring the value of tn to saved_t and undoing the prediction.
 * After execution of RKRestore, the Nordsieck array zn has the same values as before the call to RKPredict
 */
static void RKRestore (CVodeMem cv_mem, realtype saved_t)
{
  int j, k;
  
  tn = saved_t;
  for (k = 1; k <= q; ++k)
    for (j = q; j >= k; j--)
      N_VLinearSum (ONE, zn[j-1], -ONE, zn[j], zn[j-1]);
}

/* Calling RKSetAdams or RKSetBDF to set the polynomial l, the test quantity array tq, 
 * and the related variables rl1, gamma and gamrat
 */
static void RKSet (CVodeMem cv_mem)
{
  switch (lmm) {
    case CV_ADAMS:
      RKSetAdams (cv_mem);
      break;
    case CV_BDF:
      RKSetBDF (cv_mem);
      break;
  }
  rl1 = ONE / l[1];
  gamma = h * rl1;
  if (nst == 0) gammap = gamma;
  gamrat = (nst > 0) ? gamma / gammap : ONE;  // Protect x / x != 1.0
}

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
 * step may be reattempted, depending on the nature of the failure
 */
static int RKStep (CVodeMem cv_mem)
{
  realtype saved_t, dsm;
  int ncf, nef;
  int nflag, kflag, eflag;
  
  saved_t = tn;
  ncf = nef = 0;
  nflag = FIRST_CALL;

  // if ((nst > 0) && (hprime != h)) RKAdjustParams(cv_mem); // Commenting to compile
  
  // Looping point for attempts to take a step
  for(;;) {

    //RKPredict (cv_mem); // Commenting to compile
    RKSet (cv_mem);
    nflag = RKNls (cv_mem, nflag);
    kflag = RKHandleNFlag (cv_mem, &nflag, saved_t, &ncf);

    // Going back in loop if we need to predict again (nflag = PREV_CONV_FAIL)
    if (kflag == PREDICT_AGAIN) continue;

    // Returning if nonlinear solve failed and recovery is impossible
    if (kflag != DO_ERROR_TEST) return (kflag);

    // Performing error test (nflag = CV_SUCCESS)
    eflag = RKDoErrorTest(cv_mem, &nflag, saved_t, &nef, &dsm);

    // Going back in loop if we need to predict again (nflag = PREV_ERR_FAIL)
    if (eflag == TRY_AGAIN)  continue;

    // Returning if error test failed and recovery is impossible
    if (eflag != CV_SUCCESS) return (eflag);

    // If error test passed (eflag = CV_SUCCESS), breaking from loop
    break;
  }

  /* If nonlinear system solve and error test were both successful,
   * Updating data, and considering change of step and/or order */
  //RKCompleteStep (cv_mem); // Commenting to compile
  //RKPrepareNextStep (cv_mem, dsm); // Commenting to compile

  /* If Stablilty Limit Detection is turned on, calling stability limit
     detection routine for possible order reduction */
  // if (sldeton) RKBDFStab(cv_mem); // Commenting to compile
  // etamax = (nst <= SMALL_NST) ? ETAMX2 : ETAMX3; // Commenting to compile
  // etamax = (nst <= SMALL_NST) ? ETAMX2 : ETAMX3; // Commenting to compile

  // Finally, rescaling the acor array to be the estimated local error vector
  N_VScale(ONE/tq[2], acor, acor);
  
  return (CV_SUCCESS);
}

/* Handling the computation of l and tq when lmm == CV_ADAMS.
 * The components of the array l are the coefficients of a polynomial
 *   Lambda(x) = l_0 + l_1 x + ... + l_q x^q, given by 
 * 
 *                            q-1
 *   (d/dx) Lambda(x) = c * PRODUCT (1 + x / xi_i) , where 
 *                            i=1
 * 
 *   Lambda(-1) = 0, Lambda(0) = 1, and c is a normalization factor.
 * Here, xi_i = [t_n - t_(n-i)] / h.
 *
 * Setting the array tq to test quantities used in the convergence test, the error test,
 * and the selection of h at a new order.
 */
static void RKSetAdams (CVodeMem cv_mem)
{
  realtype m[L_MAX], M[3], hsum;
  
  if (q == 1) {
    l[0] = l[1] = tq[1] = tq[5] = ONE;
    tq[2] = TWO;
    tq[3] = TWELVE;
    tq[4] = nlscoef * tq[2];       // = 0.1 * tq[2]
    return;
  }
  
  hsum = RKAdamsStart (cv_mem, m);
  
  M[0] = RKAltSum (q-1, m, 1);
  M[1] = RKAltSum (q-1, m, 2);
  
  RKAdamsFinish (cv_mem, m, M, hsum);
}

// Generating in m[] the coefficients of the product polynomial needed for the Adams l and tq coefficients for q > 1
static realtype RKAdamsStart (CVodeMem cv_mem, realtype m[])
{
  realtype hsum, xi_inv, sum;
  int i, j;
  
  hsum = h;
  m[0] = ONE;
  for (i=1; i <= q; ++i) m[i] = ZERO;
  for (j=1; j < q; ++j) {
    if ((j==q-1) && (qwait == 1)) {
      sum = RKAltSum (q-2, m, 2);
      tq[1] = m[q-2] / (q * sum);
    }
    xi_inv = h / hsum;
    for (i=j; i >= 1; i--) m[i] += m[i-1] * xi_inv;
    hsum += tau[j];
    // The m[i] are coefficients of product (1 to j) (1 + x/xi_i)
  }
  return (hsum);
}

/* Returning the value of the alternating sum (i= 0 ... iend) [ (-1)^i * (a[i] / (i + k)) ].
 * If iend < 0 then returning 0.
 * This operation is needed to compute the integral, from -1 to 0,
 * of a polynomial x^(k-1) M(x) given the coefficients of M(x)
 */
static realtype RKAltSum (int iend, realtype a[], int k)
{
  int i, sign;
  realtype sum;
  
  if (iend < 0) return (ZERO);
  
  sum = ZERO;
  sign = 1;
  for (i=0; i <= iend; ++i) {
    sum += sign * (a[i] / (i+k));
    sign = -sign;
  }
  
  return (sum);
}

// Completing the calculation of the Adams l and tq
static void RKAdamsFinish (CVodeMem cv_mem, realtype m[], realtype M[], realtype hsum)
{
  int i;
  realtype M0_inv, xi, xi_inv;
  
  M0_inv = ONE / M[0];
  
  l[0] = ONE;
  for (i=1; i <= q; ++i) l[i] = M0_inv * (m[i-1] / i);
  xi = hsum / h;
  xi_inv = ONE / xi;
  
  tq[2] = xi * M[0] / M[1];
  tq[5] = xi / l[q];

  if (qwait == 1) {
    for (i=q; i >= 1; i--) m[i] += m[i-1] * xi_inv;
    M[2] = RKAltSum (q, m, 2);
    tq[3] = L * M[0] / M[2];
  }
  tq[4] = nlscoef * tq[2];
}

// Setting the test quantity array tq when lmm == CV_BDF
static void RKSetTqBDF (CVodeMem cv_mem, realtype hsum, realtype alpha0, realtype alpha0_hat, realtype  xi_inv, realtype xistar_inv)
{
  realtype A1, A2, A3, A4, A5, A6;
  realtype C, CPrime, CPrimePrime;
  
  A1 = ONE - alpha0_hat + alpha0;
  A2 = ONE + q * A1;
  tq[2] = ABS (alpha0 * (A2 / A1));
  tq[5] = ABS ((A2) / (l[q] * xi_inv/xistar_inv));
  if (qwait == 1) {
    C = xistar_inv / l[q];
    A3 = alpha0 + ONE / q;
    A4 = alpha0_hat + xi_inv;
    CPrime = A3 / (ONE - A4 + A3);
    tq[1] = ABS (CPrime / C);
    hsum += tau[q];
    xi_inv = h / hsum;
    A5 = alpha0 - (ONE / (q+1));
    A6 = alpha0_hat - xi_inv;
    CPrimePrime = A2 / (ONE - A6 + A5);
    tq[3] = ABS (CPrimePrime * xi_inv * (q+2) * A5);
  }
  
  tq[4] = nlscoef * tq[2];
}

/* Computing the coefficients l and tq when lmm == CV_BDF.
 * 
 * Calling RKSetTqBDF to set the test quantity array tq. 
 * 
 * The components of the array l are the coefficients of a
 * polynomial Lambda(x) = l_0 + l_1 x + ... + l_q x^q, given by
 * 
 *                                   q-1
 *   Lambda(x) = (1 + x / xi*_q) * PRODUCT (1 + x / xi_i) , where
 *                                   i=1
 * 
 *   xi_i = [t_n - t_(n-i)] / h.
 *
 * Setting the array tq to test quantities used in the convergence test, the error test,
 * and the selection of h at a new order
 */
static void RKSetBDF (CVodeMem cv_mem)
{
  realtype alpha0, alpha0_hat, xi_inv, xistar_inv, hsum;
  int i,j;
  
  l[0] = l[1] = xi_inv = xistar_inv = ONE;
  for (i=2; i <= q; ++i) l[i] = ZERO;
  alpha0 = alpha0_hat = -ONE;
  hsum = h;
  if (q > 1) {
    for (j=2; j < q; ++j) {
      hsum += tau[j-1];
      xi_inv = h / hsum;
      alpha0 -= ONE / j;
      for (i=j; i >= 1; i--) l[i] += l[i-1]*xi_inv;
      // The l[i] are coefficients of product (1 to j) (1 + x/xi_i)
    }
    
    /* j = q */
    alpha0 -= ONE / q;
    xistar_inv = -l[1] - alpha0;
    hsum += tau[q-1];
    xi_inv = h / hsum;
    alpha0_hat = -l[1] - xi_inv;
    for (i=q; i >= 1; i--) l[i] += l[i-1]*xistar_inv;
  }

  RKSetTqBDF (cv_mem, hsum, alpha0, alpha0_hat, xi_inv, xistar_inv);
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
  CVodeMem cv_mem;
  char err_type[10];

  // Here, data points to cv_mem
  cv_mem = (CVodeMem) data;

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

/* Performing the Newton iteration. If the iteration succeeds, returning CV_SUCCESS
 * If not, it may signal the RKNlsNewton routine to call lsetup again and reattempt the iteration, by
 * returning the value TRY_AGAIN (in this case, RKNlsNewton must set convfail to CV_FAIL_BAD_J before calling setup again).
 * Otherwise, returning one of the appropriate values CV_LSOLVE_FAIL, CV_RHSFUNC_FAIL, CONV_FAIL,
 * or RHSFUNC_RECVR back to RKNlsNewton
 */
static int RKNewtonIteration (CVodeMem cv_mem)
{
  int m, retval;
  realtype del, delp, dcon;
  N_Vector b;

  mnewt = m = 0;

  // Initializing delp to avoid compiler warning message
  del = delp = ZERO;

  /// Looping point for Newton iteration
  for (;;) {

    // Evaluating the residual of the nonlinear system
    N_VLinearSum (rl1, zn[1], ONE, acor, tempv);
    N_VLinearSum (gamma, ftemp, -ONE, tempv, tempv);

    // Calling the lsolve function
    b = tempv;
    retval = lsolve (cv_mem, b, ewt, y, ftemp); 
    nni++;
    
    if (retval < 0) return (CV_LSOLVE_FAIL);
    
    // If lsolve had a recoverable failure and Jacobian data is not current, signaling to try the solution again
    if (retval > 0) { 
      if ((!jcur) && (setupNonNull)) return (TRY_AGAIN);
      else                            return (CONV_FAIL);
    }

    // Getting WRMS norm of correction; add correction to acor and y
    del = N_VWrmsNorm (b, ewt);
    N_VLinearSum (ONE, acor, ONE, b, acor);
    N_VLinearSum (ONE, zn[0], ONE, acor, y);
    
    // Testing for convergence. If m > 0, storing an estimate of the convergence rate constant in crate, and using it in the test
    if (m > 0) {
      crate = MAX (CRDOWN * crate, del/delp);
    }
    dcon = del * MIN (ONE, crate) / tq[4];
    
    if (dcon <= ONE) {
      acnrm = (m==0) ? del : N_VWrmsNorm (acor, ewt);
      jcur = FALSE;
      return (CV_SUCCESS); // Nonlinear system was solved successfully
    }
    
    mnewt = ++m;
    
    /* Stopping at maxcor iterations or if iterations seems to be diverging.
       If still not converged and Jacobian data is not current, 
       signaling to try the solution again */
    if ((m == maxcor) || ((m >= 2) && (del > RDIV*delp))) {
      if ((!jcur) && (setupNonNull)) return (TRY_AGAIN);
      else                            return (CONV_FAIL);
    }
    
    // Saving norm of correction, evaluating f, and looping again
    delp = del;
    retval = f (tn, y, ftemp, f_data);
    nfe++;
    if (retval < 0) return (CV_RHSFUNC_FAIL);
    if (retval > 0) {
      if ((!jcur) && (setupNonNull)) return (TRY_AGAIN);
      else                            return (RHSFUNC_RECVR);
    }
  } // Loop end
}

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
 * Called by RK with k = 0 and t = t, but may also be called directly by the user
 */
int RKGetDky (void *cvode_mem, realtype t, int k, N_Vector dky)
{
  // Checking arguments
  if (cvode_mem == NULL) {
    RKProcessError (NULL, CV_MEM_NULL, "Runge-Kutta", "RKGetDky", MSGCV_NO_MEM);
    return (CV_MEM_NULL);
  }
  CVodeMem cv_mem;
  cv_mem = (CVodeMem) cvode_mem;

  if (dky == NULL) {
    RKProcessError (cv_mem, CV_BAD_DKY, "Runge-Kutta", "RKGetDky", MSGCV_NULL_DKY);
    return (CV_BAD_DKY);
  }

  if ((k < 0) || (k > q)) {
    RKProcessError (cv_mem, CV_BAD_K, "Runge-Kutta", "RKGetDky", MSGCV_BAD_K);
    return (CV_BAD_K);
  }
  
  // Allowing some slack
  realtype s, c, r;
  realtype tfuzz, tp, tn1;
  int i, j;
  // tfuzz = FUZZ_FACTOR * uround * (ABS(tn) + ABS(hu)); // Commenting to compile
  if (hu < ZERO) tfuzz = -tfuzz;
  tp = tn - hu - tfuzz;
  tn1 = tn + tfuzz;
  if ((t-tp)*(t-tn1) > ZERO) {
    RKProcessError (cv_mem, CV_BAD_T, "Runge-Kutta", "RKGetDky", MSGCV_BAD_T, t, tn-hu, tn);
    return (CV_BAD_T);
  }

  // Summing the differentiated interpolating polynomial
  s = (t - tn) / h;
  for (j=q; j >= k; j--) {
    c = ONE;
    for (i=j; i >= j-k+1; i--) c *= i;
    if (j == q)
      N_VScale(c, zn[q], dky);
    else
      N_VLinearSum (c, zn[j], s, dky, dky);
  }
  if (k == 0) return (CV_SUCCESS);
  r = RPowerI (h,-k);
  N_VScale (r, dky, dky);
  return (CV_SUCCESS);
}

/* Printing error messages for all cases of failure by RKHin and RKStep.
 * Returning to RK the value that RK is to return to the user
 */
static int RKHandleFailure (CVodeMem cv_mem, int flag)
{
  // Setting vector of absolute weighted local errors
  /*
  N_VProd(acor, ewt, tempv);
  N_VAbs(tempv, tempv);
  */

  // Depending on kflag, printing an error message and returning the error flag
  switch (flag) {
    case CV_ERR_FAILURE: 
      RKProcessError(cv_mem, CV_ERR_FAILURE, "Runge-Kutta", "RK", MSGCV_ERR_FAILS, tn, h);
      break;
    case CV_CONV_FAILURE:
      RKProcessError(cv_mem, CV_CONV_FAILURE, "Runge-Kutta", "RK", MSGCV_CONV_FAILS, tn, h);
      break;
    case CV_LSETUP_FAIL:
      RKProcessError(cv_mem, CV_LSETUP_FAIL, "Runge-Kutta", "RK", MSGCV_SETUP_FAILED, tn);
      break;
    case CV_LSOLVE_FAIL:
      RKProcessError(cv_mem, CV_LSOLVE_FAIL, "Runge-Kutta", "RK", MSGCV_SOLVE_FAILED, tn);
      break;
    case CV_RHSFUNC_FAIL:
      RKProcessError(cv_mem, CV_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RHSFUNC_FAILED, tn);
      break;
    case CV_UNREC_RHSFUNC_ERR:
      RKProcessError(cv_mem, CV_UNREC_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_UNREC, tn);
      break;
    case CV_REPTD_RHSFUNC_ERR:
      RKProcessError(cv_mem, CV_REPTD_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGCV_RHSFUNC_REPTD, tn);
      break;
    case CV_RTFUNC_FAIL:    
      RKProcessError(cv_mem, CV_RTFUNC_FAIL, "Runge-Kutta", "RK", MSGCV_RTFUNC_FAILED, tn);
      break;
    case CV_TOO_CLOSE:
      RKProcessError(cv_mem, CV_TOO_CLOSE, "Runge-Kutta", "RK", MSGCV_TOO_CLOSE);
    default:
      return (CV_SUCCESS);   
  }

  return (flag);
}

/* Taking action on the return value nflag = *nflagPtr returned by RKNls, as follows:
 *
 * If RKNls succeeded in solving the nonlinear system, then returning the constant DO_ERROR_TEST, which tells RKStep to perform the error test.
 *
 * If the nonlinear system was not solved successfully, then ncfn and ncf = *ncfPtr are incremented and Nordsieck array zn is restored.
 *
 * If the solution of the nonlinear system failed due to an unrecoverable failure by setup, returning the value CV_LSETUP_FAIL.
 * 
 * If it failed due to an unrecoverable failure in solve, then returning the value CV_LSOLVE_FAIL.
 *
 * If it failed due to an unrecoverable failure in rhs, then returning the value CV_RHSFUNC_FAIL.
 *
 * Otherwise, a recoverable failure occurred when solving the nonlinear system (RKNls returned nflag == CONV_FAIL or RHSFUNC_RECVR).
 * In this case, if ncf is now equal to maxncf or |h| = hmin, returning the value CV_CONV_FAILURE (if nflag=CONV_FAIL) or
 * CV_REPTD_RHSFUNC_ERR (if nflag = RHSFUNC_RECVR).
 * If not, setting *nflagPtr = PREV_CONV_FAIL and returning the value PREDICT_AGAIN, telling RKStep to reattempt the step
 */
static int RKHandleNFlag (CVodeMem cv_mem, int *nflagPtr, realtype saved_t, int *ncfPtr)
{
  int nflag;
  nflag = *nflagPtr;
  if (nflag == CV_SUCCESS) return (DO_ERROR_TEST);

  // The nonlinear solution failed; incrementing ncfn and restoring zn
  ncfn++;
  RKRestore(cv_mem, saved_t);
  
  // Returning if lsetup, lsolve, or rhs failed unrecoverably
  if (nflag == CV_LSETUP_FAIL)  return (CV_LSETUP_FAIL);
  if (nflag == CV_LSOLVE_FAIL)  return (CV_LSOLVE_FAIL);
  if (nflag == CV_RHSFUNC_FAIL) return (CV_RHSFUNC_FAIL);
  
  // At this point, nflag = CONV_FAIL or RHSFUNC_RECVR; incrementing ncf
  (*ncfPtr)++;
  etamax = ONE;

  // If we had maxncf failures or |h| = hmin, returning CV_CONV_FAILURE or CV_REPTD_RHSFUNC_ERR
  if ((ABS(h) <= hmin*ONEPSM) || (*ncfPtr == maxncf)) {
    if (nflag == CONV_FAIL)     return (CV_CONV_FAILURE);
    if (nflag == RHSFUNC_RECVR) return (CV_REPTD_RHSFUNC_ERR);    
  }

  // Reducing step size; returning to reattempt the step
  eta = MAX(ETACF, hmin / ABS(h));
  *nflagPtr = PREV_CONV_FAIL;
  RKRescale(cv_mem);
  return (PREDICT_AGAIN);
}

/* Attempting to solve the nonlinear system associated with a single implicit step of the linear multistep method.
 * Depending on iter, it calls RKNlsFunctional or RKNlsNewton to do the work
 */
static int RKNls (CVodeMem cv_mem, int nflag)
{
  int flag = CV_SUCCESS;
  /*
  switch(iter) {
  /case CV_FUNCTIONAL: 
    flag = CVNlsFunctional(cv_mem);
    break;
  case CV_NEWTON:
    flag = CVNlsNewton(cv_mem, nflag);
    break;
  }
*/
  return (flag);
}

static int RKRcheck1 (CVodeMem cv_mem)
{
  if (is_sundials_with_extension())
	return RKRcheck1Ext (cv_mem);
  else
	return RKRcheck1Std (cv_mem);
}

/* Completing the initialization of rootfinding memory information,
 * and checking whether g has a zero both at and very near the initial point of the IVP.
 *
 * Returning an int equal to :
 *  INITROOT = -1 if a close pair of zeros was found, and
 *  CV_SUCCESS = 0 otherwise.
 */
static int RKRcheck1Std (CVodeMem cv_mem)
{
  int i, retval;
  realtype smallh, hratio;
  booleantype zroot;

  for (i=0; i<nrtfn; ++i) iroots[i] = 0;
  tlo = tn;
  ttol = (ABS(tn) + ABS(h))*uround*HUN;

  // Evaluating g at t0 and checking for zeros
  retval = gfun (tlo, zn[0], glo, g_data);
  nge = 1;
  if (retval != 0) return (CV_RTFUNC_FAIL);

  zroot = FALSE;
  for (i=0; i<nrtfn; ++i) {
    if (ABS(glo[i]) == ZERO) zroot = TRUE;
  }
  if (!zroot) return (CV_SUCCESS);

  // At least one zero in g at t0; checking g at t0+(small increment)
  hratio = MAX(ttol/ABS(h), TENTH);
  smallh = hratio*h;
  tlo += smallh;
  N_VLinearSum (ONE, zn[0], hratio, zn[1], y);
  retval = gfun (tlo, y, glo, g_data);
  nge++;
  if (retval != 0) return (CV_RTFUNC_FAIL);

  zroot = FALSE;
  for (i=0; i<nrtfn; i++) {
    if (ABS(glo[i]) == ZERO) {
      zroot = TRUE;
      iroots[i] = 1;
    }
  }
  if (zroot) return (INITROOT);
  return (CV_SUCCESS);

}

// RKcheck1Std, but with Sundials extension
static int RKRcheck1Ext (CVodeMem cv_mem)
{
	int i, retval;
	booleantype zroot;

	for (i=0; i<nrtfn; ++i) iroots[i] = 0;
	tlo = tn;
	ttol = (ABS(tn) + ABS(h))*uround*HUN;
printf("c1e tlo = tn = %E\n", tlo);
	// Evaluating g at t0 and checking for zeros
	retval = gfun (tlo, zn[0], glo, g_data);
	nge = 1;
	if (retval != 0) return (CV_RTFUNC_FAIL);

	zroot = FALSE;
	for (i=0; i<nrtfn; ++i) {
		if (ABS(glo[i]) == ZERO) 
			iroots[i] = MASKED; // Arbitrary choice
		else
			iroots[i] = 0;
	}
	return (CV_SUCCESS);
	
	// At least one zero in g at t0; checking g at t0+(small increment)
}

/* Interface to RKRootfind to look for a root of g between tlo and either tn or tout, whichever comes first.
 * Only roots beyond tlo in the direction of integration are sought.
 *
 * Returning an int equal to:
 *      RTFOUND =  1 if a root of g was found, or
 *      CV_SUCCESS    =  0 otherwise.
 */
static int RKRootFind (CVodeMem cv_mem)
{
  /* SUNDIALS EXTENSION */
  if (is_sundials_with_extension())
	return RKRootfind (cv_mem);
  else
	return RKRootfind (cv_mem);
}

static int RKRootFindStd (CVodeMem cv_mem)
{
  int i, retval, ier;

  // Seting thi = tn or tout, whichever comes first; setting y = y(thi)
  if (taskc == CV_ONE_STEP) {
    thi = tn;
    N_VScale (ONE, zn[0], y);
  }
  if (taskc == CV_NORMAL) {
    if ((toutc - tn)*h >= ZERO) {
      thi = tn; 
      N_VScale (ONE, zn[0], y);
    }
    else {
      thi = toutc;
      (void) CVodeGetDky (cv_mem, thi, 0, y);
    }
  }

  // Setting ghi = g(thi) and call RKRootfind to search (tlo,thi) for roots
  retval = gfun (thi, y, ghi, g_data);
  nge++;
  if (retval != 0) return (CV_RTFUNC_FAIL);

  ttol = (ABS(tn) + ABS(h))*uround*HUN;
  ier = RKRootfind (cv_mem);
  tlo = trout;
  for (i = 0; i < nrtfn; ++i) glo[i] = grout[i];

  // If no root found, returning CV_SUCCESS
  if (ier == CV_SUCCESS) return (CV_SUCCESS);

  // If a root was found, interpolating to get y(trout) and returning
  (void) CVodeGetDky (cv_mem, trout, 0, y);
  return (RTFOUND);
}

static int RKRootfind (CVodeMem cv_mem)
{
	int i, retval, ier;
	booleantype zroot;

	// Setting y = y(thi) and thi = next t
	N_VScale (ONE, zn[0], y);
	thi = tn;
	
	// Setting ghi = g(thi) and searching [tlo,thi] for roots
	retval = gfun (thi, y, ghi, g_data);
	nge++;
	if (retval != 0) return (CV_RTFUNC_FAIL);

	ttol = (ABS(tn) + ABS(h))*uround*HUN;
	printf("c3e tlo = %E, thi = %E, tn = %E, h = %E\n", tlo, thi, tn, h);
	ier = RKRootF(cv_mem);
  
	// First, check if ghi has a zero
	for (i=0; i<nrtfn; ++i) if (ghi[i] == ZERO) return (RTFOUND);
  
	// Then, look for a sign change
	for (i=0; i<nrtfn; ++i) {
      if (glo[i]*ghi[i] < 0) {
		if (ghi[i] == ZERO) {
		  iroots[i] = (glo[i]> ZERO) ? -1 : 1;
		}
		else { printf("ELSE\n");
		  if (glo[i]*ghi[i] < ZERO){
			iroots[i] = (ghi[i]>glo[i]) ? 1 : -1;
			zroot = TRUE;
		  }
		  else iroots[i] = 0;
		}
        printf("rtf\n");
        return (RTFOUND);
	  }
    }
	printf("success\n");
	tlo = thi;
	for (i=0; i<nrtfn; ++i) glo[i] = ghi[i];
	return (CV_SUCCESS);

	// If no root found, returning CV_SUCCESS 
	if (ier == CV_SUCCESS) return (CV_SUCCESS);
	if (ier == CV_RTFUNC_FAIL) return (CV_RTFUNC_FAIL);
	// If a root was found, interpolating to get y(trout) and returning
	(void) RKGetDky (cv_mem, trout, 0, y);
	// return(RTFOUND);

	if (ier == RTFOUND)
	  return (RTFOUND);
	else
	  return (ZERODETACHING);
}

/* Testing g(t) for a root between tlo and thi, if one exists.
 * Only roots of odd multiplicity (i.e. with a change
 * of sign in one of the g_i), or exact zeros, are found.
 * Here the sign of tlo - thi is arbitrary, but if multiple roots
 * are found, the one closest to tlo is returned.
 *
 * The method used is the Illinois algorithm, a modified secant method.
 * Reference: Kathie L. Hiebert and Lawrence F. Shampine, Implicitly
 * Defined Output Points for Solutions of ODEs, Sandia National
 * Laboratory Report SAND80-0180, February 1980.
 *
 * This routine uses the following parameters for communication:
 *
 * nrtfn    = number of functions g_i, or number of components of
 *            the vector-valued function g(t). Input only.
 *
 * gfun     = user-defined function for g(t). Its form is
 *            (void) gfun(t, y, gt, g_data)
 *
 * nge      = cumulative counter for gfun calls.
 *
 * ttol     = a convergence tolerance for trout. Input only.
 *            When a root at trout is found, it is located only to within a tolerance of ttol.
 * 			  Typically, ttol should be set to a value on the order of
 * 			  		100 * UROUND * max (ABS(tlo), ABS(thi)),
 *            where UROUND is the unit roundoff of the machine.
 *
 * tlo, thi = endpoints of the interval in which roots are sought.
 *            On input, and must be distinct.
 * 			  The direction of integration is assumed to be from tlo to thi.
 * 			  At the end, tlo is incremented to match thi.
 *
 * glo, ghi = arrays of length nrtfn containing the vectors g(tlo)
 *            and g(thi) respectively. Input and output.
 * 			  On input, none of the glo[i] should be zero.
 *
 * iroots   = int array of length nrtfn with root information.
 *            Output only. If a root was found, iroots indicates which components g_i have a root.
 * 			  For i = 0, ..., nrtfn-1, iroots[i] = 1 if g_i has a root
 *            and iroots[i] = 0 otherwise.
 *
 * Returning an int equal to:
 *      RTFOUND =  1 if a root of g was found, or
 *      CV_SUCCESS    =  0 otherwise.
 */
static int RKRootfindExt (CVodeMem cv_mem)
{
	realtype alpha, tmid, gfrac, maxfrac, fracint, fracsub;
	int i, retval, imax, side, sideprev;
	int istuck,iunstuck,imaxold;

	booleantype zroot, umroot, sgnchg;

	imax = -1;
	istuck=-1;
	iunstuck=-1;
	maxfrac = ZERO;

	// First, checking for a sign change or a zero in ghi
	zroot = FALSE;

	for (i = 0;  i < nrtfn; ++i) {
	  if ((ABS(ghi[i]) == ZERO) && (iroots[i] != MASKED))  istuck = i;
	  if ((ABS(ghi[i]) > ZERO) && (iroots[i] == MASKED))  iunstuck = i;
	  if ((ABS(ghi[i]) > ZERO) && (glo[i]*ghi[i] <= ZERO)) {
		gfrac = ABS(ghi[i]/(ghi[i] - glo[i]));
		if (gfrac > maxfrac) { // Finding the very first root
		  maxfrac = gfrac;
		  imax = i;
		}      
	  }
	}

	if (imax >= 0)
		sgnchg = TRUE;
	else if (istuck >= 0) {
		    sgnchg = TRUE;
		    imax = istuck;
	      }
		  else if (iunstuck >= 0) {
		         sgnchg = TRUE;
		         imax = iunstuck;
				}
				else sgnchg = FALSE;

	if (!sgnchg) {
		trout = thi;
		for (i = 0; i < nrtfn; ++i) grout[i] = ghi[i];
		return (CV_SUCCESS);
	}

	// Initializing alpha to avoid compiler warning
	alpha = ONE;

	// A sign change was found. Looping to locate nearest root
	
	side = 0; sideprev = -1;
	int cnt = 0;
	for(;;) {           // Looping point
cnt++;
printf("tlo %E, thi %E, h %e, cnt %d\n", tlo, thi, h, cnt);
		/* Setting weight alpha.
		 * On the first two passes, setting alpha = 1.
		 * Thereafter, resetting alpha according to the side (low vs high) of the subinterval
		 * in which the sign change was found in the previous two passes.
		 * - If the sides were opposite, setting alpha = 1.
		 * - If the sides were the same, then doubling alpha (if high side), or halving alpha (if low side).
		 * The next guess tmid is the secant method value if alpha = 1, but is closer to tlo if alpha < 1, and closer to thi if alpha > 1
		 */

		if (sideprev == side)
		  alpha = (side == 2) ? alpha*TWO : alpha*HALF;
		else
		  alpha = ONE;
		
		/* Setting next root approximation tmid and get g(tmid).
		 * If tmid is too close to tlo or thi, adjusting it inward, by a fractional distance that is between 0.1 and 0.5
		 */
		if ((ABS(ghi[imax])==ZERO)||(ABS(glo[imax])==ZERO))
		  tmid = (tlo+alpha*thi)/(1+alpha);
		else
		  tmid = thi - (thi - tlo)*ghi[imax]/(ghi[imax] - alpha*glo[imax]);

		if (tmid+1 == tmid) {
			printf("tmid is nan\n\r ");
			exit(0);
		};

		if (ABS(tmid - tlo) < HALF*ttol) {
			fracint = ABS(thi - tlo)/ttol;
			fracsub = (fracint > FIVE) ? TENTH : HALF/fracint;
			tmid = tlo + fracsub*(thi - tlo);
		}

		if (ABS(thi - tmid) < HALF*ttol) {
			fracint = ABS(thi - tlo)/ttol;
			fracsub = (fracint > FIVE) ? TENTH : HALF/fracint;
			tmid = thi - fracsub*(thi - tlo);
		}

		(void) RKGetDky (cv_mem, tmid, 0, y);
		retval = gfun (tmid, y, grout, g_data);
		nge++;
		if (retval != 0) return (CV_RTFUNC_FAIL);

		/* Checking in which subinterval g changes sign, and resetting imax.
		 * Setting side = 1 if sign change is on low side, or 2 if on high side
		 */  

		// First, checking for a sign change or a zero in ghi
		zroot = FALSE;
		sideprev = side;
		imaxold = imax;
		imax = -1;
		istuck = -1;iunstuck = -1;
		maxfrac = ZERO;
		for (i = 0;  i < nrtfn; ++i) {
			if ((ABS(grout[i]) == ZERO) && (iroots[i] != MASKED))  istuck = i;
			if ((ABS(grout[i]) > ZERO) && (iroots[i] == MASKED))  iunstuck = i;
			if ((ABS(grout[i]) > ZERO) && (glo[i]*grout[i] <= ZERO)) {
				gfrac = ABS(grout[i]/(grout[i] - glo[i]));
				if (gfrac > maxfrac) { // Finding the very first root
					maxfrac = gfrac;
					imax = i;
				}      
			}
		}

		if (imax >= 0)
			sgnchg = TRUE;
		else if (istuck >= 0) {
			    sgnchg = TRUE;
			    imax = istuck;
			  }
			  else if (iunstuck >= 0) {
					  sgnchg = TRUE;
					  imax = iunstuck;
					}
					else {
					  sgnchg = FALSE;
					  imax = imaxold;
					}

		if (sgnchg) {
			// Sign change found in (tlo,tmid); replacing thi with tmid
			thi = tmid;
			for (i = 0; i < nrtfn; ++i) ghi[i] = grout[i];
			side = 1;
			// Stopping at root thi if converged; otherwise looping
			if (ABS(thi - tlo) <= ttol) break;
			continue;  // Return to looping point
		}

		// Here, either (ABS(thi - tlo) <= ttol) or NO SIGN CHANGE

		/* No sign change in (tlo,tmid), and no zero at tmid.
		 * Sign change must be in (tmid,thi). Replacing tlo with tmid
		 */
		tlo = tmid;
		for (i = 0; i < nrtfn; ++i) glo[i] = grout[i];
		side = 2;
		// Stopping at root thi if converged; otherwise looping back
		if (ABS(thi - tlo) <= ttol) break;

	} // End of root-search loop

	// Resetting trout and grout, setting iroots, and returning RTFOUND
	zroot = FALSE;
	umroot = FALSE;
	trout = thi;
	for (i = 0; i < nrtfn; ++i) {
		grout[i] = ghi[i];
		if (iroots[i]==MASKED) {
			if (ABS(ghi[i]) != ZERO){
				iroots[i] = (ghi[i]> ZERO) ? 2 : -2;
				umroot = TRUE;
			}
			else iroots[i] = 0;
		}
		else {
			if (ABS(ghi[i]) == ZERO){
				iroots[i] = (glo[i]> ZERO) ? -1 : 1;
				zroot = TRUE;
			}
			else {
				if (glo[i]*ghi[i] < ZERO){
					iroots[i] = (ghi[i]>glo[i]) ? 1 : -1;
					zroot = TRUE;
				}
				else iroots[i] = 0;
			}
		}
	}
	if (zroot) {
	  for (i=0; i<nrtfn; ++i) if ((iroots[i]==2) || (iroots[i]==-2)) iroots[i] = 0;
	  return (RTFOUND);
	}
	if (umroot) return (ZERODETACHING);
	return (CV_SUCCESS);
}

static int RKRootF(CVodeMem cv_mem)
{

  int retval, i;
  booleantype zroot;
  realtype tmid;
  
  // First, check if ghi has a zero
  for (i=0; i<nrtfn; ++i) if (ghi[i] == ZERO) return (RTFOUND);
  
  // Then, look for a sign change
  for (i=0; i<nrtfn; ++i) {
    if (glo[i]*ghi[i] < 0) {
		if (ghi[i] == ZERO) {
		  iroots[i] = (glo[i]> ZERO) ? -1 : 1;
		}
		else { printf("ELSE\n");
		  if (glo[i]*ghi[i] < ZERO){
			iroots[i] = (ghi[i]>glo[i]) ? 1 : -1;
			zroot = TRUE;
		  }
		  else iroots[i] = 0;
		}
      printf("rtf\n");
      return (RTFOUND);
    }
  }
  printf("success\n");
  tlo = thi;
  return (CV_SUCCESS);
}
