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
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_imag(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "imag", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "imag", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();

        int iSize = pDblIn->getSize();
        int iOne = 1;

        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());

        if (pDblIn->isComplex() == false)
        {
            memset(pDblOut->get(), 0x00, iSize * sizeof(double));
        }
        else
        {
            C2F(dcopy)(&iSize, pDblIn->getImg(), &iOne, pDblOut->getReal(), &iOne);
        }


        out.push_back(pDblOut);
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        types::Sparse* pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols());

        if (pSparseIn->isComplex() == false)
        {
            out.push_back(pSparseOut);
            return types::Function::OK;
        }

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        int* pRows = new int[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        for (int i = 0 ; i < nonZeros ; i++)
        {
            std::complex<double> cplx = pSparseIn->getImg(pRows[i] - 1, pCols[i] - 1);
            pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplx.imag(), false);
        }

        pSparseOut->finalize();

        delete[] pRows;

        out.push_back(pSparseOut);
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());

        double* dataReal = NULL;

        if (pPolyIn->isComplex() == false)
        {
            // Create a polynom null of size of input polynom
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                types::SinglePoly* pSP = new types::SinglePoly(&dataReal, 1);
                dataReal[0]  = 0;
                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        else
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                int iNewRank = rank;

                // Reduce the rank of output polynom if the last ranks are null
                for (int j = rank - 1 ; j > 0 ; j--)
                {
                    if (pPolyIn->get(i)->getCoefImg()[j] == 0.0)
                    {
                        iNewRank--;
                    }
                    else
                    {
                        break;
                    }
                }

                types::SinglePoly* pSP = new types::SinglePoly(&dataReal, iNewRank);

                for (int j = 0; j < iNewRank; j++)
                {
                    dataReal[j]  = pPolyIn->get(i)->getCoefImg()[j];
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        out.push_back(pPolyOut);
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_imag";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
