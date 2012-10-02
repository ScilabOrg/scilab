/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2012 - ...
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

#include <sundials/sundials_nvector.h>
/* SUNDIALS EXTENSION */
#include "sundials_extension.h"

#define errfp    (rk_mem->rk_errfp)

/* =============================
 * 
 * 			RKCreate
 * 
 * =============================
 *
 * Creating the problem */

void *RKCreate (int mode_integ, int mode_iter)
{
  
  // Step 1 : checking the arguments
  
  if ((mode_integ != RK_ADAMS) && (mode_integ != RK_BDF)) {
    RKProcessError (NULL, ZERO, "Runge-Kutta", "RKCreate", MSGRK_BAD_MODE_INTEG);
    return (NULL);
  }
  if ((mode_iter != RK_FUNCTIONAL) && (mode_iter != RK_NEWTON)) {
    RKProcessError (NULL, ZERO, "Runge-Kutta", "RKCreate", MSGRK_BAD_MODE_ITER);
    return (NULL);
  }
  
  // Step 2 : function body
  
  // Setting the workspace memory
  RKMem rk_mem;
  rk_mem = NULL;
  rk_mem = (RKMem) malloc(sizeof (struct RKMemRec));
  if (rk_mem == NULL) {
    RKProcessError (NULL, ZERO, "Runge-Kutta", "RKCreate", MSGRK_RKMEM_FAIL);
    return (NULL);
  }

  int maxord;
  maxord = (mode_integ == RK_ADAMS) ? ADAMS_Q_MAX : BDF_Q_MAX;
  
  // Copying input parameters into rk_mem
  rk_mem->rk_mode_integ  = mode_integ;
  rk_mem->rk_mode_iter = mode_iter;
  
  // Setting unit roundoff
  rk_mem->rk_uround = UNIT_ROUNDOFF;
  
  // Setting default values
  rk_mem->rk_f        = NULL;
  rk_mem->rk_f_data   = NULL;
  rk_mem->rk_efun     = NULL;
  rk_mem->rk_e_data   = NULL;
  rk_mem->rk_ehfun    = RKErrHandler;
  rk_mem->rk_eh_data  = (void *) rk_mem;
  rk_mem->rk_errfp    = stderr;
  rk_mem->rk_qmax     = maxord;
  rk_mem->rk_mxstep   = MXSTEP_DEFAULT;
  rk_mem->rk_mxhnil   = MXHNIL_DEFAULT;
  rk_mem->rk_sldeton  = FALSE;
  rk_mem->rk_hin      = ZERO;
  rk_mem->rk_hmin     = HMIN_DEFAULT;
  rk_mem->rk_hmax_inv = HMAX_INV_DEFAULT;
  rk_mem->rk_tstopset = FALSE;
  rk_mem->rk_maxcor   = NLS_MAXCOR;
  rk_mem->rk_maxnef   = MXNEF;
  rk_mem->rk_maxncf   = MXNCF;
  rk_mem->rk_nlscoef  = CORTES;

  // Initializing root finding variables
  rk_mem->rk_glo    = NULL;
  rk_mem->rk_ghi    = NULL;
  rk_mem->rk_grout  = NULL;
  rk_mem->rk_iroots = NULL;
  rk_mem->rk_gfun   = NULL;
  rk_mem->rk_g_data = NULL;
  rk_mem->rk_n_root_functions  = 0;

  // Setting the integ-dependent maximum order
  rk_mem->rk_qmax_alloc = maxord;
  
  // Initializing lrw and liw
  rk_mem->rk_lrw = 58 + 2*L_MAX + NUM_TESTS;
  rk_mem->rk_liw = 40;

  // No mallocs have been done yet
  rk_mem->rk_VabstolMallocDone = FALSE;
  rk_mem->rk_MallocDone        = FALSE;

  // Returning a pointer to the RK memory block */
  return ((void *)rk_mem);
}

#if 1
#define mode_iter  (rk_mem->rk_mode_iter)  
#define mode_integ (rk_mem->rk_mode_integ) 
#define lrw  		  (rk_mem->rk_lrw)
#define liw  		  (rk_mem->rk_liw)
#endif

/* =============================
 * 
 * 			RKMalloc
 * 
 * =============================
 *
 * Allocating the problem */

int RKMalloc (void *rkutta_mem, RKRhsFn f, R t0, N_Vector y0, int itol, R reltol, void *abstol)
{
  
  // Step 1 : checking the arguments
  
  if (rkutta_mem==NULL) {
    RKProcessError (NULL, RK_MEM_NULL, "Runge-Kutta", "RKMalloc", MSGRK_NO_MEM);
    return (RK_MEM_NULL);
  }
  
  RKMem rk_mem;
  rk_mem = (RKMem) rkutta_mem;
  
  if (y0==NULL) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_NULL_Y0);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_NULL_Y0);
	else
	  return (RK_ILL_INPUT);
  }
  
  if ((itol != RK_SS) && (itol != RK_SV) && (itol != RK_WF)) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_BAD_ITOL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_BAD_ITOL);
	else
	  return (RK_ILL_INPUT);

  }

  if (f == NULL) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_NULL_F);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_NULL_F);
	else
	  return (RK_ILL_INPUT);
  }
  
  // Test if all required vector operations are implemented
  bool nvectorOK;
  nvectorOK = RKCheckNvector (y0);
  if (!nvectorOK) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_BAD_NVECTOR);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_BAD_NVECTOR);
	else
	  return (RK_ILL_INPUT);
  }
  
  // Testing tolerances
  if (itol != RK_WF) {
    if (abstol == NULL) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_NULL_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (RK_NULL_ABSTOL);
	  else
		return (RK_ILL_INPUT);
    }

    if (reltol < ZERO) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_BAD_RELTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (RK_BAD_RELTOL);
	  else
		return (RK_ILL_INPUT);
    }
	
	bool neg_abstol;
    if (itol == RK_SS)
      neg_abstol = (*((R *)abstol) < ZERO);
    else
      neg_abstol = (N_VMin((N_Vector)abstol) < ZERO);

    if (neg_abstol) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKMalloc", MSGRK_BAD_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (RK_BAD_ABSTOL);
	  else
		return (RK_ILL_INPUT);
    }
  }
  
  long int lrw1, liw1;
  if (y0->ops->nvspace != NULL)
    N_VSpace (y0, &lrw1, &liw1);
  else {
    lrw1 = 0;
    liw1 = 0;
  }
  rk_mem->rk_lrw1 = lrw1;
  rk_mem->rk_liw1 = liw1;
  
  // Step 2 : function body
   
  // Allocating the vectors, taking y0 as a template
  bool allocOK;
  allocOK = RKAllocVectors(rk_mem, y0, itol);
  if (!allocOK) {
    RKProcessError(rk_mem, RK_MEM_FAIL, "Runge-Kutta", "RKMalloc", MSGRK_MEM_FAIL);
    return (RK_MEM_FAIL);
  }
  
  // Copying tolerances into memory
  rk_mem->rk_itol   = itol;
  rk_mem->rk_reltol = reltol;      

  if (itol == RK_SS)
    rk_mem->rk_Sabstol = *((R *)abstol);
  else if (itol == RK_SV)
    N_VScale(ONE, (N_Vector)abstol, rk_mem->rk_Vabstol);

  // Copying the input parameters into RK memory block

  rk_mem->rk_f  = f;
  rk_mem->rk_tn = t0;

  // Setting step parameters

  rk_mem->rk_q      = 1;
  rk_mem->rk_L      = 2;
  rk_mem->rk_qwait  = rk_mem->rk_L;
  rk_mem->rk_etamax = ETAMX1;

  rk_mem->rk_qu    = 0;
  rk_mem->rk_hu    = ZERO;
  rk_mem->rk_tolsf = ONE;

  // Setting the linear solver addresses to NULL. We check != NULL later, in RK(), if using RK_NEWTON.)
  rk_mem->rk_linit  = NULL;
  rk_mem->rk_lsetup = NULL;
  rk_mem->rk_lsolve = NULL;
  rk_mem->rk_lfree  = NULL;
  rk_mem->rk_lmem   = NULL;

  // Initializing zn[0] in the history array
  N_VScale(ONE, y0, rk_mem->rk_zn[0]);

  // Initializing all the counters
  rk_mem->rk_nSolCalls     = 0;
  rk_mem->rk_nfe     = 0;
  rk_mem->rk_ncfn    = 0;
  rk_mem->rk_netf    = 0;
  rk_mem->rk_nni     = 0;
  rk_mem->rk_nsetups = 0;
  rk_mem->rk_nhnil   = 0;
  rk_mem->rk_nstlp   = 0;
  rk_mem->rk_nscon   = 0;
  rk_mem->rk_nge     = 0;

  // Initializing other integrator optional outputs
  rk_mem->rk_h0u      = ZERO;
  rk_mem->rk_next_h   = ZERO;
  rk_mem->rk_next_q   = 0;

  // Initializing Stablilty Limit Detection data
  /* NOTE: We do this even if stab lim det was not turned on yet.
   * This way, the user can turn it on at any time */
  rk_mem->rk_nor = 0;
  int i, k;
  for (i = 1; i <= 5; ++i)
    for (k = 1; k <= 3; ++k) 
      rk_mem->rk_ssdat[i-1][k-1] = ZERO;

  // The problem has been successfully initialized
  rk_mem->rk_MallocDone = TRUE;
  return (RK_SUCCESS); 
}

