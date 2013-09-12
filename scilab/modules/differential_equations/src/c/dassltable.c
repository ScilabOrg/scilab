/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "GetFunctionByName.h"
#include "machine.h"
#include "dynlib_differential_equations.h"
/***********************************
* Search Table for dassl
***********************************/

#define ARGS_fresd double *,double*,double*,double*,int*,double*,int*
typedef int * (*fresdf)(ARGS_fresd);

#define ARGS_fjacd double *,double*,double*,double*,double*,double*,int*
typedef int * (*fjacdf)(ARGS_fjacd);

/***********************************
* Search Table for dasrt
***********************************/

#define ARGS_fsurfd int*,double *,double*,int*,double*,double*,int*
typedef int * (*fsurfdf)(ARGS_fsurfd);


#define ARGS_fsurf int*,double *,double*,int*,double*
typedef int * (*fsurff)(ARGS_fsurf);

/***********************************
* Search Table for daskr
***********************************/

#define ARGS_fpsold int*,double*,double*,double*,double*,double*,double*,double*,double*,int*,double*,double*,int*,double*,int*
typedef int * (*fpsoldf)(ARGS_fpsold);

#define ARGS_fpjacd double*,int*,int*,double*,double*,double*,double*,double*,double*,double*,double*,double*,int*,int*,double*,int*
typedef int * (*fpjacdf)(ARGS_fpjacd);

/**************** fresd ***************/
extern void C2F(dres1)(ARGS_fresd);
extern void C2F(dres2)(ARGS_fresd);
extern void C2F(res1)(ARGS_fresd);
extern void C2F(res2)(ARGS_fresd);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fresd)(ARGS_fresd);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfresd)(char *name, int *rep);

FTAB FTab_fresd[] =
{
    {"dres1", (voidf)  C2F(dres1)},
    {"dres2", (voidf)  C2F(dres2)},
    {"res1", (voidf)  C2F(res1)},
    {"res2", (voidf)  C2F(res2)},
    {(char *) 0, (voidf) 0}
};

/**************** fjacd ***************/
extern void C2F(djac1)(ARGS_fjacd);
extern void C2F(djac2)(ARGS_fjacd);
extern void C2F(jac2)(ARGS_fjacd);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fjacd)(ARGS_fjacd);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfjacd)(char *name, int *rep);

FTAB FTab_fjacd[] =
{
    {"djac1", (voidf)  C2F(djac1)},
    {"djac2", (voidf)  C2F(djac2)},
    {"jac2", (voidf)  C2F(jac2)},
    {(char *) 0, (voidf) 0}
};


/**************** fsurf ***************/
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fsurf)(ARGS_fsurf);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfsurf)(char *name, int *rep);

FTAB FTab_fsurf[] =
{
    {(char *) 0, (voidf) 0}
};
/**************** fsurfd ***************/
extern void C2F(gr1)(ARGS_fsurfd);
extern void C2F(gr2)(ARGS_fsurfd);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fsurfd)(ARGS_fsurfd);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfsurfd)(char *name, int *rep);

FTAB FTab_fsurfd[] =
{
    {"gr1", (voidf)  C2F(gr1)},
    {"gr2", (voidf)  C2F(gr2)},
    {(char *) 0, (voidf) 0}
};

/**************** fpsold ***************/
extern void C2F(psol1)(ARGS_fpsold);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fpsold)(ARGS_fpsold);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfpsold)(char *name, int *rep);

FTAB FTab_fpsold[] =
{
    {"psol1", (voidf)  C2F(psol1)},
    {(char *) 0, (voidf) 0}
};

/**************** fpjacd ***************/
extern void C2F(pjac1)(ARGS_fpjacd);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fpjacd)(ARGS_fpjacd);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfpjacd)(char *name, int *rep);

FTAB FTab_fpjacd[] =
{
    {"pjac1", (voidf)  C2F(pjac1)},
    {(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for dassl or dassrt
***********************************/

/** the current function fixed by setfresd **/

static fresdf fresdfonc ;

/** function call **/

void C2F(fresd)(double *t, double *y, double *ydot, double *res, int *ires, double *rpar, int *ipar)
{
    (*fresdfonc)(t, y, ydot, res, ires, rpar, ipar);
}

/** fixes the function associated to name **/

void C2F(setfresd)(char *name, int *rep)
{
    fresdfonc = (fresdf) GetFunctionByName(name, rep, FTab_fresd);
}


/** the current function fixed by setfjacd **/

static fjacdf fjacdfonc ;

/** function call **/

void C2F(fjacd)(double *t, double *y, double *ydot, double *pd, double *cj, double *rpar, int *ipar)
{
    (*fjacdfonc)(t, y, ydot, pd, cj, rpar, ipar);
}

/** fixes the function associated to name **/

void C2F(setfjacd)(char *name, int *rep)
{
    fjacdfonc = (fjacdf) GetFunctionByName(name, rep, FTab_fjacd);
}


/** the current function fixed by setfsurfd **/

static fsurfdf fsurfdfonc ;

/** function call **/


void C2F(fsurfd)(int *neq, double *t, double *y, int *ng, double *gout, double *rpar, int *ipar)
{
    (*fsurfdfonc)(neq, t, y, ng, gout, rpar, ipar);
}

/** fixes the function associated to name **/

void C2F(setfsurfd)(char *name, int *rep)
{
    fsurfdfonc = (fsurfdf) GetFunctionByName(name, rep, FTab_fsurfd);
}

/***********************************
* Search Table for dasrt ???
**********************************/

/** the current function fixed by setfsurf **/

static fsurff fsurffonc ;

/** function call **/

void C2F(fsurf)(int *ny, double *t, double *y, int *ng, double *gout)
{
    (*fsurffonc)(ny, t, y, ng, gout) ;
}

/** fixes the function associated to name **/

void C2F(setfsurf)(char *name, int *rep)
{
    fsurffonc = (fsurff) GetFunctionByName(name, rep, FTab_fsurf);
}


/** the current function fixed by setfpsold **/

static fpsoldf fpsoldfonc ;

/** function call **/

void C2F(fpsold)(int*neq, double*t, double*y, double*ydot, double*savr, double*wk, double*cj, double*wght, double*wp, int*iwp, double*b, double*eplin, int*ier, double*rpar, int*ipar)
{
    (*fpsoldfonc)(neq, t, y, ydot, savr, wk, cj, wght, wp, iwp, b, eplin, ier, rpar, ipar);
}

/** fixes the function associated to name **/

void C2F(setfpsold)(char *name, int *rep)
{
    fpsoldfonc = (fpsoldf) GetFunctionByName(name, rep, FTab_fpsold);
}


/** the current function fixed by setfpjacd **/

static fpjacdf fpjacdfonc ;

/** function call **/

void C2F(fpjacd)(double*res, int*ires, int*neq, double*t, double*y, double*ydot, double*rewt, double*savr, double*wk, double*h, double*cj, double*wp, int*iwp, int*ier, double*rpar, int*ipar)
{
    (*fpjacdfonc)(res, ires, neq, t, y, ydot, rewt, savr, wk, h, cj, wp, iwp, ier, rpar, ipar);
}

/** fixes the function associated to name **/

void C2F(setfpjacd)(char *name, int *rep)
{
    fpjacdfonc = (fpjacdf) GetFunctionByName(name, rep, FTab_fpjacd);
}
