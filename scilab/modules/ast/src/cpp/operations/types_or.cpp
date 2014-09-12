/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "os_swprintf.h"
}

#include "types_or.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "sparse.hxx"

using namespace types;

// ||
InternalType* GenericShortcutOr(InternalType* _pL)
{
    InternalType* pResult = NULL;

    if (_pL->isBool())
    {
        BoolOrBool(_pL->getAs<Bool>(), (Bool**)&pResult);
    }

    if (_pL->isInt())
    {
        IntOrInt(_pL, (Bool**)&pResult);
    }

    if (_pL->isSparseBool())
    {
        SparseBoolOrSparseBool(_pL, (Bool**)&pResult);
    }

    return pResult;
}

// |
InternalType* GenericLogicalOr(InternalType* _pL, InternalType* _pR)
{
    InternalType* pResult = NULL;

    if (_pL->isBool() && _pR->isBool())
    {
        Bool *pL = (Bool*)_pL;
        Bool *pR = (Bool*)_pR;

        int iResult = BoolLogicalOrBool(pL, pR, (Bool**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n").c_str(), L"|", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    if (_pL->isInt() && _pR->isInt())
    {
        int iResult = IntLogicalOrInt(_pL, _pR, &pResult);
        if (iResult != 0)
        {
            GenericType* pL = _pL->getAs<GenericType>();
            GenericType* pR = _pR->getAs<GenericType>();
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n").c_str(), L"|", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    if (_pL->isSparseBool() && _pR->isSparseBool())
    {
        int iResult = SparseBoolLogicalOrSparseBool(_pL, _pR, &pResult);
        if (iResult != 0)
        {
            GenericType* pL = _pL->getAs<GenericType>();
            GenericType* pR = _pR->getAs<GenericType>();
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n").c_str(), L"|", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    return NULL;
}

int BoolOrBool(Bool* _pL, Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //call non shorcut opearion
            *_pOut = NULL;
            return 0;
        }
    }

    *_pOut = new Bool(1); //true && something -> true
    return 0;
}

template <class K>
static int IntOrInt(K* _pL, Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //call non shorcut opearion
            *_pOut = NULL;
            return 0;
        }
    }

    *_pOut = new Bool(1); //true && something -> true
    return 0;
}

int IntOrInt(InternalType* _pL, Bool** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::ScilabInt8 :
        {
            return IntOrInt(_pL->getAs<Int8>(), _pOut);
        }
        case InternalType::ScilabUInt8 :
        {
            return IntOrInt(_pL->getAs<UInt8>(), _pOut);
        }
        case InternalType::ScilabInt16 :
        {
            return IntOrInt(_pL->getAs<Int16>(), _pOut);
        }
        case InternalType::ScilabUInt16 :
        {
            return IntOrInt(_pL->getAs<UInt16>(), _pOut);
        }
        case InternalType::ScilabInt32 :
        {
            return IntOrInt(_pL->getAs<Int32>(), _pOut);
        }
        case InternalType::ScilabUInt32 :
        {
            return IntOrInt(_pL->getAs<UInt32>(), _pOut);
        }
        case InternalType::ScilabInt64 :
        {
            return IntOrInt(_pL->getAs<Int64>(), _pOut);
        }
        case InternalType::ScilabUInt64 :
        {
            return IntOrInt(_pL->getAs<UInt64>(), _pOut);
        }
        default:
        {
            return 3;
        }
    }
}

template <class K>
static int IntLogicalOrInt(K* _pL, K* _pR, InternalType** _pOut)
{
    // left scalar bitwise or right matrix
    if (_pL->isScalar())
    {
        K *pI = new K(_pR->getDims(), _pR->getDimsArray());
        const typename K::type x = _pL->get(0);

        for (int i = 0 ; i < pI->getSize() ; i++)
        {
            pI->set(i, x | _pR->get(i));
        }

        *_pOut = pI;
        return 0;
    }

    // right scalar bitwise or left matrix
    if (_pR->isScalar())
    {
        K *pI = new K(_pL->getDims(), _pL->getDimsArray());
        const typename K::type x = _pL->get(0);

        for (int i = 0 ; i < pI->getSize() ; i++)
        {
            pI->set(i, x | _pL->get(i));
        }

        *_pOut = pI;
        return 0;
    }

    // check dims
    if (_pL->getDims() != _pR->getDims())
    {
        return 1;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < _pL->getDims() ; i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return 1;
        }
    }

    // left matrix bitwise or right matrix
    K* pI = new K(_pR->getDims(), _pR->getDimsArray());
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pI->set(i, _pL->get(i) | _pR->get(i));
    }

    *_pOut = pI;
    return 0;
}

