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
#include "basic_functions.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_conj(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyOut    = NULL;

    double dLeatOne = -1;
    int iOne        = 1;
    int iSize       = 0;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "conj", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "conj", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        pDblOut = in[0]->clone()->getAs<types::Double>();
    }
    else if (in[0]->isPoly())
    {
        pPolyOut = in[0]->clone()->getAs<types::Polynom>();
        pDblOut = pPolyOut->getCoef();
    }
    else
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_conj";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    iSize = pDblOut->getSize();
    if (pDblOut->isComplex())
    {
        C2F(dscal)(&iSize, &dLeatOne, pDblOut->getImg(), &iOne);
    }

    if (in[0]->isDouble())
    {
        out.push_back(pDblOut);
    }
    else if (in[0]->isPoly())
    {
        pPolyOut->setCoef(pDblOut);
        out.push_back(pPolyOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
