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
#include <math.h>

#include <sundials/sundials_nvector.h>
#include <nvector/nvector_serial.h>
/* SUNDIALS EXTENSION */
#include "sundials_extension.h"

#if 1

// Math operators
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define SQR(A)    ((A)*(A))
#define SQRT      sqrt
#define EXP       exp
#define ABS       fabs
#define ZERO      RCONST(0.0)     /* real 0.0     */

// mode_integ
#define RK_ADAMS 1
#define RK_BDF   2

// mode_iter
#define RK_FUNCTIONAL 1
#define RK_NEWTON     2

// itol
#define RK_SS 1
#define RK_SV 2
#define RK_WF 3

// itask
#define RK_NORMAL         1
#define RK_ONE_STEP       2
#define RK_NORMAL_TSTOP   3
#define RK_ONE_STEP_TSTOP 4

// Orders
#define ADAMS_Q_MAX 12     /* max value of q for mode_integ == ADAMS     */
#define BDF_Q_MAX    5     /* max value of q for mode_integ == BDF       */
#define Q_MAX  ADAMS_Q_MAX /* max value of q for either mode_integ       */
#define L_MAX  (Q_MAX+1)   /* max value of L for either mode_integ       */
#define NUM_TESTS    5     /* number of error test quantities            */

// Step sizes
#define HMIN_DEFAULT     RCONST(0.0)    /* hmin default value     */
#define HMAX_INV_DEFAULT RCONST(0.0)    /* hmax_inv default value */
#define MXHNIL_DEFAULT   10             /* mxhnil default value   */
#define MXSTEP_DEFAULT   500            /* mxstep default value   */

/*
 * ----------------------------------------
 * RK return flags
 * ----------------------------------------
 */
/* SUNDIALS EXTENSION */
#define RK_ZERO_DETACH_RETURN    3

#define RK_SUCCESS               0
#define RK_TSTOP_RETURN          1
#define RK_ROOT_RETURN           2

#define RK_WARNING              99

#define RK_TOO_MUCH_WORK        -1
#define RK_TOO_MUCH_ACC         -2
#define RK_ERR_FAILURE          -3
#define RK_CONV_FAILURE         -4

#define RK_LINIT_FAIL           -5
#define RK_LSETUP_FAIL          -6
#define RK_LSOLVE_FAIL          -7
#define RK_RHSFUNC_FAIL         -8
#define RK_FIRST_RHSFUNC_ERR    -9
#define RK_REPTD_RHSFUNC_ERR    -10
#define RK_UNREC_RHSFUNC_ERR    -11
#define RK_RTFUNC_FAIL          -12

#define RK_MEM_FAIL             -20
#define RK_MEM_NULL             -21
#define RK_ILL_INPUT            -22
#define RK_NO_MALLOC            -23
#define RK_BAD_K                -24
#define RK_BAD_T                -25
#define RK_BAD_DKY              -26
#define RK_TOO_CLOSE            -27

/* SUNDIALS EXTENSION */
#define RK_YOUT_NULL            -28
#define RK_TOLD_NULL            -29
#define RK_BAD_ITASK            -30
#define RK_NO_TSTOP             -31
#define RK_BAD_H0               -32
#define RK_BAD_TSTOP            -33
#define RK_BAD_INIT_ROOT        -34
#define RK_NO_EFUN              -35
#define RK_EWT_FAIL             -36
#define RK_BAD_EWT              -37
#define RK_LSOLVE_NULL          -38
#define RK_NULL_Y0              -39
#define RK_BAD_ITOL             -40
#define RK_NULL_F               -41
#define RK_BAD_NVECTOR          -42
#define RK_NULL_ABSTOL          -43
#define RK_BAD_RELTOL           -44
#define RK_BAD_ABSTOL           -45
#define RK_NULL_G               -46
#define RK_BAD_TOUT             -47
#define RK_CLOSE_ROOTS          -48
#define RK_ADD_VEC              -49

#define rk_NO_FAILURES 0
#define rk_FAIL_BAD_J  1
#define rk_FAIL_OTHER  2

// Initialization and I/O error messages
#define MSG_TIME      "t = %g"
#define MSG_TIME_H    "t = %g and h = %g"
#define MSG_TIME_INT  "t = %g is not between tcur - hu = %g and tcur = %g."
#define MSG_TIME_TOUT "tout = %g"