#if 1
#define lrw1 (rk_mem->rk_lrw1)
#define liw1 (rk_mem->rk_liw1)
#endif

/* =============================
 * 
 * 			RKReInit
 * 
 * =============================
 *
 * Reinitializing the problem */

int RKReInit (void *rkutta_mem, RKRhsFn f, R t0, N_Vector y0, int itol, R reltol, void *abstol)
{
	
  // Step 1 : checking the arguments
  
  // Checking rkutta_mem
  if (rkutta_mem==NULL) {
    RKProcessError (NULL, RK_MEM_NULL, "Runge-Kutta", "RKReInit", MSGRK_NO_MEM);
    return (RK_MEM_NULL);
  }
  RKMem rk_mem;
  rk_mem = (RKMem) rkutta_mem;
  
  // Checking if rk_mem was allocated
  if (rk_mem->rk_MallocDone == FALSE) {
    RKProcessError (rk_mem, RK_NO_MALLOC, "Runga-Kutta", "RKReInit", MSGRK_NO_MALLOC);
    return (RK_NO_MALLOC);
  }
  
  // Checking the other arguments
  if (y0 == NULL) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGRK_NULL_Y0);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_NULL_Y0);
	else
	  return (RK_ILL_INPUT);
  }
  
  if ((itol != RK_SS) && (itol != RK_SV) && (itol != RK_WF)) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGRK_BAD_ITOL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_BAD_ITOL);
	else
	  return (RK_ILL_INPUT);
  }

  if (f == NULL) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGRK_NULL_F);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_NULL_F);
	else
	  return (RK_ILL_INPUT);
  }

  // Testing tolerances
  if (itol != RK_WF) {

    if (abstol == NULL) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGRK_NULL_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (RK_NULL_ABSTOL);
	  else
	    return (RK_ILL_INPUT);
    }

    if (reltol < ZERO) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKReInit", MSGRK_BAD_RELTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (RK_BAD_RELTOL);
	  else
	    return (RK_ILL_INPUT);
    }
    
    bool neg_abstol;
    if (itol == RK_SS)
      neg_abstol = (*((R *)abstol) < ZERO);
    else
      neg_abstol = (N_VMin((N_Vector)abstol) < ZERO);
    
    if (neg_abstol) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKodeReInit", MSGRK_BAD_ABSTOL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
		return (RK_BAD_ABSTOL);
	  else
        return (RK_ILL_INPUT);
    }
  }

  // Step 2 : function body
  
  // Recopying tolerances into memory
  if ( (itol != RK_SV) && (rk_mem->rk_VabstolMallocDone) ) {
    N_VDestroy(rk_mem->rk_Vabstol);
    lrw -= lrw1;
    liw -= liw1;
    rk_mem->rk_VabstolMallocDone = FALSE;
  }

  if ( (itol == RK_SV) && !(rk_mem->rk_VabstolMallocDone) ) {
    rk_mem->rk_Vabstol = NULL;
    rk_mem->rk_Vabstol = N_VClone(y0);
    lrw += lrw1;
    liw += liw1;
    rk_mem->rk_VabstolMallocDone = TRUE;
  }

  rk_mem->rk_itol   = itol;
  rk_mem->rk_reltol = reltol;    
  
  if (itol == RK_SS)
    rk_mem->rk_Sabstol = *((realtype *)abstol);
  else if (itol == RK_SV)
    N_VScale(ONE, (N_Vector)abstol, rk_mem->rk_Vabstol);
  
  // Recopying the input parameters into RK memory block
  rk_mem->rk_f = f;
  rk_mem->rk_tn = t0;
  
  // Resetting step parameters
  rk_mem->rk_q      = 1;
  rk_mem->rk_L      = 2;
  rk_mem->rk_qwait  = rk_mem->rk_L;
  rk_mem->rk_etamax = ETAMX1;

  rk_mem->rk_qu    = 0;
  rk_mem->rk_hu    = ZERO;
  rk_mem->rk_tolsf = ONE;

  // Reinitializing zn[0] in the history array
  N_VScale(ONE, y0, rk_mem->rk_zn[0]);
 
  // Reinitializing all the counters
  rk_mem->rk_nSolCalls     = 0;
  rk_mem->rk_nfe     = 0;
  rk_mem->rk_ncfn    = 0;
  rk_mem->rk_netf    = 0;
  rk_mem->rk_nni     = 0;
  rk_mem->rk_nsetups = 0;
  rk_mem->rk_nhnil   = 0;
  rk_mem->rk_nstlp   = 0;
  rk_mem->rk_nscon   = 0;
  rk_mem->rk_nge     = 0;

  // Reinitializing other integrator optional outputs
  rk_mem->rk_h0u      = ZERO;
  rk_mem->rk_next_h   = ZERO;
  rk_mem->rk_next_q   = 0;

  // Initialize Stablilty Limit Detection data
  rk_mem->rk_nor = 0;
  int i, k;
  for (i = 1; i <= 5; ++i)
    for (k = 1; k <= 3; ++k) 
      rk_mem->rk_ssdat[i-1][k-1] = ZERO;
  
  // The problem has been successfully re-initialized
  return (RK_SUCCESS);
}

#if 1
#define gfun   (rk_mem->rk_gfun)
#define g_data (rk_mem->rk_g_data) 
#define glo    (rk_mem->rk_glo)
#define ghi    (rk_mem->rk_ghi)
#define grout  (rk_mem->rk_grout)
#define iroots (rk_mem->rk_iroots)
#endif

/* =============================
 * 
 * 			RKRootInit
 * 
 * =============================
 *
 * Initializing the Root Finding problem */

