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
#include "graphichandle.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createHandleMatrix)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar) new types::GraphicHandle(dim, dims);
}

scilabVar API_PROTO(createHandleMatrix2d)(scilabEnv env, int row, int col)
{
    return (scilabVar) new types::GraphicHandle(row, col);
}

scilabVar API_PROTO(createHandle)(scilabEnv env)
{
    return (scilabVar) new types::GraphicHandle(-1);
}

int API_PROTO(getHandleArray)(scilabEnv env, scilabVar var, long long** vals)
{
    *vals = ((types::GraphicHandle*)var)->get();
    return 1;
}

int API_PROTO(setHandleArray)(scilabEnv env, scilabVar var, long long* vals)
{
    ((types::GraphicHandle*)var)->set(vals);
    return 1;
}
