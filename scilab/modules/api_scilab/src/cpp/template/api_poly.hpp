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
#include "polynom.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createPolyMatrix)(scilabEnv env, wchar_t* varname, int dim, int* dims, int complex)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createPolyMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createPolyMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Polynom* p = new types::Polynom(varname, dim, dims);
    if (p == nullptr)
    {
        scilab_setInternalError(env, L"createPolyMatrix", _W("memory allocation error"));
        return nullptr;
    }
    
    p->setComplex(complex != 0);
    return (scilabVar)p;
}

scilabVar API_PROTO(createPolyMatrix2d)(scilabEnv env, wchar_t* varname, int row, int col, int complex)
{
    int dims[2] = {row, col};
    return scilab_createPolyMatrix(env, varname, 2, dims, complex);
}

int API_PROTO(getPolyArray)(scilabEnv env, scilabVar var, int index, double** real)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false)
    {
        scilab_setInternalError(env, L"getPolyArray", _W("var must be a polynomial variable"));
        return API_ERROR;
    }
#endif
    types::SinglePoly* sp = p->get(index);
    *real = sp->get();
    return sp->getRank();
}

int API_PROTO(getComplexPolyArray)(scilabEnv env, scilabVar var, int index, double** real, double** img)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false || p->isComplex() == false)
    {
        scilab_setInternalError(env, L"getComplexPolyArray", _W("var must be a polynomial variable"));
        return API_ERROR;
    }
#endif
    types::SinglePoly* sp = p->get(index);
    *real = sp->get();
    *img = sp->getImg();
    return sp->getRank();
}

api_return API_PROTO(setPolyArray)(scilabEnv env, scilabVar var, int index, int rank, double* real)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false)
    {
        scilab_setInternalError(env, L"setPolyArray", _W("var must be a polynomial variable"));
        return API_ERROR;
    }
#endif

    if (index < 0 || index >= p->getSize())
    {
#ifdef __API_SCILAB_SAFE__
        scilab_setInternalError(env, L"setPolyArray", _W("index out of bounds"));
#endif
        return API_ERROR;
    }
    types::SinglePoly* sp = p->get(index);
    if (sp)
    {
        sp->setRank(rank);
        sp->set(real);
    }
    else
    {
        sp = new types::SinglePoly();
        sp->setRank(rank);
        sp->set(real);
        p->set(index, sp);
    }

    return API_OK;
}

api_return API_PROTO(setComplexPolyArray)(scilabEnv env, scilabVar var, int index, int rank, double* real, double* img)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false || p->isComplex() == false)
    {
        scilab_setInternalError(env, L"setComplexPolyArray", _W("var must be a polynomial complex variable"));
        return API_ERROR;
    }
#endif

    if (index < 0 || index >= p->getSize())
    {
#ifdef __API_SCILAB_SAFE__
        scilab_setInternalError(env, L"setComplexPolyArray", _W("index out of bounds"));
#endif
        return API_ERROR;
    }

    types::SinglePoly* sp = p->get(index);
    if (sp)
    {
        sp->setRank(rank);
        sp->set(real);
        sp->setImg(img);
    }
    else
    {
        sp = new types::SinglePoly();
        sp->setRank(rank);
        sp->set(real);
        sp->setImg(img);
        p->set(index, sp);
    }

    return API_OK;
}