int RKRootInit (void *rkutta_mem, int n_root_functions, RKRootFn g, void *gdata)
{
	
  // Step 1 : checking the arguments
  
  // Checking rkutta_mem
  if (rkutta_mem == NULL) {
    RKProcessError (NULL, RK_MEM_NULL, "Runge-Kutta", "RKRootInit", MSGRK_NO_MEM);
    return (RK_MEM_NULL);
  }
  RKMem rk_mem;
  rk_mem = (RKMem) rkutta_mem;
  
  int nrt;
  nrt = (n_root_functions < 0) ? 0 : n_root_functions;

  /* If rerunning RKRootInit() with a different number of root
     functions (changing number of gfun components), then free
     currently held memory resources */
  if ((nrt != rk_mem->rk_n_root_functions) && (rk_mem->rk_n_root_functions > 0)) {
    free(glo); glo = NULL;
    free(ghi); ghi = NULL;
    free(grout); grout = NULL;
    free(iroots); iroots = NULL;

    lrw -= 3* (rk_mem->rk_n_root_functions);
    liw -= rk_mem->rk_n_root_functions;
  }
  
  /* If RKRootInit() was called with n_root_functions == 0, then set rk_n_root_functions to
     zero and rk_gfun to NULL before returning */
  if (nrt == 0) {
    rk_mem->rk_n_root_functions = nrt;
    gfun = NULL;
    g_data = NULL;
    return (RK_SUCCESS);
  }
  
  // Storing user's data pointer
  g_data = gdata;
  
  /* If rerunning RKRootInit() with the same number of root functions
     (not changing number of gfun components), then check if the root
     function argument has changed */
  /* If g != NULL then return as currently reserved memory resources
     will suffice */
  if (nrt == rk_mem->rk_n_root_functions) {
    if (g != gfun) {
      if (g == NULL) {
		free(glo); glo = NULL;
		free(ghi); ghi = NULL;
		free(grout); grout = NULL;
		free(iroots); iroots = NULL;
        lrw -= 3*nrt;
        liw -= nrt;
        RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKRootInit", MSGRK_NULL_G);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (RK_NULL_G);
		else
		  return (RK_ILL_INPUT);
      }
      else {
		gfun = g;
		return (RK_SUCCESS);
      }
    }
    else return (RK_SUCCESS);
  }

  // Step 2 : function body
  
  // Setting variables in RK memory block
  rk_mem->rk_n_root_functions = nrt;
  if (g == NULL) {
    RKProcessError(rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKRootInit", MSGRK_NULL_G);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_NULL_G);
	else
	  return (RK_ILL_INPUT);
  }
  else gfun = g;

  /* Allocate necessary memory and return */
  glo = NULL;
  glo = (realtype *) malloc(nrt*sizeof(realtype));
  if (glo == NULL) {
    RKProcessError(rk_mem, RK_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGRK_MEM_FAIL);
    return(RK_MEM_FAIL);
  }

  ghi = NULL;
  ghi = (realtype *) malloc(nrt*sizeof(realtype));
  if (ghi == NULL) {
    free(glo); glo = NULL;
    RKProcessError(rk_mem, RK_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGRK_MEM_FAIL);
    return(RK_MEM_FAIL);
  }

  grout = NULL;
  grout = (realtype *) malloc(nrt*sizeof(realtype));
  if (grout == NULL) {
    free(glo); glo = NULL;
    free(ghi); ghi = NULL;
    RKProcessError(rk_mem, RK_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGRK_MEM_FAIL);
    return(RK_MEM_FAIL);
  }

  iroots = NULL;
  iroots = (int *) malloc(nrt*sizeof(int));
  if (iroots == NULL) {
    free(glo); glo = NULL; 
    free(ghi); ghi = NULL;
    free(grout); grout = NULL;
    RKProcessError(rk_mem, RK_MEM_FAIL, "Runge-Kutta", "RKRootInit", MSGRK_MEM_FAIL);
    return(RK_MEM_FAIL);
  }

  lrw += 3*nrt;
  liw += nrt;

  return (RK_SUCCESS);
}

#if 1
/* =================================================================
 * Readibility Constants
 * =================================================================*/
#define f              (rk_mem->rk_f)      
#define f_data         (rk_mem->rk_f_data) 
#define efun           (rk_mem->rk_efun)
#define e_data         (rk_mem->rk_e_data) 
#define qmax           (rk_mem->rk_qmax)
#define mxstep         (rk_mem->rk_mxstep)
#define mxhnil         (rk_mem->rk_mxhnil)
#define sldeton        (rk_mem->rk_sldeton)
#define hin            (rk_mem->rk_hin)
#define hmin           (rk_mem->rk_hmin)
#define hmax_inv       (rk_mem->rk_hmax_inv)
#define istop          (rk_mem->rk_istop)
#define tstop          (rk_mem->rk_tstop)
#define tstopset       (rk_mem->rk_tstopset)
#define maxnef         (rk_mem->rk_maxnef)
#define maxncf         (rk_mem->rk_maxncf)
#define maxcor         (rk_mem->rk_maxcor)
#define nlscoef        (rk_mem->rk_nlscoef)
#define itol           (rk_mem->rk_itol)         
#define reltol         (rk_mem->rk_reltol)       
#define Sabstol        (rk_mem->rk_Sabstol)
#define Vabstol        (rk_mem->rk_Vabstol)

#define uround         (rk_mem->rk_uround)  
#define zn             (rk_mem->rk_zn) 
#define ewt            (rk_mem->rk_ewt)  
#define y              (rk_mem->rk_y)
#define acor           (rk_mem->rk_acor)
#define tempv          (rk_mem->rk_tempv)
#define ftemp          (rk_mem->rk_ftemp) 
#define q              (rk_mem->rk_q)
#define qprime         (rk_mem->rk_qprime)
#define next_q         (rk_mem->rk_next_q)
#define qwait          (rk_mem->rk_qwait)
#define L              (rk_mem->rk_L)
#define h              (rk_mem->rk_h)
#define hprime         (rk_mem->rk_hprime)
#define next_h         (rk_mem->rk_next_h)
#define eta            (rk_mem->rk_eta) 
#define etaqm1         (rk_mem->rk_etaqm1) 
#define etaq           (rk_mem->rk_etaq) 
#define etaqp1         (rk_mem->rk_etaqp1) 
#define nscon          (rk_mem->rk_nscon)
#define hscale         (rk_mem->rk_hscale)
#define tn             (rk_mem->rk_tn)
#define tau            (rk_mem->rk_tau)
#define tq             (rk_mem->rk_tq)
#define l              (rk_mem->rk_l)
#define rl1            (rk_mem->rk_rl1)
#define gamma          (rk_mem->rk_gamma) 
#define gammap         (rk_mem->rk_gammap) 
#define gamrat         (rk_mem->rk_gamrat)
#define crate          (rk_mem->rk_crate)
#define acnrm          (rk_mem->rk_acnrm)
#define mnewt          (rk_mem->rk_mnewt)
#define etamax         (rk_mem->rk_etamax)
#define nSolCalls            (rk_mem->rk_nSolCalls)
#define nfe            (rk_mem->rk_nfe)
#define ncfn           (rk_mem->rk_ncfn)
#define netf           (rk_mem->rk_netf)
#define nni            (rk_mem->rk_nni)
#define nsetups        (rk_mem->rk_nsetups)
#define nhnil          (rk_mem->rk_nhnil)
#define linit          (rk_mem->rk_linit)
#define lsetup         (rk_mem->rk_lsetup)
#define lsolve         (rk_mem->rk_lsolve) 
#define lfree          (rk_mem->rk_lfree) 
#define lmem           (rk_mem->rk_lmem) 
#define qu             (rk_mem->rk_qu)          
#define nstlp          (rk_mem->rk_nstlp)  
#define h0u            (rk_mem->rk_h0u)
#define hu             (rk_mem->rk_hu)         
#define saved_tq5      (rk_mem->rk_saved_tq5)  
#define indx_acor      (rk_mem->rk_indx_acor)
#define jcur           (rk_mem->rk_jcur)         
#define tolsf          (rk_mem->rk_tolsf)      
#define setupNonNull   (rk_mem->rk_setupNonNull) 
#define nor            (rk_mem->rk_nor)
#define ssdat          (rk_mem->rk_ssdat)

#define n_root_functions (rk_mem->rk_n_root_functions)
#define tlo            (rk_mem->rk_tlo)
#define thi            (rk_mem->rk_thi)
#define toldlast       (rk_mem->rk_toldlast)
#define toutc          (rk_mem->rk_toutc)
#define trout          (rk_mem->rk_trout)
#define ttol           (rk_mem->rk_ttol)
#define taskc          (rk_mem->rk_taskc)
#define irfnd          (rk_mem->rk_irfnd)
#define nge            (rk_mem->rk_nge)
#endif



/* =============================
 * 
 * 				RK
 * 
 * =============================
 *
 * Solving the problem */

int RK (void *rkutta_mem, R t, N_Vector yout, R *told, int pass_tF_or_back)
{
  
  // Step 1 : checking the arguments

  // Checking rkutta_mem
  if (rkutta_mem == NULL) {
    RKProcessError (NULL, RK_MEM_NULL, "Runge-Kutta", "RK", MSGRK_NO_MEM);
    return (RK_MEM_NULL);
  }
  RKMem rk_mem;
  rk_mem = (RKMem) rkutta_mem;

  // Checking rk_mem was allocated
  if (rk_mem->rk_MallocDone == FALSE) {
    RKProcessError (rk_mem, RK_NO_MALLOC, "Runge-Kutta", "RK", MSGRK_NO_MALLOC);
    return (RK_NO_MALLOC);
  }
  
  // Checking yout
  if ((y = yout) == NULL) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_YOUT_NULL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_YOUT_NULL);
	else
	  return (RK_ILL_INPUT);
  }

  // Checking told
  if (told == NULL) {
    RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_TOLD_NULL);
	/* SUNDIALS EXTENSION */
	if (is_sundials_with_extension())
	  return (RK_TOLD_NULL);
	else
	  return (RK_ILL_INPUT);
  }
  
  // Checking itask
  if ((pass_tF_or_back != RK_NORMAL)       && 
      (pass_tF_or_back != RK_ONE_STEP)     &&
      (pass_tF_or_back != RK_NORMAL_TSTOP) &&
      (pass_tF_or_back != RK_ONE_STEP_TSTOP) ) {
		RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_BAD_ITASK);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (RK_BAD_ITASK);
		else
		  return (RK_ILL_INPUT);
  }

  // Splitting itask into task and istop
  if ((pass_tF_or_back == RK_NORMAL_TSTOP) || (pass_tF_or_back == RK_ONE_STEP_TSTOP)) {
    if (tstopset == FALSE) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_NO_TSTOP);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (RK_NO_TSTOP);
	  else
	    return (RK_ILL_INPUT);
    }
    istop = TRUE;
  }
  else istop = FALSE;
  int task;
  if ((pass_tF_or_back == RK_NORMAL) || (pass_tF_or_back == RK_NORMAL_TSTOP)) {
    task = RK_NORMAL; toutc = t;
  }
  else task = RK_ONE_STEP;
  taskc = task;
  
  /* Step 2 : nSolCalls = 0 means that the solver is called for the first time.
   * Performing initializations :
   *    - initial setup
   *    - initializing Nordsieck history array
   *    - computing initial step size
   *    - check for approach to tstop
   *    - check for approach to a root
   */
  
  // Initializing some variables. retval is used like an error flag
  long int nSolCallsloc;
  int i, retval, hflag, kflag, istate, ier, irfndp;
  int ewtsetOK;
  R troundoff, rh, nrm;
  
  
  
  
  
  
  
  N_Vector val_temp[4];
  
  /* Proceeding to :
   * k1 = F(Tn, Yn),
   * k2 = F(Tn + h/2, Yn + (h/2)*k1),
   * k3 = F(Tn + h/2, Yn + (h/2)*k2),
   * k4 = F(Tn + h, Yn + h*k3),
   * Yn+1 = Yn + (h/6)*(K1 + 2K2 + 2K3 + K4)
   */
  y = zn[0];	// y = y0
  
  while (tn < tstop) {
    retval = f(tn, y, val_temp[0], f_data);		// k1
    retval = N_VAdd(y, val_temp[0], y);			// y += k1
    retval = f(tn + h/2, y, val_temp[1], f_data);	// k2 = f(tn+h/2, y),
    retval = N_VAdd(y, val_temp[1], y);			// y += k2
    retval = f(tn + h/2, y, val_temp[2], f_data);	// k3 = f(tn+h/2, y),
    retval = N_VAdd(y, val_temp[2], y);			// y += k3
    retval = f(tn + h, y, val_temp[3], f_data);	// k4 = f(tn+h, y),
  
	if (retval == -49) { // Error in the additions
	  RKProcessError (rk_mem, RK_ADD_VEC, "Runge-Kutta", "N_VAdd", MSGRK_BAD_LENGTH, tn);
      return (RK_RHSFUNC_FAIL);
	}
	if (retval < 0) { // Error in the evalutations of f
      RKProcessError(rk_mem, RK_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGRK_RHSFUNC_FAILED, tn);
      return(RK_RHSFUNC_FAIL);
    }
	if (retval > 0) { // Error in the evalutations of f
	  RKProcessError(rk_mem, RK_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGRK_RHSFUNC_FIRST);
      return (RK_FIRST_RHSFUNC_ERR);
	}
	
    // Multiplying k2 and k3 by 2 and performing the sum Yn+1
    retval = N_VMulScal(2, val_temp[1]);						// k2 *= 2
    retval = N_VMulScal(2, val_temp[2]);						// k3 *= 2
    retval = N_VAdd(val_temp[0], val_temp[1], val_temp[1]);	// k2 += k1
    retval = N_VAdd(val_temp[2], val_temp[3], val_temp[3]);	// k4 += k3
    retval = N_VAdd(val_temp[1], val_temp[3], val_temp[3]);	// k4 += k2
    retval = N_VMulScal(h/6, val_temp[3]);						// k4 *= h/6
    retval = N_VAdd(y, val_temp[3], yout);						// yout = y + k4
    
    if (retval) {
	  RKProcessError (rk_mem, RK_ADD_VEC, "Runge-Kutta", "N_VAdd", MSGRK_BAD_LENGTH);
      return (RK_RHSFUNC_FAIL);
	}
	
	tn += h;
  }
  
  
  
  
  
  
  // Rest to see later
  
  if (nSolCalls == 0) {

    ier = RKInitialSetup(rk_mem);
    if (ier!= RK_SUCCESS) return (ier);
    
    /* Calling f at (t0,y0), setting zn[1] = y'(t0), setting initial h (from H0 or RKHin), and scaling zn[1] by h.
       Also checking for zeros of root function g at and near t0 */
    retval = f (tn, zn[0], zn[1], f_data);
    nfe++;
    if (retval < 0) {
      RKProcessError (rk_mem, RK_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGRK_RHSFUNC_FAILED);
      return (RK_RHSFUNC_FAIL);
    }
    if (retval > 0) {
      RKProcessError(rk_mem, RK_FIRST_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGRK_RHSFUNC_FIRST);
      return (RK_FIRST_RHSFUNC_ERR);
    }

    // Setting initial h (from H0 or RKHin)
    h = hin;
    if ( (h != ZERO) && ((t-tn)*h < ZERO) ) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_BAD_H0);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (RK_BAD_H0);
	  else
		return (RK_ILL_INPUT);
    }
    if (h == ZERO) {
      hflag = RKHin (rk_mem, t);
      if (hflag != RK_SUCCESS) {
        istate = RKHandleFailure(rk_mem, hflag);
        return (istate);
      }
    }
    rh = ABS(h)*hmax_inv;
    if (rh > ONE) h /= rh;
    if (ABS(h) < hmin) h *= hmin/ABS(h);

    // Checking for approach to tstop
    if (istop) {
      if ((tstop - tn)*h < ZERO) {
        RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_BAD_TSTOP, tn);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (RK_BAD_TSTOP);
		else
		  return (RK_ILL_INPUT);
      }
      if ( (tn + h - tstop)*h > ZERO ) 
        h = (tstop - tn)*(ONE-FOUR*uround);
    }

    // Scaling zn[1] by h
    hscale = h; 
    h0u    = h;
    hprime = h;
    N_VScale(h, zn[1], zn[1]);

    // Checking for zeros of root function g at and near t0
    if (n_root_functions > 0) {
      retval = RKRcheck1(rk_mem);
      if (retval == INITROOT) {
        RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKRcheck1", MSGRK_BAD_INIT_ROOT);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (RK_BAD_INIT_ROOT);
		else
		  return (RK_ILL_INPUT);
      }
      else if (retval == RK_RTFUNC_FAIL) {
        RKProcessError (rk_mem, RK_RTFUNC_FAIL, "Runge-Kutta", "RKRcheck1", MSGRK_RTFUNC_FAILED, tn);
        return (RK_RTFUNC_FAIL);
      }
    }

  } // End of first call block
  
  /* Step 3 : following steps.
   * Performing stop tests:
   *    - checking for root in last step
   *    - checking if we passed tstop
   *    - checking if we passed tt (NORMAL mode)
   *    - checking if current tn was returned (ONE_STEP mode)
   *    - checking if we are close to tstop
   *      (adjust step size if needed)
   */
  
  if (nSolCalls > 0) {

    /* Estimating an infinitesimal time interval to be used as
     * a roundoff for time quantities (based on current time 
     * and step size) */
    troundoff = FUZZ_FACTOR*uround*(ABS(tn) + ABS(h));

    /* First, checking for a root in the last step taken, other than the last root found, if any.
     * If task = RK_ONE_STEP and y(tn) was not returned because of an intervening root, return y(tn) now */
    if (n_root_functions > 0) {
      irfndp = irfnd;
      retval = RKRcheck2(rk_mem);
      if (retval == CLOSERT) {
        RKProcessError (rk_mem, RK_ILL_INPUT, "RKODE", "RKRcheck2", MSGRK_CLOSE_ROOTS, tlo);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (RK_CLOSE_ROOTS);
		else
		  return (RK_ILL_INPUT);
      }
      else if (retval == RK_RTFUNC_FAIL) {
        RKProcessError (rk_mem, RK_RTFUNC_FAIL, "Runge-Kutta", "RKRcheck2", MSGRK_RTFUNC_FAILED, tlo);
        return (RK_RTFUNC_FAIL);
      }
      else if (retval == RTFOUND) {
        toldlast = *told = tlo;
        return (RK_ROOT_RETURN);
      }

      // If tn is distinct from tretlast (within roundoff), checking remaining interval for roots
      if ( ABS(tn - toldlast) > troundoff ) {
        retval = RKRcheck3(rk_mem);
        if (retval == RK_SUCCESS) {     // No root found
          irfnd = 0;
          if ((irfndp == 1) && (task == RK_ONE_STEP)) {
            toldlast = *told = tn;
            N_VScale(ONE, zn[0], yout);
            return (RK_SUCCESS);
          }
        }
        else if (retval == RTFOUND) {  // A new root was found
          irfnd = 1;
          toldlast = *told = tlo;
          return (RK_ROOT_RETURN);
        }
        else if (retval == RK_RTFUNC_FAIL) {  // g failed
          RKProcessError (rk_mem, RK_RTFUNC_FAIL, "Runge-Kutta", "RKRcheck3", MSGRK_RTFUNC_FAILED, tlo);
          return (RK_RTFUNC_FAIL);
        }
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension()) {
		  if (retval == ZERODETACHING) {  // Zero detaching
	        irfnd = 1;
	        toldlast = *told = tlo;
            return (RK_ZERO_DETACH_RETURN);
	      }
		}
      }
    } // End of root stop check
    
    // Testing for tn past tstop
    if ( istop && ((tstop - tn)*h < ZERO) ) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_BAD_TSTOP, tn);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	    return (RK_BAD_TSTOP);
	  else
		return (RK_ILL_INPUT);
    }

    /// In RK_NORMAL mode, testing if told was reached
    if ( (task == RK_NORMAL) && ((tn-t)*h >= ZERO) ) {
      toldlast = *told = t;
      ier = RKGetDky(rk_mem, t, 0, yout);
      if (ier != RK_SUCCESS) {
        RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_BAD_TOUT, told);
		/* SUNDIALS EXTENSION */
		if (is_sundials_with_extension())
		  return (RK_BAD_TOUT);
		else
		  return (RK_ILL_INPUT);
      }
      return (RK_SUCCESS);
    }

    // In RK_ONE_STEP mode, testing if tn was returned
    if ( task == RK_ONE_STEP && ABS(tn - toldlast) > troundoff ) {
      toldlast = *told = tn;
      N_VScale(ONE, zn[0], yout);
      return (RK_SUCCESS);
    }

    // Testing for tn at tstop or near tstop
    if (istop) {
      if ( ABS(tn - tstop) <= troundoff) {
        ier = RKGetDky(rk_mem, tstop, 0, yout);
        if (ier != RK_SUCCESS) {
          RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_BAD_TSTOP, tn);
		  /* SUNDIALS EXTENSION */
		  if (is_sundials_with_extension())
			return (RK_BAD_TSTOP);
		  else
			return (RK_ILL_INPUT);
        }
        toldlast = *told = tstop;
        return (RK_TSTOP_RETURN);
      }
      
      // If next step would overtake tstop, adjust stepsize
      if ( (tn + hprime - tstop)*h > ZERO ) {
        hprime = (tstop - tn)*(ONE-FOUR*uround);
        eta = hprime/h;
      }
    } // End of istop tests block
    
  } // End stopping tests block */ 
  
  /* Step 4 : Looping point for internal steps
   * - Checking for errors (too many steps, too much accuracy requested, step size too small),
   * - Taking a new step (call RKStep),
   * - Stopping on error,
   * - Performing stop tests:
   *   - checking for root in last step,
   *   - checking if t was passed,
   *   - checking if close to tstop,
   *   - checking if in ONE_STEP mode (must return).
   */

  nSolCallsloc = 0;
  for(;;) {
    next_h = h;
    next_q = q;
    
    // Resetting and checking ewt
    if (nSolCalls > 0) {
      ewtsetOK = efun(zn[0], ewt, e_data);
      if (ewtsetOK != 0) {
        if (itol == RK_WF) 
          RKProcessError(rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_EWT_NOW_FAIL, tn);
        else 
          RKProcessError(rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RK", MSGRK_EWT_NOW_BAD, tn);
        istate = RK_ILL_INPUT;
        toldlast = *told = tn;
        N_VScale(ONE, zn[0], yout);
        break;
      }
    }
    
    // Checking for too many steps
    if (nSolCallsloc >= mxstep) {
      RKProcessError(rk_mem, RK_TOO_MUCH_WORK, "Runge-Kutta", "RK", MSGRK_MAX_STEPS, tn);
      istate = RK_TOO_MUCH_WORK;
      toldlast = *told = tn;
      N_VScale(ONE, zn[0], yout);
      break;
    }

    // Checking for too much accuracy requested
    nrm = N_VWrmsNorm(zn[0], ewt);
    tolsf = uround * nrm;
    if (tolsf > ONE) {
      RKProcessError(rk_mem, RK_TOO_MUCH_ACC, "Runge-Kutta", "RK", MSGRK_TOO_MUCH_ACC, tn);
      istate = RK_TOO_MUCH_ACC;
      toldlast = *told = tn;
      N_VScale(ONE, zn[0], yout);
      tolsf *= TWO;
      break;
    }
    else tolsf = ONE;

    // Check if h below roundoff level in tn
    if (tn + h == tn) {
      nhnil++;
      if (nhnil <= mxhnil) 
        RKProcessError(rk_mem, RK_WARNING, "Runge-Kutta", "RK", MSGRK_HNIL, tn, h);
      if (nhnil == mxhnil) 
        RKProcessError(rk_mem, RK_WARNING, "Runge-Kutta", "RK", MSGRK_HNIL_DONE);
    }

    // Calling RKStep to take a step
    kflag = RKStep(rk_mem);

    // Processing failed step cases, and exiting loop
    if (kflag != RK_SUCCESS) {
      istate = RKHandleFailure(rk_mem, kflag);
      toldlast = *told = tn;
      N_VScale(ONE, zn[0], yout);
      break;
    }
    
    nSolCallsloc++;

    // Checking for root in last step taken
    if (n_root_functions > 0) {
      retval = RKRcheck3(rk_mem);
      if (retval == RTFOUND) {  // A new root was found
        irfnd = 1;
        istate = RK_ROOT_RETURN;
        toldlast = *told = tlo;
        break;
      }
      else if (retval == RK_RTFUNC_FAIL) { // g failed
        RKProcessError(rk_mem, RK_RTFUNC_FAIL, "Runge-Kutta", "RKRcheck3", MSGRK_RTFUNC_FAILED, tlo);
        istate = RK_RTFUNC_FAIL;
        break;
      }
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension()) {
	    if (retval == ZERODETACHING) {  /* Zero detaching */
	      irfnd = 1;
          istate = RK_ZERO_DETACH_RETURN;
          toldlast = *told = tlo;
          break;
        }
	  }
    }

    // In NORMAL mode, checking if t is reached
    if ( (task == RK_NORMAL) &&  (tn-t)*h >= ZERO ) {
      istate = RK_SUCCESS;
      toldlast = *told = t;
      (void) RKGetDky (rk_mem, t, 0, yout);
      next_q = qprime;
      next_h = hprime;
      break;
    }

    // Checking if tn is at tstop or near tstop
    if (istop) {
      troundoff = FUZZ_FACTOR*uround*(ABS(tn) + ABS(h));
      if ( ABS(tn - tstop) <= troundoff) {
        (void) RKGetDky(rk_mem, tstop, 0, yout);
        toldlast = *told = tstop;
        istate = RK_TSTOP_RETURN;
        break;
      }
      if ( (tn + hprime - tstop)*h > ZERO ) {
        hprime = (tstop - tn)*(ONE-FOUR*uround);
        eta = hprime/h;
      }
    }

    // In ONE_STEP mode, copying y and exiting loop
    if (task == RK_ONE_STEP) {
      istate = RK_SUCCESS;
      toldlast = *told = tn;
      N_VScale(ONE, zn[0], yout);
      next_q = qprime;
      next_h = hprime;
      break;
    }
    
  } // Ending loop for internal steps
  
  return(istate);
}



