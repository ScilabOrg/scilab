/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "overload.hxx"
#include "sparse.hxx"
#include "polynom.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_real(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "real", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "real", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        if (pDblIn->isComplex() == false)
        {
            out.push_back(pDblIn);
            return types::Function::OK;
        }

        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
        pDblOut->set(pDblIn->get()) ;
        out.push_back(pDblOut);
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        if (pSparseIn->isComplex() == false)
        {
            out.push_back(pSparseIn);
            return types::Function::OK;
        }

        types::Sparse* pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols());
        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        int* pRows = new int[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        for (int i = 0 ; i < nonZeros ; i++)
        {
            double real = pSparseIn->getReal(pRows[i] - 1, pCols[i] - 1);
            pSparseOut->set(pRows[i] - 1, pCols[i] - 1, real, false);
        }

        pSparseOut->finalize();

        delete[] pRows;

        out.push_back(pSparseOut);
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        if (pPolyIn->isComplex() == false)
        {
            out.push_back(pPolyIn);
            return types::Function::OK;
        }

        int* piRanks = new int[pPolyIn->getSize()];
        pPolyIn->getRank(piRanks);

        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray(), piRanks);
        for (int i = 0; i < pPolyIn->getSize(); i++)
        {
            memcpy(pPolyOut->get(i)->get(), pPolyIn->get(i)->get(), (piRanks[i] + 1) * sizeof(double));
        }

        delete[] piRanks;
        pPolyOut->updateRank();
        out.push_back(pPolyOut);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_real";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
