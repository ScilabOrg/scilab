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
#include "execvisitor.hxx"


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

        int iSize = pDblIn->getSize();

        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());

        if (pDblIn->isComplex() == false)
        {
            out.push_back(pDblIn);
           return types::Function::OK;
       }
        else
        {
            pDblOut->set(pDblIn->get()) ;
            out.push_back(pDblOut);
            return types::Function::OK;
        }
        
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
            double real = pSparseIn->getReal(pRows[i] - 1, pCols[i] - 1);
            pSparseOut->set(pRows[i] - 1, pCols[i] - 1, real, false);
        }

        pSparseOut->finalize();

        delete[] pRows;

        out.push_back(pSparseOut);
        return types::Function::OK;
    }
    else if (in[0]->isPoly())
    {
        types::Double* pDblIn = NULL;
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());


        double* pReal = NULL;

        types::SinglePoly *pInPoly = NULL;	
        types::SinglePoly *pOutPoly = NULL;
        
        if (pPolyIn->isComplex() == false)
        {
            out.push_back(pPolyIn);
            return types::Function::OK;
        }
        else
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                pInPoly = pPolyIn->get(i);	
                pDblIn = pInPoly->getCoef();
                
                pReal = new double[pInPoly->getRank()];

                pOutPoly = new types::SinglePoly(&pReal, pInPoly->getRank());


                for (int j = 0; j < pInPoly->getRank(); j++)	
                {	
                    pReal[j] = pDblIn->getReal()[j];	
                }

                pPolyOut->set(i, pOutPoly);
            }
            
           
            out.push_back(pPolyOut);
            return types::Function::OK;
            
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_real";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