// Freeing the problem memory allocated by RKMalloc
void RKFree (void **rkutta_mem)
{
  RKMem rk_mem;
  if (*rkutta_mem == NULL) return;

  rk_mem = (RKMem) (*rkutta_mem);
  
  RKFreeVectors (rk_mem);

  if (mode_iter == RK_NEWTON && lfree != NULL) lfree (rk_mem);

  if (n_root_functions > 0) {
    free(glo); glo = NULL;
    free(ghi); ghi = NULL;
    free(grout); grout = NULL;
    free(iroots); iroots = NULL;
  }

  free(*rkutta_mem);
  *rkutta_mem = NULL;
}

// Freeing the RK vectors allocated in RKAllocVectors
static void RKFreeVectors (RKMem rk_mem)
{
  int j, maxord;
  
  maxord = rk_mem->rk_qmax_alloc;

  N_VDestroy(ewt);
  N_VDestroy(acor);
  N_VDestroy(tempv);
  N_VDestroy(ftemp);
  for (j=0; j <= maxord; ++j) N_VDestroy(zn[j]);

  lrw -= (maxord + 5)*lrw1;
  liw -= (maxord + 5)*liw1;

  if (rk_mem->rk_VabstolMallocDone) {
    N_VDestroy(Vabstol);
    lrw -= lrw1;
    liw -= liw1;
  }
}

