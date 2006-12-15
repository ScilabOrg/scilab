/*
 * int MlistGetFieldNumber(int *ptr, const char *string)
 * int inttimescicos(fname,fname_len)
 * int intduplicate(fname,fname_len)
 * int intdiffobjs(fname,fname_len)
 * int inttree2(fname,fname_len)
 * int inttree3(fname,fname_len)
 * int inttree4 _PARAMS((char *fname,unsigned long fname_len))
 * int intxproperty(fname,fname_len)
 * int intphasesim(fname,fname_len)
 * int intsetxproperty(fname,fname_len)
 * int intsetblockerror(fname,fname_len)
 * void duplicata(n,v,w,ww,nw)
 * void comp_size(v,nw,n)
 * int intscicosimc(fname,fname_len)
 * int CopyVarFromlistentry(int lw, int *header, int i)
 * int var2sci(void *x,int n,int m,int typ_var)
 * int createblklist(scicos_block *Blocks, int *ierr)
 * int intgetscicosvarsc(fname,fname_len)
 * int intcurblkc(fname,fname_len)
 * int intbuildouttb(fname,fname_len)
 * int intpermutobj_c(fname,fname_len)
 */

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../machine.h"
#include "scicos_block4.h"
#include "intcscicos.h"
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#include "../stack-c.h"

/* fonction pour recuperer le nombre du champs a partir de son nom */
int MlistGetFieldNumber(int *ptr, const char *string)
{
  int nf, longueur, istart, k, ilocal, retval;
  int *headerstr;
  static char str[24];
 
  headerstr = listentry(ptr,1);
  nf=headerstr[1]*headerstr[2]-1;  /* number of fields */
  retval=-1;
  for (k=0; k<nf; k++) {
    longueur=Min(headerstr[6+k]-headerstr[5+k],24);  /* size of kth fieldname */
    istart=5+nf+headerstr[5+k];    /* start of kth fieldname code */
    /*    istart=8+headerstr[4+nf+k]; */
    C2F(cvstr)(&longueur, &headerstr[istart], str, (ilocal=1, &ilocal),longueur);
    str[longueur]='\0';
    if (strcmp(string, str) == 0) {
      retval=k+2;
      break;}
  }
  return retval;
}

int inttimescicos(fname,fname_len)
     /* renvoi le temps de simulation t=get_scicos_time() */
     char *fname;
     unsigned long fname_len;
{
  int un,l1;
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVar(1,"d",(un=1,&un),(un=1,&un),&l1);
  *stk(l1)=get_scicos_time();
  LhsVar(1)=1;
  return 0;
}

int intduplicate(fname,fname_len)

     /* v=duplicate(u,count) 
      * returns v=[u(1)*ones(count(1),1);
      *            u(2)*ones(count(2),1);
      *            ...
      */

     char *fname;
     unsigned long fname_len;
{
  int m1,m2,m3,n1,n2,n3,l1,l2,l3,n;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  n=m1*n1;
  if (n==0) {
    m3=0;
    CreateVar(3, "d", &m3, &m3, &l3);
    LhsVar(1) = 3;
    return 0;
  }
  if (n!=m2*n2) 
    {
      Scierror(999,"%s: 1st and 2nd argument must have equal size \r\n",fname);
      return 0;
    }
  comp_size(stk(l2),&n3,n);
  m3=1;
  CreateVar(3, "d", &n3, &m3, &l3);
  duplicata(&n,stk(l1),stk(l2),stk(l3),&n3);
  LhsVar(1) = 3;
  return 0;
}

int intdiffobjs(fname,fname_len)
     /*   diffobjs(A,B) returns 0 if A==B and 1 if A and B differ */
     char *fname;
     unsigned long fname_len;
{
  int un,l3,k;
  int size1;int size2;
  int *header1;int *header2;
  CheckRhs(2,2);
  CheckLhs(1,1);
  header1 = GetData(1);
  header2 = GetData(2);
  CreateVar(3,"d",(un=1,&un),(un=1,&un),&l3);
  LhsVar(1) = 3;
  size1=2*(*Lstk(Top-Rhs+2)-*Lstk(Top-Rhs+1));
  size2=2*(*Lstk(Top-Rhs+3)-*Lstk(Top-Rhs+2));

  if (size1 != size2) {
    *stk(l3)=1;
    return 0;
  }
  for (k=0; k<size1; k++) {
    if (header1[k] != header2[k]) {
      *stk(l3)=1;
      return 0;
    }
    *stk(l3)=0;

  }
  return 0;
}

int inttree2(fname,fname_len)
     /* [ord,ok]=ctree2(vec,outoin,outoinptr,dep_u,dep_uptr) */
     char *fname;
     unsigned long fname_len;
{
  int un=1,ipvec,nvec,mvec,noin,moin,ipoin,noinr,moinr,ipoinr;
  int ndep,mdep,ipdep,ndepuptr,mdepuptr,ipdepuptr,ipord,ipok,n,nord;

  CheckRhs(5,5);
  CheckLhs(2,2);

  GetRhsVar(1,"i",&nvec,&mvec,&ipvec);
  GetRhsVar(2,"i",&noin,&moin,&ipoin);
  GetRhsVar(3,"i",&noinr,&moinr,&ipoinr);
  GetRhsVar(4,"i",&ndep,&mdep,&ipdep);
  GetRhsVar(5,"i",&ndepuptr,&mdepuptr,&ipdepuptr);
  n=nvec*mvec;
  CreateVar(6,"i",&n,&un,&ipord);
  CreateVar(7,"i",&un,&un,&ipok);

  ctree2(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(ipoin),istk(ipoinr),
	 istk(ipord),&nord,istk(ipok));
  *istk(iadr(C2F(intersci).iwhere[5])+1)=nord;

  LhsVar(1)=6;
  LhsVar(2)=7;

  return 0;
}

int inttree3(fname,fname_len)
     /* [r2,ok2]=ctree3(vec,dd,dep_uptr,typ_l,bexe,boptr,blnk,blptr)*/
     char *fname;
     unsigned long fname_len;
{
  int un=1,ipvec,nvec,mvec,ntyp,mtyp,iptyp,nbex,mbex,ipbex;
  int ndep,mdep,ipdep,ndepuptr,mdepuptr,ipdepuptr,ipord,ipok,n,nord;
  int nbop,mbop,ipbop,nbln,mbln,ipbln,nblr,mblr,ipblr;
  
  CheckRhs(8,8);
  CheckLhs(2,2);

  GetRhsVar(1,"i",&nvec,&mvec,&ipvec);
  GetRhsVar(2,"i",&ndep,&mdep,&ipdep);
  GetRhsVar(3,"i",&ndepuptr,&mdepuptr,&ipdepuptr);
  GetRhsVar(4,"i",&ntyp,&mtyp,&iptyp);
  GetRhsVar(5,"i",&nbex,&mbex,&ipbex);
  GetRhsVar(6,"i",&nbop,&mbop,&ipbop);
  GetRhsVar(7,"i",&nbln,&mbln,&ipbln);
  GetRhsVar(8,"i",&nblr,&mblr,&ipblr);

  n=nvec*mvec;
  CreateVar(9,"i",&n,&un,&ipord);
  CreateVar(10,"i",&un,&un,&ipok);

  ctree3(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(iptyp),istk(ipbex),
	 istk(ipbop),istk(ipbln),istk(ipblr),istk(ipord),&nord,istk(ipok));
  *istk(iadr(C2F(intersci).iwhere[8])+1)=nord;

  LhsVar(1)=9;
  LhsVar(2)=10;

  return 0;
}

int inttree4 _PARAMS((char *fname,unsigned long fname_len))
     /* [r1,r2]=ctree4(vec,outoin,outoinptr,nd,ddd) */
{
  int un=1,ipvec,nvec,mvec,noin,moin,ipoin,noinr,moinr,ipoinr;
  int nnd,mnd,ipnd,ntyp,mtyp,iptyp,ipr1,ipr2,n,nr,nn;

  CheckRhs(5,5);
  CheckLhs(2,2);

  GetRhsVar(1,"i",&nvec,&mvec,&ipvec);
  GetRhsVar(2,"i",&noin,&moin,&ipoin);
  GetRhsVar(3,"i",&noinr,&moinr,&ipoinr);
  GetRhsVar(4,"i",&nnd,&mnd,&ipnd);
  GetRhsVar(5,"i",&ntyp,&mtyp,&iptyp);
  n=nvec*mvec;
  nn=nnd*mnd;
  CreateVar(6,"i",&un,&nn,&ipr1);
  CreateVar(7,"i",&un,&nn,&ipr2);

  ctree4(istk(ipvec),n,istk(ipnd),mnd,istk(iptyp),istk(ipoin),
	 istk(ipoinr),istk(ipr1),istk(ipr2),&nr);

  LhsVar(1)=6;
  LhsVar(2)=7;
  /*      nbcols(6)=nr */
  *istk(iadr(C2F(intersci).iwhere[5])+2)=nr;
  /*      nbcols(7)=nr */
  *istk(iadr(C2F(intersci).iwhere[6])+2)=nr;
  return 0;
}


int intxproperty(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un;
  extern int* pointer_xproperty;
  extern int n_pointer_xproperty;
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVarFromPtr(1,"i",&n_pointer_xproperty,(un=1,&un),&pointer_xproperty);
  LhsVar(1)=1;
  return 0;
}
 
int intphasesim(fname,fname_len)
     /* renvoi la phase de simulation phase=get_phase_simulation() */
     char *fname;
     unsigned long fname_len;
{ 
  int un,l1;
  CheckRhs(-1,0);
  CheckLhs(1,1);
  CreateVar(1,"i",(un=1,&un),(un=1,&un),&l1);
  *istk(l1)=get_phase_simulation();
  LhsVar(1)=1;
  return 0;
}

int intsetxproperty(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un,l1,m1;
  CheckRhs(1,1);
  GetRhsVar(1,"i",&m1,(un=1,&un),&l1);
  set_pointer_xproperty(istk(l1));
  LhsVar(1)=0; 
  return 0;
}

