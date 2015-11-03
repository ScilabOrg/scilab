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
#include "bool.hxx"

extern "C"
{
#include "api_scilab.h"
}

/* Scilab 6 API*/
scilabVar API_PROTO(createBooleanMatrix)(scilabEnv env, int dim, int* dims)
{
    types::Bool* b = new types::Bool(dim, dims);
    return (scilabVar)b;
}

scilabVar API_PROTO(createBooleanMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
    types::Bool* b = new types::Bool(2, dims);
    return (scilabVar)b;
}

scilabVar API_PROTO(createBoolean)(scilabEnv env, int val)
{
    types::Bool* b = new types::Bool(val);
    return (scilabVar)b;
}

api_return API_PROTO(getBooleanArray)(scilabEnv env, scilabVar var, int** vals)
{
    types::Bool* b = (types::Bool*)var;
    *vals = b->get();
    return API_OK;
}

api_return API_PROTO(setBooleanArray)(scilabEnv env, scilabVar var, int* vals)
{
    types::Bool* b = (types::Bool*)var;
    b->set(vals);
    return API_OK;
}