// Allocating the RK vectors ewt, acor, tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
static bool RKAllocVectors (RKMem rk_mem, N_Vector tmpl, int tol)
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

  // Allocating zn[0] ... zn[qmax]
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

  if (tol == RK_SV) {
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
    rk_mem->rk_VabstolMallocDone = TRUE;
  }

  // Storing the value of qmax used here
  rk_mem->rk_qmax_alloc = qmax;
  
  return (TRUE);
}

/* Setting the error weight vector ewt according to tol_type as follows:
 *
 *  (1) ewt[i] = 1 / (reltol * ABS(ycur[i]) + *abstol), i=0,...,neq-1
 *     if tol_type = RK_SS
 *  (2) ewt[i] = 1 / (reltol * ABS(ycur[i]) + abstol[i]), i=0,...,neq-1
 *     if tol_type = RK_SV
 */
int RKEwtSet (N_Vector ycur, N_Vector weight, void *data)
{
  int flag = 0;

  // Here, data points to rk_mem
  RKMem rk_mem;
  rk_mem = (RKMem) data;

  switch (itol) {
    case RK_SS:
      flag = RKEwtSetSS (rk_mem, ycur, weight);
      break;
    case RK_SV:
      flag = RKEwtSetSV (rk_mem, ycur, weight);
      break;
  }
  
  return (flag);
}