#define MSGRK_NO_MEM "rk_mem = NULL illegal."
#define MSGRK_RKMEM_FAIL "Allocation of rk_mem failed."
#define MSGRK_MEM_FAIL "A memory request failed."
#define MSGRK_BAD_MODE_INTEG  "Illegal value for mode_integ. The legal values are RK_ADAMS and RK_BDF."
#define MSGRK_BAD_MODE_ITER  "Illegal value for mode_iter. The legal values are RK_FUNCTIONAL and RK_NEWTON."
#define MSGRK_BAD_ITOL "Illegal value for itol. The legal values are RK_SS, RK_SV, and RK_WF."
#define MSGRK_NO_MALLOC "Attempt to call before RKMalloc."
#define MSGRK_NEG_MAXORD "maxord <= 0 illegal."
#define MSGRK_BAD_MAXORD  "Illegal attempt to increase maximum method order."
#define MSGRK_NEG_MXSTEPS "mxsteps < 0 illegal."
#define MSGRK_SET_SLDET  "Attempt to use stability limit detection with the RK_ADAMS method illegal."
#define MSGRK_NEG_HMIN "hmin < 0 illegal."
#define MSGRK_NEG_HMAX "hmax < 0 illegal."
#define MSGRK_BAD_HMIN_HMAX "Inconsistent step size limits: hmin > hmax."
#define MSGRK_BAD_RELTOL "reltol < 0 illegal."
#define MSGRK_BAD_ABSTOL "abstol has negative component(s) (illegal)."
#define MSGRK_NULL_ABSTOL "abstol = NULL illegal."
#define MSGRK_NULL_Y0 "y0 = NULL illegal."
#define MSGRK_NULL_F "f = NULL illegal."
#define MSGRK_NULL_G "g = NULL illegal."
#define MSGRK_BAD_NVECTOR "A required vector operation is not implemented."
#define MSGRK_BAD_K "Illegal value for k."
#define MSGRK_NULL_DKY "dky = NULL illegal."
#define MSGRK_BAD_T "Illegal value for t." MSG_TIME_INT

// RK error messages
#define MSGRK_LSOLVE_NULL "The linear solver's solve routine is NULL."
#define MSGRK_YOUT_NULL "yout = NULL illegal."
#define MSGRK_TOLD_NULL "told = NULL illegal."
#define MSGRK_BAD_EWT "Initial ewt has component(s) equal to zero (illegal)."
#define MSGRK_EWT_NOW_BAD "At " MSG_TIME ", a component of ewt has become <= 0."
#define MSGRK_BAD_ITASK "Illegal value for itask."
#define MSGRK_BAD_H0 "h0 and tout - t0 inconsistent."
#define MSGRK_BAD_INIT_ROOT "Root found at and very near initial t."
#define MSGRK_BAD_TOUT "Trouble interpolating at " MSG_TIME_TOUT ". tout too far back in direction of integration"
#define MSGRK_NO_EFUN "itol = RK_WF but no EwtSet function was provided."
#define MSGRK_NO_TSTOP "itask = RK_NORMAL_TSTOP or itask = RK_ONE_STEP_TSTOP but tstop was not set."
#define MSGRK_EWT_FAIL "The user-provide EwtSet function failed."
#define MSGRK_EWT_NOW_FAIL "At " MSG_TIME ", the user-provide EwtSet function failed."
#define MSGRK_LINIT_FAIL "The linear solver's init routine failed."
#define MSGRK_HNIL_DONE "The above warning has been issued mxhnil times and will not be issued again for this problem."
#define MSGRK_TOO_CLOSE "tout too close to t0 to start integration."
#define MSGRK_MAX_STEPS "At " MSG_TIME ", mxstep steps taken before reaching tout."
#define MSGRK_TOO_MUCH_ACC "At " MSG_TIME ", too much accuracy requested."
#define MSGRK_HNIL "Internal " MSG_TIME_H " are such that t + h = t on the next step. The solver will continue anyway."
#define MSGRK_ERR_FAILS "At " MSG_TIME_H ", the error test failed repeatedly or with |h| = hmin."
#define MSGRK_CONV_FAILS "At " MSG_TIME_H ", the corrector convergence test failed repeatedly or with |h| = hmin."
#define MSGRK_SETUP_FAILED "At " MSG_TIME ", the setup routine failed in an unrecoverable manner."
#define MSGRK_SOLVE_FAILED "At " MSG_TIME ", the solve routine failed in an unrecoverable manner."
#define MSGRK_RHSFUNC_FAILED "At " MSG_TIME ", the right-hand side routine failed in an unrecoverable manner."
#define MSGRK_RHSFUNC_UNREC "At " MSG_TIME ", the right-hand side failed in a recoverable manner, but no recovery is possible."
#define MSGRK_RHSFUNC_REPTD "At " MSG_TIME "repeated recoverable right-hand side function errors."
#define MSGRK_RHSFUNC_FIRST "The right-hand side routine failed at the first call."
#define MSGRK_RTFUNC_FAILED "At " MSG_TIME ", the rootfinding routine failed in an unrecoverable manner."
#define MSGRK_CLOSE_ROOTS "Root found at and very near " MSG_TIME "."
#define MSGRK_BAD_TSTOP "tstop is behind current  MSG_TIME in the direction of integration."
#define MSGRK_BAD_LENGTH "Incompatible vector sizes for addition."
#endif

