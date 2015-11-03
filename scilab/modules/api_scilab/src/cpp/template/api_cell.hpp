/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include "gatewaystruct.hxx"
#include "cell.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createCell)(scilabEnv env, int dim, int* dims)
{
    types::Cell* c = new types::Cell(dim, dims);
    return (scilabVar)c;
}

scilabVar API_PROTO(createCell2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
    types::Cell* c = new types::Cell(2, dims);
    return (scilabVar)c;
}

int API_PROTO(getCellArray)(scilabEnv env, scilabVar var, scilabVar** vals)
{
    types::Cell* c = (types::Cell*)var;
    *vals = (scilabVar*)c->get();
    return 0;
}

int API_PROTO(setCellArray)(scilabEnv env, scilabVar var, scilabVar* vals)
{
    types::Cell* c = (types::Cell*)var;
    c->set((types::InternalType**)vals);
    return 0;
}
