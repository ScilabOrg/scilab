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
* Search Table for colnew
*   corr uses : fcoldg , fcolg, fcoldf,fcolf,fcolgu
***********************************/

#define ARGS_fcoldg int*,double *,double*
typedef void (*fcoldgf)(ARGS_fcoldg);

#define ARGS_fcolg int*,double *,double*
typedef void (*fcolgf)(ARGS_fcolg);

#define ARGS_fcoldf double *,double *,double*
typedef void (*fcoldff)(ARGS_fcoldf);

#define ARGS_fcolf double *,double *,double*
typedef void (*fcolff)(ARGS_fcolf);

#define ARGS_fcolgu double *,double *,double*
typedef void (*fcolguf)(ARGS_fcolgu);

/**************** fcoldg ***************/
extern void C2F(cndg)(ARGS_fcoldg);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fcoldg)(ARGS_fcoldg);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfcoldg)(char *name, int *rep);

FTAB FTab_fcoldg[] =
{
    {"cndg", (voidf)  C2F(cndg)},
    {(char *) 0, (voidf) 0}
};
/**************** fcolg ***************/
extern void C2F(cng)(ARGS_fcolg);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fcolg)(ARGS_fcolg);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfcolg)(char *name, int *rep);

FTAB FTab_fcolg[] =
{
    {"cng", (voidf)  C2F(cng)},
    {(char *) 0, (voidf) 0}
};
/**************** fcolf ***************/
extern void C2F(cnf)(ARGS_fcolf);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fcolf)(ARGS_fcolf);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfcolf)(char *name, int *rep);

FTAB FTab_fcolf[] =
{
    {"cnf", (voidf)  C2F(cnf)},
    {(char *) 0, (voidf) 0}
};
/**************** fcoldf ***************/
extern void C2F(cndf)(ARGS_fcoldf);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fcoldf)(ARGS_fcoldf);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfcoldf)(char *name, int *rep);

FTAB FTab_fcoldf[] =
{
    {"cndf", (voidf)  C2F(cndf)},
    {(char *) 0, (voidf) 0}
};
/**************** fcolgu ***************/
extern void C2F(cngu)(ARGS_fcolgu);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fcolgu)(ARGS_fcolgu);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfcolgu)(char *name, int *rep);

FTAB FTab_fcolgu[] =
{
    {"cngu", (voidf)  C2F(cngu)},
    {(char *) 0, (voidf) 0}
};


/***********************************
* Search Table for colnew
*   corr uses : fcoldg , fcolg, fcoldf,fcolf,fcolgu
***********************************/

/** the current function fixed by setfcoldg **/

static fcoldgf fcoldgfonc ;

/** function call **/

void C2F(fcoldg)(int *i, double *z, double *dg)
{
    (*fcoldgfonc)(i, z, dg);
}

/** fixes the function associated to name **/

void C2F(setfcoldg)(char *name, int *rep)
{
    fcoldgfonc = (fcoldgf) GetFunctionByName(name, rep, FTab_fcoldg);
}


/** the current function fixed by setfcolg **/

static fcolgf fcolgfonc ;

/** function call **/

void C2F(fcolg)(int *i, double *z, double *g)
{
    (*fcolgfonc)(i, z, g);
}

/** fixes the function associated to name **/

void C2F(setfcolg)(char *name, int *rep)
{
    fcolgfonc = (fcolgf) GetFunctionByName(name, rep, FTab_fcolg);
}



/** the current function fixed by setfcoldf **/

static fcoldff fcoldffonc ;

/** function call **/

void C2F(fcoldf)(double *x, double *z, double *df)
{
    (*fcoldffonc)(x, z, df);
}

/** fixes the function associated to name **/

void C2F(setfcoldf)(char *name, int *rep)
{
    fcoldffonc = (fcoldff) GetFunctionByName(name, rep, FTab_fcoldf);
}


/** the current function fixed by setfcolf **/

static fcolff fcolffonc ;

/** function call **/

void C2F(fcolf)(double *x, double *z, double *df)
{
    (*fcolffonc)(x, z, df);
}

/** fixes the function associated to name **/

void C2F(setfcolf)(char *name, int *rep)
{
    fcolffonc = (fcolff) GetFunctionByName(name, rep, FTab_fcolf);
}

/** the current function fixed by setfcolgu **/

static fcolguf fcolgufonc ;

/** function call **/

void C2F(fcolgu)(double *x, double *z, double *dmval)
{
    (*fcolgufonc)(x, z, dmval);
}

/** fixes the function associated to name **/

void C2F(setfcolgu)(char *name, int *rep)
{
    fcolgufonc = (fcolguf) GetFunctionByName(name, rep, FTab_fcolgu);
}