#define TRUE 1
#define FALSE 0
typedef int bool;
typedef double R;
typedef int  (*RKRootFn) (R t, N_Vector y, R *gout, void *g_data);
typedef int  (*RKRhsFn) (R t, N_Vector y, N_Vector ydot, void *f_data);
typedef int  (*RKEwtFn) (N_Vector y, N_Vector ewt, void *e_data);
typedef void (*RKErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *eh_data); 

#if 1
#define ehfun    (rk_mem->rk_ehfun)
#define eh_data  (rk_mem->rk_eh_data)
#define ONEPSM   RCONST(1.000001)
#define ZERO   RCONST(0.0)     /* real 0.0     */
#define TINY   RCONST(1.0e-10) /* small number */
#define TENTH  RCONST(0.1)     /* real 0.1     */
#define POINT2 RCONST(0.2)     /* real 0.2     */
#define FOURTH RCONST(0.25)    /* real 0.25    */
#define HALF   RCONST(0.5)     /* real 0.5     */
#define ONE    RCONST(1.0)     /* real 1.0     */
#define TWO    RCONST(2.0)     /* real 2.0     */
#define THREE  RCONST(3.0)     /* real 3.0     */
#define FOUR   RCONST(4.0)     /* real 4.0     */
#define FIVE   RCONST(5.0)     /* real 5.0     */
#define TWELVE RCONST(12.0)    /* real 12.0    */
#define HUN    RCONST(100.0)   /* real 100.0   */

/*=================================================================*/
/*             RK Routine-Specific Constants                       */
/*=================================================================*/

/* 
 * Control constants for lower-level functions used by RKStep 
 * ----------------------------------------------------------
 *
 * RKHin return values:
 *    RK_SUCCESS
 *    RK_RHSFUNC_FAIL
 *    RK_TOO_CLOSE
 *
 * RKStep control constants:
 *    DO_ERROR_TEST
 *    PREDICT_AGAIN
 *
 * RKStep return values: 
 *    RK_SUCCESS,
 *    RK_LSETUP_FAIL,  RK_LSOLVE_FAIL, 
 *    RK_RHSFUNC_FAIL, RK_RTFUNC_FAIL
 *    RK_CONV_FAILURE, RK_ERR_FAILURE,
 *    RK_FIRST_RHSFUNC_ERR
 *
 * RKNls input nflag values:
 *    FIRST_CALL
 *    PREV_CONV_FAIL
 *    PREV_ERR_FAIL
 *    
 * RKNls return values: 
 *    RK_SUCCESS,
 *    RK_LSETUP_FAIL, RK_LSOLVE_FAIL, RK_RHSFUNC_FAIL,
 *    CONV_FAIL, RHSFUNC_RECVR
 * 
 * RKNewtonIteration return values:
 *    RK_SUCCESS, 
 *    RK_LSOLVE_FAIL, RK_RHSFUNC_FAIL
 *    CONV_FAIL, RHSFUNC_RECVR,
 *    TRY_AGAIN
 * 
 */

#define DO_ERROR_TEST    +2
#define PREDICT_AGAIN    +3

#define CONV_FAIL        +4 
#define TRY_AGAIN        +5

