/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "types_divide.hxx"
#include "types_finite.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include "matrix_right_division.h"
#include "matrix_right_division.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericRDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{

    InternalType *pResult = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE / DOUBLE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Double *pR			= _pRightOperand->getAs<Double>();

        int iResult = DivideDoubleByDouble(pL, pR, (Double**)&pResult);
        if(iResult)
        {//manage errors
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** POLY / DOUBLE
    */
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        MatrixPoly *pL          = _pLeftOperand->getAsPoly();
        Double *pR              = _pRightOperand->getAs<Double>();

        int iResult = DividePolyByDouble(pL, pR, (MatrixPoly**)&pResult);
        if(iResult)
        {//manage errors
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE / POLY
    */
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL      = _pLeftOperand->getAs<Double>();
        MatrixPoly *pR  = _pRightOperand->getAsPoly();

        int iResult = DivideDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);
        if(iResult)
        {//manage errors
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}

int DivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double **_pDoubleOut)
{
    bool bComplex1		= _pDouble1->isComplex();
    bool bComplex2		= _pDouble2->isComplex();
    bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
    bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;

    //check finite values of _pDouble1 and _pDouble2
    if(isDoubleFinite(_pDouble1) == false || isDoubleFinite(_pDouble2) == false)
    {
        return 2;
    }

    if(_pDouble1->size_get() == 0 || _pDouble2->size_get() == 0)
    {
        //return an empty matrix of double
        *_pDoubleOut	= Double::Empty();
    }
    else if(bScalar2)
    {//[] / x
        int iErr				= 0;
        int iRowResult	= _pDouble1->rows_get();
        int iColResult	= _pDouble1->cols_get();
        int iInc1				= 1;
        int iInc2				= 0;
        int iSizeResult	= iRowResult * iColResult;

        *_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

        if(bComplex1 == false && bComplex2 == false)
        {// Real1 \ Real2 -> Real2 / Real1
            iErr = iRightDivisionRealMatrixByRealMatrix(
                _pDouble1->get_real(), iInc1, 
                _pDouble2->get_real(), iInc2, 
                (*_pDoubleOut)->get_real(), 1, iSizeResult);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {// Real \ Complex -> Complex / Real
            iErr = iRightDivisionRealMatrixByComplexMatrix(
                _pDouble1->get_real(), iInc1, 
                _pDouble2->get_real(), _pDouble2->get_img(), iInc2, 
                (*_pDoubleOut)->get_real(),	(*_pDoubleOut)->get_img(), 1, iSizeResult);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {// Complex \ Real -> Real / Complex
            iErr = iRightDivisionComplexMatrixByRealMatrix(
                _pDouble1->get_real(), _pDouble1->get_img(), iInc1, 
                _pDouble2->get_real(), iInc2,
                (*_pDoubleOut)->get_real(),	(*_pDoubleOut)->get_img(), 1, iSizeResult);
        }
        else if(bComplex1 == true && bComplex2 == true)
        {// Complex \ Complex
            iErr = iRightDivisionComplexMatrixByComplexMatrix(
                _pDouble1->get_real(), _pDouble1->get_img(), iInc1,
                _pDouble2->get_real(), _pDouble2->get_img(), iInc2, 
                (*_pDoubleOut)->get_real(),	(*_pDoubleOut)->get_img(), 1, iSizeResult);
        }
    }
    else if(bScalar1)
    {
        double dblSavedR = 0;
        double dblSavedI = 0;
        Double *pdblTemp = NULL;

        int iRowResult = _pDouble2->cols_get();
        int iColResult = _pDouble2->rows_get();

        //in this case, we have to create a temporary square matrix
        pdblTemp	= new Double(iRowResult, iRowResult, _pDouble1->isComplex());
        pdblTemp->zero_set();

        if(_pDouble1->isComplex())
        {
            dblSavedR = _pDouble1->get_real()[0];
            dblSavedI = _pDouble1->get_img()[0];
            for(int i = 0 ; i < iRowResult ; i++)
            {
                pdblTemp->set(i, i, dblSavedR);
                pdblTemp->set_img(i, i, dblSavedI);
            }
        }
        else
        {
            dblSavedR = _pDouble1->get_real()[0];
            for(int i = 0 ; i < iRowResult ; i++)
            {
                pdblTemp->set(i, i, dblSavedR);
            }
        }

        *_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

        if((*_pDoubleOut)->isComplex())
        {
            double dblRcond = 0;
            iRightDivisionOfComplexMatrix(
                pdblTemp->get_real(), pdblTemp->get_img(), pdblTemp->rows_get(), pdblTemp->cols_get(),
                _pDouble2->get_real(), _pDouble2->get_img(), _pDouble2->rows_get(), _pDouble2->cols_get(),
                (*_pDoubleOut)->get_real(), (*_pDoubleOut)->get_img(), iRowResult, iColResult, &dblRcond);
        }
        else
        {
            double dblRcond = 0;
            iRightDivisionOfRealMatrix(
                pdblTemp->get_real(), pdblTemp->rows_get(), pdblTemp->cols_get(),
                _pDouble2->get_real(), _pDouble2->rows_get(), _pDouble2->cols_get(),
                (*_pDoubleOut)->get_real(), iRowResult, iColResult, &dblRcond);
        }
        delete pdblTemp;
    }
    else if(_pDouble1->cols_get() == _pDouble2->cols_get())
    {
        *_pDoubleOut = new Double(_pDouble1->rows_get(), _pDouble2->rows_get(), _pDouble1->isComplex() || _pDouble2->isComplex());
        if((*_pDoubleOut)->isComplex())
        {
            double dblRcond = 0;
            iRightDivisionOfComplexMatrix(
                _pDouble1->get_real(), _pDouble1->get_img(), _pDouble1->rows_get(), _pDouble1->cols_get(),
                _pDouble2->get_real(), _pDouble2->get_img(), _pDouble2->rows_get(), _pDouble2->cols_get(),
                (*_pDoubleOut)->get_real(), (*_pDoubleOut)->get_img(), _pDouble1->rows_get(), _pDouble2->rows_get(), &dblRcond);
        }
        else
        {
            double dblRcond = 0;
            iRightDivisionOfRealMatrix(
                _pDouble1->get_real(), _pDouble1->rows_get(), _pDouble1->cols_get(),
                _pDouble2->get_real(), _pDouble2->rows_get(), _pDouble2->cols_get(),
                (*_pDoubleOut)->get_real(), _pDouble1->rows_get(), _pDouble2->rows_get(), &dblRcond);
        }
    }
    else
    {
        //Bad dimensions
        return 1;
    }

    return 0;
}

int DividePolyByDouble(MatrixPoly* _pPoly, Double* _pDouble, MatrixPoly** _pPolyOut)
{
    bool bComplex1		= _pPoly->isComplex();
    bool bComplex2		= _pDouble->isComplex();
    bool bScalar1			= _pPoly->rows_get() == 1		&& _pPoly->cols_get() == 1;
    bool bScalar2			= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;

    MatrixPoly *pTemp	= NULL; //use only is _pPoly is scalar and _pDouble not.

    int iRowResult 	= 0;
    int iColResult	= 0;
    int *piRank			= NULL;

    /*	if(bScalar1 && bScalar2)
    {
    iRowResult = 1;
    iColResult = 1;

    piRank = new int[1];
    piRank[0]	= _pPoly->poly_get(0)->rank_get();
    }
    else */if(bScalar2)
    {
        iRowResult = _pPoly->rows_get();
        iColResult = _pPoly->cols_get();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->poly_get(i)->rank_get();
        }
    }
    else if(bScalar1)
    {
        //in this case, we have to create a temporary square polinomial matrix
        iRowResult = _pDouble->cols_get();
        iColResult = _pDouble->rows_get();

        piRank = new int[iRowResult * iRowResult];
        for(int i = 0 ; i < iRowResult * iRowResult ; i++)
        {
            piRank[i] = _pPoly->rank_max_get();
        }

        pTemp = new MatrixPoly(_pPoly->var_get(), iRowResult, iRowResult, piRank);
        if(bComplex1 || bComplex2)
        {
            pTemp->complex_set(true);
        }

        Double *pdblData = _pPoly->poly_get(0)->coef_get();
        for(int i = 0 ; i < iRowResult ; i++)
        {
            pTemp->poly_set(i,i, pdblData);
        }
    }
    /*	else if(pR->size_get() == 1)
    {
    iRowResult = pL->rows_get();
    iColResult = pL->cols_get();

    piRank = new int[iRowResult * iColResult];
    for(int i = 0 ; i < iRowResult * iColResult ; i++)
    {
    piRank[i] = pL->poly_get(i)->rank_get();
    }
    }
    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
    {//Je ne sais pas encore comment ca marche ce machin la !!!
    iRowResult = pR->rows_get();
    iColResult = pR->cols_get();
    }
    */

    (*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), iRowResult, iColResult, piRank);
    delete[] piRank;
    if(bComplex1 || bComplex2)
    {
        (*_pPolyOut)->complex_set(true);
    }

    if(bScalar2)
    {//[p] * cst
        for(int i = 0 ; i < _pPoly->size_get() ; i++)
        {
            Poly *pPolyIn			= _pPoly->poly_get(i);
            double* pRealIn		= pPolyIn->coef_get()->get_real();
            double* pImgIn		= pPolyIn->coef_get()->get_img();

            Poly *pPolyOut		= (*_pPolyOut)->poly_get(i);
            double* pRealOut	= pPolyOut->coef_get()->get_real();
            double* pImgOut		= pPolyOut->coef_get()->get_img();

            if(bComplex1 == false && bComplex2 == false)
            {
                iRightDivisionRealMatrixByRealMatrix(
                    pRealIn, 1,
                    _pDouble->get_real(),	0, 
                    pRealOut, 1, pPolyOut->rank_get());
            }
            else if(bComplex1 == false && bComplex2 == true)
            {
                iRightDivisionRealMatrixByComplexMatrix(
                    pRealIn, 1,
                    _pDouble->get_real(),	_pDouble->get_img(), 0, 
                    pRealOut,	pImgOut, 1, pPolyOut->rank_get());
            }
            else if(bComplex1 == true && bComplex2 == false)
            {
                iRightDivisionComplexMatrixByRealMatrix(
                    pRealIn, pImgIn, 1,
                    _pDouble->get_real(), 0, 
                    pRealOut, pImgOut, 1, pPolyOut->rank_get());
            }
            else if(bComplex1 == true && bComplex2 == true)
            {
                iRightDivisionComplexMatrixByComplexMatrix(
                    pRealIn, pImgIn, 1,
                    _pDouble->get_real(),	_pDouble->get_img(), 0, 
                    pRealOut, pImgOut, 1, pPolyOut->rank_get());
            }
        }
    }
    else if(bScalar1)
    {
        Double *pResultCoef = new Double(iRowResult, iColResult, (*_pPolyOut)->isComplex());
        double *pReal				= pResultCoef->get_real();
        double *pImg				= pResultCoef->get_img();

        for(int i = 0 ; i < pTemp->poly_get(0)->rank_get() ; i++)
        {
            Double *pCoef				= pTemp->extract_coef(i);
            Double *pResultCoef = new Double(iRowResult, iColResult, pCoef->isComplex());
            double *pReal				= pResultCoef->get_real();
            double *pImg				= pResultCoef->get_img();

            if(bComplex1 == false && bComplex2 == false)
            {
                double dblRcond = 0;
                iRightDivisionOfRealMatrix(
                    pCoef->get_real(), iRowResult, iRowResult,
                    _pDouble->get_real(), _pDouble->rows_get(), _pDouble->cols_get(), 
                    pReal, iRowResult, iColResult, &dblRcond);
            }
            else
            {
                double dblRcond = 0;
                iRightDivisionOfComplexMatrix(
                    pCoef->get_real(), pCoef->get_img(), iRowResult, iRowResult,
                    _pDouble->get_real(), _pDouble->get_img(), _pDouble->rows_get(), _pDouble->cols_get(), 
                    pReal, pImg, iRowResult, iColResult, &dblRcond);
            }

            (*_pPolyOut)->insert_coef(i, pResultCoef);
            delete pCoef;
        }
        delete pResultCoef;
    }
    return 0;
}

int DivideDoubleByPoly(Double* _pDouble, MatrixPoly* _pPoly, MatrixPoly** _pPolyOut)
{
    return 0;
}
