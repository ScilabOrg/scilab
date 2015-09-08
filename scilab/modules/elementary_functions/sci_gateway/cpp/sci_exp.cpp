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

#include <complex>

#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "sparse.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "exp.h"
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();exp(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();exp(a);toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_exp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "exp", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "exp", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());
        double* pInR = pDblIn->get();
        double* pOutR = pDblOut->get();
        int size = pDblIn->getSize();

        if (pDblIn->isComplex())
        {
            double* pInI = pDblIn->getImg();
            double* pOutI = pDblOut->getImg();
            for (int i = 0; i < size; i++)
            {
                zexps(pInR[i], pInI[i], pOutR + i, pOutI + i);
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pOutR[i] = std::exp(pInR[i]);
            }
        }

        out.push_back(pDblOut);
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        types::Sparse* pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols(), pSparseIn->isComplex());

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        int* pRows = new int[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        double* pNonZeroR = new double[nonZeros];
        double* pNonZeroI = new double[nonZeros];
        pSparseIn->outputValues(pNonZeroR, pNonZeroI);

        if (pSparseIn->isComplex())
        {
            for (int i = 0; i < nonZeros; i++)
            {
                std::complex<double> complex;
                double dblReal = complex.real();
                double dblImg = complex.imag();
                zexps(pNonZeroR[i], pNonZeroI[i], &dblReal, &dblImg);
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, complex, false);
            }
        }
        else
        {
            for (int i = 0; i < nonZeros; i++)
            {
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, dexps(pNonZeroR[i]), false);
            }
        }

        pSparseOut->finalize();

        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;

        out.push_back(pSparseOut);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_exp";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
