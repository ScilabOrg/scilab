/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_kronecker.hxx"
#include "int.hxx"

extern "C"{
#include "matrix_kronecker.h"
}



int KroneckerMultiplyIntByInt(types::GenericType* _pGT1, types::GenericType* _pGT2, types::GenericType** _pGTOut);

// DOUBLE .*. DOUBLE
types::InternalType *GenericKrontimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    types::InternalType *pResult = NULL;
    types::GenericType::RealType TypeL = _pLeftOperand->getType();
    types::GenericType::RealType TypeR = _pRightOperand->getType();

    if(_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        types::Double *pL = _pLeftOperand->getAs<types::Double>();
        types::Double *pR = _pRightOperand->getAs<types::Double>();

        int iResult = KroneckerMultiplyDoubleByDouble(pL, pR, (types::Double**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }
    else if(_pLeftOperand->isInt() && _pRightOperand->isInt())
    {
        types::GenericType *pL = _pLeftOperand->getAs<types::GenericType>();
        types::GenericType *pR = _pRightOperand->getAs<types::GenericType>();
        int iResult = KroneckerMultiplyIntByInt(pL, pR, (types::GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    // Default case : Return NULL will Call Overloading.
    return NULL;
}

types::GenericType* IntFactory(bool _bUnsigned, int _iPrec, int _iRows, int _iCols)
{
    types::GenericType* pRet = NULL;

    int iPrec = _iPrec * _bUnsigned ? -1 : 1;
    switch(iPrec)
    {
    case 1 : 
        pRet = new types::Int8(_iRows, _iCols);
        break;
    case 2 : 
        pRet = new types::Int16(_iRows, _iCols);
        break;
    case 4 : 
        pRet = new types::Int32(_iRows, _iCols);
        break;
    case 8 : 
        pRet = new types::Int64(_iRows, _iCols);
        break;
    case -1 : 
        pRet = new types::UInt8(_iRows, _iCols);
        break;
    case -2 : 
        pRet = new types::UInt16(_iRows, _iCols);
        break;
    case -4 : 
        pRet = new types::UInt32(_iRows, _iCols);
        break;
    case -8 : 
        pRet = new types::UInt64(_iRows, _iCols);
        break;
    }
    return pRet;
}

template<typename T1, typename T2, typename T3>
void KronInt(T1* _pData1, int _iRowsIn1, int _iColsIn1, T2* _pData2, int _iRowsIn2, int _iColsIn2, T3* _pOut)
{
    int iSize1 = _iRowsIn1 * _iColsIn1;
    int iSize2 = _iRowsIn2 * _iColsIn2;

    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int idx = 0;

    for(i = 0 ; i < iSize1 ; i += _iRowsIn1)
    {
        int i2 = i + _iRowsIn1;
        for(j = 0 ; j < iSize2 ; j += _iRowsIn2)
        {
            int j2 = j + _iRowsIn2;
            for(k = i ; k < i2 ; k++)
            {
                //double c = _pdblDataIn1[k];
                for(l = j ; l < j2 ; l++)
                {
                    _pOut[idx] = _pData1[k] * _pData2[l];
                    idx++;
                }
            }
        }
    }
}

int KroneckerMultiplyIntByInt(types::GenericType* _pGT1, types::GenericType* _pGT2, types::GenericType** _pGTOut)
{
    int iRowResult = _pGT1->getRows() * _pGT2->getRows();
    int iColResult = _pGT1->getCols() * _pGT2->getCols();

    switch(_pGT1->getType())
    {
    case types::InternalType::RealInt8 :
        {
            types::Int8* pI1 = _pGT1->getAs<types::Int8>();
            switch(_pGT2->getType())
            {
            case types::InternalType::RealInt8 :
                {
                    types::Int8* pIOut = new types::Int8(iRowResult, iColResult);
                    types::Int8* pI2 = _pGT2->getAs<types::Int8>();
                    KronInt<char, char, char>(pI1->get(), pI1->getRows(), pI1->getCols(), pI2->get(), pI2->getRows(), pI2->getCols(), pIOut->get());
                    *_pGTOut = pIOut;
                    break;
                }
            }
            break;
        }
    case types::InternalType::RealInt16 :
        break;
    case types::InternalType::RealInt32 :
        break;
    case types::InternalType::RealInt64 :
        break;
    }

    return 0;
}

int KroneckerMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut)
{
    bool bComplex1 = _pDouble1->isComplex();
    bool bComplex2 = _pDouble2->isComplex();

    int iRowResult = _pDouble1->getRows() * _pDouble2->getRows();
    int iColResult = _pDouble1->getCols() * _pDouble2->getCols();

    //Output variables
    bool bComplexOut = bComplex1 || bComplex2;
    (*_pDoubleOut)   = new types::Double(iRowResult, iColResult, bComplexOut);

    if(bComplex1 && bComplex2) // Complex .*. Complex
    {
        vKronC( _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iRowResult);
    }
    else if(bComplex1) // Complex .*. Real
    {
        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult);

        vKronR( _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getImg(), iRowResult);
    }
    else if(bComplex2) // Real .*. Complex
    {
        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult);

        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getImg(), iRowResult);
    }
    else // Real .*. Real
    {
        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult);
    }

    return 0; //No Error;
}