int intsetblockerror(fname,fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
     char *fname;
     unsigned long fname_len;
{
  int un,l1;
  CheckRhs(1,1);
  GetRhsVar(1,"i",(un=1,&un),(un=1,&un),&l1);
  set_block_error(*istk(l1));
  LhsVar(1)=0; 
  return 0;
}


void  duplicata(n,v,w,ww,nw)
     double *v,*w,*ww;
     int *n,*nw;
{
  int i,j,k;
  k=0;
  for (i=0;i<*n;i++) {
    for (j=0;j<(int) w[i];j++) {
      ww[k]=v[i];
      k=k+1;
    }
  }
  *nw=k;
}

void  comp_size(v,nw,n)
     double *v;
     int *nw,n;
{
  int i;
  *nw=0;
  for (i=0;i<n;i++) {
    if (v[i]>0) *nw=*nw+(int) v[i];
  }
}

/* intsicosimc scicosim interface routine.
 *
 * [state,t] = scicosim(state,tcur,tf,sim,str,tol)
 *
 * rhs 1 state : Tlist
 *        - 1  : state(1)     : !xcs  x  z  iz  tevts  evtspt  pointi  outtb  !
 *        - 2  : state.x      : column vector of real
 *        - 3  : state.z      : column vector of real
 *        - 4  : state.iz     : column vector of real (empty object with flag "finish")
 *        - 5  : state.tevts  : column vector of real
 *        - 6  : state.evtspt : column vector of real
 *        - 7  : state.pointi : real scalar
 *        - 8  : state.outtb  : list of scilab object
 * rhs 2 tcur  : real scalar
 * rhs 3 tf    : real scalar
 * rhs 4 sim   : Tlist
 *        - 1  : sim(1) : !scs     funs    xptr    zptr    zcptr   inpptr
 *                         outptr  inplnk  outlnk  rpar    rpptr   ipar
 *                         ipptr   clkptr  ordptr  execlk  ordclk  cord
 *                         oord    zord    critev  nb      ztyp    nblk
 *                         ndcblk  subscr  funtyp  iord    labels  modptr  !
 *        - 2  : sim.funs   : list of strings and/or scilab function
 *        - 3  : sim.xptr   : column vector of real
 *        - 4  : sim.zptr   : column vector of real
 *        - 5  : sim.zcptr  : column vector of real
 *        - 6  : sim.inpptr : column vector of real
 *        - 7  : sim.outptr : column vector of real
 *        - 8  : sim.inplnk : column vector of real
 *        - 9  : sim.outlnk : column vector of real
 *        - 10 : sim.rpar   : column vector of real
 *        - 11 : sim.rpptr  : column vector of real
 *        - 12 : sim.ipar   : column vector of real
 *        - 13 : sim.ipptr  : column vector of real
 *        - 14 : sim.clkptr : column vector of real
 *        - 15 : sim.ordptr : column vector of real
 *        - 16 : sim.execlk : matrix of real
 *        - 17 : sim.ordclk : matrix of real
 *        - 18 : sim.cord   : matrix of real
 *        - 19 : sim.oord   : matrix of real
 *        - 20 : sim.zord   : column vector ? of real
 *        - 21 : sim.critev : column vector of real
 *        - 22 : sim.nb     : real scalar
 *        - 23 : sim.ztyp   : column vector of real
 *        - 24 : sim.nblk   : real scalar
 *        - 25 : sim.ndcblk : real scalar
 *        - 26 : sim.subscr : column vector of real
 *        - 27 : sim.funtyp : column vector of real
 *        - 28 : sim.iord   : column vector of real
 *        - 29 : sim.labels : column vector of strings
 *        - 30 : sim.modptr : column vector of real
 *
 * rhs 5 str   : string flag : 'start','run','finish','linear'
 * rhs 6 tol   : real vector of size (7,1) minimum (4,1)
 *               [atol rtol ttol [deltat realtimescale solver hmax]]'
 *
 * 16/03/06, A.Layec : Rewritten from original fortran
 * source code intsscicos in intcos.f.
 *
 * 29/03/06, Alan    : Improvement in accordance to c_pass2
 * (int32 parameters)
 *
 * 31/05/06, Alan    : Add global variable int *il_state_save
 * and int *il_sim_save in intcscicos.h to store stack address
 * of list %cpr.state and %cpr.sim (to use with
 * get/setscicosvars)
 *
 * 14/06/06, Alan    : Save common intersci before calling scicos
 * (to disable scilab crash with scifunc.f)
 *
 * 13/11/06, Alan    : Get back to double parameters for sim and state
 *                     (for better compatibility with scilab-4.x families-)
 *                     Remove il_sim_save global variable
 *
 * 15/12/06, Alan    : Warnings compilation removed.
 *                     This can crash scilab/scicos.
 *                     Please report.
 *
 */

#define freeintparam \
     FREE(l_sim_iord);\
     FREE(l_sim_funtyp);\
     FREE(l_sim_subscr);\
     FREE(l_sim_ndcblk);\
     FREE(l_sim_nblk);\
     FREE(l_sim_ztyp);\
     FREE(l_sim_nb);\
     FREE(l_sim_critev);\
     FREE(l_sim_zord);\
     FREE(l_sim_oord);\
     FREE(l_sim_cord);\
     FREE(l_sim_ordclk);\
     FREE(l_sim_execlk);\
     FREE(l_sim_ordptr);\
     FREE(l_sim_clkptr);\
     FREE(l_sim_ipar);\
     FREE(l_sim_rpptr);\
     FREE(l_sim_outlnk);\
     FREE(l_sim_inplnk);\
     FREE(l_sim_outptr);\
     FREE(l_sim_inpptr);\
     FREE(l_sim_zcptr);\
     FREE(l_sim_zptr);\
     FREE(l_sim_xptr);\
     FREE(l_state_evtspt);\
     FREE(l_pointi)

/* prototype */
int intscicosimc(fname,fname_len)
                 char *fname;
                 unsigned long fname_len;
{
 /*********************************************
  * external structure and function declaration
  *********************************************/
 /*declaration of funnum (in scicos.c)*/
 extern int C2F(funnum) __PARAMS((char *fname));

 /************************************
  * variables and constants d�inition
  ************************************/
 static int id[nsiz];

 /*declaration of static structure*/
 static struct {integer idb;} C2F(dbcos);   /*declaration of dbcos*/

 typedef struct inter_s_
 {
   int iwhere,nbrows,nbcols,itflag,ntypes,lad,ladc,lhsvar;
 } intersci_state ;

 typedef struct inter_l
 {
  intersci_state *state ;
  int nbvars;
 } intersci_list ;
 intersci_list *loc;
 intersci_state *new ;

 /* declaration of outtb_elem */
 outtb_el *outtb_elem=NULL;
 static int nelem;

 /*auxilary variables for dimension and address*/
 static int m1;                  /*state*/
 static int *il_state;
 static int m1e2,n1e2;           /*state.x*/
 static int *il_state_x;
 static double *l_state_x;
 static int m1e3,n1e3;           /*state.z*/
 static int *il_state_z;
 static double *l_state_z;
 static int m1e4,n1e4;           /*state.iz*/
 static int *il_state_iz;
 static void **l_state_iz;
 static int m1e5,n1e5;           /*state.tevts*/
 static int *il_state_tevts;
 static double *l_state_tevts;
 static int m1e6,n1e6;           /*state.evtspt*/
 static int *il_state_evtspt;
 static int *l_state_evtspt;
 static int m1e7,n1e7;           /*state.pointi*/
 static int *il_pointi;
 static int *l_pointi;
 static int *il_state_outtb;     /*state.outtb*/
 static int nlnk;
 static void **outtbptr;
 static int *outtbsz;
 static int *outtbtyp;

 static int m2,n2;               /*tcur*/
 static int *il_tcur;
 static double *l_tcur;

 static int m3,n3;               /*tf*/
 static int *il_tf;
 static double *l_tf;

 static int m4,n4,l4,il4;        /*sim*/
 static int *il_sim;
 static int l4e2,il4e2;          /*sim.funs*/
 static int *il_sim_fun;
 static int nblk;
 static int m4e3,n4e3;           /*sim.xptr*/
 static int *il_sim_xptr;
 static int *l_sim_xptr;
 static int m4e4,n4e4;           /*sim.zptr*/
 static int *il_sim_zptr;
 static int *l_sim_zptr;
 static int m4e5,n4e5;           /*sim.zcptr*/
 static int *il_sim_zcptr;
 static int *l_sim_zcptr;
 static int m4e6,n4e6;           /*sim.inpptr*/
 static int *il_sim_inpptr;
 static int *l_sim_inpptr;
 static int m4e7,n4e7;           /*sim.outptr*/
 static int *il_sim_outptr;
 static int *l_sim_outptr;
 static int m4e8,n4e8;           /*sim.inplnk*/
 static int *il_sim_inplnk;
 static int *l_sim_inplnk;
 static int m4e9,n4e9;           /*sim.outlnk*/
 static int *il_sim_outlnk;
 static int *l_sim_outlnk;
 static int m4e10,n4e10;         /*sim.rpar*/
 static int *il_sim_rpar;
 static double *l_sim_rpar;
 static int m4e11,n4e11;         /*sim.rpptr*/
 static int *il_sim_rpptr;
 static int *l_sim_rpptr;
 static int m4e12,n4e12;         /*sim.ipar*/
 static int *il_sim_ipar;
 static int *l_sim_ipar;
 static int m4e13,n4e13;         /*sim.ipptr*/
 static int *il_sim_ipptr;
 static int *l_sim_ipptr;
 static int m4e14,n4e14;         /*sim.clkptr*/
 static int *il_sim_clkptr;
 static int *l_sim_clkptr;
 static int m4e15,n4e15;         /*sim.ordptr*/
 static int *il_sim_ordptr;
 static int *l_sim_ordptr;
 static int m4e16,n4e16;         /*sim.execlk*/
 static int *il_sim_execlk;
 static int *l_sim_execlk;
 static int m4e17,n4e17;         /*sim.ordclk*/
 static int *il_sim_ordclk;
 static int *l_sim_ordclk;
 static int m4e18,n4e18;         /*sim.cord*/
 static int *il_sim_cord;
 static int *l_sim_cord;
 static int m4e19,n4e19;         /*sim.oord*/
 static int *il_sim_oord;
 static int *l_sim_oord;
 static int m4e20,n4e20;         /*sim.zord*/
 static int *il_sim_zord;
 static int *l_sim_zord;
 static int m4e21,n4e21;         /*sim.critev*/
 static int *il_sim_critev;
 static int *l_sim_critev;
 static int m4e22,n4e22;         /*sim.nb*/
 static int *il_sim_nb;
 static int *l_sim_nb;
 static int m4e23,n4e23;         /*sim.ztyp*/
 static int *il_sim_ztyp;
 static int *l_sim_ztyp;
 static int m4e24,n4e24;         /*sim.nblk*/
 static int *il_sim_nblk;
 static int *l_sim_nblk;
 static int m4e25,n4e25;         /*sim.ndcblk*/
 static int *il_sim_ndcblk;
 static int *l_sim_ndcblk;
 static int m4e26,n4e26;         /*sim.subscr*/
 static int *il_sim_subscr;
 static int *l_sim_subscr;
 static int m4e27,n4e27;         /*sim.funtyp*/
 static int *il_sim_funtyp;
 static int *l_sim_funtyp;
 static int m4e28,n4e28;         /*sim.iord*/
 static int *il_sim_iord;
 static int *l_sim_iord;
 static int m4e29,n4e29;         /*sim.labels*/
 static int *il_sim_lab;
 static int *il_sim_labptr;
 static int *l_sim_lab;
 static int m4e30,n4e30;         /*sim.modptr*/
 static int *il_sim_modptr;
 static int *l_sim_modptr;

 static int m5,n5;               /*str*/
 static int *il_str;
 static int *l_str;
 static int flag;

 static int m6,n6;               /*tol*/
 static int *il_tol;
 static double *l_tol;

 /*auxilary variables*/
 static double simpar[7];
 static int solver;
 static int *lfunpt;     /*for function table*/
 static int lf,ilf,ifun; /*for function table*/
 static int ierr,istate; /*error variable of scicos.c*/

 /*local variable*/
 int *subheader; /*pointer to get address of a subvariable in a list*/
 int i,j,k;        /*local counter variable*/
 int sz_str;     /*local variable to store size of string*/
 int err_check;  /*local variable for cross var. checking dimension*/

 /*definition of min/max input/output argument*/
 static int minlhs=1, maxlhs=2, minrhs=6, maxrhs=6;

 /*************************************
  * Check number of inputs and outputs
  *************************************/
 CheckRhs(minrhs,maxrhs);
 CheckLhs(minlhs,maxlhs);

   /****************
    * state (rhs 1)
    ****************/
    il_state = (int *) GetData(1);

    il_state_save = il_state; /* make a copy of il_state in a global variabe */

    if(il_state[0]!=16) /*Check if state is a tlist*/
    {
     Scierror(56,"%s : First argument must be a Tlist.\n",fname);
     C2F(iop).err=1;
     return 0;
    }
    m1 = il_state[1];

    /*2 : state.x      */
    il_state_x = (int *) (listentry(il_state,2));
    l_state_x  = (double *) (il_state_x+4);
    m1e2 = il_state_x[1];
    n1e2 = il_state_x[2];

    /*3 : state.z      */
    il_state_z = (int *) (listentry(il_state,3));
    l_state_z  = (double *) (il_state_z+4);
    m1e3 = il_state_z[1];
    n1e3 = il_state_z[2];

    /*4 : state.iz     */
    il_state_iz = (int *) (listentry(il_state,4));
    l_state_iz  = (void **) (il_state_iz+4);
    m1e4 = il_state_iz[1];
    n1e4 = il_state_iz[2];

    /*5 : state.tevts  */
    il_state_tevts = (int *) (listentry(il_state,5));
    l_state_tevts  = (double *) (il_state_tevts+4);
    m1e5 = il_state_tevts[1];
    n1e5 = il_state_tevts[2];

    /*6 : state.evtspt */
    il_state_evtspt = (int *) (listentry(il_state,6));
    m1e6 = il_state_evtspt[1];
    n1e6 = il_state_evtspt[2];
    if ((m1e6*n1e6)==0) l_state_evtspt=NULL;
    else
    {
     if ((l_state_evtspt=(int *) MALLOC((m1e6*n1e6)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      return 0;
     }
     else
     {
      for(i=0;i<(m1e6*n1e6);i++) l_state_evtspt[i]= (int) ((double *)(il_state_evtspt+4))[i];
     }
    }

    /*7 : state.pointi */
    il_pointi = (int *) (listentry(il_state,7));
    m1e7 = il_pointi[1];
    n1e7 = il_pointi[2];
    if ((l_pointi=(int *) MALLOC((m1e7*n1e7)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_state_evtspt);
     return 0;
    }
    else
    {
     for(i=0;i<(m1e7*n1e7);i++) l_pointi[i]= (int) ((double *)(il_pointi+4))[i];
    }

    /*8 : state.outtb  */
    il_state_outtb = (int *) (listentry(il_state,8));
    if(il_state_outtb[0]!=15) /*check if il_state_outtb is a list*/
    {
     Scierror(56,"%s : outtb element of state must be a list.\n",fname);
     C2F(iop).err=4;
     return 0;
    }
    nlnk = il_state_outtb[1]; /*nlnk is the dimension of the list state_louttb*/

 /***************
  * tcur (rhs 2)
  ***************/
 il_tcur = (int *) GetData(2);
 if(il_tcur[0]!=1) /*Check if tcur is a real or complex matrix*/
 {
  Scierror(53,"%s : Second argument must be a scalar.\n",fname);
  C2F(iop).err=2;
  return 0;
 }
 l_tcur = (double *) (il_tcur+4);
 m2 = il_tcur[1];
 n2 = il_tcur[2];
 CheckScalar(2,m2,n2);
 CheckDims(2,m2,n2,1,1);

 /*************
  * tf (rhs 3)
  *************/
 il_tf = (int *) GetData(3);
 if(il_tf[0]!=1) /*Check if tf is a real or complex matrix*/
 {
  Scierror(53,"%s : Third argument must be a scalar.\n",fname);
  C2F(iop).err=3;
  return 0;
 }
 l_tf = (double *) (il_tf+4);
 m3 = il_tf[1];
 n3 = il_tf[2];
 CheckScalar(3,m3,n3);
 CheckDims(3,m3,n3,1,1);

   /*************
    * sim (rhs 4)
    *************/
    il_sim = (int *) GetData(4);

    if(il_sim[0]!=16)  /*Check if sim is a tlist*/
    {
     Scierror(56,"%s : Fourth argument must be a Tlist.\n",fname);
     C2F(iop).err=4;
     return 0;
    }
    m4 = il_sim[1];
    n4 = il_sim[2];

    /*2  : sim.funs*/
    il_sim_fun = (int *) (listentry(il_sim,2));
    if(il_sim_fun[0]!=15) /*check if sim.funs is a list*/
    {
     Scierror(56,"%s : Second element of sim must be a list.\n",fname);
     C2F(iop).err=4;
     return 0;
    }
    nblk = il_sim_fun[1]; /*nblk is the dimension of the list sim.funs*/

    /*3  : sim.xptr   */
    il_sim_xptr = (int *) (listentry(il_sim,3));
    m4e3 = il_sim_xptr[1];
    n4e3 = il_sim_xptr[2];
    if ((l_sim_xptr=(int *) MALLOC((m4e3*n4e3)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e3*n4e3);i++) l_sim_xptr[i]= (int) ((double *)(il_sim_xptr+4))[i];
    }

    /*4  : sim.zptr   */
    il_sim_zptr = (int *) (listentry(il_sim,4));
    m4e4 = il_sim_zptr[1];
    n4e4 = il_sim_zptr[2];
    if ((l_sim_zptr=(int *) MALLOC((m4e4*n4e4)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e4*n4e4);i++) l_sim_zptr[i]= (int) ((double *)(il_sim_zptr+4))[i];
    }

    /*5  : sim.zcptr  */
    il_sim_zcptr = (int *) (listentry(il_sim,5));
    m4e5 = il_sim_zcptr[1];
    n4e5 = il_sim_zcptr[2];
    if ((l_sim_zcptr=(int *) MALLOC((m4e5*n4e5)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e5*n4e5);i++) l_sim_zcptr[i]= (int) ((double *)(il_sim_zcptr+4))[i];
    }

    /*6  : sim.inpptr */
    il_sim_inpptr = (int *) (listentry(il_sim,6));
    m4e6 = il_sim_inpptr[1];
    n4e6 = il_sim_inpptr[2];
    if ((l_sim_inpptr=(int *) MALLOC((m4e6*n4e6)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e6*n4e6);i++) l_sim_inpptr[i]= (int) ((double *)(il_sim_inpptr+4))[i];
    }

    /*7  : sim.outptr */
    il_sim_outptr = (int *) (listentry(il_sim,7));
    m4e7 = il_sim_outptr[1];
    n4e7 = il_sim_outptr[2];
    if ((l_sim_outptr=(int *) MALLOC((m4e7*n4e7)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e7*n4e7);i++) l_sim_outptr[i]= (int) ((double *)(il_sim_outptr+4))[i];
    }

    /*8  : sim.inplnk */
    il_sim_inplnk = (int *) (listentry(il_sim,8));
    m4e8 = il_sim_inplnk[1];
    n4e8 = il_sim_inplnk[2];
    if ((l_sim_inplnk=(int *) MALLOC((m4e8*n4e8)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e8*n4e8);i++) l_sim_inplnk[i]= (int) ((double *)(il_sim_inplnk+4))[i];
    }

    /*9  : sim.outlnk */
    il_sim_outlnk = (int *) (listentry(il_sim,9));
    m4e9 = il_sim_outlnk[1];
    n4e9 = il_sim_outlnk[2];
    if ((l_sim_outlnk=(int *) MALLOC((m4e9*n4e9)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e9*n4e9);i++) l_sim_outlnk[i]= (int) ((double *)(il_sim_outlnk+4))[i];
    }

    /*10 : sim.rpar   */
    il_sim_rpar = (int *) (listentry(il_sim,10));
    m4e10 = il_sim_rpar[1];
    n4e10 = il_sim_rpar[2];
    l_sim_rpar = (double *) (il_sim_rpar+4);

    /*11 : sim.rpptr  */
    il_sim_rpptr = (int *) (listentry(il_sim,11));
    m4e11 = il_sim_rpptr[1];
    n4e11 = il_sim_rpptr[2];
    if ((l_sim_rpptr=(int *) MALLOC((m4e11*n4e11)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e11*n4e11);i++) l_sim_rpptr[i]= (int) ((double *)(il_sim_rpptr+4))[i];
    }

    /*12 : sim.ipar   */
    il_sim_ipar = (int *) (listentry(il_sim,12));
    m4e12 = il_sim_ipar[1];
    n4e12 = il_sim_ipar[2];
    if (m4e12*n4e12==0) l_sim_ipar=NULL;
    else
    {
     if ((l_sim_ipar=(int *) MALLOC((m4e12*n4e12)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e12*n4e12);i++) l_sim_ipar[i]= (int) ((double *)(il_sim_ipar+4))[i];
     }
    }

    /*13 : sim.ipptr  */
    il_sim_ipptr = (int *) (listentry(il_sim,13));
    m4e13 = il_sim_ipptr[1];
    n4e13 = il_sim_ipptr[2];
    if ((l_sim_ipptr=(int *) MALLOC((m4e13*n4e13)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e13*n4e13);i++) l_sim_ipptr[i]= (int) ((double *)(il_sim_ipptr+4))[i];
    }

    /*14 : sim.clkptr */
    il_sim_clkptr = (int *) (listentry(il_sim,14));
    m4e14 = il_sim_clkptr[1];
    n4e14 = il_sim_clkptr[2];
    if ((l_sim_clkptr=(int *) MALLOC((m4e14*n4e14)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e14*n4e14);i++) l_sim_clkptr[i]= (int) ((double *)(il_sim_clkptr+4))[i];
    }

    /*15 : sim.ordptr */
    il_sim_ordptr = (int *) (listentry(il_sim,15));
    m4e15 = il_sim_ordptr[1];
    n4e15 = il_sim_ordptr[2];
    if ((l_sim_ordptr=(int *) MALLOC((m4e15*n4e15)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e15*n4e15);i++) l_sim_ordptr[i]= (int) ((double *)(il_sim_ordptr+4))[i];
    }

    /*16 : sim.execlk */
    il_sim_execlk = (int *) (listentry(il_sim,16));
    m4e16 = il_sim_execlk[1];
    n4e16 = il_sim_execlk[2];
    if (m4e16*n4e16==0) l_sim_execlk=NULL;
    else
    {
     if ((l_sim_execlk=(int *) MALLOC((m4e16*n4e16)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e16*n4e16);i++) l_sim_execlk[i]= (int) ((double *)(il_sim_execlk+4))[i];
     }
    }

    /*17 : sim.ordclk */
    il_sim_ordclk = (int *) (listentry(il_sim,17));
    m4e17 = il_sim_ordclk[1];
    n4e17 = il_sim_ordclk[2];    
    if (m4e17*n4e17==0) l_sim_ordclk=NULL;
    else
    {
     if ((l_sim_ordclk=(int *) MALLOC((m4e17*n4e17)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e17*n4e17);i++) l_sim_ordclk[i]= (int) ((double *)(il_sim_ordclk+4))[i];
     }
    }

    /*18 : sim.cord   */
    il_sim_cord = (int *) (listentry(il_sim,18));
    m4e18 = il_sim_cord[1];
    n4e18 = il_sim_cord[2];
    if (m4e18*n4e18==0) l_sim_cord=NULL;
    else
    {
     if ((l_sim_cord=(int *) MALLOC((m4e18*n4e18)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e18*n4e18);i++) l_sim_cord[i]= (int) ((double *)(il_sim_cord+4))[i];
     }
    }

    /*19 : sim.oord   */
    il_sim_oord = (int *) (listentry(il_sim,19));
    m4e19 = il_sim_oord[1];
    n4e19 = il_sim_oord[2];
    if (m4e19*n4e19==0) l_sim_oord=NULL;
    else
    {
     if ((l_sim_oord=(int *) MALLOC((m4e19*n4e19)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e19*n4e19);i++) l_sim_oord[i]= (int) ((double *)(il_sim_oord+4))[i];
     }
    }

    /*20 : sim.zord   */
    il_sim_zord = (int *) (listentry(il_sim,20));
    m4e20 = il_sim_zord[1];
    n4e20 = il_sim_zord[2];
    if (m4e20*n4e20==0) l_sim_zord=NULL;
    else
    {
     if ((l_sim_zord=(int *) MALLOC((m4e20*n4e20)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e20*n4e20);i++) l_sim_zord[i]= (int) ((double *)(il_sim_zord+4))[i];
     }
    }

    /*21 : sim.critev */
    il_sim_critev = (int *) (listentry(il_sim,21));
    m4e21 = il_sim_critev[1];
    n4e21 = il_sim_critev[2];
    if (m4e21*n4e21==0) l_sim_critev=NULL;
    else
    {
     if ((l_sim_critev=(int *) MALLOC((m4e21*n4e21)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e21*n4e21);i++) l_sim_critev[i]= (int) ((double *)(il_sim_critev+4))[i];
     }
    }

    /*22 : sim.nb     */
    il_sim_nb = (int *) (listentry(il_sim,22));
    m4e22 = il_sim_nb[1];
    n4e22 = il_sim_nb[2];
    if ((l_sim_nb=(int *) MALLOC((m4e22*n4e22)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
     FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e22*n4e22);i++) l_sim_nb[i]= (int) ((double *)(il_sim_nb+4))[i];
    }
    if (l_sim_nb[0]!=nblk) /*value of nb must be equal to nblk*/
    {
     Scierror(42,"%s : Incompatible sim.nb RHS parameter.\n",fname);
     return 0;
    }

    /*23 : sim.ztyp   */
    il_sim_ztyp = (int *) (listentry(il_sim,23));
    m4e23 = il_sim_ztyp[1];
    n4e23 = il_sim_ztyp[2];
    if ((l_sim_ztyp=(int *) MALLOC((m4e23*n4e23)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
     FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e23*n4e23);i++) l_sim_ztyp[i]= (int) ((double *)(il_sim_ztyp+4))[i];
    }

    /*24 : sim.nblk   */
    il_sim_nblk = (int *) (listentry(il_sim,24));
    m4e24 = il_sim_nblk[1];
    n4e24 = il_sim_nblk[2];
    if ((l_sim_nblk=(int *) MALLOC((m4e24*n4e24)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
     FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e24*n4e24);i++) l_sim_nblk[i]= (int) ((double *)(il_sim_ztyp+4))[i];
    }

    /*25 : sim.ndcblk */
    il_sim_ndcblk = (int *) (listentry(il_sim,25));
    m4e25 = il_sim_ndcblk[1];
    n4e25 = il_sim_ndcblk[2];
    if ((l_sim_ndcblk=(int *) MALLOC((m4e25*n4e25)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
     FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e25*n4e25);i++) l_sim_ndcblk[i]= (int) ((double *)(il_sim_ndcblk+4))[i];
    }

    /*26 : sim.subscr */
    il_sim_subscr = (int *) (listentry(il_sim,26));
    m4e26 = il_sim_subscr[1];
    n4e26 = il_sim_subscr[2];
    if (m4e26*n4e26==0) l_sim_subscr=NULL;
    else
    {
     if ((l_sim_subscr=(int *) MALLOC((m4e26*n4e26)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_ndcblk);
      FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
      FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e26*n4e26);i++) l_sim_subscr[i]= (int) ((double *)(il_sim_subscr+4))[i];
     }
    }

    /*27 : sim.funtyp */
    il_sim_funtyp = (int *) (listentry(il_sim,27));
    m4e27 = il_sim_funtyp[1];
    n4e27 = il_sim_funtyp[2];
    if ((l_sim_funtyp=(int *) MALLOC((m4e27*n4e27)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     FREE(l_sim_subscr); FREE(l_sim_ndcblk);
     FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
     FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
     FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
     FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
     FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
     FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
     FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
     return 0;
    }
    else
    {
     for(i=0;i<(m4e27*n4e27);i++) l_sim_funtyp[i]= (int) ((double *)(il_sim_funtyp+4))[i];
    }

    /*28 : sim.iord   */
    il_sim_iord = (int *) (listentry(il_sim,28));
    m4e28 = il_sim_iord[1];
    n4e28 = il_sim_iord[2];
    if (m4e28*n4e28==0) l_sim_iord=NULL;
    else
    {
     if ((l_sim_iord=(int *) MALLOC((m4e28*n4e28)*sizeof(int))) ==NULL )
     {
      Scierror(999,"%s : Memory allocation error.\n",fname);
      FREE(l_sim_funtyp); FREE(l_sim_subscr); FREE(l_sim_ndcblk);
      FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
      FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
      FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr);
      FREE(l_sim_clkptr); FREE(l_sim_clkptr); FREE(l_sim_ipar);
      FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
      FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
      FREE(l_sim_zptr); FREE(l_sim_xptr); FREE(l_state_evtspt); FREE(l_pointi);
      return 0;
     }
     else
     {
      for(i=0;i<(m4e28*n4e28);i++) l_sim_iord[i]= (int) ((double *)(il_sim_iord+4))[i];
     }
    }

    /*29 : sim.labels */
    il_sim_lab = (int *) (listentry(il_sim,29));
    m4e29 = il_sim_lab[1];
    n4e29 = il_sim_lab[2];
    il_sim_labptr = &il_sim_lab[4];  /*get address-1 of first pointer in labels*/
    l_sim_lab = (int *) (il_sim_lab+m4e29+5); /*get address of first string in labels*/

    /*30 : sim.modptr */
    il_sim_modptr = (int *) (listentry(il_sim,30));
    m4e30 = il_sim_modptr[1];
    n4e30 = il_sim_modptr[2];
    if ((l_sim_modptr=(int *) MALLOC((m4e30*n4e30)*sizeof(int))) ==NULL )
    {
     Scierror(999,"%s : Memory allocation error.\n",fname);
     freeintparam;
     return 0;
    }
    else
    {
     for(i=0;i<(m4e30*n4e30);i++) l_sim_modptr[i]= (int) ((double *)(il_sim_modptr+4))[i];
    }

 /*************
  * str (rhs 5)
  *************/
 il_str = (int *) GetData(5);
 m5 = il_str[1];
 n5 = il_str[2];
 l_str = (int *) (il_str+6);
 CheckDims(5,m5,n5,m5,1);

 sz_str=il_str[5]-1; /*store the length of str*/
 C2F(cha1).buf[0]=' ';
 C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(j=1,&j),sz_str); /*codetoascii*/
 C2F(cha1).buf[sz_str]='\0';
 if (strcmp(C2F(cha1).buf,"start") == 0) flag=1;
 else if (strcmp(C2F(cha1).buf,"run") == 0) flag=2;
 else if (strcmp(C2F(cha1).buf,"finish") == 0) flag=3;
 else if (strcmp(C2F(cha1).buf,"linear") == 0) flag=4;
 else
 {
  Scierror(44,"%s : Fifth argument is incorrect.\n",fname);
  freeintparam;
  C2F(iop).err=5;
  return 0;
 }

 /*************
  * tol (rhs 6)
  *************/
 il_tol = (int *) GetData(6);
 l_tol = (double *) (il_tol+4);
 m6 = il_tol[1];
 n6 = il_tol[2];
 m6 = m6*n6;
 if (m6<4) /*Check if tol has a minimun of four elements*/
 {
  Scierror(89,"%s : Sixth argument must have at least four elements.\n",fname);
  freeintparam;
  C2F(iop).err=6;
  return 0;
 }
 else if(m6>7) /*Check if tol has a maximum of seven elements*/
 {
  Scierror(89,"%s : Sixth argument must have a maximum of seven elements.\n",fname);
  freeintparam;
  C2F(iop).err=6;
  return 0;
 }

 /******************
  * set simpar array
  ******************/
 if (m6==4) {for(i=0;i<4;i++) simpar[i]=l_tol[i];simpar[4]=0;simpar[5]=0;simpar[6]=0;}
 else if (m6==5) {for(i=0;i<5;i++) simpar[i]=l_tol[i];simpar[5]=0;simpar[6]=0;}
 else if (m6==6) {for(i=0;i<6;i++) simpar[i]=l_tol[i];simpar[6]=0;}
 else for(i=0;i<7;i++) simpar[i]=l_tol[i];
 solver=(int)simpar[5]; /*set solver variable*/

 /******************************
  * cross variable size checking
  ******************************/
 err_check = 0;
 if (m1e5!=m1e6)       err_check=1;        /*tevts vs evtspt*/
 else if (m4e3!=m4e4)  err_check=2;        /*xptr vs zptr*/
 else if (m4e3!=m4e5)  err_check=3;        /*xptr vs zcptr*/
 else if (m4e3!=m4e6)  err_check=4;        /*xptr vs npptr*/
 else if (m4e3!=m4e7)  err_check=5;        /*xptr vs outptr*/
 else if (m4e3!=m4e11) err_check=6;        /*xptr vs rpptr*/
 else if (m4e3!=m4e13) err_check=7;        /*xptr vs ipptr*/
 else if (m4e3!=m4e14) err_check=8;        /*xptr vs clkptr*/
 else if ((n4e17!=2)&(m4e17!=0)) err_check=9;  /*sim.ordclk*/
 else if ((n4e18!=2)&(m4e18!=0)) err_check=10; /*sim.cord*/
 else if ((n4e19!=2)&(m4e19!=0)) err_check=11; /*sim.oord*/
 else if ((n4e20!=2)&(m4e20!=0)) err_check=12; /*sim.zord*/
 else if ((n4e28!=2)&(m4e28!=0)) err_check=13; /*sim.iord*/
 if (err_check!=0)
 {
  /* please write an error table here  */
  Scierror(42,"%s : error in cross variable size checking : %d\n",\
           fname,err_check);
  freeintparam;
  return 0;
 }

 /*******************************
  * set function table for blocks
  *******************************/
 il4 = iadr(*Lstk(Top-Rhs+4));  /*sim*/
 l4 = sadr(il4+m4+3);
 il4e2 = iadr(l4+*istk(il4+3)-1);
 l4e2 = sadr(il4e2+nblk+3);
 lf = l4e2;  /*first element of the list sim.funs*/
 /*define new variable lfunpt*/
 if ((lfunpt=(int *) MALLOC(nblk*sizeof(int))) ==NULL )
 {
  Scierror(999,"%s : Memory allocation error.\n",fname);
  freeintparam;
  return 0;
 }

 /*for each block*/
 for (i=0;i<nblk;i++)
 {
   ilf=iadr(lf); /*should be removed later*/
   subheader=(int *)(listentry(il_sim_fun,i+1));
   /*Block is defined by a scilab function*/
   if ((subheader[0]==11)|(subheader[0]==13)) lfunpt[i]=-lf;
   /*Block is defined by a function described by a characater strings*/
   else if(subheader[0]==10)
   {
    sz_str=subheader[5]-1; /*store the length of function name*/
    C2F(cha1).buf[0]=' ';
    C2F(cvstr)(&sz_str,&subheader[6],&C2F(cha1).buf[0],(j=1,&j),sz_str); /*codetoascii*/
    C2F(cha1).buf[sz_str]='\0';
    ifun=C2F(funnum)(C2F(cha1).buf); /*search associated function number of function name*/
    /*Block is defined by a C or Fortran function*/
    if (ifun>0) lfunpt[i]=ifun;
    /*Block is defined by a predefined scilab function*/
    else 
    {
      C2F(namstr)(id,&subheader[6],&sz_str,(j=0,&j));
      C2F(com).fin=0;
      C2F(funs)(id);
      if ((C2F(com).fun==-1)|(C2F(com).fun==-2)) lfunpt[i]=-*Lstk(C2F(com).fin);
      else
      {
       C2F(curblk).kfun=i+1;
       freeintparam;
       Scierror(888,"%s : unknown block : %s\n",fname,C2F(cha1).buf);
       FREE(lfunpt);
       return 0;
      }
    }
   }
   else
   {
    C2F(iop).err=4;
    freeintparam;
    Scierror(44,"%s : error\n",fname);
    FREE(lfunpt);
    return 0;
   }
   lf=lf+*istk(il4e2+3+i)-*istk(il4e2+i+2);
 }

 /*******************************
  * set outtbptr,outtbsz,outtbtyp
  *******************************/
 /*Allocation of outtbptr*/
 if ((outtbptr=(void **) MALLOC(nlnk*sizeof(void *)))==NULL )
 {
  Scierror(999,"%s : Memory allocation error.\n",fname);
  freeintparam;
  FREE(lfunpt);
  return 0;
 }
 /*Allocation of outtbsz*/
 if ((outtbsz=(int *) MALLOC(nlnk*2*sizeof(int)))==NULL )
 {
  Scierror(999,"%s : Memory allocation error.\n",fname);
  freeintparam;
  FREE(outtbptr);
  FREE(lfunpt);
  return 0;
 }
 /*Allocation of outtbtyp*/
 if ((outtbtyp=(int *) MALLOC(nlnk*sizeof(int)))==NULL )
 {
  Scierror(999,"%s : Memory allocation error.\n",fname);
  freeintparam;
  FREE(outtbsz);
  FREE(outtbptr);
  FREE(lfunpt);
  return 0;
 }

 /*initalize nelem*/
 nelem=0;

 /*set vectors of outtb*/
 for (j=0;j<nlnk;j++) /*for each link*/
 {
  subheader=(int *)(listentry(il_state_outtb,j+1)); /*get header of outtbl(j+1)*/
  outtbsz[j*2]=subheader[1]; /*store dimensions*/
  outtbsz[(j*2)+1]=subheader[2];

  switch (subheader[0]) /*store type and address*/
  {
   /*matrix of double*/
   case 1  : switch (subheader[3])
             {
              case 0  : outtbtyp[j]=10;  /*double real matrix*/
                        outtbptr[j]=(double *)(subheader+4);
                        break;

              case 1  : outtbtyp[j]=11;  /*double complex matrix*/
                        outtbptr[j]=(double *)(subheader+4);
                        break;

              default : Scierror(888,\
                                "%s : error. Type %d of double scalar matrix not yet supported.\n",\
                                fname,subheader[3]);
                        freeintparam;
                        FREE(outtbptr);
                        FREE(outtbtyp);
                        FREE(outtbsz);
                        FREE(lfunpt);
                        if (outtb_elem!=NULL) FREE(outtb_elem);
                        break;
             }
             break;

   /*matrix of integers*/
   case 8  : switch (subheader[3])
             {
              case 1  : outtbtyp[j]=81;  /*int8*/
                        outtbptr[j]=(char *)(subheader+4);
                        break;

              case 2  : outtbtyp[j]=82;  /*int16*/
                        outtbptr[j]=(short *)(subheader+4);
                        break;

              case 4  : outtbtyp[j]=84;  /*int32*/
                        outtbptr[j]=(long *)(subheader+4);
                        break;

              case 11 : outtbtyp[j]=811; /*uint8*/
                        outtbptr[j]=(unsigned char *)(subheader+4);
                        break;

              case 12 : outtbtyp[j]=812; /*uint16*/
                        outtbptr[j]=(unsigned short *)(subheader+4);
                        break;

              case 14 : outtbtyp[j]=814; /*uint32*/
                        outtbptr[j]=(unsigned long *)(subheader+4);
                        break;

              default : Scierror(888,\
                                "%s : error. Type %d of integer scalar matrix not yet supported.\n",\
                                fname,subheader[3]);
                        freeintparam;
                        FREE(outtbptr);
                        FREE(outtbtyp);
                        FREE(outtbsz);
                        FREE(lfunpt);
                        if (outtb_elem!=NULL) FREE(outtb_elem);
                        break;
             }
             break;


   default : Scierror(888,"%s : error. Type %d not yet supported.\n",fname,subheader[0]);
             freeintparam;
             FREE(outtbptr);
             FREE(outtbtyp);
             FREE(outtbsz);
             FREE(lfunpt);
             if (outtb_elem!=NULL) FREE(outtb_elem);
             return 0;
             break;
  }

  /* store lnk and pos in outtb_elem */
  k=nelem;
  nelem+=outtbsz[j*2]*outtbsz[(j*2)+1];
  if ((outtb_elem=(outtb_el *) REALLOC(outtb_elem,nelem*sizeof(outtb_el)))==NULL)
  {
   Scierror(999,"%s : No more free memory.\n",fname);
   freeintparam;
   FREE(outtbptr);
   FREE(outtbtyp);
   FREE(outtbsz);
   FREE(lfunpt);
   if (outtb_elem!=NULL) FREE(outtb_elem);
   return 0;
  }
  for (i=0;i<outtbsz[j*2]*outtbsz[(j*2)+1];i++)
  {
   outtb_elem[k+i].lnk=j;
   outtb_elem[k+i].pos=i;
  }
 }

 /********************************
  * save intersci common
  * see intersci_push in stack2.c
  ********************************/
  Nbvars = Rhs; /*because of the use of getdata*/
  new = MALLOC(Rhs*sizeof(intersci_state));
  if (new == NULL)
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   freeintparam;
   FREE(outtbptr);
   FREE(outtbtyp);
   FREE(outtbsz);
   FREE(lfunpt);
   FREE(outtb_elem);
   return 0;
  }
  loc = MALLOC(sizeof(intersci_list));
  if (loc == NULL)
  {
   Scierror(999,"%s : Memory allocation error.\n",fname);
   freeintparam;
   FREE(outtbptr);
   FREE(outtbtyp);
   FREE(outtbsz);
   FREE(lfunpt);
   FREE(outtb_elem);
   FREE(new);
   return 0;
  }
  loc->state = new; 
  loc->nbvars = Nbvars;
  for (i=0;i<Rhs;i++)
  {
   loc->state[i].iwhere = C2F(intersci).iwhere[i];
   loc->state[i].ntypes = C2F(intersci).ntypes[i];
   loc->state[i].lad    = C2F(intersci).lad[i];
   loc->state[i].lhsvar = C2F(intersci).lhsvar[i];
  }

 /************************
  * call scicos simulator
  ************************/
 if (C2F(iop).ddt!=0) C2F(dbcos).idb=1;   /*debug mode if ddt=0*/

 /* Calling sequence :
  *   int C2F(scicos)
  *   (x_in, xptr_in, z__, work,zptr,modptr_in, iz, izptr, t0_in, tf_in, tevts_in, 
  *    evtspt_in, nevts, pointi_in, outtbptr_in, outtbsz_in, outtbtyp_in, nlnk1,
  *    funptr, funtyp_in, inpptr_in, outptr_in,
  *    inplnk_in, outlnk_in, rpar, rpptr, ipar, ipptr, clkptr_in,
  *    ordptr_in, nordptr1, ordclk_in, cord_in, ncord1, iord_in, niord1, oord_in, noord1,
  *    zord_in, nzord1, critev_in, nblk1, ztyp, zcptr_in, subscr, nsubs, simpar,
  *    flag__, ierr_out)
  *
  *   double *x_in,*z__;
  *   void **work;
  *   integer *modptr_in;
  *   integer *xptr_in;
  *   integer *zptr, *iz, *izptr;
  *   double *t0_in, *tf_in, *tevts_in;
  *   integer *evtspt_in, *nevts, *pointi_in;
  *   void **outtbptr_in;
  *   integer *outtbsz_in;
  *   integer *outtbtyp_in;
  *   integer *nlnk1, *funptr, *funtyp_in, *inpptr_in, *outptr_in;
  *   integer *inplnk_in, *outlnk_in;
  *   double *rpar;
  *   integer *rpptr, *ipar, *ipptr, *clkptr_in, *ordptr_in, *nordptr1;
  *   integer *ordclk_in, *cord_in, *ncord1, *iord_in, *niord1, *oord_in;
  *   integer *noord1, *zord_in, *nzord1, *critev_in, *nblk1, *ztyp, *zcptr_in;
  *   integer *subscr, *nsubs;
  *   double *simpar;
  *   integer *flag__, *ierr_out;
  */

C2F(scicos)(l_state_x,l_sim_xptr,l_state_z, \
            l_state_iz,l_sim_zptr, \
            l_sim_modptr, \
            l_sim_lab,il_sim_labptr,l_tcur,l_tf,l_state_tevts, \
            l_state_evtspt,&m1e5,l_pointi,outtbptr,outtbsz,outtbtyp, \
            outtb_elem,&nelem,&nlnk, \
            lfunpt,l_sim_funtyp,l_sim_inpptr, \
            l_sim_outptr,l_sim_inplnk,l_sim_outlnk,  \
            l_sim_rpar,l_sim_rpptr, \
            l_sim_ipar,l_sim_ipptr,l_sim_clkptr, \
            l_sim_ordptr,&m4e15, \
            l_sim_ordclk,l_sim_cord,&m4e18, \
            l_sim_iord,&m4e28, \
            l_sim_oord,&m4e19,l_sim_zord, &m4e20, \
            l_sim_critev,&nblk,l_sim_ztyp,l_sim_zcptr, \
            l_sim_subscr,&m4e26,simpar,&flag,&ierr);

 C2F(dbcos).idb=0;  /*return in normal mode*/

 /******************************
  * retrieve intersci common
  * see intersci_pop in stack2.c
  ******************************/
 Nbvars = loc->nbvars;
 for (i=0;i<Rhs;i++)
 {
  C2F(intersci).iwhere[i] = loc->state[i].iwhere ;
  C2F(intersci).ntypes[i] = loc->state[i].ntypes ;
  C2F(intersci).lad[i]    = loc->state[i].lad    ;
  C2F(intersci).lhsvar[i] = loc->state[i].lhsvar ;
 }
 FREE(loc->state);
 FREE(loc);

 /**********************
  * Free allocated array
  **********************/
 FREE(outtbptr);
 FREE(outtbtyp);
 FREE(outtbsz);
 FREE(lfunpt);
 FREE(outtb_elem);

 /*************************************
  * switch to appropriate message error
  *************************************/
 if (ierr>0)
 {
  switch (ierr)
  {
   case 1  : strcpy(C2F(cha1).buf,"scheduling problem");
             C2F(curblk).kfun=0;
             break;

   case 2  : strcpy(C2F(cha1).buf,"input to zero-crossing stuck on zero");
             C2F(curblk).kfun=0;
             break;

   case 3  : strcpy(C2F(cha1).buf,"event conflict");
             C2F(curblk).kfun=0;
             break;

   case 4  : strcpy(C2F(cha1).buf,"algrebraic loop detected");
             C2F(curblk).kfun=0;
             break;

   case 5  : strcpy(C2F(cha1).buf,"cannot allocate memory");
             C2F(curblk).kfun=0;
             break;

   case 6  : strcpy(C2F(cha1).buf,"a block has been called with input out of its domain");
             break;

   case 7  : strcpy(C2F(cha1).buf,"singularity in a block");
             break;

   case 8  : strcpy(C2F(cha1).buf,"block produces an internal error");
             break;

   case 20  : strcpy(C2F(cha1).buf,"initial conditions not converging");
              C2F(curblk).kfun=0;
              break;

   case 21  : sprintf(C2F(cha1).buf, "cannot allocate memory in block=%d", \
                     C2F(curblk).kfun);
              C2F(curblk).kfun=0;
              break;

   case 22  : strcpy(C2F(cha1).buf,"sliding mode condition, cannot integrate");
              C2F(curblk).kfun=0;
              break;

   default  : if(ierr>=1000)
                strcpy(C2F(cha1).buf,"unknown or erroneous block");
              else if (ierr>=100)
              {
               istate=-(ierr-100);
               sprintf(C2F(cha1).buf, "integration problem istate=%d",istate);
               C2F(curblk).kfun=0;
              }
              else
              {
               strcpy(C2F(cha1).buf,"scicos unexpected error,please report...");
               C2F(curblk).kfun=0;
              }
              break;
  }
  if (! (C2F(errgst).err1>0||C2F(iop).err>0))
  {
   Scierror(888,"%s\n",C2F(cha1).buf);
   C2F(curblk).kfun=0;
   C2F(com).fun=0; /*set common fun=0 (this disable bug in debug mode)*/
   freeintparam;
   return 0;
  }
 }

 if (C2F(iop).err>0) 
 {
  freeintparam;
  return 0;
 }

 C2F(curblk).kfun=0;
 C2F(com).fun=0;

 /*********************
  * return Lsh variable
  *********************/
 /*copy int parameters of state in double parameters*/
 for(i=0;i<(m1e6*n1e6);i++) ((double *)(il_state_evtspt+4))[i] = (double) l_state_evtspt[i];
 for(i=0;i<(m1e7*n1e7);i++) ((double *)(il_pointi+4))[i] = (double) l_pointi[i];
 /*set lsh var*/
 if (Lhs>=1) LhsVar(1) = 1; /*return state in LhsVar(1)*/
 if (Lhs==2) LhsVar(2) = 2; /*return tcur in LhsVar(2)*/

 /* end */
 freeintparam;
 return 0;
 }

/*-----------------------------------------------------------------
 * CopyVarFromlistentry
 *    Copy a Scilab object in a list to the variable position  lw
 *
 * Calling sequence :
 *  int CopyVarFromlistentry(int lw, int *header, int i)
 *
 * Input parameters : lw : integer, the free position
 *                    header : integer pointer, a pointer of a list.
 *                    i : integer, give the number of the element to copy
 *
 * Output : FALSE if failed, TRUE else.
 *
 * Examples of use 
 *
 * 1 -  put the third element of a list given in position lw=1 
 *      to position lw=2 :
 *
 *  int *il_list;
 *  il_list = (int *) Getdata(1);
 *  CopyVarFromlistentry(2, il_list, 3)
 *
 * 2 - put the second element of a list stored in the fourth element 
 *     of a list in position lw=1 to position lw=3 :
 *
 *  int *il_list;
 *  il_list = (int *) Getdata(1);
 *  int *il_sublist;
 *  il_sublist = (int *) listentry(il_list,4);
 *  CopyVarFromlistentry(3, il_sublist, 2)
 *----------------------------------------------------------------*/

int CopyVarFromlistentry(int lw, int *header, int i)
{
   /* Local variablle definition*/
   int ret,un=1;
   double *l;
   int n;

   /* Test if we receive a NULL ptr header */
   if (header==NULL) return FALSE_;

   /* Get the start address of the i element of the input list*/
   if ((l = (double *) listentry(header,i))==NULL) return FALSE_;

   /* Compute the length of the i element in double word */
   n = header[i+2]-header[i+1];

   /* Create empty data of a size n*sizeof(double) at the position lw */
   if ((ret=C2F(createdata)(&lw, n*sizeof(double)))==FALSE_) return ret;

   /* Copy the element i to position lw*/
   C2F(unsfdcopy)(&n,l,&un,stk(*Lstk(lw + Top - Rhs)),&un);
   return TRUE_;
}

/* var2sci function to convert an array of scicos
 * blocks to scilab object in the Top+1 position
 * in the stack.
 *
 * Input parameters :
 * *x       : void ptr, scicos blocks array to store
 *            in the Top+1 position in the stack.
 * n        : integer, number of rows.
 * m        : integer, number of columns.
 * typ_var  : integer, type of scicos data :
 *            10  : double real
 *            11  : double complex
 *            80  : int
 *            81  : int8
 *            82  : int16
 *            84  : int32
 *            800 : uint
 *            811 : uint8
 *            812 : uint16
 *            814 : uint32
 *
 *
 * Output parameters : int (<1000), error flag
 *                     (0 if no error)
 *
 * 07/06/06, Alan    : initial version.
 *
 * 23/06/06, Alan    : moved in intcscicos.c to do
 *                     the connection with getscicosvars("blocks")
 */

/* prototype */
int var2sci(void *x,int n,int m,int typ_var)
{
  /************************************
   * variables and constants d�inition
   ************************************/
  /*counter and address variable declaration*/
  int nm,il,l,j,err;

  /*define all type of accepted ptr */
  double *x_d,*ptr_d;
  char *x_c,*ptr_c;
  unsigned char *x_uc,*ptr_uc;
  short *x_s,*ptr_s;
  unsigned short *x_us,*ptr_us;
  int *x_i,*ptr_i;
  unsigned int *x_ui,*ptr_ui;
  long *x_l,*ptr_l;
  unsigned long *x_ul,*ptr_ul;

  /* Check if the stack is not full */
  if (Top >= Bot)
  {
   err = 1;
   return err;
  }
  else
  {
   Top = Top + 1;
   il = iadr(*Lstk(Top));
   l = sadr(il+4);
  }

  /* set number of double needed to store data */
  if (typ_var==10) nm=n*m; /*double real matrix*/
  else if (typ_var==11)  nm=n*m*2; /*double real matrix*/
  else if (typ_var==80)  nm=(int)(ceil((n*m)/2)+1); /*int*/
  else if (typ_var==81)  nm=(int)(ceil((n*m)/8)+1); /*int8*/
  else if (typ_var==82)  nm=(int)(ceil((n*m)/4)+1); /*int16*/
  else if (typ_var==84)  nm=(int)(ceil((n*m)/2)+1); /*int32*/
  else if (typ_var==800) nm=(int)(ceil((n*m)/2)+1); /*uint*/
  else if (typ_var==811) nm=(int)(ceil((n*m)/8)+1); /*uint8*/
  else if (typ_var==812) nm=(int)(ceil((n*m)/4)+1); /*uint16*/
  else if (typ_var==814) nm=(int)(ceil((n*m)/2)+1); /*uint32*/
  else nm=n*m; /*double real matrix*/

  /*check if there is free space for new data*/
  err = l + nm - *Lstk(Bot);
  if (err > 0)
  {
   err = 2;
   return err;
  }

  /**************************
   * store data on the stack
   *************************/
  switch (typ_var) /*for each type of data*/
  {
       case 10  : /* set header */
                  *istk(il) = 1; /*double real matrix*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 0;
                  x_d = (double *) x;
                  ptr_d = (double *) stk(l);
                  for (j=0;j<m*n;j++) ptr_d[j] = x_d[j];
                  break;

       case 11  : /* set header */
                  *istk(il) = 1; /*double complex matrix*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 1;
                  x_d = (double *) x;
                  ptr_d = (double *) stk(l);
                  for (j=0;j<2*m*n;j++) ptr_d[j] = x_d[j];
                  break;

       case 80  : /* set header */
                  *istk(il) = 8; /*int*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 4;
                  x_i = (int *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_i = (int *) istk(il+4);
                   ptr_i[j] = x_i[j];
                  }
                  break;

       case 81  : /* set header */
                  *istk(il) = 8; /*int8*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 1;
                  x_c = (char *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_c = (char *) istk(il+4);
                   ptr_c[j] = x_c[j];
                  }
                  break;

       case 82  : /* set header */
                  *istk(il) = 8; /*int16*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 2;
                  x_s = (short *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_s = (short *) istk(il+4);
                   ptr_s[j] = x_s[j];
                  }
                  break;

       case 84  : /* set header */
                  *istk(il) = 8; /*int32*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 4;
                  x_l = (long *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_l = (long *) istk(il+4);
                   ptr_l[j] = x_l[j];
                  }
                  break;

       case 800 : /* set header */
                  *istk(il) = 8; /*uint*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 14;
                  x_ui = (unsigned int *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_ui = (unsigned int *) istk(il+4);
                   ptr_ui[j] = x_ui[j];
                  }
                  break;

       case 811 : /* set header */
                  *istk(il) = 8; /*uint8*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 11;
                  x_uc = (unsigned char *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_uc = (unsigned char *) istk(il+4);
                   ptr_uc[j] = x_uc[j];
                  }
                  break;

       case 812 : /* set header */
                  *istk(il) = 8; /*uint16*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 12;
                  x_us = (unsigned short *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_us = (unsigned short *) istk(il+4);
                   ptr_us[j] = x_us[j];
                  }
                  break;

       case 814 : /* set header */
                  *istk(il) = 8; /*uint32*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 14;
                  x_ul = (unsigned long *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_ul = (unsigned long *) istk(il+4);
                   ptr_ul[j] = x_ul[j];
                  }
                  break;

       default  : /* set header */
                  *istk(il) = 1; /*double real matrix*/
                  *istk(il+1) = n;
                  *istk(il+2) = m;
                  *istk(il+3) = 0;
                  x_d = (double *) x;
                  for (j=0;j<m*n;j++)
                  {
                   ptr_d = (double *) stk(il+4);
                   ptr_d[j] = x_d[j];
                  }
                  break;
  }

  /* set value in lstk */
  *Lstk(Top+1) = l + nm;

  /*return error flag = 0 */
  err = 0;
  return 0;
}

/* createblklist : function to create a Typed List
 *                 of a scicos_block structure
 *                 at the top+1 postion of the stack
 *
 * needs/depends : var2sci, C2F(mklist), C2F(mtklist),
 *                 vvtosci, string.h, C2F(scierr), str2sci
 *
 * input argument : Blocks :scicos_block  ptr on a scicos_block structure
 *                  ierr : int ptr, an error flag
 *                  flag_imp : if flag_imp>=0 then use
 *                             import structure for x, xd and g.
 *                             In this case flag_imp is the block number.
 *
 * output argument : return 0 if failed, 1 else.
 *
 *
 * 23/06/06, Alan : extracted from sciblk4 to be used
 *                  in intgetscicosvars
 *
 * 26/06/06, Alan : Add flag_imp in input arguments.
 *                  This is done to disable scilab crash with getscicosvars("blocks")
 *                  because when calling at the beginning of the simulation, x, xd and
 *                  g are not yet informed for all blocks with nx!=0 and ng!=0.
 *                  (They are not yet called with callf in scicos.c)
 *
 */

/*prototype*/
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp)
{
  /*local variable declaration*/
  int k;
  int nu,mu,ny,my;
  int u_typ,y_typ;

  /*variable used when imp_flag>=0*/
  int nv,mv;          /* length of data                                        */
  int nblk,ng;        /* to store number of blocks and number of zero cross.   */
  void *ptr;          /* ptr for data comming from import structure            */
  int *ptr_int;       /* ptr to store ptr on integer                           */
  double *ptr_double; /* ptr to store ptr on double                            */
  int *xptr, *zcptr;  /* to retrieve xptr by import and zcptr of scicos_blocks */
  double *x,*xd,*g;   /* ptr for x, xd and g for scicos_blocks              */

  /* set length of block list -please update me-                           */
  static int nblklst=31;
  /* set string of first element of scilab Blocks tlist -please update me- */
  static char *str_blklst[]={ "scicos_block", "nevprt" , "funpt" , "type"  ,
                              "scsptr"      , "nz"     , "z"     , "nx"    ,
                              "x"           , "xd"     , "res"   , "nin"   ,
                              "insz"        , "inptr"  , "nout"  , "outsz" ,
                              "outptr"      , "nevout" , "evout" , "nrpar" ,
                              "rpar"        , "nipar"  , "ipar"  , "ng"    ,
                              "g"           , "ztyp"   , "jroot" , "label" ,
                              "work"        , "nmode"  , "mode"};

  /* char ptr for str2sci - see below - */
  char **str1;


  /* set nblk, x, xd ptr coming from import strucuture,
   * if flag_imp >=0
   */
  if (flag_imp>=0)
  {
   /*retrieve nblk by import structure*/
   strcpy(C2F(cha1).buf,"nblk");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   if (*ierr==0) return 0;
   ptr_int = (int *) ptr;
   nblk = *ptr_int;

   /* retrieve ng by import structure */
   strcpy(C2F(cha1).buf,"ng");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
   ng = *ptr_int;

   /*retrieve xptr by import structure*/
   strcpy(C2F(cha1).buf,"xptr");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   if (*ierr==0) return 0;
   ptr_int = (int *) ptr;
   xptr = ptr_int;

   /*retrieve zcptr by import structure*/
   strcpy(C2F(cha1).buf,"zcptr");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   ptr_int = (int *) ptr;
   zcptr = ptr_int;

   /*retrieve x and xd by import structure*/
   strcpy(C2F(cha1).buf,"x");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   if (*ierr==0) return 0;
   ptr_double = (double *) ptr;
   x = ptr_double;
   xd = &x[xptr[nblk]-1];

   /*retrieve g by import structure*/
   strcpy(C2F(cha1).buf,"g");
   *ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
   ptr_double = (double *) ptr;
   g = ptr_double;
  }

  /****************************
   * create scilab tlist Blocks
   ****************************/
  /* 1 - scicos_block */
  str2sci(str_blklst,1,31);

  /* 2 - nevprt */
  *ierr=var2sci(&Blocks[0].nevprt,1,1,80);
  if (*ierr!=0) return 0;

  /* 3 - funpt */
  *ierr=var2sci(&Blocks[0].funpt,0,1,80); /* !!!! */
  if (*ierr!=0) return 0;

  /* 4 - type */
  *ierr=var2sci(&Blocks[0].type,1,1,80); /* !!!! */
  if (*ierr!=0) return 0;

  /* 5 - scsptr */
  *ierr=var2sci(&Blocks[0].scsptr,0,1,80); /* !!!! */
  if (*ierr!=0) return 0;

  /* 6 - nz */
  *ierr=var2sci(&Blocks[0].nz,1,1,80);
  if (*ierr!=0) return 0;

  /* 7 - z */
  if(Blocks[0].scsptr>0)
  {
   C2F(vvtosci)(Blocks[0].z,&Blocks[0].nz);
   if (C2F(scierr)()!=0) return 0; 
  }
  else
  {
   *ierr=var2sci(Blocks[0].z,Blocks[0].nz,1,10);
   if (*ierr!=0) return 0; 
  }

  /* 8 - nx */
  *ierr=var2sci(&Blocks[0].nx,1,1,80);
  if (*ierr!=0) return 0;

  /* 9 - x */
  if (flag_imp>=0) *ierr=var2sci(&x[xptr[flag_imp]-1],Blocks[0].nx,1,10);
  else *ierr=var2sci(Blocks[0].x,Blocks[0].nx,1,10);
  if (*ierr!=0) return 0; 

  /* 10 - xd */
  if (flag_imp>=0) *ierr=var2sci(&xd[xptr[flag_imp]-1],Blocks[0].nx,1,10);
  else *ierr=var2sci(Blocks[0].xd,Blocks[0].nx,1,10);
  if (*ierr!=0) return 0; 

  /* 11 - res */
  *ierr=var2sci(Blocks[0].res,Blocks[0].nx,1,10);
  if (*ierr!=0) return 0;

  /* 12 - nin */
  *ierr=var2sci(&Blocks[0].nin,1,1,80);
  if (*ierr!=0) return 0;

  /* 13 - insz */
  *ierr=var2sci(Blocks[0].insz,3*Blocks[0].nin,1,80);
  if (*ierr!=0) return 0;

  /* 14 - inptr */
  for (k=0;k<Blocks[0].nin;k++) 
  {
   nu=Blocks[0].insz[k]; /* retrieve number of rows */
   mu=Blocks[0].insz[Blocks[0].nin+k]; /* retrieve number of cols */
   u_typ=Blocks[0].insz[2*Blocks[0].nin+k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].inptr[k],nu,mu,u_typ);
   if (*ierr!=0) return 0;
  }
  C2F(mklist)(&Blocks[0].nin); /*create inptr list*/

  /* 15 - nout */
  *ierr=var2sci(&Blocks[0].nout,1,1,80);
  if (*ierr!=0) return 0;

  /* 16 - outsz */
  *ierr=var2sci(Blocks[0].outsz,3*Blocks[0].nout,1,80);
  if (*ierr!=0) return 0;

  /* 17 - outptr */
  for (k=0;k<Blocks[0].nout;k++) 
  {
   ny=Blocks[0].outsz[k]; /* retrieve number of rows */
   my=Blocks[0].outsz[Blocks[0].nout+k]; /* retrieve number of cols */
   y_typ=Blocks[0].outsz[2*Blocks[0].nout+k]; /* retrieve type */
   *ierr=var2sci(Blocks[0].outptr[k],ny,my,y_typ);
   if (*ierr!=0) return 0;
  }
  C2F(mklist)(&Blocks[0].nout); /*create outptr list*/

  /* 18 - nevout */
  *ierr=var2sci(&Blocks[0].nevout,1,1,80);
  if (*ierr!=0) return 0;

  /* 19 - evout */
  *ierr=var2sci(Blocks[0].evout,Blocks[0].nevout,1,10);
  if (*ierr!=0) return 0; 

  /* 20 - nrpar */
  *ierr=var2sci(&Blocks[0].nrpar,1,1,80);
  if (*ierr!=0) return 0;

  /* 21 - rpar */
  if(Blocks[0].scsptr>0)
  {
   C2F(vvtosci)(Blocks[0].rpar,&Blocks[0].nrpar);
   if (C2F(scierr)()!=0) return 0;
  }
  else
  {
   *ierr=var2sci(Blocks[0].rpar,Blocks[0].nrpar,1,10);
   if (*ierr!=0) return 0; 
  }

  /* 22 - nipar */
  *ierr=var2sci(&Blocks[0].nipar,1,1,80);
  if (*ierr!=0) return 0;

  /* 23 - ipar */
  *ierr=var2sci(Blocks[0].ipar,Blocks[0].nipar,1,80);
  if (*ierr!=0) return 0;

  /* 24 - ng */
  *ierr=var2sci(&Blocks[0].ng,1,1,80);
  if (*ierr!=0) return 0;

  /* 25 - g */
  if (flag_imp>=0) *ierr=var2sci(&g[zcptr[flag_imp]-1],Blocks[0].ng,1,10);
  else *ierr=var2sci(Blocks[0].g,Blocks[0].ng,1,10);
  if (*ierr!=0) return 0;

  /* 26 - ztyp */
  *ierr=var2sci(&Blocks[0].ztyp,1,1,80);
  if (*ierr!=0) return 0;

  /* 27 - jroot */
  *ierr=var2sci(Blocks[0].jroot,Blocks[0].ng,1,80);
  if (*ierr!=0) return 0;

  /* 28 - label */
  if ((str1=MALLOC(sizeof(char*))) ==NULL )  return 0;
  if ((str1[0]=MALLOC(sizeof(char)*(strlen(Blocks[0].label)+1))) ==NULL )  return 0;
  (str1[0])[strlen(Blocks[0].label)]='\0';
  strncpy(str1[0],Blocks[0].label,strlen(Blocks[0].label));
  str2sci(str1,1,1);
  FREE(str1[0]);
  FREE(str1);
  if (C2F(scierr)()!=0) return 0; 

  /* 29 - work*/
  C2F(vvtosci)(*Blocks[0].work,(k=0,&k));
  if (C2F(scierr)()!=0) return 0; 

  /* 30 - nmode*/
  *ierr=var2sci(&Blocks[0].nmode,1,1,80);
  if (*ierr!=0) return 0;

  /* 31 - mode */
  *ierr=var2sci(Blocks[0].mode,Blocks[0].nmode,1,80);
  if (*ierr!=0) return 0;

  C2F(mktlist)(&nblklst); /*create Blocks list*/
  if (C2F(scierr)()!=0) return 0;

  /*return 1 if succeeded */
  return 1;
}

