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


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "expm.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_expm(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expmected.\n"), "expm", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expmected.\n"), "expm", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_expm";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->getDims() > 2)
    {
        std::wstring wstFuncName = L"%hm_expm";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    if (pDblIn->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (pDblIn->getCols() != pDblIn->getRows())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A square matrix expected.\n"), "expm", 1);
        return types::Function::Error;
    }

    pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

    if (pDblIn->isComplex())
    {
        zexpms2(pDblIn->get(), pDblIn->getImg(), pDblOut->get(), pDblOut->getImg(), pDblIn->getCols());
    }
    else
    {
        dexpms2(pDblIn->get(), pDblOut->get(), pDblIn->getCols());
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
