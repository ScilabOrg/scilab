/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_degree(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblOut = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "degree", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "degree", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
        double* pdblOut = pDblOut->get();
        memset(pdblOut, 0x00, pDblOut->getSize() * sizeof(double));
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        pDblOut = new types::Double(pPolyIn->getDims(), pPolyIn->getDimsArray());
        double* pdblOut = pDblOut->get();

        for (int i = 0; i < pDblOut->getSize(); i++)
        {
            pdblOut[i] = (double)pPolyIn->get(i)->getRank();
        }
    }
    else
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_coeff";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

