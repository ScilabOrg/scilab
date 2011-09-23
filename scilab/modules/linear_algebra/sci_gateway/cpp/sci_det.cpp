/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "det.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_det(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl             = NULL;
    types::Double* pDblMantissa     = NULL;
    types::Double* pDblExponent     = NULL;
    double* pData                   = NULL;

    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"det", 1);
        return types::Function::Error;
    }

    if(_iRetCount > 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"det", 1, 2);
        return types::Function::Error;
    }

    if((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_det";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDbl = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if(pDbl->isComplex())
    {
        pData = (double *)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        if(!pData)
        {
            ScierrorW(999,_W("%ls: Cannot allocate more memory.\n"),L"det");
            return types::Function::Error;
        }
    }
    else
    {
        pData = pDbl->getReal();
    }

    if(pDbl->getRows() != pDbl->getCols())
    {
        ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"det", 1);
        return types::Function::Error;
    }

    if((pDbl->getRows() == -1)) // manage eye case
    {
        ScierrorW(271,_W("%ls: Size varying argument a*eye(), (arg %d) not allowed here.\n"), L"det", 1);
        return types::Function::Error;
    }

    if(pDbl->getCols() == -1)
    {
        types::Double* pDblEyeMatrix = new types::Double(-1, -1);
        out.push_back(pDblEyeMatrix);
        return types::Function::Error;
    }

    if(pDbl->isComplex())
    {
        pData = (double *)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        if(!pData)
        {
            ScierrorW(999,_W("%ls: Cannot allocate more memory.\n"),L"det");
            return types::Function::Error;
        }
    }
    else
    {
        pData = pDbl->getReal();
    }

    pDblMantissa = new types::Double(1, 1, pDbl->isComplex());

    if(_iRetCount == 2)
    {
        pDblExponent = new types::Double(1,1);
    }

    int iExponent = 0;
    int iRet= iDetM(pData, pDbl->getCols(), pDblMantissa->getReal(), pDbl->isComplex() ? pDblMantissa->getImg() : NULL, pDblExponent ? &iExponent : NULL);
    if(iRet != 0)
    {
	    ScierrorW(999, _W("%ls: LAPACK error n°%d.\n"), L"det",iRet);
        return types::Function::Error;
    }

    if(pDblExponent)
    {
        pDblExponent->set(0, iExponent);
    }

    if(pDbl->isComplex())
    {
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }

    if(_iRetCount == 2)
    {
        out.push_back(pDblExponent);
    }

    out.push_back(pDblMantissa);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