/* intgetscicosvarsc getscicosvars interface routine
 * retrieve some informations during simulation.
 *
 * [myvar]=getscicosvars(str)
 *
 * rhs 1  : str : a character string matrix with choice,
 *               - 'x' to retrieve continuous state
 *               - 'xptr' to retrieve ptr of continuous state
 *               - 'z' to retrieve discrete state
 *               - 'zptr' to retrieve ptr of discrete state
 *               - 'rpar' to retrieve real parameters
 *               - 'rpptr' to retrieve ptr of real parameters
 *               - 'ipar' to retrieve integer parameters
 *               - 'ipptr' to retrieve  ptr of integer parameters
 *               - 'outtb' to retrieve output register (list of scilb object)
 *               - 'inpptr' to retrieve number of input ports
 *               - 'outptr' to retrieve number of output ports
 *               - 'inplnk' to retrieve link number of input ports
 *               - 'outlnk' to retrieve link number of output ports
 *               ...... -see below-
 *
 * lhs 1  : myvar : matrix of int32 or double, or list or a Tlist
 *
 * 31/05/06, Alan : Rewritten from original fortran
 * source code intgetscicosvars in coselm.f.
 *
 * 22/06/06, Alan : Allow multiple string in rhs(1).
 *                  Create Tlist for Lhs(1).
 *
 * 23/06/06, Alan : Create blocks list for Lhs(1).
 *
 * 13/11/06, Alan : Remove il_sim_save global variable (all in sim
 *                  come from import struct now)
 *                  evtspt & pointi of state come from import struct
 */

