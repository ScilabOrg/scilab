/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "types_kronecker.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_kron(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblA    = NULL;
    types::Double* pDblB    = NULL;
    types::Double* pDblOut  = NULL;

    if(in.size() != 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"kron", 2);
        return types::Function::Error;
    }

    if(_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"kron", 1);
        return types::Function::Error;
    }

    if(in[0]->isDouble() && in[1]->isDouble()) // double .*. double
    {
        pDblA = in[0]->getAs<types::Double>();
        pDblB = in[1]->getAs<types::Double>();

        KroneckerMultiplyDoubleByDouble(pDblA, pDblB, &pDblOut);
    }
    else if(in[0]->isInt() || in[1]->isInt())
    { // int .*. double || double .*. int || int .*. int
        std::wstring wstFuncName = L"%_kron";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_kron";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