static int RKEwtSetSS (RKMem rk_mem, N_Vector ycur, N_Vector weight)
{
  N_VAbs (ycur, tempv);
  N_VScale (reltol, tempv, tempv);
  N_VAddConst (tempv, Sabstol, tempv);
  if (N_VMin(tempv) <= ZERO) return (-1);
  N_VInv(tempv, weight);
  return (0);
}

static int RKEwtSetSV (RKMem rk_mem, N_Vector ycur, N_Vector weight)
{
  N_VAbs (ycur, tempv);
  N_VLinearSum(reltol, tempv, ONE, Vabstol, tempv);
  if (N_VMin(tempv) <= ZERO) return (-1);
  N_VInv(tempv, weight);
  return (0);
}

// Specifying the time beyond which the integration is not to proceed
int RKSetStopTime (void* rkutta_mem, R itstop)
{
  if (rkutta_mem==NULL) {
    RKProcessError (NULL, RK_MEM_NULL, "Runge-Kutta", "RKSetStopTime", MSGRK_NO_MEM);
    return (RK_MEM_NULL);
  }
  RKMem rk_mem;
  rk_mem = (RKMem) rkutta_mem;
  rk_mem->rk_tstop = itstop;
  rk_mem->rk_tstopset = TRUE;

  return (RK_SUCCESS);
}

//Checking input consistency
static int RKInitialSetup (RKMem rk_mem)
{
  int ier;
  int ewtsetOK;

  // Checking if efun is provided
  if (itol != RK_WF) {
    efun = RKEwtSet;
    e_data = (void *)rk_mem;
  }
  else {
    if (efun == NULL) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGRK_NO_EFUN);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		  return (RK_NO_EFUN);
	  }
	  else
	  {
		  return (RK_ILL_INPUT);
	  }
    }
  }

  ewtsetOK = efun (zn[0], ewt, e_data);
  if (ewtsetOK != 0) {

    if (itol == RK_WF){ 
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGRK_EWT_FAIL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		return (RK_EWT_FAIL);
	  }
	  else
	  {
		return (RK_ILL_INPUT);
	  }
    }
    else { 
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGRK_BAD_EWT);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		  return (RK_BAD_EWT);
	  }
	  else
	  {
		  return (RK_ILL_INPUT);
	  }
    }
  }
  
  // Checking if lsolve function exists (if needed) and calling linit function (if it exists)
  if (mode_iter == RK_NEWTON) {
    if (lsolve == NULL) {
      RKProcessError (rk_mem, RK_ILL_INPUT, "Runge-Kutta", "RKInitialSetup", MSGRK_LSOLVE_NULL);
	  /* SUNDIALS EXTENSION */
	  if (is_sundials_with_extension())
	  {
		return (RK_LSOLVE_NULL);
	  }
	  else
	  {
		return (RK_ILL_INPUT);
	  }
    }
    if (linit != NULL) {
      ier = linit (rk_mem);
      if (ier != 0) {
        RKProcessError (rk_mem, RK_LINIT_FAIL, "Runge-Kutta", "RKInitialSetup", MSGRK_LINIT_FAIL);
        return (RK_LINIT_FAIL);
      }
    }
  }
  
  return (RK_SUCCESS);
}

/* Performing various update operations when the solution to the nonlinear system has passed the local error test. 
 * Incrementing the step counter nSolCalls, recording the values hu and qu, updating the tau array, and applying the corrections to the zn array.
 * The tau[i] are the last q values of h, with tau[1] the most recent.
 * Decrementing qwait, and if qwait == 1 (and q < qmax), saving acor and tq[5] for a possible order increase.
 */
