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
* Search Table for int3d
***********************************/

#define ARGS_fint3d double *,int*,double *
typedef void (*fint3df)(ARGS_fint3d);

/**************** fint3d ***************/
extern void C2F(int3dex)(ARGS_fint3d);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fint3d)(ARGS_fint3d);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfint3d)(char *name, int *rep);

FTAB FTab_fint3d[] =
{
    {"int3dex", (voidf)  C2F(int3dex)},
    {(char *) 0, (voidf) 0}
};


/***********************************
* Search Table for int3d
*    uses : fint3d
***********************************/

/** the current function fixed by setfint3d **/

static fint3df fint3dfonc ;

/** function call : WARNING fintg returns a double  **/

void C2F(fint3d)(double *xyz, int *numfun, double *v)
{
    (*fint3dfonc)(xyz, numfun, v);
}

/** fixes the function associated to name **/

void C2F(setfint3d)(char *name, int *rep)
{
    fint3dfonc = (fint3df) GetFunctionByName(name, rep, FTab_fint3d);
}
