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
#include "execvisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "rcond.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_rcond(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl     = NULL;
    types::Double* result   = NULL;
    int iRet                = 0;

    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"rcond", 1);
        return types::Function::Error;
    }

    if((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_rcond";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDbl = in[0]->getAs<types::Double>();

    if(pDbl->getRows() != pDbl->getCols())
    {
		ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"rcond", 1);
        return types::Function::Error;            
    }

    result = new types::Double(1,1,pDbl->isComplex());

    if(pDbl->getRows() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else if(pDbl->getRows() == -1) // manage eye case
    {
        result->set(0,1);
        out.push_back(result);
        return types::Function::OK;
    }

    if(pDbl->isComplex())
    {
        double* pData = (double*)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        iRet = iRcondM(pData, pDbl->getCols(), true /*isComplex*/, result->get());
		vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }
    else
    {
        iRet = iRcondM(pDbl->get(), pDbl->getCols(), false /*isComplex*/, result->get());
    }

    if(iRet != 0)
    {
        if(iRet == -1)
        {
		    ScierrorW(999, _W("%ls: Allocation failed.\n"), L"chol");
        }
        else
        {
		    ScierrorW(999, _W("%ls: LAPACK error n°%d.\n"), L"chol",iRet);
        }
        return types::Function::Error;
    }

    out.push_back(result);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

