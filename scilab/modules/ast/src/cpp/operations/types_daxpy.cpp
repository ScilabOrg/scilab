/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_optimized.hxx"
#include "operations.hxx"
#include "double.hxx"
#include "int.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include <stdio.h>
#include "core_math.h"
#include "matrix_addition.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_swprintf.h"
#include "elem_common.h" //dset
}

using namespace types;


//define arrays on operation functions
static daxpy_function pDAXPYfunction[types::InternalType::IdLast][types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillDAXPYFunction()
{
#define scilab_fill_daxpy(id1, id2, id3, func) \
    pDAXPYfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2][types::InternalType::Id ## id3] = (daxpy_function)&daxpy_##func

    //a * X + Y
    scilab_fill_daxpy(ScalarDouble, Double, Double, S_M_M);
    //A * x + Y
    scilab_fill_daxpy(Double, ScalarDouble, Double, M_S_M);
    //a * x + y
    scilab_fill_daxpy(ScalarDouble, ScalarDouble, ScalarDouble, S_S_S);
#undef scilab_fill_daxpy
}

//a(1,1) .* X(p,q) + Y(p,q)
InternalType* daxpy_S_M_M(Double* pA, Double* pX, types::Double* pY)
{
    //X and Y must have same dimensions
    if (pX->getRows() != pY->getRows() || pX->getCols() != pY->getCols())
    {
        //let normal way to throw exception
        return NULL;
    }

    int one = 1;
    int size = pX->getSize();

    Double* pOut = NULL;
    if (pY->isDeletable())
    {
        pOut = pY;
    }
    else
    {
        pOut = (Double*)pY->clone();
    }

    C2F(daxpy)(&size, pA->get(), pX->get(), &one, pOut->get(), &one);
    return pOut;
}

InternalType* daxpy_M_S_M(Double* pA, Double* pX, types::Double* pY)
{
    return daxpy_S_M_M(pX, pA, pY);
}

InternalType* daxpy_S_S_S(Double* pA, Double* pX, types::Double* pY)
{
    return new Double(pA->get(0) * pX->get(0) + pY->get(0));
}

InternalType *GenericDAXPY(InternalType *_pA, InternalType *_pX, InternalType *_pY)
{
    InternalType *pResult = NULL;

    daxpy_function daxpy = pDAXPYfunction[_pA->getId()][_pX->getId()][_pY->getId()];
    if (daxpy)
    {
        pResult = daxpy(_pA, _pX, _pY);
        if (pResult)
        {
            return pResult;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}

