/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "funcmanager.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_zeros(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    for(int i = 0 ; i < in.size() ; i++)
    {
        if(in[i]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix expected.\n"), L"zeros", i + 1);
            return Function::Error;
        }
    }

    Double* pOut = NULL;
    if(in.size() == 0)
    {
        pOut = new Double(0);
    }
    else if(in.size() == 1)
    {
        Double* pIn = in[0]->getAs<Double>();
        int iDims = pIn->get_dims();
        int* piDims = pIn->get_dims_array();

        pOut = new Double(iDims, piDims);
    }
    else //size > 1
    {
        int iDims = static_cast<int>(in.size());
        int* piDims = new int[iDims];
        for(int i = 0 ; i < iDims ; i++)
        {
            Double* pIn = in[i]->getAs<Double>();
            if(pIn->size_get() != 1 || pIn->isComplex())
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Real scalar expected.\n"), L"zeros", i + 1);
                return Function::Error;
            }
            piDims[i] = static_cast<int>(pIn->get_real()[0]);
        }
        pOut = new Double(iDims, piDims);
        delete[] piDims;
    }

    pOut->zero_set();
    //for(int i = 0 ; i < pOut->size_get() ; i++)
    //{
    //    pOut->set(i,i);
    //}
    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
