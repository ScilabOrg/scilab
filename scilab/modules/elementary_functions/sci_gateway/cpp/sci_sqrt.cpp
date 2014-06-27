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

template<class T> inline types::Double* TypeToDouble(T *_pL);
template<class T> inline bool isNegative(T *_pL);

types::Function::ReturnValue sci_sqrt(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn       = NULL;
    types::Double* pDblOut      = NULL;

    if (in.size() != 1 )
    {
        Scierror(999, _("%s: Wrong number of input argument: %d expected.\n"), "sqrt", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "sqrt", 1);
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
        pDblIn = TypeToDouble(in[0]->getAs<types::Int8>());
    }
    else if (in[0]->isInt16())
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::Int16>());
    }
    else if (in[0]->isInt32())
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::Int32>());
    }
    else if (in[0]->isInt64()) // uint
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::Int64>());
    }
    else if (in[0]->isUInt8())
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::UInt8>());
    }
    else if (in[0]->isUInt16())
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::UInt16>());
    }
    else if (in[0]->isUInt32())
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::UInt32>());
    }
    else if (in[0]->isUInt64())
    {
        pDblIn = TypeToDouble(in[0]->getAs<types::UInt64>());
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_sum";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    if(pDblIn->isComplex()|isNegative(pDblIn))
    {
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(),true);
        double* pdblImgRet = pDblOut->getImg();
        double* pdblRealRet = pDblOut->getReal();
            for (int i = 0 ; i < pDblOut->getSize() ; i++)
            {
                if(pDblIn->isComplex())
                    zsqrts(pDblIn->get(i), pDblIn->getImg(i), &pdblRealRet[i], &pdblImgRet[i]);
                else
                    zsqrts(pDblIn->get(i), 0, &pdblRealRet[i], &pdblImgRet[i]);
            }
    }
    else
    {
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
        for (int i = 0; i < pDblOut->getSize(); i++)
        { 
            pDblOut->set(i,dsqrts(pDblIn->get(i)));
        }
    }

    if(!in[0]->isDouble())
        delete pDblIn;

    out.push_back(pDblOut);
    return types::Function::OK;
}

template<class T> inline types::Double* TypeToDouble(T *_pL)
{
        types::Double* pDblOut = NULL; 
        T* pIIn = _pL->getAs<T>();
        pDblOut = new types::Double(pIIn->getDims(), pIIn->getDimsArray(),pIIn->isComplex());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblOut->set(i, static_cast<double>(pIIn->get(i)));
        }

        return pDblOut;
}

template<class T> inline bool isNegative(T *_pL)
{
    double* pdblRealRet = _pL->getReal();
    double* pdblImgRet  = NULL;
    if(_pL->isComplex())
        pdblImgRet = _pL->getImg();
    for (int i = 0; i < _pL->getSize(); i++)
    {
        if(pdblRealRet[i]<0)
            return true;
        if(pdblImgRet !=  NULL)
            if(pdblImgRet[i]<0)
            return true;
    }
    return false;
}
/*--------------------------------------------------------------------------*/
