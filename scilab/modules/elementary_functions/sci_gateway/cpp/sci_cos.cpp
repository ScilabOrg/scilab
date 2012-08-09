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
#include "elem_common.h"
#include "cos.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_cos(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;

    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"cos", 1);
        return types::Function::Error;
    }

    if(_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"cos", 1);
        return types::Function::Error;
    }

    if(in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

        if(pDblIn->isComplex())
        {
            for(int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                zcoss(pDblIn->get(i), pDblIn->getImg(i), pDblOut->get()+i, pDblOut->getImg()+i);
            }
        }
        else
        {
            for(int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                pDblOut->set(i, dcoss(pDblIn->get(i)));
            }
        }

        out.push_back(pDblOut);
    }
    else if(in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        types::Double* pDblOut = new types::Double(pSparseIn->getRows(), pSparseIn->getCols(), pSparseIn->isComplex());

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        double* pRows = new double[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        double* pCols = pRows + nonZeros;

        double* pNonZeroR = new double[nonZeros];
        double* pNonZeroI = new double[nonZeros];
        pSparseIn->outputValues(pNonZeroR, pNonZeroI);

        int iSize = pSparseIn->getSize();
        int iOne    = 1;
        double dOne = 1;
        int iZero   = 0;

        C2F(dcopy)(&iSize, &dOne, &iZero, pDblOut->get(), &iOne);

        if(pSparseIn->isComplex())
        {
            for(int i = 0 ; i < nonZeros ; i++)
            {
                int iPos = (pCols[i] - 1) * pSparseIn->getRows() + (pRows[i] - 1);
                zcoss(pNonZeroR[i], pNonZeroI[i], pDblOut->get()+iPos, pDblOut->getImg()+iPos);
            }
        }
        else
        {
            for(int i = 0 ; i < nonZeros ; i++)
            {
                int iPos = (pCols[i] - 1) * pSparseIn->getRows() + (pRows[i] - 1);
                pDblOut->set(iPos, dcoss(pNonZeroR[i]));
            }
        }

        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;

        out.push_back(pDblOut);
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_cos";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
