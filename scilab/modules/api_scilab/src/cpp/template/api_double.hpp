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
#include "double.hxx"

extern "C"
{
#include "doublecomplex.h"
#include "api_scilab.h"
}
/* Scilab 6 API*/
scilabVar API_PROTO(createDoubleMatrix)(scilabEnv env, int dim, int* dims, int complex)
{
    types::Double* d = new types::Double(dim, dims, complex == 1);
    return (scilabVar)d;
}

scilabVar API_PROTO(createDoubleMatrix2d)(scilabEnv env, int row, int col, int complex)
{
    int dims[2] = {row, col};
    types::Double* d = new types::Double(2, dims, complex == 1);
    return (scilabVar)d;
}

scilabVar API_PROTO(createDouble)(scilabEnv env, double val)
{
    types::Double* d = new types::Double(val);
    return (scilabVar)d;
}

scilabVar API_PROTO(createDoubleComplex)(scilabEnv env, double real, double img)
{
    types::Double* d = new types::Double(real, img);
    return (scilabVar)d;
}

api_return API_PROTO(getDoubleArray)(scilabEnv env, scilabVar var, double** real)
{
    types::Double* d = (types::Double*)var;
    *real = d->get();
    return API_OK;
}

api_return API_PROTO(getDoubleComplexArray)(scilabEnv env, scilabVar var, double** real, double** img)
{
    types::Double* d = (types::Double*)var;
    *real = d->get();
    *img = d->getImg();
    return API_OK;
}

api_return API_PROTO(getDouble)(scilabEnv env, scilabVar var, double* real)
{
    types::Double* d = (types::Double*)var;
    *real = d->get()[0];
    return API_OK;
}

api_return API_PROTO(getDoubleComplex)(scilabEnv env, scilabVar var, double* real, double* img)
{
    types::Double* d = (types::Double*)var;
    *real = d->get()[0];
    *img = d->getImg()[0];
    return API_OK;
}

api_return API_PROTO(setDoubleArray)(scilabEnv env, scilabVar var, double* real)
{
    types::Double* d = (types::Double*)var;
    d->set(real);
    return API_OK;
}

api_return API_PROTO(setDoubleComplexArray)(scilabEnv env, scilabVar var, double* real, double* img)
{
    types::Double* d = (types::Double*)var;
    d->set(real);
    d->setImg(img);
    return API_OK;
}

api_return API_PROTO(setDouble)(scilabEnv env, scilabVar var, double real)
{
    types::Double* d = (types::Double*)var;
    d->get()[0] = real;
    return API_OK;
}

api_return API_PROTO(setDoubleComplex)(scilabEnv env, scilabVar var, double real, double img)
{
    types::Double* d = (types::Double*)var;
    d->get()[0] = real;
    d->getImg()[0] = img;
    return API_OK;
}

