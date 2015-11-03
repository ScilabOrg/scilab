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

scilabVar API_PROTO(createPoly)(scilabEnv env, wchar_t* varname, int dim, int* dims, int complex)
{
    types::Polynom* p = new types::Polynom(varname, dim, dims);
    p->setComplex(complex != 0);
    return (scilabVar)p;
}

scilabVar API_PROTO(createPoly2d)(scilabEnv env, wchar_t* varname, int row, int col, int complex)
{
    int dims[2] = {row, col};
    return scilab_createPoly(env, varname, 2, dims, complex);
}

int API_PROTO(getPoly)(scilabEnv env, scilabVar var, int index, double** real)
{
    types::Polynom* p = (types::Polynom*)var;
    types::SinglePoly* sp = p->get(index);
    *real = sp->get();
    return sp->getRank();
}

int API_PROTO(getComplexPoly)(scilabEnv env, scilabVar var, int index, double** real, double** img)
{
    types::Polynom* p = (types::Polynom*)var;
    types::SinglePoly* sp = p->get(index);
    *real = sp->get();
    *img = sp->getImg();
    return sp->getRank();
}

api_return API_PROTO(setPoly)(scilabEnv env, scilabVar var, int index, int rank, double* real)
{
    types::Polynom* p = (types::Polynom*)var;
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

api_return API_PROTO(setComplexPoly)(scilabEnv env, scilabVar var, int index, int rank, double* real, double* img)
{
    types::Polynom* p = (types::Polynom*)var;
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
