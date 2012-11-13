/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "boolean_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "bool.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_bool2s(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pOut = NULL;
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "bool2s", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pIn = in[0]->getAs<types::Double>();

        if (pIn->isComplex())
        {
            std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_bool2s";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }

        types::Double* pD = new types::Double(pIn->getDims(), pIn->getDimsArray());

        double* pdblIn  = pIn->get();
        double* pdblOut = pD->get();

        for (int i = 0 ; i < pIn->getSize() ; i++)
        {
            pdblOut[i] = static_cast<double>(pdblIn[i] != 0);
        }
        pOut = pD;
    }
    else if (in[0]->isBool())
    {
        types::Bool* pIn = in[0]->getAs<types::Bool>();
        types::Double* pD = new types::Double(pIn->getDims(), pIn->getDimsArray());

        int* piIn = pIn->get();
        double* pdblOut = pD->get();

        for (int i = 0 ; i < pIn->getSize() ; i++)
        {
            pdblOut[i] = static_cast<double>(piIn[i]);
        }
        pOut = pD;
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSpIn = in[0]->getAs<types::Sparse>();

        if (pSpIn->isComplex())
        {
            std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_bool2s";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }

        types::Sparse* pSpOut = new types::Sparse(pSpIn->getRows(), pSpIn->getCols());

        size_t iNonZeros = pSpIn->nonZeros();

        //coords
        double* pRows = new double[iNonZeros * 2];
        pSpIn->outputRowCol(pRows);
        double* pCols = pRows + iNonZeros;

        //values
        double* pNonZeroR = new double[iNonZeros];
        double* pNonZeroI = new double[iNonZeros];
        pSpIn->outputValues(pNonZeroR, pNonZeroI);

        for (int i = 0; i < iNonZeros; i++)
        {
            pSpOut->set((int)pRows[i] - 1, (int)pCols[i] - 1, static_cast<double>(pNonZeroR[i] != 0));
        }

        pOut = pSpOut;
    }
    else if (in[0]->isSparseBool())
    {
        types::SparseBool* pSpbIn = in[0]->getAs<types::SparseBool>();
        types::Sparse* pSpOut = new types::Sparse(pSpbIn ->getRows(), pSpbIn ->getCols());

        size_t iNonZeros = pSpbIn ->nbTrue();

        //coords
        double* pRows = new double[iNonZeros * 2];
        pSpbIn ->outputRowCol(pRows);
        double* pCols = pRows + iNonZeros;

        for (int i = 0; i < iNonZeros; i++)
        {
            pSpOut->set((int)pRows[i] - 1, (int)pCols[i] - 1, 1);
        }

        pOut = pSpOut;
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_bool2s";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