int IntLogicalOrInt(InternalType* _pL, InternalType*  _pR, InternalType** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::ScilabInt8 :
        {
            Int8* pI1 = _pL->getAs<Int8>();
            Int8* pI2 = _pR->getAs<Int8>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabUInt8 :
        {
            UInt8* pI1 = _pL->getAs<UInt8>();
            UInt8* pI2 = _pR->getAs<UInt8>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabInt16 :
        {
            Int16* pI1 = _pL->getAs<Int16>();
            Int16* pI2 = _pR->getAs<Int16>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabUInt16 :
        {
            UInt16* pI1 = _pL->getAs<UInt16>();
            UInt16* pI2 = _pR->getAs<UInt16>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabInt32 :
        {
            Int32* pI1 = _pL->getAs<Int32>();
            Int32* pI2 = _pR->getAs<Int32>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabUInt32 :
        {
            UInt32* pI1 = _pL->getAs<UInt32>();
            UInt32* pI2 = _pR->getAs<UInt32>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabInt64 :
        {
            Int64* pI1 = _pL->getAs<Int64>();
            Int64* pI2 = _pR->getAs<Int64>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::ScilabUInt64 :
        {
            UInt64* pI1 = _pL->getAs<UInt64>();
            UInt64* pI2 = _pR->getAs<UInt64>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        default:
        {
            return 3;
        }
    }
}

int BoolLogicalOrBool(Bool* _pL, Bool*  _pR, Bool** _pOut)
{
    int* piR = _pR->get();
    int* piL = _pL->get();
    int* piB = NULL;

    // M | scalar
    if (_pR->getSize() == 1)
    {
        *_pOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        piB = (*_pOut)->get();
        for (int i = 0 ; i < _pL->getSize(); i++)
        {
            piB[i] = (piR[0] == 1) || (piL[i] == 1);
        }

        return 0;
    }

    if (_pL->getSize() == 1)
    {
        // scalar | M
        *_pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        piB = (*_pOut)->get();
        for (int i = 0 ; i < _pR->getSize(); i++)
        {
            piB[i] = (piR[i] == 1) || (piL[0] == 1);
        }

        return 0;
    }

    if (_pL->getDims() != _pR->getDims())
    {
        return 1;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < _pL->getDims(); i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return 1;
        }
    }

    // M | N (generic case)
    *_pOut = new Bool(_pR->getDims(), piDimsR);
    piB = (*_pOut)->get();
    for (int i = 0 ; i < _pR->getSize(); i++)
    {
        piB[i] = (piR[i] == 1) || (piL[i] == 1);
    }

    return 0;
}

int SparseBoolOrSparseBool(InternalType* _pL, Bool** _pOut)
{
    SparseBool* pL = _pL->getAs<SparseBool>();
    if (pL->nbTrue() == (size_t)pL->getSize())
    {
        *_pOut = new Bool(1);
        return 0;
    }

    *_pOut = NULL;
    return 0;
}

int SparseBoolLogicalOrSparseBool(InternalType* _pL, InternalType*  _pR, InternalType** _pOut)
{
    SparseBool *pL = (SparseBool*)_pL;
    SparseBool *pR = (SparseBool*)_pR;

    if (pL->isScalar())
    {
        if (pL->get(0, 0))
        {
            *_pOut = _pR->clone();
        }
        else
        {
            *_pOut = new SparseBool(pR->getRows(), pR->getCols());
        }

        return 0;
    }

    if (pR->isScalar())
    {
        if (pR->get(0, 0))
        {
            *_pOut = _pL->clone();
        }
        else
        {
            *_pOut = new SparseBool(pL->getRows(), pL->getCols());
        }

        return 0;
    }

    if (pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
    {
        *_pOut = pL->newLogicalOr(*pR);
        return 0;
    }
    return 1;
}