int intgetscicosvarsc(fname,fname_len)
                 char *fname;
                 unsigned long fname_len;
{
  /************************************
   * variables and constants d�inition
   ************************************/
  /* auxilary variables for dimension and address */
  int m1,n1;     /* dimension of input character string               */
  int *il_str;   /* address of the description of the input parameter */
  int *l_str;    /* address of the data of the input parameter        */
  int l_tmp;     /* temp variables to store address                   */
  int *il_tmp;

  /* definition of min/max input/output argument */
  static int minrhs=0, maxrhs=1;

  /* auxilary variables */
  int nv,mv;                /* length of data                                      */
  void *ptr;                /* ptr for data comming from import structure          */
  int *ptr_int;             /* ptr to store ptr on integer                         */
  double *ptr_dd;           /* ptr to store ptr on double                          */
  scicos_block *ptr_scsblk; /* ptr to store ptr of scicos_block structure          */
  outtb_el *ptr_elem;       /* ptr to store ptr of outtb_el structure              */
  int nblk,ng;              /* to store number of blocks and number of zero cross. */
  int *xptr, *zcptr;        /* to store xptr and zcptr of scicos_blocks            */
  double *x,*xd,*g;         /* ptr for x, xd and g for scicos_blocks               */

  int ierr;       /* error flag                                */
  int errc;       /* error flag for ceateblklst                */
  int ptr_pos;    /* to store position in scilab string object */
  int Topsave;    /* to save the Top position                  */

  int sz_str;     /* local variabe to store size of string */
  int i,j,k;      /* local counter variable                */

  /* number of entries -please update me-                        */
  static int nentries=60;
  /* define accepted entries of getscicosvars -please update me- */
  static char *entry[]={ "x"        , "nx"      , "xptr"     , "zcptr"  , "z"      ,
                         "nz"       , "zptr"    , "rpar"     , "rpptr"  , "ipar"   ,
                         "ipptr"    , "outtb"   , "inpptr"   , "outptr" , "inplnk" ,
                         "outlnk"   , "subs"    , "tevts"    , "evtspt" , "pointi" ,
                         "iord"     , "oord"    , "zord"     , "funtyp" , "ztyp"   ,
                         "cord"     , "ordclk"  , "clkptr"   , "ordptr" , "critev" ,
                         "mod"      , "nmod"    , "iz"       , "nblk"   , "izptr"  ,
                         "outtbptr" , "outtbsz" , "outtbtyp" , "nlnk"   , "nsubs"  ,
                         "nevts"    , "niord"   , "noord"    , "nzord"  , "funptr" ,
                         "ncord"    , "nordptr" , "iwa"      , "blocks" , "ng"     ,
                         "g"        , "t0"      , "tf"       , "Atol"   , "rtol"   ,
                         "ttol"     , "deltat"  , "hmax"     , "nelem"  , "outtb_elem"};

  char **dyn_char; /* for allocation of first entry in tlist */

  /****************************************
   * Check number of inputs and outputs Rhs
   ****************************************/
  CheckRhs(minrhs,maxrhs);

  /* Display usage of getscicosvars function if Rhs==0 */
  if (Rhs==0)
  {
   sciprint("\ngetscicosvars : utility function to retrieve\n" 
            "                scicos arrays during simulation.\n\n"
            "Usage : [myvar]=getscicosvars([\"str1\";\"str2\";...]);\n\n"
            "- myvar : an int32 or double matrix or a Tlist.\n"
            "- [\"str1\";\"str2\",...] is a string matrix\n"
            "  that must be informed with the following values :\n");

   /* display allowed entries */
   i=0;
   for (j=0;j<nentries;j++)
   {
    if (j==nentries-1) sciprint("\"%s\" ",entry[j]);
    else sciprint("\"%s\", ",entry[j]);
    i++;
    if (i==6) {sciprint("\n");i=0;}
   }
   sciprint("\n");
   return 0;
  }

  /*******************
   * Check str (rhs 1)
   *******************/
  il_str = (int *) GetData(1); /* get ptr of integer header of rsh 1 */
  if(il_str[0]!=10) /* Check if input argument is a character string matrix */
  {
   Scierror(55,"%s : First argument must be a string.\n",fname);
   C2F(iop).err=1;
   return 0;
  }

  /*retrieve dimension of input string matrix*/
  m1 = il_str[1]; /* number of row    */
  n1 = il_str[2]; /* number of column */

  /*Create header of Tlist*/
  if ((dyn_char=MALLOC((1+m1*n1)*sizeof(char *)))==NULL)
  {
   Scierror(999,"%s : No more memory.\n",fname);
   return 0;
  }

  /* Type of list is scicosvar */
  if ((dyn_char[0]=MALLOC((strlen("scicosvar")+1)*sizeof(char)))==NULL)
  {
   FREE(dyn_char);
   Scierror(999,"%s : No more memory.\n",fname);
   return 0;
  }
  else strcpy(dyn_char[0],"scicosvar");

  /*Check string matrix  */
  for (j=0;j<m1*n1;j++)
  {
    sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
    /*get current position in the istk*/
    if (j==0)
      ptr_pos=5+m1*n1; 
    else
      ptr_pos += il_str[5+j-1]-il_str[4+j-1];
    l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
    /* codetoascii convertion */
    C2F(cha1).buf[0]=' ';
    C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
    C2F(cha1).buf[sz_str]='\0';
    /* search if string is in accordance with entry*/
    ierr=TRUE_;
    for (i=0;i<nentries;i++)
    {
     if (strcmp(C2F(cha1).buf,entry[i]) == 0) 
     {
      /* Store string in header of Tlist */
      if ((dyn_char[j+1]=MALLOC(sizeof(char)*strlen(entry[i])+1))==NULL)
      {
       FREE(dyn_char);
       Scierror(999,"%s : No more memory.\n",fname);
       return 0;
      }
      else strcpy(dyn_char[j+1],entry[i]);

      ierr=FALSE_;
      break;
     }
    }
    /* if failed then display an error message and exit*/
    if (ierr==TRUE_)
    {
     FREE(dyn_char);
     Scierror(999,"%s : Undefined field in string matrix position : %d.\n",fname,j+1);
     return 0;
    }
  }

  /* store dyn_char on stack*/
  if (n1*m1>1) str2sci(dyn_char,1,n1*m1+1);

  /* return asked array */
  for (j=0;j<m1*n1;j++)
  {
   sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
   /*get current position in the istk of the string*/
   if (j==0)
     ptr_pos=5+m1*n1; 
   else
     ptr_pos += il_str[5+j-1]-il_str[4+j-1];
   l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
   /* codetoascii convertion */
   C2F(cha1).buf[0]=' ';
   C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
   C2F(cha1).buf[sz_str]='\0';

   /*****************************************************************
    * entries that can be retrieve by il_state_save global variable
    *****************************************************************/
   if (strcmp(C2F(cha1).buf,"x") == 0)           /* retrieve continuous state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,2);
   else if (strcmp(C2F(cha1).buf,"z") == 0)      /* retrieve discrete state */
    ierr=CopyVarFromlistentry(j+2,il_state_save,3);
   else if (strcmp(C2F(cha1).buf,"outtb") == 0)  /* retrieve outtb */
    ierr=CopyVarFromlistentry(j+2,il_state_save,8);
   else if (strcmp(C2F(cha1).buf,"tevts") == 0)  /* retrieve tevts */
    ierr=CopyVarFromlistentry(j+2,il_state_save,5);

   /*************************************************
    * integer variables coming from import structure
    *************************************************/
   else if ((strcmp(C2F(cha1).buf,"mod") == 0)      || /* retrieve mode */
            (strcmp(C2F(cha1).buf,"nmod") == 0)     || /* retrieve nmode */
            (strcmp(C2F(cha1).buf,"iz") == 0)       || /* label integer code of blocks */
            (strcmp(C2F(cha1).buf,"nblk") == 0)     || /* number of block */
            (strcmp(C2F(cha1).buf,"izptr") == 0)    || /* label integer code of blocks ptr*/
            (strcmp(C2F(cha1).buf,"outtbptr") == 0) || /* outtb ptr */
            (strcmp(C2F(cha1).buf,"outtbsz") == 0)  || /* outtb size */
            (strcmp(C2F(cha1).buf,"outtbtyp") == 0) || /* outtb type */
            (strcmp(C2F(cha1).buf,"nlnk") == 0)     || /* number of link */
            (strcmp(C2F(cha1).buf,"nsubs") == 0)    || /* length of nsubs */
            (strcmp(C2F(cha1).buf,"nevts") == 0)    || /* length of evtspt & tevts */
            (strcmp(C2F(cha1).buf,"niord") == 0)    || /* length of iord */
            (strcmp(C2F(cha1).buf,"noord") == 0)    || /* length of oord */
            (strcmp(C2F(cha1).buf,"nzord") == 0)    || /* length of zord */
            (strcmp(C2F(cha1).buf,"funptr") == 0)   || /* retrieve function ptr */
            (strcmp(C2F(cha1).buf,"ncord") == 0)    || /* retrieve ncord */
            (strcmp(C2F(cha1).buf,"nordptr") == 0)  || /* retrieve nordptr */
            (strcmp(C2F(cha1).buf,"iwa") == 0)      || /* retrieve iwa */
            (strcmp(C2F(cha1).buf,"ng") == 0)       || /* retrieve ng */
            (strcmp(C2F(cha1).buf,"nx") == 0)       || /* retrieve nx */
            (strcmp(C2F(cha1).buf,"nz") == 0)       || /* retrieve nz */
            (strcmp(C2F(cha1).buf,"nelem") == 0)    || /* retrieve nelem */
            (strcmp(C2F(cha1).buf,"xptr") == 0)     || /* retrieve xptr */
            (strcmp(C2F(cha1).buf,"zcptr") == 0)    || /* retrieve zcptr */
            (strcmp(C2F(cha1).buf,"zptr") == 0)     || /* retrieve zptr */
            (strcmp(C2F(cha1).buf,"rpptr") == 0)    || /* retrieve rpptr */
            (strcmp(C2F(cha1).buf,"ipar") == 0)     || /* retrieve ipar */
            (strcmp(C2F(cha1).buf,"ipptr") == 0)    || /* retrieve ipptr */
            (strcmp(C2F(cha1).buf,"inpptr") == 0)   || /* retrieve inpptr */
            (strcmp(C2F(cha1).buf,"outptr") == 0)   || /* retrieve outptr */
            (strcmp(C2F(cha1).buf,"inplnk") == 0)   || /* retrieve inplnk */
            (strcmp(C2F(cha1).buf,"outlnk") == 0)   || /* retrieve outlnk */
            (strcmp(C2F(cha1).buf,"subs") == 0)     || /* retrieve subs */
            (strcmp(C2F(cha1).buf,"iord") == 0)     || /* retrieve iord */
            (strcmp(C2F(cha1).buf,"oord") == 0)     || /* retrieve iord */
            (strcmp(C2F(cha1).buf,"zord") == 0)     || /* retrieve iord */
            (strcmp(C2F(cha1).buf,"funtyp") == 0)   || /* retrieve funtyp */
            (strcmp(C2F(cha1).buf,"ztyp") == 0)     || /* retrieve ztyp */
            (strcmp(C2F(cha1).buf,"cord") == 0)     || /* retrieve cord */
            (strcmp(C2F(cha1).buf,"ordclk") == 0)   || /* retrieve ordclk */
            (strcmp(C2F(cha1).buf,"clkcpr") == 0)   || /* retrieve clkcpr */
            (strcmp(C2F(cha1).buf,"ordptr") == 0)   || /* retrieve ordptr */
            (strcmp(C2F(cha1).buf,"critev") == 0)   || /* retrieve critev */
            /* state */
            (strcmp(C2F(cha1).buf,"evtspt") == 0)   || /* retrieve evtspt */
            (strcmp(C2F(cha1).buf,"pointi") == 0)      /* retrieve nelem */
                                                     )
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     l_tmp = I_INT32; /* define type of integer */
     CreateVar(j+2,"I",&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 addr. of the stack */
     il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
     ptr_int = (int *) ptr;             /* cast void* ptr to int* ptr */
     for (i=0;i<nv*mv;i++) il_tmp[i] = ptr_int[i]; /* copy returned array in istk */
    }
   }

  /*************************************************
   * double variables coming from import structure
   *************************************************/
   else if ((strcmp(C2F(cha1).buf,"rpar") == 0)   || /* retrieve rpar      */
            (strcmp(C2F(cha1).buf,"g") == 0)      || /* retrieve g      */
            (strcmp(C2F(cha1).buf,"t0") == 0)     || /* retrieve t0     */
            (strcmp(C2F(cha1).buf,"tf") == 0)     || /* retrieve tf     */
            (strcmp(C2F(cha1).buf,"Atol") == 0)   || /* retrieve Atol   */
            (strcmp(C2F(cha1).buf,"rtol") == 0)   || /* retrieve rtol   */
            (strcmp(C2F(cha1).buf,"ttol") == 0)   || /* retrieve ttol   */
            (strcmp(C2F(cha1).buf,"deltat") == 0) || /* retrieve deltat */
            (strcmp(C2F(cha1).buf,"hmax") == 0)      /* retrieve hmax   */
                                                 )
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     ptr_dd = (double *) ptr;
     CreateVar(j+2,"d",&nv,&mv,&l_tmp); /* Create double variable at the top+j+1 addr. of the stack */
     for (i=0;i<nv*mv;i++) *stk(l_tmp+i) = ptr_dd[i]; /* copy returned array in istk */
    }
   }

   /*************************************************
    * scicos_block ptr coming from import structure
    *************************************************/
   else if ((strcmp(C2F(cha1).buf,"blocks") == 0)
                                                )
   {
    /* retrieve scicos_block prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     /* store ptr in ptrscs_blk */
     ptr_scsblk = (scicos_block *) ptr;

     /* retrieve nblk by import structure */
     strcpy(C2F(cha1).buf,"nblk");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
     nblk = *ptr_int;

     /* retrieve ng by import structure */
     strcpy(C2F(cha1).buf,"ng");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
     ng = *ptr_int;

     /*retrieve xptr by import structure*/
     strcpy(C2F(cha1).buf,"xptr");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr;
     xptr = ptr_int;

     /*retrieve zcptr by import structure*/
     strcpy(C2F(cha1).buf,"zcptr");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_int = (int *) ptr;
     zcptr = ptr_int;

     /*retrieve x and xd by import structure*/
     strcpy(C2F(cha1).buf,"x");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_dd = (double *) ptr;
     x = ptr_dd;
     xd = &x[xptr[nblk]-1];

     /*retrieve g by import structure*/
     strcpy(C2F(cha1).buf,"g");
     ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
     ptr_dd = (double *) ptr;
     g = ptr_dd;

     /* store blklst on stack */
     Topsave=Top;     /* save Top counter */
     Top=Top-Rhs+1+j; /* adjust Top counter */

     for (k=0;k<nblk;k++)
     {/* for each block, call createblklist */

      /* set flag_imp <0 for createblklst */
      i=-1;

      /* the following test is done in order to know if block k
       * have been already called with callf in scicos.c
       */
      if (ptr_scsblk[k].nx!=0)
      {
       if (ptr_scsblk[k].x!=&x[xptr[k]-1])
       {
         /*fprintf(stderr,"k=%d,X,xd Non initialis�n",k);*/
        /* set flag_imp=k for createblklst <0 */
        i=k;
       }
      }
      if (ptr_scsblk[k].ng!=0)
      {
       if ((ptr_scsblk[k].g!=&g[zcptr[k]-1]) && (ptr_scsblk[k].g!=&x[xptr[k]-1]))
       {
        /*fprintf(stderr,"k=%d,g Non initialis�n",k);*/
        /* set flag_imp=k for createblklst <0 */
        i=k;
       }
      }
      /* call createblklist */
      ierr=createblklist(&ptr_scsblk[k], &errc,i);

      /* if an error occurs in createblklist */
      if (ierr==FALSE_)
      {
       Top=Topsave;
       break;
      }
     }
     /* if success, create a list of Typed list scicos_block */
     if (ierr==TRUE_)
     {
      C2F(mklist)(&nblk);
      Top=Topsave; /* adjust Top counter */
      CreateVar(j+2,"l",&nblk,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
     }

    }
   }

  /*******************************************
   * outtb_elem coming from import structure
   *******************************************/
   else if ((strcmp(C2F(cha1).buf,"outtb_elem") == 0)) /* retrieve outtb_eleme */
   {
    /* retrieve dims and prt of asked array with getscicosvarsfromimport */
    ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

    /* check ierr flag */
    if (ierr==TRUE_)
    {
     l_tmp = I_INT32; /* define type of integer */
     CreateVar(j+2,"I",&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 addr. of the stack */
     il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
     ptr_elem = (outtb_el *) ptr;       /* cast void* ptr to int* ptr */
     for (i=0;i<nv;i++) /* copy returned array in istk */
     {
      il_tmp[i] = ptr_elem[i].lnk + 1; /* +1 is for the connection with outtb list */
      il_tmp[nv+i] = ptr_elem[i].pos + 1;
     }
    }
   }

   /* if input argument doesn't match with any accepted string
    * then display an error message.
    */
   else
   {
    Scierror(999,"%s : Undefined field.\n",fname);
    FREE(dyn_char);
    return 0;
   }

   /* if return a FALSE_ value in
    * error flag then display an error message.
    */
   if(ierr!=TRUE_)
   {
    Scierror(999,"%s : Error.\n",fname);
    FREE(dyn_char);
    return 0;
   }
  }
  /**********************
   * Create lsh argument
   **********************/
   /* create a tlist if number of string in rhs 1 is >1*/
   if (j>1) 
   {
    Top=Top+j;
    j++;
    C2F(mktlist)(&j);
    Top=Top-1;
    CreateVar(2,"t",&j,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
   }

   /* put new variable in lsh argument */
   LhsVar(1)=2;

   /* end */
   FREE(dyn_char);
   return 0;
}

