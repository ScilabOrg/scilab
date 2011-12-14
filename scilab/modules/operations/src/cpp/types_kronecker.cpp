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

extern "C"{
#include "matrix_kronecker.h"
}

types::InternalType *GenericKrontimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    types::Double *pResult = NULL;
    types::GenericType::RealType TypeL = _pLeftOperand->getType();
    types::GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE *.* DOUBLE
    */
    if(TypeL == types::GenericType::RealDouble && TypeR == types::GenericType::RealDouble)
    {
        types::Double *pL = _pLeftOperand->getAs<types::Double>();
        types::Double *pR = _pRightOperand->getAs<types::Double>();

        int iResult = KroneckerMultiplyDoubleByDouble(pL, pR, &pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

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

    double *pReal   = (*_pDoubleOut)->getReal();
    double *pImg    = (*_pDoubleOut)->getImg();

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