#define FIRST_CALL       +6
#define PREV_CONV_FAIL   +7
#define PREV_ERR_FAIL    +8

#define RHSFUNC_RECVR    +9

/*
 * Control constants for lower-level rootfinding functions
 * -------------------------------------------------------
 *
 * RKRcheck1 return values:
 *    RK_SUCCESS,
 *    RK_RTFUNC_FAIL,
 *    INITROOT
 * RKRcheck2 return values:
 *    RK_SUCCESS
 *    RK_RTFUNC_FAIL,
 *    CLOSERT
 *    RTFOUND
 * RKRcheck3 return values:
 *    RK_SUCCESS
 *    RK_RTFUNC_FAIL,
 *    RTFOUND
 * RKRootFind return values:
 *    RK_SUCCESS
 *    RK_RTFUNC_FAIL,
 *    RTFOUND
 */

#define RTFOUND          +1
#define INITROOT         +2
#define CLOSERT          +3

/* SUNDIALS EXTENSION */
#define ZERODETACHING    +4
#define MASKED           55

/*
 * Algorithmic constants
 * ---------------------
 *
 * RKGetDky and RKStep
 *
 *    FUZZ_FACTOR
 *
 * RKHin
 *
 *    HLB_FACTOR
 *    HUB_FACTOR
 *    H_BIAS
 *    MAX_ITERS
 *
 * RKCreate 
 *
 *   CORTES
 *
 * RKStep
 *
 *    THRESH
 *    ETAMX1
 *    ETAMX2
 *    ETAMX3
 *    ETAMXF
 *    ETAMIN
 *    ETACF
 *    ADDON
 *    BIAS1
 *    BIAS2
 *    BIAS3
 *    ONEPSM
 *
 *    SMALL_NST   nSolCalls > SMALL_NST => use ETAMX3 
 *    MXNCF       max no. of convergence failures during one step try
 *    MXNEF       max no. of error test failures during one step try
 *    MXNEF1      max no. of error test failures before forcing a reduction of order
 *    SMALL_NEF   if an error failure occurs and SMALL_NEF <= nef <= MXNEF1, then
 *                reset eta =  MIN (eta, ETAMXF)
 *    LONG_WAIT   number of steps to wait before considering an order change when
 *                q==1 and MXNEF1 error test failures have occurred
 *
 * RKNls
 *    
 *    NLS_MAXCOR  maximum no. of corrector iterations for the nonlinear solver
 *    CRDOWN      constant used in the estimation of the convergence rate (crate)
 *                of the iterates for the nonlinear equation
 *    DGMAX       mode_iter == RK_NEWTON, |gamma/gammap-1| > DGMAX => call lsetup
 *    RDIV        declare divergence if ratio del/delp > RDIV
 *    MSBP        max no. of steps between lsetup calls
 *    
 */


#define FUZZ_FACTOR RCONST(100.0)

#define HLB_FACTOR RCONST(100.0)
#define HUB_FACTOR RCONST(0.1)
#define H_BIAS     HALF
#define MAX_ITERS  4

#define CORTES RCONST(0.1)

#define THRESH RCONST(1.5)
#define ETAMX1 RCONST(10000.0) 
#define ETAMX2 RCONST(10.0)
#define ETAMX3 RCONST(10.0)
#define ETAMXF RCONST(0.2)
#define ETAMIN RCONST(0.1)
#define ETACF  RCONST(0.25)
#define ADDON  RCONST(0.000001)
#define BIAS1  RCONST(6.0)
#define BIAS2  RCONST(6.0)
#define BIAS3  RCONST(10.0)
#define ONEPSM RCONST(1.000001)

#define SMALL_NST    10
#define MXNCF        10
#define MXNEF         7
#define MXNEF1        3
#define SMALL_NEF     2
#define LONG_WAIT    10

#define NLS_MAXCOR 3
#define CRDOWN     RCONST(0.3)
#define DGMAX      RCONST(0.3)

#define RDIV      TWO
#define MSBP       20
#endif

