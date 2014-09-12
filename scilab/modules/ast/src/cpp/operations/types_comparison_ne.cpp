/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_comparison_ne.hxx"
#include "types_comparison_eq.hxx"
#include "bool.hxx"
#include "mlist.hxx"
#include "context.hxx"

using namespace types;

InternalType *GenericComparisonNonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    if (_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        InternalType *pResult = NULL;
        Sparse* pL = _pLeftOperand->getAs<Sparse>();
        Sparse* pR = _pRightOperand->getAs<Sparse>();

        int iResult = NotEqualToSparseAndSparse(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }
    else if ((_pLeftOperand->isTList() && _pRightOperand->isTList()) ||
             (_pLeftOperand->isMList() && _pRightOperand->isMList()))
    {
        TList* pLL = _pLeftOperand->getAs<TList>();
        TList* pLR = _pRightOperand->getAs<TList>();

        // check if an overload exists
        std::wstring function_name;
        function_name = L"%" + pLL->getShortTypeStr() + L"_n_" + pLR->getShortTypeStr();
        InternalType* pFunc = symbol::Context::getInstance()->get(symbol::Symbol(function_name));

        if (pFunc)
        {
            //call overload
            return NULL;
        }

        if (pLL->getSize() != pLR->getSize())
        {
            return new Bool(true);
        }

        if (pLL->getSize() == 0 && pLR->getSize() == 0)
        {
            //list() == list() -> return true
            return new Bool(false);
        }

        Bool* pB = new Bool(1, pLL->getSize());
        for (int i = 0 ; i < pLL->getSize() ; i++)
        {
            pB->set(i, *pLL->get(i) != *pLR->get(i));
        }

        return pB;
    }
    else
    {
        InternalType* pResult = GenericComparisonEqual(_pLeftOperand, _pRightOperand);
        if (pResult == NULL)
        {
            //to call overloading
            return NULL;
        }

        if (pResult->isBool())
        {
            Bool *pB = pResult->getAs<Bool>();
            for (int i = 0 ; i < pB->getSize() ; i++)
            {
                pB->set(i, pB->get(i) == 0);
            }
            return pB;
        }
        else if (pResult->isSparseBool())
        {
            SparseBool *pSB = pResult->getAs<SparseBool>();
            for (int i = 0 ; i < pSB->getRows() ; i++)
            {
                for (int j = 0 ; j < pSB->getCols() ; j++)
                {
                    pSB->set(i, j, !pSB->get(i, j));
                }
            }
            return pSB;
        }
    }

    return NULL;
}

int NotEqualToSparseAndSparse(Sparse* _pSparse1, Sparse* _pSparse2, GenericType** _pOut)
{
    if ((_pSparse1->getRows() == _pSparse2->getRows() && _pSparse1->getCols() == _pSparse2->getCols()) //matrix case
            || _pSparse1->isScalar() || _pSparse2->isScalar()) //scalar cases
    {
        *_pOut = _pSparse1->newNotEqualTo(*_pSparse2);
    }
    else
    {
        *_pOut = new Bool(true);
    }

    return 0;
}
