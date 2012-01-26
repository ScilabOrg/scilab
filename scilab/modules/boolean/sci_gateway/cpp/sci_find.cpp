/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#include "string.hxx"
#include "bool.hxx"
#include "sparse.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_find(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iMax = -1;
    if(in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"find", 1, 2);
        return types::Function::Error;
    }

    if(in.size() == 2)
    {
        if(in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->get(0) <= 0)
        {
            ScierrorW(999, _W("%ls:  Wrong type for input argument #%d: Scalar positive integer expected.\n"), L"find", 2);
            return types::Function::Error;
        }

        iMax = in[0]->getAs<types::Double>()->get(0);

        wchar_t pwst[10];
        os_swprintf(pwst, 10, L"Max : %d\n", iMax);
        scilabWriteW(pwst);
    }

    double* pdblIndex = 0;
    int iValues = 0;

    if(in[0]->isGenericType())
    {
        types::GenericType* pGT = in[0]->getAs<types::GenericType>();
        pdblIndex = new double[pGT->getSize()];
    }

    if(in[0]->isBool())
    {
        types::Bool* pB = in[0]->getAs<types::Bool>();
        for(int i = 0 ; (iMax == -1 || iValues < iMax) && i < pB->getSize() ; i++)
        {
            if(pB->get(i))
            {
                pdblIndex[iValues] = i + 1;
                iValues++;
            }
        }
    }
    else if(in[0]->isDouble())
    {
        types::Double* pD = in[0]->getAs<types::Double>();
        for(int i = 0 ; (iMax == -1 || iValues < iMax) && i < pD->getSize() ; i++)
        {
            if(pD->get(i))
            {
                wchar_t pwst[50];
                os_swprintf(pwst, 50, L"add %d @ pos %d\n", i + 1, iValues);
                scilabWriteW(pwst);

                pdblIndex[iValues] = i + 1;
                iValues++;
            }
        }
    }
    else if(in[0]->isSparse())
    {
        types::Sparse* pSP = in[0]->getAs<types::Sparse>();
        int iNNZ = pSP->nonZeros();
        int iRows = pSP->getRows();
        double* pRows = new double[iNNZ * 2];
        
        pSP->outputRowCol(pRows);
        double *pCols = pRows + iNNZ;

        for(int i = 0 ; (iMax == -1 || iValues < iMax) && i < iNNZ ; i++)
        {
            pdblIndex[iValues] = pCols[i] * iRows + pRows[i];
            iValues++;
        }
    }
    else if(in[0]->isSparseBool())
    {
        types::SparseBool* pSB = in[0]->getAs<types::SparseBool>();
        int iNNZ = pSB->nbTrue();
        int iRows = pSB->getRows();
        double* pRows = new double[iNNZ * 2];
        
        pSB->outputRowCol(pRows);
        double *pCols = pRows + iNNZ;

        for(int i = 0 ; (iMax == -1 || iValues < iMax) && i < iNNZ ; i++)
        {
            pdblIndex[iValues] = pCols[i] * iRows + pRows[i];
            iValues++;
        }
    }
    else
    {//call overload for other types
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_find";
        Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        return Function::OK;
    }

    if(iValues == 0)
    {
        out.push_back(Double::Empty());
    }
    else
    {
        types::Double* pOut = new Double(1, iValues);
        pOut->set(pdblIndex);
        out.push_back(pOut);
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