/* intcurblkc curblock interface routine
 *
 * [nblk]=curblock()
 *
 * rhs : empty
 * lhs : nblk : the current block (int32 scilab object)
 *
 * 20/06/06, Alan : Rewritten from original fortran
 * source code intcurblk in coselm.f.
 *
 */
int intcurblkc(fname,fname_len)
                 char *fname;
                 unsigned long fname_len;
{
  /***********************
   * variables declaration
   ***********************/
  /* address of the data of the output parameter */
  int l1;
  /* local counter variable */
  int j,k;
  /* definition of min/max output argument */
  static int minlhs=1, maxlhs=1;

  /**************************
   * Check number of  outputs
   **************************/
  CheckLhs(minlhs,maxlhs);

  /************************
   * Create int32 variable
   ************************/
  /* define type of integer */
  l1 = I_INT32;
  /* Create int32 variable at the top addr. of the stack */
  CreateVar(1,"I",(j=1,&j),(k=1,&k),&l1);
  /* Store value of C2F(curblk).kfun at the l1 address in istk */
  *istk(l1) = C2F(curblk).kfun;

  /* return the value stored at Top address to lhs variable */
  LhsVar(1) = 1;

  /* return 0 as default value */
  return 0;
}

/* intbuildouttb build an initialized outtb list
 *
 * [outtb]=buildouttb(lnksz,lnktyp)
 *
 * rhs 1 : lnksz, give the size of scilab object in outtb
 *         can be all int type or double matrix
 *         can have n,2 or 2,n size
 *
 * rhs 2 : lnktyp, gives the type of scilab objetc in outtb
 *         1 : double
 *         2 : complex
 *         3 : int32
 *         4 : int16
 *         5 : int8
 *         6 : uint32
 *         7 : uint16
 *         8 : uint8
 *         else : double
 *         can be all int type or double matrix
 *         can have n,1 or 1,n size
 *
 * lhs 1 : a list of size n
 *
 * 02/07/06, Alan : Initial version.
 * 21/11/06, Alan : Allow void rhs input param.
 *
 */