typedef struct RKMemRec {

    R rk_uround;    // Machine unit roundoff

    /*-------------------------- 
      Problem Specification Data 
      --------------------------*/
    RKRhsFn rk_f;         /* y' = f(t,y(t))                        */
    void *rk_f_data;     /* user pointer passed to f               */
    int rk_mode_integ;   /* mode_integ = RK_ADAMS or RK_BDF        */
    int rk_mode_iter;    /* mode_iter = RK_FUNCTIONAL or RK_NEWTON */
    int rk_itol;         /* itol = RK_SS or RK_SV                  */
    R rk_reltol; 		 /* relative tolerance                      */
    R rk_Sabstol; 		 /* scalar absolute tolerance               */
    N_Vector rk_Vabstol; /* vector absolute tolerance         		*/
    RKEwtFn rk_efun;     /* function to set ewt              		*/
    void *rk_e_data;    /* user pointer passed to efun     		*/

    /*-----------------------
      Nordsieck History Array 
      -----------------------*/
    N_Vector rk_zn[L_MAX];
    /* Nordsieck array, of size N x (q+1).  */
    /* zn[j] is a vector of length N (j=0,...,q)   */
    /* zn[j] = [1/factorial(j)] * h^j * (jth       */
    /* derivative of the interpolating polynomial  */

    /*--------------------------
      other vectors of length N 
      -------------------------*/
    N_Vector rk_ewt;     /* error weight vector                          */
    N_Vector rk_y;       /* y is used as temporary storage by the solver */
    /* The memory is provided by the user to RK  */
    /* where the vector is named yout.           */
    N_Vector rk_acor;    /* In the context of the solution of the        */
    /* nonlinear equation, acor = y_n(m) - y_n(0).  */
    /* On return, this vector is scaled to give     */
    /* the estimated local error in y.              */
    N_Vector rk_tempv;   /* temporary storage vector                     */
    N_Vector rk_ftemp;   /* temporary storage vector                     */

    /*-----------------
      Tstop information
      -----------------*/
    bool rk_tstopset;
    bool rk_istop;
    R rk_tstop;

    /*---------
      Step Data 
      ---------*/  
    int rk_q;         /* current order                           */
    int rk_qprime;    /* order to be used on the next step       */ 
    /* = q-1, q, or q+1                        */
    int rk_next_q;    /* order to be used on the next step       */
    int rk_qwait;     /* number of internal steps to wait before */
    /* considering a change in q               */
    int rk_L;         /* L = q + 1                               */

    R rk_hin;    /* initial step size                      */
    R rk_h;      /* current step size                      */
    R rk_hprime; /* step size to be used on the next step  */ 
    R rk_next_h; /* step size to be used on the next step  */ 
    R rk_eta;    /* eta = hprime / h                       */
    R rk_hscale; /* value of h used in zn                  */
    R rk_tn;     /* current internal value of t            */
    R rk_toldlast; /* value of told last returned by RK */

    R rk_tau[L_MAX+1];    /* array of previous q+1 successful step     */
    /* sizes indexed from 1 to q+1               */
    R rk_tq[NUM_TESTS+1]; /* array of test quantities indexed from     */
    /* 1 to NUM_TESTS(=5)                        */
    R rk_l[L_MAX];        /* coefficients of l(x) (degree q poly)      */

    R rk_rl1;     /* the scalar 1/l[1]            */
    R rk_gamma;   /* gamma = h * rl1              */
    R rk_gammap;  /* gamma at the last setup call */
    R rk_gamrat;  /* gamma / gammap               */

    R rk_crate;   /* estimated corrector convergence rate     */
    R rk_acnrm;   /* | acor | wrms                            */
    R rk_nlscoef; /* coeficient in nonlinear convergence test */
    int  rk_mnewt;       /* Newton iteration counter                 */

    /*------
      Limits 
      ------*/
    int rk_qmax;        /* q <= qmax                                          */
    long int rk_mxstep; /* maximum number of internal steps for one user call */
    int rk_maxcor;      /* maximum number of corrector iterations for the     */
    /* solution of the nonlinear equation                 */
    int rk_mxhnil;      /* maximum number of warning messages issued to the   */
    /* user that t + h == t for the next internal step    */
    int rk_maxnef;      /* maximum number of error test failures              */
    int rk_maxncf;      /* maximum number of nonlinear convergence failures   */

    R rk_hmin;     /* |h| >= hmin       */
    R rk_hmax_inv; /* |h| <= 1/hmax_inv */
    R rk_etamax;   /* eta <= etamax     */

    /*--------
      Counters 
      --------*/
    long int rk_nSolCalls;              /* number of internal steps taken             */
    long int rk_nfe;              /* number of f calls                          */
    long int rk_ncfn;             /* number of corrector convergence failures   */
    long int rk_netf;             /* number of error test failures              */
    long int rk_nni;              /* number of Newton iterations performed      */
    long int rk_nsetups;          /* number of setup calls                      */
    int rk_nhnil;                 /* number of messages issued to the user that */
    
    /* t + h == t for the next internal step */

    R rk_etaqm1;      /* ratio of new to old h for order q-1        */
    R rk_etaq;        /* ratio of new to old h for order q          */
    R rk_etaqp1;      /* ratio of new to old h for order q+1        */

    /*----------------------------
      Space requirements for RKODE 
      ----------------------------*/
    long int rk_lrw1;        /* no. of R words in 1 N_Vector         		*/ 
    long int rk_liw1;        /* no. of integer words in 1 N_Vector         */ 
    long int rk_lrw;         /* no. of R words in RK work vectors 			*/
    long int rk_liw;         /* no. of integer words in RK work vectors  	*/

    /*------------------
      Linear Solver Data 
      ------------------*/
    // Linear Solver functions to be called
    int (*rk_linit) (struct RKMemRec *rk_mem);
    int (*rk_lsetup) (struct RKMemRec *rk_mem, int convfail, N_Vector ypred,
                     N_Vector fpred, bool *jcurPtr, N_Vector vtemp1,
                     N_Vector vtemp2, N_Vector vtemp3); 
    int (*rk_lsolve) (struct RKMemRec *rk_mem, N_Vector b, N_Vector weight,
                     N_Vector ycur, N_Vector fcur);
    void (*rk_lfree) (struct RKMemRec *rk_mem);

    // Linear Solver specific memory
    void *rk_lmem;           

    /*------------
      Saved Values
      ------------*/

    int rk_qu;            /* last successful q value used                        */
    long int rk_nstlp;    /* step number of last setup call                      */
    R rk_h0u;     		   /* actual initial stepsize                             */
    R rk_hu;      		   /* last successful h value used                        */
    R rk_saved_tq5;		   /* saved value of tq[5]                                */
    bool rk_jcur;  		   /* Is the Jacobian info used by linear solver current? */
    R rk_tolsf;     	   /* tolerance scale factor                              */
    int rk_qmax_alloc;    /* value of qmax used when allocating memory           */
    int rk_indx_acor;     /* index of the zn vector in which acor is saved       */
    bool rk_setupNonNull; /* Does setup do something?                      		  */

    bool rk_VabstolMallocDone;
    bool rk_MallocDone;  

    /*-------------------------------------------
      Error handler function and error ouput file 
      -------------------------------------------*/

    RKErrHandlerFn rk_ehfun;    /* Error messages are handled by ehfun      */
    void *rk_eh_data;           /* user pointer passed to ehfun            */
    FILE *rk_errfp;             /* RKODE error messages are sent to errfp  */

    /*-------------------------
      Stability Limit Detection
      -------------------------*/

    bool rk_sldeton;     /* Is Stability Limit Detection on?         */
    R rk_ssdat[6][4];    /* scaled data array for STALD              */
    int rk_nscon;               /* counter for STALD method                 */
    long int rk_nor;            /* counter for number of order reductions   */

    /*----------------
      Rootfinding Data
      ----------------*/

    RKRootFn rk_gfun;     /* Function g for roots sought                     */
    int rk_n_root_functions;         /* number of components of g                       */
    void *rk_g_data;      /* pointer to user data for g                      */
    int *rk_iroots;       /* int array for root information                  */
    R rk_tlo;      /* nearest endpoint of interval in root search     */
    R rk_thi;      /* farthest endpoint of interval in root search    */
    R rk_trout;    /* t value returned by rootfinding routine         */
    R *rk_glo;     /* saved array of g values at t = tlo              */
    R *rk_ghi;     /* saved array of g values at t = thi              */
    R *rk_grout;   /* array of g values at t = trout                  */
    R rk_toutc;    /* copy of tout (if NORMAL mode)                   */
    R rk_ttol;     /* tolerance on root location                      */
    int rk_taskc;         /* copy of parameter task                          */
    int rk_irfnd;         /* flag showing whether last step had a root       */
    long int rk_nge;      /* counter for g evaluations                       */

  } *RKMem;

