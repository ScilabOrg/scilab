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
#include "string.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createStringMatrix)(scilabEnv env, int dim, int* dims)
{
    types::String* s = new types::String(dim, dims);
    return (scilabVar)s;
}

scilabVar API_PROTO(createStringMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
    types::String* s = new types::String(2, dims);
    return (scilabVar)s;
}

scilabVar API_PROTO(createString)(scilabEnv env, const wchar_t* val)
{
    types::String* s = new types::String(val);
    return (scilabVar)s;
}

int API_PROTO(getStringArray)(scilabEnv env, scilabVar var, wchar_t*** strs)
{
    types::String* s = (types::String*)var;
    *strs = s->get();
    return 0;
}

int API_PROTO(setStringArray)(scilabEnv env, scilabVar var, const wchar_t* const* strs)
{
    types::String* s = (types::String*)var;
    s->set(strs);
    return 0;
}