static void RKCompleteStep (RKMem rk_mem)
{
  int i, j;
  
  nSolCalls++;
  nscon++;
  hu = h;
  qu = q;

  for (i=q; i >= 2; i--)  tau[i] = tau[i-1];
  if ((q==1) && (nSolCalls > 1)) tau[2] = tau[1];
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
 * Then rescaling h by eta, and resetting hscale to h.
 */
static void RKRescale (RKMem rk_mem)
{
  int j;
  R factor;
  
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

// Adjusting the history array on a change of order q by deltaq when mode_integ == RK_ADAMS
static void RKAdjustAdams (RKMem rk_mem, int deltaq)
{
  int i, j;
  R xi, hsum;

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

/* Handling adjustments to the history array on a change of order by deltaq when mode_integ == RK_BDF.
 * Calling RKIncreaseBDF if deltaq = +1 and RKDecreaseBDF if deltaq = -1 to do the actual work.
 */
static void RKAdjustBDF (RKMem rk_mem, int deltaq)
{
  switch (deltaq) {
    case 1 :
      RKIncreaseBDF (rk_mem);
      return;
    case -1:
      RKDecreaseBDF (rk_mem);
      return;
  }
}

/* Adjusting the history array on an increase in the order q in the case that mode_integ == RK_BDF.  
 * Setting a new column zn[q+1] equal to a multiple of the saved vector (= acor) in zn[indx_acor].
 * Then adjusting each zn[j] by a multiple of zn[q+1].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h.
 */
static void RKIncreaseBDF (RKMem rk_mem)
{
  R alpha0, alpha1, prod, xi, xiold, hsum, A1;
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

/* Adjusting the history array on a decrease in the order q when mode_integ == RK_BDF.  
 * Adjusting each zn[j] by a multiple of zn[q].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h.
 */
static void RKDecreaseBDF (RKMem rk_mem)
{
  R hsum, xi;
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
 * Otherwise, calling RKAdjustAdams or RKAdjustBDF to handle the order change (depending on the value of mode_integ).
 */
static void RKAdjustOrder (RKMem rk_mem, int deltaq)
{
  if ((q==2) && (deltaq != 1)) return;
  
  switch (mode_integ) {
    case RK_ADAMS: 
      RKAdjustAdams (rk_mem, deltaq);
      break;
    case RK_BDF:   
      RKAdjustBDF (rk_mem, deltaq);
      break;
  }
}

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
static bool RKDoErrorTest (RKMem rk_mem, int *nflagPtr, R saved_t, int *nefPtr, R *dsmPtr)
{
  R dsm;
  int retval;
  
  dsm = acnrm / tq[2];

  // If estimated local error norm dsm passes test, returning RK_SUCCESS  
  *dsmPtr = dsm;
  if (dsm <= 1) return (RK_SUCCESS);
  
  // If test failed, incrementing counters, setting nflag and restoring zn array
  (*nefPtr)++;
  netf++;
  *nflagPtr = PREV_ERR_FAIL;
  RKRestore (rk_mem, saved_t);

  // At maxnef failures or |h| = hmin, returning RK_ERR_FAILURE */
  if ((ABS (h) <= hmin*ONEPSM) || (*nefPtr == maxnef)) return (RK_ERR_FAILURE);

  // Setting etamax = 1 to prevent step size increase at end of this step
  etamax = 1;

  // Setting h ratio eta from dsm, rescaleing, and returning for new try of step 
  if (*nefPtr <= MXNEF1) {
    eta = 1 / (RPowerR(BIAS2*dsm,1/L) + ADDON);
    eta = MAX(ETAMIN, MAX(eta, hmin / ABS (h)));
    if (*nefPtr >= SMALL_NEF) eta = MIN (eta, ETAMXF);
    RKRescale (rk_mem);
    return (TRY_AGAIN);
  }
  
  // After MXNEF1 failures, forcing an order reduction and retrying step
  if (q > 1) {
    eta = MAX(ETAMIN, hmin / ABS (h));
    RKAdjustOrder (rk_mem,-1);
    L = q;
    q--;
    qwait = L;
    RKRescale (rk_mem);
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
  if (retval < 0)  return (RK_RHSFUNC_FAIL);
  if (retval > 0)  return (RK_UNREC_RHSFUNC_ERR);

  N_VScale (h, tempv, zn[1]);

  return (TRY_AGAIN);
}

/* Restoring the value of tn to saved_t and undoing the prediction.
 * After execution of RKRestore, the Nordsieck array zn has the same values as before the call to RKPredict.
 */
static void RKRestore (RKMem rk_mem, R saved_t)
{
  int j, k;
  
  tn = saved_t;
  for (k = 1; k <= q; ++k)
    for (j = q; j >= k; j--)
      N_VLinearSum (ONE, zn[j-1], -ONE, zn[j], zn[j-1]);
}

/* Calling RKSetAdams or RKSetBDF to set the polynomial l, the test quantity array tq, 
 * and the related variables rl1, gamma and gamrat.
 */
static void RKSet (RKMem rk_mem)
{
  switch (mode_integ) {
    case RK_ADAMS:
      RKSetAdams (rk_mem);
      break;
    case RK_BDF:
      RKSetBDF (rk_mem);
      break;
  }
  rl1 = ONE / l[1];
  gamma = h * rl1;
  if (nSolCalls == 0) gammap = gamma;
  gamrat = (nSolCalls > 0) ? gamma / gammap : ONE;  // Protect x / x != 1.0
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
 * step may be reattempted, depending on the nature of the failure.
 */
static int RKStep (RKMem rk_mem)
{
  R saved_t, dsm;
  int ncf, nef;
  int nflag, kflag, eflag;
  
  saved_t = tn;
  ncf = nef = 0;
  nflag = FIRST_CALL;

  if ((nSolCalls > 0) && (hprime != h)) RKAdjustParams(rk_mem);
  
  // Looping point for attempts to take a step
  for(;;) {

    RKPredict (rk_mem);
    RKSet (rk_mem);
    nflag = RKNls (rk_mem, nflag);
    kflag = RKHandleNFlag (rk_mem, &nflag, saved_t, &ncf);

    // Going back in loop if we need to predict again (nflag = PREV_CONV_FAIL)
    if (kflag == PREDICT_AGAIN) continue;

    // Returning if nonlinear solve failed and recovery is impossible
    if (kflag != DO_ERROR_TEST) return (kflag);

    // Performing error test (nflag = RK_SUCCESS)
    eflag = RKDoErrorTest(rk_mem, &nflag, saved_t, &nef, &dsm);

    // Going back in loop if we need to predict again (nflag = PREV_ERR_FAIL)
    if (eflag == TRY_AGAIN)  continue;

    // Returning if error test failed and recovery is impossible
    if (eflag != RK_SUCCESS) return (eflag);

    // If error test passed (eflag = RK_SUCCESS), breaking from loop
    break;
  }

  /* If nonlinear system solve and error test were both successful,
   * Updating data, and considering change of step and/or order */
  RKCompleteStep (rk_mem);
  RKPrepareNextStep (rk_mem, dsm);

  /* If Stablilty Limit Detection is turned on, calling stability limit
     detection routine for possible order reduction */
  if (sldeton) RKBDFStab(rk_mem);
  etamax = (nSolCalls <= SMALL_NST) ? ETAMX2 : ETAMX3;

  // Finally, rescaling the acor array to be the estimated local error vector
  N_VScale(ONE/tq[2], acor, acor);
  
  return(RK_SUCCESS);
}

/* Handling the computation of l and tq when mode_integ == RK_ADAMS.
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
static void RKSetAdams (RKMem rk_mem)
{
  R m[L_MAX], M[3], hsum;
  
  if (q == 1) {
    l[0] = l[1] = tq[1] = tq[5] = ONE;
    tq[2] = TWO;
    tq[3] = TWELVE;
    tq[4] = nlscoef * tq[2];       // = 0.1 * tq[2]
    return;
  }
  
  hsum = RKAdamsStart (rk_mem, m);
  
  M[0] = RKAltSum (q-1, m, 1);
  M[1] = RKAltSum (q-1, m, 2);
  
  RKAdamsFinish (rk_mem, m, M, hsum);
}

// Generating in m[] the coefficients of the product polynomial needed for the Adams l and tq coefficients for q > 1
static R RKAdamsStart (RKMem rk_mem, R m[])
{
  R hsum, xi_inv, sum;
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
 * of a polynomial x^(k-1) M(x) given the coefficients of M(x).
 */
static R RKAltSum (int iend, R a[], int k)
{
  int i, sign;
  R sum;
  
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
static void RKAdamsFinish (RKMem rk_mem, R m[], R M[], R hsum)
{
  int i;
  R M0_inv, xi, xi_inv;
  
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

// Setting the test quantity array tq when mode_integ == RK_BDF
static void RKSetTqBDF (RKMem rk_mem, R hsum, R alpha0, R alpha0_hat, R  xi_inv, R xistar_inv)
{
  R A1, A2, A3, A4, A5, A6;
  R C, CPrime, CPrimePrime;
  
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

/* Computing the coefficients l and tq when mode_integ == RK_BDF.
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
 * and the selection of h at a new order.
 */
static void RKSetBDF (RKMem rk_mem)
{
  R alpha0, alpha0_hat, xi_inv, xistar_inv, hsum;
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

  RKSetTqBDF (rk_mem, hsum, alpha0, alpha0_hat, xi_inv, xistar_inv);
}

// Error handling function
void RKProcessError (RKMem rk_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
  va_list ap;
  char msg[256];

  // Initializing the argument pointer variable (msgfmt is the last required argument to RKProcessError)
  va_start (ap, msgfmt);

  if (rk_mem == NULL) {
    // Writing to stderr
	#ifndef NO_FPRINTF_OUTPUT
      fprintf (stderr, "\n[%s ERROR]  %s\n  ", module, fname);
      fprintf (stderr, msgfmt);
      fprintf (stderr, "\n\n");
	#endif
  }
  else {
    // Composing the message
    vsprintf (msg, msgfmt, ap);

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
  RKMem rk_mem;
  char err_type[10];

  // Here, data points to rk_mem
  rk_mem = (RKMem) data;

  if (error_code == RK_WARNING) sprintf(err_type,"WARNING");
  else sprintf(err_type,"ERROR");

  #ifndef NO_FPRINTF_OUTPUT
  if (errfp!=NULL) {
    fprintf(errfp,"\n[%s %s]  %s\n",module,err_type,function);
    fprintf(errfp,"  %s\n\n",msg);
  }
  #endif
  return;
}

// Checking if all required vector operations are present
static bool RKCheckNvector (N_Vector tmpl)
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

/* Performing the Newton iteration. If the iteration succeeds, returning RK_SUCCESS
 * If not, it may signal the RKNlsNewton routine to call lsetup again and reattempt the iteration, by
 * returning the value TRY_AGAIN (in this case, RKNlsNewton must set convfail to RK_FAIL_BAD_J before calling setup again).
 * Otherwise, returning one of the appropriate values RK_LSOLVE_FAIL, RK_RHSFUNC_FAIL, CONV_FAIL,
 * or RHSFUNC_RECVR back to RKNlsNewton.
 */
static int RKNewtonIteration (RKMem rk_mem)
{
  int m, retval;
  R del, delp, dcon;
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
    retval = lsolve (rk_mem, b, ewt, y, ftemp); 
    nni++;
    
    if (retval < 0) return (RK_LSOLVE_FAIL);
    
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
      return (RK_SUCCESS); // Nonlinear system was solved successfully
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
    if (retval < 0) return (RK_RHSFUNC_FAIL);
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
 * Called by RK with k = 0 and t = t, but may also be called directly by the user.
 */
int RKGetDky (void *rkutta_mem, R t, int k, N_Vector dky)
{
  // Checking arguments
  if (rkutta_mem == NULL) {
    RKProcessError (NULL, RK_MEM_NULL, "Runge-Kutta", "RKGetDky", MSGRK_NO_MEM);
    return (RK_MEM_NULL);
  }
  RKMem rk_mem;
  rk_mem = (RKMem) rkutta_mem;

  if (dky == NULL) {
    RKProcessError (rk_mem, RK_BAD_DKY, "Runge-Kutta", "RKGetDky", MSGRK_NULL_DKY);
    return (RK_BAD_DKY);
  }

  if ((k < 0) || (k > q)) {
    RKProcessError (rk_mem, RK_BAD_K, "Runge-Kutta", "RKGetDky", MSGRK_BAD_K);
    return (RK_BAD_K);
  }
  
  // Allowing some slack
  R s, c, r;
  R tfuzz, tp, tn1;
  int i, j;
  tfuzz = FUZZ_FACTOR * uround * (ABS(tn) + ABS(hu));
  if (hu < ZERO) tfuzz = -tfuzz;
  tp = tn - hu - tfuzz;
  tn1 = tn + tfuzz;
  if ((t-tp)*(t-tn1) > ZERO) {
    RKProcessError (rk_mem, RK_BAD_T, "Runge-Kutta", "RKGetDky", MSGRK_BAD_T, t, tn-hu, tn);
    return (RK_BAD_T);
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
  if (k == 0) return (RK_SUCCESS);
  r = RPowerI (h,-k);
  N_VScale (r, dky, dky);
  return (RK_SUCCESS);
}

/* Printing error messages for all cases of failure by RKHin and RKStep.
 * Returning to RK the value that RK is to return to the user.
 */
static int RKHandleFailure (RKMem rk_mem, int flag)
{
  // Setting vector of absolute weighted local errors
  /*
  N_VProd(acor, ewt, tempv);
  N_VAbs(tempv, tempv);
  */

  // Depending on kflag, printing an error message and returning the error flag
  switch (flag) {
    case RK_ERR_FAILURE: 
      RKProcessError(rk_mem, RK_ERR_FAILURE, "Runge-Kutta", "RK", MSGRK_ERR_FAILS, tn, h);
      break;
    case RK_CONV_FAILURE:
      RKProcessError(rk_mem, RK_CONV_FAILURE, "Runge-Kutta", "RK", MSGRK_CONV_FAILS, tn, h);
      break;
    case RK_LSETUP_FAIL:
      RKProcessError(rk_mem, RK_LSETUP_FAIL, "Runge-Kutta", "RK", MSGRK_SETUP_FAILED, tn);
      break;
    case RK_LSOLVE_FAIL:
      RKProcessError(rk_mem, RK_LSOLVE_FAIL, "Runge-Kutta", "RK", MSGRK_SOLVE_FAILED, tn);
      break;
    case RK_RHSFUNC_FAIL:
      RKProcessError(rk_mem, RK_RHSFUNC_FAIL, "Runge-Kutta", "RK", MSGRK_RHSFUNC_FAILED, tn);
      break;
    case RK_UNREC_RHSFUNC_ERR:
      RKProcessError(rk_mem, RK_UNREC_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGRK_RHSFUNC_UNREC, tn);
      break;
    case RK_REPTD_RHSFUNC_ERR:
      RKProcessError(rk_mem, RK_REPTD_RHSFUNC_ERR, "Runge-Kutta", "RK", MSGRK_RHSFUNC_REPTD, tn);
      break;
    case RK_RTFUNC_FAIL:    
      RKProcessError(rk_mem, RK_RTFUNC_FAIL, "Runge-Kutta", "RK", MSGRK_RTFUNC_FAILED, tn);
      break;
    case RK_TOO_CLOSE:
      RKProcessError(rk_mem, RK_TOO_CLOSE, "Runge-Kutta", "RK", MSGRK_TOO_CLOSE);
    default:
      return(RK_SUCCESS);   
  }

  return(flag);
}

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
static int RKHandleNFlag (RKMem rk_mem, int *nflagPtr, R saved_t, int *ncfPtr)
{
  int nflag;
  nflag = *nflagPtr;
  if (nflag == RK_SUCCESS) return (DO_ERROR_TEST);

  // The nonlinear solution failed; incrementing ncfn and restoring zn
  ncfn++;
  RKRestore(rk_mem, saved_t);
  
  // Returning if lsetup, lsolve, or rhs failed unrecoverably
  if (nflag == RK_LSETUP_FAIL)  return (RK_LSETUP_FAIL);
  if (nflag == RK_LSOLVE_FAIL)  return (RK_LSOLVE_FAIL);
  if (nflag == RK_RHSFUNC_FAIL) return (RK_RHSFUNC_FAIL);
  
  // At this point, nflag = CONV_FAIL or RHSFUNC_RECVR; incrementing ncf
  (*ncfPtr)++;
  etamax = ONE;

  // If we had maxncf failures or |h| = hmin, returning RK_CONV_FAILURE or RK_REPTD_RHSFUNC_ERR
  if ((ABS(h) <= hmin*ONEPSM) || (*ncfPtr == maxncf)) {
    if (nflag == CONV_FAIL)     return (RK_CONV_FAILURE);
    if (nflag == RHSFUNC_RECVR) return (RK_REPTD_RHSFUNC_ERR);    
  }

  // Reducing step size; returning to reattempt the step
  eta = MAX(ETACF, hmin / ABS(h));
  *nflagPtr = PREV_CONV_FAIL;
  RKRescale(rk_mem);
  return (PREDICT_AGAIN);
}

/* End of the program
======================== */
