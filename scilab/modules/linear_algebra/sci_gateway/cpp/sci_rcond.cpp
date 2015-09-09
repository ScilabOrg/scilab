/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "rcond.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_rcond(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl     = NULL;
    types::Double* result   = NULL;
    int iRet                = 0;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "rcond", 1);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_rcond";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if (pDbl->getRows() != pDbl->getCols())
    {
        Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "rcond", 1);
        return types::Function::Error;
    }

    if (pDbl->getRows() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else if (pDbl->getRows() == -1) // manage eye case
    {
        out.push_back(new types::Double(1));
        return types::Function::OK;
    }

    result = new types::Double(1, 1);

    if (pDbl->isComplex())
    {
        double* pData = (double*)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        iRet = iRcondM(pData, pDbl->getCols(), true /*isComplex*/, result->get());
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }
    else
    {
        iRet = iRcondM(pDbl->get(), pDbl->getCols(), false /*isComplex*/, result->get());
    }

    delete pDbl;
    pDbl = NULL;

    if (iRet == -1)
    {
        Scierror(999, _("%s: Allocation failed.\n"), "rcond");
        return types::Function::Error;
    }

    out.push_back(result);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