// Creating the problem
void* RKCreate (int mode_integ, int mode_resol);

// Allocating the problem
int RKMalloc (void *rk_mem, RKRhsFn f, R t0, N_Vector y0, int itol, R reltol, void *abstol);

// Reinitializing the problem
int RKReInit (void *rk_mem, RKRhsFn f, R t0, N_Vector y0, int itol, R reltol, void *abstol);

// Initializing the root-finding problem
int RKRootInit (void *rk_mem, int n_root_functions, RKRootFn g, void *gdata);

// Solving the problem
int RK (void *rk_mem, R t, N_Vector yout, R *told, int pass_tF_or_back);

// Freeing the problem memory allocated by RKMalloc
void RKFree (void **rkutta_mem);

// Freeing the RK vectors allocated in RKAllocVectors
static void RKFreeVectors (RKMem rk_mem);

// Allocating the CVODE vectors ewt, acor, tempv, ftemp, *zn[0], ..., zn[maxord], lrw and liw */
static bool RKAllocVectors (RKMem rk_mem, N_Vector tmpl, int tol);

/* Setting the error weight vector ewt according to tol_type as follows:
 *
 *  (1) ewt[i] = 1 / (reltol * ABS(ycur[i]) + *abstol), i=0,...,neq-1
 *     if tol_type = CV_SS
 *  (2) ewt[i] = 1 / (reltol * ABS(ycur[i]) + abstol[i]), i=0,...,neq-1
 *     if tol_type = CV_SV */
