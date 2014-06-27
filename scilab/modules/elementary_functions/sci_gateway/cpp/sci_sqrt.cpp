/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "api_scilab.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "basic_functions.h"
#include "sqrt.h"
}

template<class T> inline void TypeToDouble(T *_pL, types::Double* pDblOut);
bool isNegative(types::Double* _pL);

types::Function::ReturnValue sci_sqrt(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn  = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument: %d expected.\n"), "sqrt", 1);
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
    else if (in[0]->isInt()) // int
    {
        types::GenericType* pGTIn = in[0]->getAs<types::GenericType>();
        pDblIn = new types::Double(pGTIn->getDims(), pGTIn->getDimsArray());
        switch(in[0]->getType())
        {
        case types::InternalType::ScilabInt8 :
            TypeToDouble(in[0]->getAs<types::Int8>()->get(), pDblIn); break;
        case types::InternalType::ScilabInt16 :
            TypeToDouble(in[0]->getAs<types::Int16>()->get(), pDblIn); break;
        case types::InternalType::ScilabInt32 :
            TypeToDouble(in[0]->getAs<types::Int32>()->get(), pDblIn); break;
        case types::InternalType::ScilabInt64 :
            TypeToDouble(in[0]->getAs<types::Int64>()->get(), pDblIn); break;
        case types::InternalType::ScilabUInt8 :
            TypeToDouble(in[0]->getAs<types::UInt8>()->get(), pDblIn); break;
        case types::InternalType::ScilabUInt16 :
            TypeToDouble(in[0]->getAs<types::UInt16>()->get(), pDblIn); break;
        case types::InternalType::ScilabUInt32 :
            TypeToDouble(in[0]->getAs<types::UInt32>()->get(), pDblIn); break;
        case types::InternalType::ScilabUInt64 :
            TypeToDouble(in[0]->getAs<types::UInt64>()->get(), pDblIn); break;
        default:{} // never occurred
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_sqrt";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    /***** perform operation *****/
    if (pDblIn->isComplex() || isNegative(pDblIn))
    {
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), true);
        double* pdblImgRet = pDblOut->getImg();
        double* pdblRealRet = pDblOut->getReal();
        if (pDblIn->isComplex())
        {
            for (int i = 0 ; i < pDblOut->getSize() ; i++)
            {
                zsqrts(pDblIn->get(i), pDblIn->getImg(i), &pdblRealRet[i], &pdblImgRet[i]);
            }
        }
        else
        {
            for (int i = 0 ; i < pDblOut->getSize() ; i++)
            {
                zsqrts(pDblIn->get(i), 0, &pdblRealRet[i], &pdblImgRet[i]);
            }
        }
    }
    else
    {
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
        for (int i = 0; i < pDblOut->getSize(); i++)
        {
            pDblOut->set(i, dsqrts(pDblIn->get(i)));
        }
    }

    if (in[0]->isDouble() == false)
    {
        delete pDblIn;
    }

    /***** return data *****/
    out.push_back(pDblOut);
    return types::Function::OK;
}

template<class T> inline void TypeToDouble(T* _pL, types::Double* pDblOut)
{
    double* pdblOut = pDblOut->get();
    for (int i = 0; i < pDblOut->getSize(); i++)
    {
        pdblOut[i] = static_cast<double>(_pL[i]);
    }
}

bool isNegative(types::Double* _pL)
{
    double* pdblRealRet = _pL->getReal();
    double* pdblImgRet  = NULL;
    if(_pL->isComplex())
    {
        pdblImgRet = _pL->getImg();
        for (int i = 0; i < _pL->getSize(); i++)
        {
            if (pdblRealRet[i] < 0 || pdblImgRet[i] < 0)
            {
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < _pL->getSize(); i++)
        {
            if (pdblRealRet[i] < 0)
            {
                return true;
            }
        }
    }

    return false;
}
/*--------------------------------------------------------------------------*/