int intbuildouttb(fname,fname_len)
 char *fname;
 unsigned long fname_len;
{
 static int l1, m1,   n1;
 static int l2, m2,   n2;
 static int l3, n3=1;
 SciIntMat M1,M2,M3;

 int n_lnksz,n_lnktyp;
 int *lnksz=NULL,*lnktyp=NULL;

 double *ptr_d,*ptr_dc;
 int *ptr_i;
 short *ptr_s;
 char *ptr_c;
 int *ptr_ui;
 short *ptr_us;
 char *ptr_uc;

 int nm,i,j,ierr=0;

 static int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;

 /*check number of lhs/rhs*/
 CheckLhs(minlhs,maxlhs);
 CheckRhs(minrhs,maxrhs);

 /*check type of Rhs 1*/
 if (VarType(1)==1)
 {
  GetRhsVar(1, "d", &m1, &n1, &l1);
 }
 else if (VarType(1)==8)
 {
  GetRhsVar(1, "I", &m1, &n1, &M1);
 }
 else
 {
  Scierror(888,"%s : lnksz argument must be double or integer.\n",fname);
  return 0;
 }

 /*check type of Rhs 2*/
 if (VarType(2)==1)
 {
  GetRhsVar(2, "d", &m2, &n2, &l2);
 }
 else if (VarType(2)==8)
 {
  GetRhsVar(2, "I", &m2, &n2, &M2);
 }
 else
 {
  Scierror(888,"%s : lnktyp argument must be double or integer.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
  return 0;
 }

 /*check size of Rhs 1*/
 if (m1==2) n_lnksz=n1;
 else if (n1==2) n_lnksz=m1;
 /*void double input give void list output*/
 else if ((n1==0)||(m1==0))
 {
  if ((n2==0)||(m2==0))
  {
    /*manually code a void list on rhs(1)*/
    ptr_i=(int*)GetData(1);
    ptr_i[0]=15;
    ptr_i[1]=0;
    ptr_i[2]=1;
    LhsVar(1)=1;
    return 0;
  }
  else
  {
   Scierror(888,"%s : inconsistent dimension between lnksz and lnktyp.\n",fname);
   return 0;
  }
 }
 else
 {
  Scierror(888,"%s : bad dimension for lnksz argument.\n",fname);
  return 0;
 }

 /*check size of Rhs 2*/
 if (m2==1) n_lnktyp=n2;
 else if (n2==1) n_lnktyp=m2;
 else if ((n2==0)||(m2==0))
 {
  if ((n1!=0)&&(m1!=0))
  {
   Scierror(888,"%s : inconsistent dimension between lnksz and lnktyp.\n",fname);
   return 0;
  }
 }
 else
 {
  Scierror(888,"%s : bad dimension for lnktyp argument.\n",fname);
  return 0;
 }

 /*cross size checking*/
 if (n_lnksz!=n_lnktyp)
 {
  Scierror(888,"%s : lnksz and lnktyp argument must have "
               "the same length.\n",fname);
  return 0;
 }

 /*allocate lnksz*/
 if ((lnksz=MALLOC(2*n_lnksz*sizeof(int)))==NULL)
 {
  Scierror(999,"%s : No more free memory.\n",fname);
  return 0;
 }

 /* Allocate lnktyp*/
 if ((lnktyp=MALLOC(n_lnktyp*sizeof(int)))==NULL)
 {
  Scierror(999,"%s : No more free memory.\n",fname);
  if (lnksz!=NULL) FREE(lnksz);
  return 0;
 }

 /*store rhs 1 in lnksz */
 if ((m1==n1)&&(m2==m1)) m1=-1; /* this is done for matrix 2,2 */
 if (VarType(1)==8)
 {
  switch (M1.it)
  {
   case I_CHAR   : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_CHAR(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_CHAR(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_CHAR(M1.D)[j];
                   }
                   break;

   case I_INT16  : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_INT16(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_INT16(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_INT16(M1.D)[j];
                   }
                   break;

   case I_INT32  : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_INT32(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_INT32(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_INT32(M1.D)[j];
                   }
                   break;

   case I_UCHAR  : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_UCHAR(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_UCHAR(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_UCHAR(M1.D)[j];
                   }
                   break;

   case I_UINT16 : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_UINT16(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_UINT16(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_UINT16(M1.D)[j];
                   }
                   break;

   case I_UINT32 : if (m1==2)
                   {
                    for(j=0;j<n_lnksz;j++)
                    {
                     lnksz[j]=(int) IC_UINT32(M1.D)[j*2];
                     lnksz[n_lnksz+j]=(int) IC_UINT32(M1.D)[2*j+1];
                    }
                   }
                   else
                   {
                    for(j=0;j<2*n_lnksz;j++)
                      lnksz[j]=(int) IC_UINT32(M1.D)[j];
                   }
                   break;
  }
 }
 else
 {
  if (m1==2)
  {
   for(j=0;j<n_lnksz;j++)
   {
    lnksz[j]=(int) ((double *) stk(l1))[j*2];
    lnksz[n_lnksz+j]=(int) ((double *) stk(l1))[2*j+1];
   }
  }
  else
  {
   for(j=0;j<2*n_lnksz;j++)
     lnksz[j]=(int) ((double *) stk(l1))[j];
  }
 }

 /*store rhs 2 in lnktyp */
 if (VarType(2)==8)
 {
  switch (M2.it)
  {
   case I_CHAR   : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_CHAR(M2.D)[j];
                   break;

   case I_INT16  : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_INT16(M2.D)[j];
                   break;

   case I_INT32  : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_INT32(M2.D)[j];
                   break;

   case I_UCHAR  : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_UCHAR(M2.D)[j];
                   break;

   case I_UINT16 : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_UINT16(M2.D)[j];
                   break;

   case I_UINT32 : for(j=0;j<n_lnktyp;j++)
                     lnktyp[j]=(int) IC_UINT32(M2.D)[j];
                   break;
  }
 }
 else
 {
  for(j=0;j<n_lnktyp;j++)
    lnktyp[j]=(int) ((double *) stk(l2))[j];
 }

 /* build output list */
 CreateVar(3,"l",&n_lnktyp,&n3,&l3);

 for(i=0;i<n_lnktyp;i++)
 {
  nm=lnksz[i]*lnksz[i+n_lnktyp];
  switch (lnktyp[i])
  {
   case 1  : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_d[j]=0;
             CreateListVarFromPtr(3,i+1,"d",&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
             FREE(ptr_d);
             break;

   case 2  : if ((ptr_d=MALLOC(2*nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<2*nm;j++) ptr_d[j]=0;
             ptr_dc = &ptr_d[nm];
             CreateListCVarFromPtr(3,i+1,"d",(j=1,&j),&lnksz[i],&lnksz[i+n_lnktyp],&ptr_d,&ptr_dc);
             FREE(ptr_d);
             break;

   case 3  : if ((ptr_i=MALLOC(nm*sizeof(int)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_i[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 4;
             M3.l = -1;
             M3.D = ptr_i;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_i);
             break;

   case 4  : if ((ptr_s=MALLOC(nm*sizeof(short)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_s[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 2;
             M3.l = -1;
             M3.D = ptr_s;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_s);
             break;

   case 5  : if ((ptr_c=MALLOC(nm*sizeof(char)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_c[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 1;
             M3.l = -1;
             M3.D = ptr_c;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_c);
             break;

   case 6  : if ((ptr_ui=MALLOC(nm*sizeof(unsigned int)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_ui[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 14;
             M3.l = -1;
             M3.D = ptr_ui;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_ui);
             break;

   case 7  : if ((ptr_us=MALLOC(nm*sizeof(unsigned short)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_us[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 12;
             M3.l = -1;
             M3.D = ptr_us;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_us);
             break;

   case 8  : if ((ptr_uc=MALLOC(nm*sizeof(unsigned char)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_uc[j]=0;
             M3.m = lnksz[i];
             M3.n = lnksz[i+n_lnktyp];
             M3.it = 11;
             M3.l = -1;
             M3.D = ptr_uc;
             CreateListVarFromPtr(3,i+1,"I",&M3.m,&M3.n,&M3);
             FREE(ptr_uc);
             break;

   default : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
             {
              ierr=-1;
              break;
             }
             for (j=0;j<nm;j++) ptr_d[j]=0;
             CreateListVarFromPtr(3,i+1,"d",&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
             FREE(ptr_d);
             break;
  }

  if (ierr==-1)
  {
   Scierror(999,"%s : No more free memory.\n",fname);
   FREE(lnksz);
   FREE(lnktyp);
   return 0;
  }
 }

 LhsVar(1)=3;

 FREE(lnksz);
 FREE(lnktyp);
 return 0;
}

/* permutobj : concurrent version
 * of swap_handles for scicos editor.
 *
 * Copyright INRIA
 * A. Layec/S. Mannori - 13/12/06
 */
#include "../interf/intcommongraphics.h"
#include "scoMisc.h"

/* intpermutobj_c
 *
 * -->permutobj(hdl1,hdl2);
 *
 * exchange graphicals objects
 * of two children handles
 *
 * hdl1,hdl2 : valid children handles
 *             in valid parent handle(s)
 *
 * no outputs
 *
 * Alan-02/12/06, initial rev.
 */

int intpermutobj_c(char *fname,
                   unsigned long fname_len)
{
 /*local variables declaration*/
 unsigned long hdl_1, hdl_2;
 sciPointObj *pobj_1, *pparent_1;
 sciPointObj *pobj_2,* pparent_2;
 static int m1, n1, l1;
 static int m2, n2, l2;

 /*check number of rhs parameters*/
 CheckRhs(2,2);

 /*get/check rhs 1*/
 GetRhsVar(1,"h",&m1,&n1,&l1);
 if (m1!=1||n1!=1) {
  Scierror(999,"%s : Bad size "
               "for Rhs(1).\n",fname);
  return 0; }
 hdl_1=(unsigned long)*hstk(l1);
 pobj_1=sciGetPointerFromHandle(hdl_1);
 if (pobj_1==NULL) {
  Scierror(999,"%s :the handle is not "
               "or no more valid.\n",fname);
  return 0; }
 pparent_1=sciGetParentSubwin(pobj_1);

 /*get/check rhs 2*/
 GetRhsVar(2,"h",&m2,&n2,&l2);
 if (m2!=1||n2!=1) {
  Scierror(999,"%s : Bad size "
               "for Rhs(2).\n",fname);
  return 0; }
 hdl_2=(unsigned long)*hstk(l2);
 pobj_2=sciGetPointerFromHandle(hdl_2);
 if (pobj_2==NULL) {
  Scierror(999,"%s :the handle is not "
               "or no more valid.\n",fname);
  return 0; }
 pparent_2=sciGetParentSubwin(pobj_2);

 /*call permutobj*/
 permutobj(pobj_1,pparent_1,pobj_2,pparent_2);

 /* end */
 return 0;
}