int RKEwtSet (N_Vector ycur, N_Vector weight, void *data);

static int RKEwtSetSS (RKMem rk_mem, N_Vector ycur, N_Vector weight);

static int RKEwtSetSV (RKMem rk_mem, N_Vector ycur, N_Vector weight);

// Specifying the time beyond which the integration is not to proceed
int RKSetStopTime (void* rkutta_mem, R itstop);

// Checking input consistency
static int RKInitialSetup (RKMem rk_mem);

/* Performing various update operations when the solution to the nonlinear system has passed the local error test. 
 * Incrementing the step counter nSolCalls, recording the values hu and qu, updating the tau array, and applying the corrections to the zn array.
 * The tau[i] are the last q values of h, with tau[1] the most recent.
 * Decrementing qwait, and if qwait == 1 (and q < qmax), saving acor and tq[5] for a possible order increase.
 */
static void RKCompleteStep (RKMem rk_mem);

/* Rescaling the Nordsieck array by multiplying the jth column zn[j] by eta^j, j = 1, ..., q.
 * Then rescaling h by eta, and resetting hscale to h.
 */
static void RKRescale (RKMem rk_mem);

// Adjusting the history array on a change of order q by deltaq, in the case that mode_integ == RK_ADAMS.
static void RKAdjustAdams (RKMem rk_mem, int deltaq);

/* Handling adjustments to the history array on a change of order by deltaq when mode_integ == CV_BDF.
 * Calling RKIncreaseBDF if deltaq = +1 and RKDecreaseBDF if deltaq = -1 to do the actual work.
 */
static void RKAdjustBDF (RKMem rk_mem, int deltaq);

/* Adjusting the history array on an increase in the order q in the case that mode_integ == RK_BDF.  
 * Setting a new column zn[q+1] equal to a multiple of the saved vector (= acor) in zn[indx_acor].
 * Then adjusting each zn[j] by a multiple of zn[q+1].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h.
 */
static void RKIncreaseBDF (RKMem rk_mem);

/* Adjusting the history array on a decrease in the order q when mode_integ == RK_BDF.  
 * Adjusting each zn[j] by a multiple of zn[q].
 * The coefficients in the adjustment are the ones of the polynomial x*x*(x+xi_1)*...*(x+xi_j),
 * where xi_j = [t_n - t_(n-j)]/h.
 */
static void RKDecreaseBDF (RKMem rk_mem);

/* Handling an order change by a deltaq (= +1 or -1) amount.
 * If a decrease in order is requested and q==2, then returning the routine immediately.
 * Otherwise, calling RKAdjustAdams or RKAdjustBDF to handle the order change (depending on the value of mode_integ).
 */