// DOUBLE ./. DOUBLE
types::InternalType *GenericKronrdivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    types::Double *pResult = NULL;
    types::GenericType::RealType TypeL = _pLeftOperand->getType();
    types::GenericType::RealType TypeR = _pRightOperand->getType();

    if(TypeL == types::GenericType::RealDouble && TypeR == types::GenericType::RealDouble)
    {
        types::Double *pL = _pLeftOperand->getAs<types::Double>();
        types::Double *pR = _pRightOperand->getAs<types::Double>();

        int iErr = KroneckerRDivideDoubleByDouble(pL, pR, &pResult);
        if(iErr == 1)
        {
            throw ast::ScilabError(_W("Division by zero...\n"));
        }
        else if(iErr == 2)
        {
            throw ast::ScilabError(_W("Bad value in the left or right operand.\n"));
        }
        else if(iErr == 3)
        {
            throw ast::ScilabError(_W("Bad size for left or right operand.\n"));
        }

        return pResult;
    }

    // Default case : Return NULL will Call Overloading.
    return NULL;
}

int KroneckerRDivideDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut)
{
    int iErr = 0;
    types::Double* clone = _pDouble2->clone()->getAs<types::Double>();

    if(_pDouble2->isComplex())
    {
        iErr = conv_img_input(clone->getReal(), clone->getImg(), clone->getSize());
    }
    else
    {
        iErr = conv_real_input(clone->get(), clone->getSize());
    }

    if(iErr)
    {
        delete clone;
        return iErr;
    }

    iErr = KroneckerMultiplyDoubleByDouble(_pDouble1, clone, _pDoubleOut);
    delete clone;

    return iErr;
}

// DOUBLE .\. DOUBLE
types::InternalType *GenericKronldivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    types::Double *pResult = NULL;
    types::GenericType::RealType TypeL = _pLeftOperand->getType();
    types::GenericType::RealType TypeR = _pRightOperand->getType();

    if(TypeL == types::GenericType::RealDouble && TypeR == types::GenericType::RealDouble)
    {
        types::Double *pL = _pLeftOperand->getAs<types::Double>();
        types::Double *pR = _pRightOperand->getAs<types::Double>();

        int iErr = KroneckerLDivideDoubleByDouble(pL, pR, &pResult);
        if(iErr == 1)
        {
            throw ast::ScilabError(_W("Division by zero...\n"));
        }
        else if(iErr == 2)
        {
            throw ast::ScilabError(_W("Bad value in the left operand.\n"));
        }

        return pResult;
    }

    // Default case : Return NULL will Call Overloading.
    return NULL;
}

int KroneckerLDivideDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut)
{
    int iErr = 0;
    types::Double* clone = _pDouble1->clone()->getAs<types::Double>();
    if(_pDouble1->isComplex())
    {
        iErr = conv_img_input(clone->getReal(), clone->getImg(), clone->getSize());
    }
    else
    {
        iErr = conv_real_input(clone->get(), clone->getSize());
    }

    if(iErr)
    {
        delete clone;
        return iErr;
    }

    iErr = KroneckerMultiplyDoubleByDouble(clone, _pDouble2, _pDoubleOut);
    delete clone;

    return iErr;
}

