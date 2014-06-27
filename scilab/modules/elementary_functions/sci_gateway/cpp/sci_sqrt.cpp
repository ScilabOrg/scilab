/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
//#include <string.h>
#include "elem_func_gw.hxx"
//#include "api_scilab.h"

#include "function.hxx"
#include "api_scilab.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
    #include "basic_functions.h"
    #include "sqrt.h"
}


types::Function::ReturnValue sci_sqrt(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn       = NULL;
    types::Double* pDblOut      = NULL;
    double* pdbReal    = NULL;

    int iOrientation    = 0;
    int iOuttype        = 1; // 1 = native | 2 = double (type of output value)

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "sum", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "sum", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    if (in[0]->isDouble()) // double
    {
        pDblIn = in[0]->getAs<types::Double>();
    }
    else if (in[0]->isBool()) // bool
    {
        out.push_back(in[0]);
        return types::Function::OK;
    }
    else if (in[0]->isInt8()) // int
    {
        types::Int8* pIIn = in[0]->getAs<types::Int8>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isInt16())
    {
        types::Int16* pIIn = in[0]->getAs<types::Int16>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isInt32())
    {
        types::Int32* pIIn = in[0]->getAs<types::Int32>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isInt64()) // uint
    {
        types::Int64* pIIn = in[0]->getAs<types::Int64>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt8())
    {
        types::UInt8* pIIn = in[0]->getAs<types::UInt8>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16* pIIn = in[0]->getAs<types::UInt16>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt32())
    {
        types::UInt32* pIIn = in[0]->getAs<types::UInt32>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt64())
    {
        types::UInt64* pIIn = in[0]->getAs<types::UInt64>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_sum";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    pDblOut = pDblIn->clone()->getAs<types::Double>();
    if(pDblIn->isComplex())
    {
        double* pdblImgRet = new double[pDblOut->getSize()];
        double* pdblRealRet = new double[pDblOut->getSize()];
            for (int i = 0 ; i < pDblOut->getSize() ; i++)
            {
                zsqrts(pDblIn->get(i), pDblIn->getImg(i), &pdblRealRet[i], &pdblImgRet[i]);
            }
        pDblOut->set(pdblRealRet);
        pDblOut->setImg(pdblImgRet);
    }
    else
    {
        for (int i = 0; i < pDblOut->getSize(); i++)
        { 
            pDblOut->set(i,dsqrts(pDblIn->get(i)));
        }
    }

    
    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
