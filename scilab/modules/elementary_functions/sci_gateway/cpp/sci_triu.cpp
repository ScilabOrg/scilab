/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "int.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

template<class T, class U> types::InternalType* triu_const(T *_pL, U* _data, int iOffset);

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_triu(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iOffset = 0;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "triu", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "triu", 1);
        return types::Function::Error;
    }


    if (in[0]->isGenericType() == false)
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_triu";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    if (in[0]->getAs<types::GenericType>()->getDims() > 2)
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_triu";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "triu", 2);
            return types::Function::Error;
        }

        types::Double* pDblOffset = in[1]->getAs<types::Double>();

        if (pDblOffset->isScalar() == false || pDblOffset->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "triu", 2);
            return types::Function::Error;
        }

        iOffset = (int)pDblOffset->get(0);
    }

    /***** get data *****/
    if (in[0]->isDouble() || in[0]->isInt()) // double
    {
        types::InternalType* pOut = NULL;
        switch (in[0]->getType())
        {
            case types::InternalType::ScilabDouble:
                pOut = triu_const(in[0]->getAs<types::Double>(), in[0]->getAs<types::Double>()->get(), iOffset);
                break;
            case types::InternalType::ScilabInt8:
                pOut = triu_const(in[0]->getAs<types::Int8>(), in[0]->getAs<types::Int8>()->get(), iOffset);
                break;
            case types::InternalType::ScilabInt16:
                pOut = triu_const(in[0]->getAs<types::Int16>(), in[0]->getAs<types::Int16>()->get(), iOffset);
                break;
            case types::InternalType::ScilabInt32:
                pOut = triu_const(in[0]->getAs<types::Int32>(), in[0]->getAs<types::Int32>()->get(), iOffset);
                break;
            case types::InternalType::ScilabInt64:
                pOut = triu_const(in[0]->getAs<types::Int64>(), in[0]->getAs<types::Int64>()->get(), iOffset);
                break;
            case types::InternalType::ScilabUInt8:
                pOut = triu_const(in[0]->getAs<types::UInt8>(), in[0]->getAs<types::UInt8>()->get(), iOffset);
                break;
            case types::InternalType::ScilabUInt16:
                pOut = triu_const(in[0]->getAs<types::UInt16>(), in[0]->getAs<types::UInt16>()->get(), iOffset);
                break;
            case types::InternalType::ScilabUInt32:
                pOut = triu_const(in[0]->getAs<types::UInt32>(), in[0]->getAs<types::UInt32>()->get(), iOffset);
                break;
            case types::InternalType::ScilabUInt64:
                pOut = triu_const(in[0]->getAs<types::UInt64>(), in[0]->getAs<types::UInt64>()->get(), iOffset);
                break;
            default:
            {} // never occurred
        }
        out.push_back(pOut);
    }
    else if (in[0]->isPoly()) // polynom
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        int iRows = pPolyIn->getRows();
        int iCols = pPolyIn->getCols();
        int* piRanks = new int[iRows * iCols];
        memset(piRanks, 0x00, iRows * iCols * sizeof(int));
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), iRows, iCols, piRanks);
        delete[] piRanks;
        pPolyOut->setZeros();

        for (int i = 0 ; i < iCols ; i++)
        {
            int iSize = min(max(i + 1 - iOffset, 0), iRows);
            for (int j = 0; j < iSize; j++)
            {
                int iPos = i * iRows + j;
                pPolyOut->set(iPos, pPolyIn->get(iPos));
            }
        }

        out.push_back(pPolyOut);
    }
    else
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_triu";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

template<class T, class U> types::InternalType* triu_const(T *_pL, U* _data, int iOffset)
{
    int iCols = _pL->getCols();
    int iRows = _pL->getRows();
    U* pInReal = _data;

    T* pOut = new T(iRows, iCols);
    pOut->setComplex(_pL->isComplex());
    U* pOutReal = pOut->get();
    memset(pOutReal, 0x00, iRows * iCols * sizeof(U));

    if (_pL->isComplex())
    {
        U* pInImg = _pL->getImg();
        U* pOutImg = pOut->getImg();
        memset(pOutImg, 0x00, iRows * iCols * sizeof(U));
        for (int i = 0; i < iCols; i++)
        {
            int iSize = min(max(i + 1 - iOffset, 0), iRows);
            memcpy(&pOutReal[i * iRows], &pInReal[i * iRows], iSize * sizeof(U));
            memcpy(&pOutImg[i * iRows], &pInImg[i * iRows], iSize * sizeof(U));
        }
    }
    else
    {
        for (int i = 0; i < iCols; i++)
        {
            int iSize = min(max(i + 1 - iOffset, 0), iRows);
            memcpy(&pOutReal[i * iRows], &pInReal[i * iRows], iSize * sizeof(U));
        }
    }
    return pOut;
}