static void RKAdjustOrder (RKMem rk_mem, int deltaq);

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
static bool RKDoErrorTest (RKMem rk_mem, int *nflagPtr, R saved_t, int *nefPtr, R *dsmPtr);

/* Restoring the value of tn to saved_t and undoing the prediction.
 * After execution of RKRestore, the Nordsieck array zn has the same values as before the call to RKPredict.
 */
static void RKRestore (RKMem rk_mem, R saved_t);

/* Calling RKSetAdams or RKSetBDF to set the polynomial l, the test quantity array tq, 
 * and the related variables rl1, gamma and gamrat.
 */
static void RKSet (RKMem rk_mem);

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
static int RKStep (RKMem rk_mem);

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
static void RKSetAdams (RKMem rk_mem);

// Generating in m[] the coefficients of the product polynomial needed for the Adams l and tq coefficients for q > 1.
static R RKAdamsStart (RKMem rk_mem, R m[]);

/* Returning the value of the alternating sum (i= 0 ... iend) [ (-1)^i * (a[i] / (i + k)) ].
 * If iend < 0 then returning 0.
 * This operation is needed to compute the integral, from -1 to 0,
 * of a polynomial x^(k-1) M(x) given the coefficients of M(x).
 */
static R RKAltSum (int iend, R a[], int k);

// Completing the calculation of the Adams l and tq.
static void RKAdamsFinish (RKMem rk_mem, R m[], R M[], R hsum);

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
static void RKSetBDF (RKMem rk_mem);

// Setting the test quantity array tq when mode_integ == RK_BDF
static void RKSetTqBDF (RKMem rk_mem, R hsum, R alpha0, R alpha0_hat, R  xi_inv, R xistar_inv);

/* Performing the Newton iteration. If the iteration succeeds, returning RK_SUCCESS.
 * If not, it may signal the RKNlsNewton routine to call lsetup again and reattempt the iteration, by
 * returning the value TRY_AGAIN (in this case, RKNlsNewton must set convfail to CV_FAIL_BAD_J before calling setup again).
 * Otherwise, returning one of the appropriate values CV_LSOLVE_FAIL, CV_RHSFUNC_FAIL, CONV_FAIL,
 * or RHSFUNC_RECVR back to RKNlsNewton.
 */
static int RKNewtonIteration (RKMem rk_mem);

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
int RKGetDky (void *rkutta_mem, R t, int k, N_Vector dky);

// Error handling function
void RKProcessError (RKMem rk_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

// Default error handling function
void RKErrHandler (int error_code, const char *module, const char *function, char *msg, void *data);

// Checking if all required vector operations are present
static bool RKCheckNvector (N_Vector tmpl);

/* Printing error messages for all cases of failure by RKHin and RKStep.
 * Returning to RK the value that RK is to return to the user.
 */
static int RKHandleFailure (RKMem rk_mem, int flag);

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
static int RKHandleNFlag (RKMem rk_mem, int *nflagPtr, R saved_t, int *ncfPtr);

R RPowerI (R base, int exponent)
{
  int i, expt;
  R prod;

  prod = ONE;
  expt = abs(exponent);
  for(i = 1; i <= expt; i++) prod *= base;
  if (exponent < 0) prod = ONE/prod;
  return(prod);
}

R RPowerR (R base, R exponent)
{
  if (base <= ZERO) return (ZERO);
  return (pow(base, exponent));
}

int N_VAdd (N_Vector x, N_Vector y, N_Vector z)
{
  long int i, M, N, O;
  R *xd, *yd,*zd;
  xd = zd = yd = NULL;

  M  = NV_LENGTH_S(x);
  N  = NV_LENGTH_S(y);
  O  = NV_LENGTH_S(z);
  if ((M != N) || (M != O)) return (RK_ADD_VEC);
  
  xd = NV_DATA_S(x);
  yd = NV_DATA_S(y);
  zd = NV_DATA_S(z);

  for (i = 0; i < N; i++) zd[i] = xd[i]+yd[i];

  return (RK_SUCCESS);
}

int N_VMulScal (R a, N_Vector x)
{
  long int i, N;
  R *xd;
  xd = NULL;
  N  = NV_LENGTH_S(x);
  xd = NV_DATA_S(x);

  for (i = 0; i < N; ++i)
    xd[i] *= a;

  return (RK_SUCCESS);
}


/* End of the program
======================== */
