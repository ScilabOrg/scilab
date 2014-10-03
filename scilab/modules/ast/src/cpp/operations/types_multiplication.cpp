/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "double.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "polynom.hxx"
#include "singlepoly.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include "matrix_multiplication.h"
#include "matrix_addition.h"
#include "operation_f.h"
#include "localization.h"
#include "charEncoding.h"
#include "elem_common.h"
}


using namespace types;

InternalType *GenericTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

    if (TypeL == GenericType::ScilabDouble && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    if (TypeR == GenericType::ScilabDouble && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE * DOUBLE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = MultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE * POLY
    */
    else if (TypeL == InternalType::ScilabDouble && TypeR == InternalType::ScilabPolynom)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Polynom *pR     = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyDoubleByPoly(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY * DOUBLE
    */
    else if (TypeL == InternalType::ScilabPolynom && TypeR == InternalType::ScilabDouble)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Double *pR              = _pRightOperand->getAs<Double>();

        int iResult = MultiplyPolyByDouble(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY * POLY
    */
    else if (TypeL == InternalType::ScilabPolynom && TypeR == InternalType::ScilabPolynom)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR          = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyPolyByPoly(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE * SPARSE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabSparse)
    {
        Sparse *pL   = _pLeftOperand->getAs<Sparse>();
        Sparse *pR   = _pRightOperand->getAs<Sparse>();

        int iResult = MultiplySparseBySparse(pL, pR, (Sparse**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE * SPARSE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabSparse)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Sparse *pR   = _pRightOperand->getAs<Sparse>();

        int iResult = MultiplyDoubleBySparse(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE * DOUBLE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabDouble)
    {
        Sparse *pL   = _pLeftOperand->getAs<Sparse>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = MultiplySparseByDouble(pL, pR, (GenericType**)&pResult);
        if (iResult)
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

int MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    if (_pDouble1->isScalar())
    {
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), bComplex1 | bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble2->isScalar())
    {
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), bComplex1 | bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            //Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            //Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->get(0, 0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->get(0, 0), _pDouble2->getImg(0, 0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble1->getDims() > 2 || _pDouble2->getDims() > 2 || _pDouble1->getCols() != _pDouble2->getRows())
    {
        return 1;
    }

    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    (*_pDoubleOut) = new Double(_pDouble1->getRows(), _pDouble2->getCols(), bComplex1 | bComplex2);

    if (bComplex1 == false && bComplex2 == false)
    {
        //Real Matrix by Real Matrix
        iMultiRealMatrixByRealMatrix(
            _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        //Real Matrix by Matrix Complex
        iMultiRealMatrixByComplexMatrix(
            _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        //Complex Matrix by Real Matrix
        iMultiComplexMatrixByRealMatrix(
            _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        //Complex Matrix by Complex Matrix
        iMultiComplexMatrixByComplexMatrix(
            _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    return 0;
}

int DotMultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double**  _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->isScalar();
    bool bScalar2   = _pDouble2->isScalar();

    if (bScalar1)
    {
        (*_pDoubleOut) = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), _pDouble1->isComplex() | _pDouble2->isComplex());
        if (bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (bScalar2)
    {
        (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), _pDouble1->isComplex() | _pDouble2->isComplex());
        if (bComplex1 == false && bComplex2 == false)
        {
            //Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            //Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble1->getDims() != _pDouble2->getDims())
    {
        return 1;
    }

    int* piDims1 = _pDouble1->getDimsArray();
    int* piDims2 = _pDouble2->getDimsArray();

    for (int i = 0 ; i < _pDouble1->getDims() ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 0;
        }
    }

    (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), _pDouble1->isComplex() | _pDouble2->isComplex());
    if (bComplex1 == false && bComplex2 == false)
    {
        iDotMultiplyRealMatrixByRealMatrix(_pDouble1->get(), _pDouble2->get(), (*_pDoubleOut)->get(), _pDouble1->getSize(), 1);
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iDotMultiplyRealMatrixByComplexMatrix(_pDouble1->get(), _pDouble2->get(), _pDouble2->getImg(), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), _pDouble1->getSize(), 1);
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iDotMultiplyComplexMatrixByRealMatrix(_pDouble1->get(), _pDouble1->getImg(), _pDouble2->get(), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), _pDouble1->getSize(), 1);
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        iDotMultiplyComplexMatrixByComplexMatrix(_pDouble1->get(), _pDouble1->getImg(), _pDouble2->get(), _pDouble2->getImg(), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), _pDouble1->getSize(), 1);
    }

    return 0;
}

int MultiplyDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    bool bComplex1  = _pDouble->isComplex();
    bool bComplex2  = _pPoly->isComplex();

    if (_pDouble->isScalar())
    {
        int* piRank = new int[_pPoly->getSize()];
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(i).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            double* pRealIn         = _pPoly->get(i).get();
            double* pImgIn          = _pPoly->getImg(i).get();

            double* pRealOut        = (*_pPolyOut)->get(i).get();
            double* pImgOut         = (*_pPolyOut)->getImg(i).get();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(_pDouble->get(0), pRealIn, 1, _pPoly->get(i).getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(_pDouble->get(0), pRealIn, pImgIn, 1, _pPoly->get(i).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, 1, _pPoly->get(i).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, pImgIn, 1, _pPoly->get(i).getSize(), pRealOut, pImgOut);
            }
        }
        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly->isScalar())
    {
        int* piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        double *pDoubleR    = _pDouble->get();
        double *pDoubleI    = _pDouble->getImg();

        double* pRealIn     = _pPoly->get(0).get();
        double* pImgIn      = _pPoly->get(0).getImg();

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            double* pRealOut        = (*_pPolyOut)->get(i).get();
            double* pImgOut         = (*_pPolyOut)->get(i).getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(pDoubleR[i], pRealIn, 1, _pPoly->get(0).getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(pDoubleR[i], pRealIn, pImgIn, 1, _pPoly->get(0).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(pDoubleR[i], pDoubleI[i], pRealIn, 1, _pPoly->get(0).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(pDoubleR[i], pDoubleI[i], pRealIn, pImgIn, 1, _pPoly->get(0).getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly->getDims() > 2 || _pDouble->getDims() > 2 || _pDouble->getCols() != _pPoly->getRows())
    {
        return 1;
    }

    int* piRank = new int[_pDouble->getRows() * _pPoly->getCols()];
    int iMaxRank = _pPoly->getMaxRank();
    for (int i = 0 ; i < _pDouble->getRows() * _pPoly->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pPoly->getCols(), piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    Double *pCoef = _pPoly->getCoef();
    Double *pTemp = new Double(_pDouble->getRows(), pCoef->getCols(), bComplex1 || bComplex2);

    if (bComplex1 == false && bComplex2 == false)
    {
        iMultiRealMatrixByRealMatrix(_pDouble->get(), _pDouble->getRows(), _pDouble->getCols(),
                                     pCoef->get(), pCoef->getRows(), pCoef->getCols(),
                                     pTemp->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iMultiRealMatrixByComplexMatrix(_pDouble->get(), _pDouble->getRows(), _pDouble->getCols(),
                                        pCoef->get(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                                        pTemp->get(), pTemp->getImg());

    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iMultiComplexMatrixByRealMatrix(_pDouble->get(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                                        pCoef->get(), pCoef->getRows(), pCoef->getCols(),
                                        pTemp->get(), pTemp->getImg());
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        iMultiComplexMatrixByComplexMatrix(_pDouble->get(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                                           pCoef->get(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                                           pTemp->get(), pTemp->getImg());
    }

    (*_pPolyOut)->setCoef(pTemp);
    (*_pPolyOut)->updateRank();
    delete pTemp;
    return 0;
}

int MultiplyPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom **_pPolyOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bComplex2  = _pDouble->isComplex();
    bool bScalar1   = _pPoly->isScalar();
    bool bScalar2   = _pDouble->isScalar();

    if (bScalar1)
    {
        int* piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        double *pDoubleR    = _pDouble->get();
        double *pDoubleI    = _pDouble->getImg();

        double* pRealIn     = _pPoly->get(0).get();
        double* pImgIn      = _pPoly->get(0).getImg();

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            double* pRealOut        = (*_pPolyOut)->get(i).get();
            double* pImgOut         = (*_pPolyOut)->get(i).getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(pDoubleR[i], pRealIn, 1, _pPoly->get(0).getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(pDoubleR[i], pDoubleI[i], pRealIn, 1, _pPoly->get(0).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(pDoubleR[i], pRealIn, pImgIn, 1, _pPoly->get(0).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(pDoubleR[i], pDoubleI[i], pRealIn, pImgIn, 1, _pPoly->get(0).getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }
    else if (bScalar2)
    {
        int* piRank = new int[_pPoly->getSize()];
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(i).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            double* pRealIn = _pPoly->get(i).get();
            double* pImgIn  = _pPoly->get(i).getImg();

            double* pRealOut = (*_pPolyOut)->get(i).get();
            double* pImgOut  = (*_pPolyOut)->get(i).getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(_pDouble->get(0), pRealIn, 1, _pPoly->get(i).getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, 1, _pPoly->get(i).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(_pDouble->get(0), pRealIn, pImgIn, 1, _pPoly->get(i).getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, pImgIn, 1, _pPoly->get(i).getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pDouble->getDims() > 2 || _pPoly->getDims() > 2 || _pPoly->getCols() != _pDouble->getRows())
    {
        return 1;
    }

    int* piRank = new int[_pPoly->getRows() * _pDouble->getCols()];
    int iMaxRank = _pPoly->getMaxRank();
    for (int i = 0 ; i < _pPoly->getRows() * _pDouble->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getRows(), _pDouble->getCols(), piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    //Distribution a la mano par appels a des sous-fonctions ( iMulti...ScalarBy...Scalar ) plus iAdd...To... )

    //for each line of _pPoly
    for (int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)
    {
        //for each col of _pDouble
        for (int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
        {
            (*_pPolyOut)->get(iRow1, iCol2).setZeros();

            //for each rows of _pDouble / cols of _pPoly
            for (int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
            {
                // SinglePoly(iRow1, iRow2) * Double(iRow2, iCol2)
                int iSize = _pPoly->get(iRow1, iRow2).getSize();
                double* pdblMult = new double[iSize];

                if (bComplex1 == false && bComplex2 == false)
                {
                    //Real Matrix by Real Scalar
                    iMultiRealScalarByRealMatrix(_pDouble->get(iRow2, iCol2), _pPoly->get(iRow1, iRow2).get(), iSize, 1, pdblMult);
                    add((*_pPolyOut)->get(iRow1, iCol2).get(), (long long)iSize, pdblMult, (*_pPolyOut)->get(iRow1, iCol2).get());
                }
                else if (bComplex1 == false && bComplex2 == true)
                {
                    //Real Matrix by Scalar Complex
                    double* pdblMultImg = new double[iSize];
                    iMultiComplexScalarByRealMatrix(_pDouble->get(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2), _pPoly->get(iRow1, iRow2).get(), _pPoly->get(iRow1, iRow2).getSize(), 1, pdblMult, pdblMultImg);
                    add((*_pPolyOut)->get(iRow1, iCol2).get(), (*_pPolyOut)->get(iRow1, iCol2).getImg(), (long long)iSize, pdblMult, pdblMultImg, (*_pPolyOut)->get(iRow1, iCol2).get(), (*_pPolyOut)->get(iRow1, iCol2).getImg());
                    delete[] pdblMultImg;
                }
                else if (bComplex1 == true && bComplex2 == false)
                {
                    double* pdblMultImg = new double[iSize];
                    iMultiRealScalarByComplexMatrix(_pDouble->get(iRow2, iCol2), _pPoly->get(iRow1, iRow2).get(), _pPoly->get(iRow1, iRow2).getImg(), _pPoly->get(iRow1, iRow2).getSize(), 1, pdblMult, pdblMultImg);
                    add((*_pPolyOut)->get(iRow1, iCol2).get(), (*_pPolyOut)->get(iRow1, iCol2).getImg(), (long long)iSize, pdblMult, pdblMultImg, (*_pPolyOut)->get(iRow1, iCol2).get(), (*_pPolyOut)->get(iRow1, iCol2).getImg());
                    delete[] pdblMultImg;
                }
                else //if(bComplex1 == true && bComplex2 == true)
                {
                    double* pdblMultImg = new double[iSize];
                    iMultiComplexScalarByComplexMatrix(_pDouble->get(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2), _pPoly->get(iRow1, iRow2).get(), _pPoly->get(iRow1, iRow2).getImg(), _pPoly->get(iRow1, iRow2).getSize(), 1, pdblMult, pdblMultImg);
                    add((*_pPolyOut)->get(iRow1, iCol2).get(), (*_pPolyOut)->get(iRow1, iCol2).getImg(), (long long)iSize, pdblMult, pdblMultImg, (*_pPolyOut)->get(iRow1, iCol2).get(), (*_pPolyOut)->get(iRow1, iCol2).getImg());
                    delete[] pdblMultImg;
                }

                delete[] pdblMult;
            }//for(int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
        }//for(int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
    }//for(int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)

    (*_pPolyOut)->updateRank();
    return 0;
}

int MultiplyPolyByPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom** _pPolyOut)
{
    bool bComplex1  = _pPoly1->isComplex();
    bool bComplex2  = _pPoly2->isComplex();

    if (_pPoly1->isScalar() && _pPoly2->isScalar())
    {
        //poly1(0) * poly2(0)
        int iRank = _pPoly1->get(0).getRank() + _pPoly2->get(0).getRank();
        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), 1, 1, &iRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        if (bComplex1 == false && bComplex2 == false)
        {
            (*_pPolyOut)->get(0).setZeros();

            iMultiScilabPolynomByScilabPolynom(
                _pPoly1->get(0).get(), _pPoly1->get(0).getSize(),
                _pPoly2->get(0).get(), _pPoly2->get(0).getSize(),
                (*_pPolyOut)->get(0).get(), (*_pPolyOut)->get(0).getSize());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            (*_pPolyOut)->get(0).setZeros();

            iMultiScilabPolynomByComplexPoly(
                _pPoly1->get(0).get(), _pPoly1->get(0).getSize(),
                _pPoly2->get(0).get(), _pPoly2->get(0).getImg(), _pPoly2->get(0).getSize(),
                (*_pPolyOut)->get(0).get(), (*_pPolyOut)->get(0).getImg(), (*_pPolyOut)->get(0).getSize());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            (*_pPolyOut)->get(0).setZeros();

            iMultiComplexPolyByScilabPolynom(
                _pPoly1->get(0).get(), _pPoly1->get(0).getImg(), _pPoly1->get(0).getSize(),
                _pPoly2->get(0).get(), _pPoly2->get(0).getSize(),
                (*_pPolyOut)->get(0).get(), (*_pPolyOut)->get(0).getImg(), (*_pPolyOut)->get(0).getSize());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            (*_pPolyOut)->get(0).setZeros();

            iMultiComplexPolyByComplexPoly(
                _pPoly1->get(0).get(), _pPoly1->get(0).getImg(), _pPoly1->get(0).getSize(),
                _pPoly2->get(0).get(), _pPoly2->get(0).getImg(), _pPoly2->get(0).getSize(),
                (*_pPolyOut)->get(0).get(), (*_pPolyOut)->get(0).getImg(), (*_pPolyOut)->get(0).getSize());
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly1->isScalar())
    {
        //poly1(0) * poly2(n)
        int* piRank = new int[_pPoly2->getSize()];
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            piRank[i] = _pPoly1->get(0).getRank() + _pPoly2->get(i).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly2->getDims(), _pPoly2->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }
        delete[] piRank;


        if (bComplex1 == false && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    _pPoly1->get(0).get(), _pPoly1->get(0).getSize(),
                    _pPoly2->get(iPoly).get(), _pPoly2->get(iPoly).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiScilabPolynomByComplexPoly(
                    _pPoly1->get(0).get(), _pPoly1->get(0).getSize(),
                    _pPoly2->get(iPoly).get(), _pPoly2->get(iPoly).getImg(), _pPoly2->get(iPoly).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getImg(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiComplexPolyByScilabPolynom(
                    _pPoly1->get(0).get(), _pPoly1->get(0).getImg(), _pPoly1->get(0).getSize(),
                    _pPoly2->get(iPoly).get(), _pPoly2->get(iPoly).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getImg(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiComplexPolyByComplexPoly(
                    _pPoly1->get(0).get(), _pPoly1->get(0).getImg(), _pPoly1->get(0).getSize(),
                    _pPoly2->get(iPoly).get(), _pPoly2->get(iPoly).getImg(), _pPoly2->get(iPoly).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getImg(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly2->isScalar())
    {
        //poly1(n) * poly2(0)
        int* piRank = new int[_pPoly1->getSize()];
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            piRank[i] = _pPoly2->get(0).getRank() + _pPoly1->get(i).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }
        delete[] piRank;

        if (bComplex1 == false && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    _pPoly1->get(iPoly).get(), _pPoly1->get(iPoly).getSize(),
                    _pPoly2->get(0).get(), _pPoly2->get(0).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiScilabPolynomByComplexPoly(
                    _pPoly1->get(iPoly).get(), _pPoly1->get(iPoly).getSize(),
                    _pPoly2->get(0).get(), _pPoly2->get(0).getImg(), _pPoly2->get(0).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getImg(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiComplexPolyByScilabPolynom(
                    _pPoly1->get(iPoly).get(), _pPoly1->get(iPoly).getImg(), _pPoly1->get(iPoly).getSize(),
                    _pPoly2->get(0).get(), _pPoly2->get(0).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getImg(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                (*_pPolyOut)->get(iPoly).setZeros();

                iMultiComplexPolyByComplexPoly(
                    _pPoly1->get(iPoly).get(), _pPoly1->get(iPoly).getImg(), _pPoly1->get(iPoly).getSize(),
                    _pPoly2->get(0).get(), _pPoly2->get(0).getImg(), _pPoly2->get(0).getSize(),
                    (*_pPolyOut)->get(iPoly).get(), (*_pPolyOut)->get(iPoly).getImg(), (*_pPolyOut)->get(iPoly).getSize());
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly1->getDims() > 2 || _pPoly2->getDims() > 2 || _pPoly1->getCols() != _pPoly2->getRows())
    {
        return 1;
    }

    // matrix by matrix
    int* piRank = new int[_pPoly1->getRows() * _pPoly2->getCols()];
    int iMaxRank = _pPoly1->getMaxRank() + _pPoly2->getMaxRank();
    for (int i = 0 ; i < _pPoly1->getRows() * _pPoly2->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getRows(), _pPoly2->getCols(), piRank);
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    delete[] piRank;


    if (bComplex1 == false && bComplex2 == false)
    {
        double *pReal = NULL;
        SinglePoly pTemp((*_pPolyOut)->getVariableName(), &pReal, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                (*_pPolyOut)->get(iRow, iCol).setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    pTemp.setZeros();

                    iMultiScilabPolynomByScilabPolynom(
                        _pPoly1->get(iRow, iCommon).get(), _pPoly1->get(iRow, iCommon).getSize(),
                        _pPoly2->get(iCommon, iCol).get(), _pPoly2->get(iCommon, iCol).getSize(),
                        pTemp.get(), _pPoly1->get(iRow, iCommon).getRank() + _pPoly2->get(iCommon, iCol).getRank() + 1);

                    iAddScilabPolynomToScilabPolynom(
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        pTemp.get(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getSize());
                }
            }
        }
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly pTemp((*_pPolyOut)->getVariableName(), &pReal, (*_pPolyOut)->getMaxRank());
        SinglePoly pTempImg((*_pPolyOut)->getVariableName(), &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                (*_pPolyOut)->get(iRow, iCol).setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    pTemp.setZeros();

                    iMultiScilabPolynomByComplexPoly(
                        _pPoly1->get(iRow, iCommon).get(), _pPoly1->get(iRow, iCommon).getSize(),
                        _pPoly2->get(iCommon, iCol).get(), _pPoly2->get(iCommon, iCol).getImg(), _pPoly2->get(iCommon, iCol).getSize(),
                        pTemp.get(), pTemp.getImg(), _pPoly1->get(iRow, iCommon).getRank() + _pPoly2->get(iCommon, iCol).getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getImg(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        pTemp.get(), pTemp.getImg(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getImg(), (*_pPolyOut)->get(iRow, iCol).getSize());
                }
            }
        }
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly pTemp(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                (*_pPolyOut)->get(iRow, iCol).setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    pTemp.setZeros();

                    iMultiScilabPolynomByComplexPoly(
                        _pPoly2->get(iCommon, iCol).get(), _pPoly2->get(iCommon, iCol).getSize(),
                        _pPoly1->get(iRow, iCommon).get(), _pPoly1->get(iRow, iCommon).getImg(), _pPoly1->get(iRow, iCommon).getSize(),
                        pTemp.get(), pTemp.getImg(), _pPoly1->get(iRow, iCommon).getRank() + _pPoly2->get(iCommon, iCol).getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getImg(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        pTemp.get(), pTemp.getImg(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getImg(), (*_pPolyOut)->get(iRow, iCol).getSize());
                }
            }
        }
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly pTemp(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                (*_pPolyOut)->get(iRow, iCol).setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    pTemp.setZeros();

                    iMultiComplexPolyByComplexPoly(
                        _pPoly1->get(iRow, iCommon).get(), _pPoly1->get(iRow, iCommon).getImg(), _pPoly1->get(iRow, iCommon).getSize(),
                        _pPoly2->get(iCommon, iCol).get(), _pPoly2->get(iCommon, iCol).getImg(), _pPoly2->get(iCommon, iCol).getSize(),
                        pTemp.get(), pTemp.getImg(), _pPoly1->get(iRow, iCommon).getRank() + _pPoly2->get(iCommon, iCol).getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getImg(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        pTemp.get(), pTemp.getImg(), (*_pPolyOut)->get(iRow, iCol).getSize(),
                        (*_pPolyOut)->get(iRow, iCol).get(), (*_pPolyOut)->get(iRow, iCol).getImg(), (*_pPolyOut)->get(iRow, iCol).getSize());
                }
            }
        }
    }
    (*_pPolyOut)->updateRank();

    return 0;
}

int MultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pSparseOut)
{
    if (_pSparse1->isScalar())
    {
        //scalar * sp
        Double* pDbl = NULL;
        if (_pSparse1->isComplex())
        {
            std::complex<double> dbl = _pSparse1->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse1->get(0, 0));
        }

        MultiplyDoubleBySparse(pDbl, _pSparse2, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse2->isScalar())
    {
        //sp * scalar
        Double* pDbl = NULL;
        if (_pSparse2->isComplex())
        {
            std::complex<double> dbl = _pSparse2->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse2->get(0, 0));
        }

        MultiplySparseByDouble(_pSparse1, pDbl, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse1->getCols() != _pSparse2->getRows())
    {
        return 1;
    }

    *_pSparseOut = _pSparse1->multiply(*_pSparse2);
    return 0;
}

int MultiplyDoubleBySparse(Double* _pDouble, Sparse *_pSparse, GenericType** _pOut)
{
    //D * SP
    if (_pDouble->isScalar())
    {
        //d * SP -> SP
        Sparse* pOut = NULL;
        if (_pDouble->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            pOut = _pSparse->multiply(dbl);
        }
        else
        {
            pOut = _pSparse->multiply(_pDouble->get(0));
        }
        *_pOut = pOut;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D * sp -> D .* d
        Double* pD = NULL;

        if (_pSparse->isComplex())
        {
            std::complex<double> dbl(_pSparse->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pSparse->get(0, 0));
        }

        InternalType* pIT = GenericDotTimes(_pDouble, pD);
        *_pOut = pIT->getAs<GenericType>();
        delete pD;
        return 0;
    }

    if (_pDouble->getCols() != _pSparse->getRows())
    {
        return 1;
    }

    //try to be smart and only compute for non zero values

    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pSparse->outputValues(pValR, pValI);

    Double* pOut = new Double(_pDouble->getRows(), _pSparse->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dbl = pValR[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblVal = _pDouble->get(j, iRow) * dbl;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblVal);
            }
        }
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        //a * (b ci) -> ab ac
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR;
                double dblValI = _pDouble->get(j, iRow) * dblI;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        //(a bi) * c -> ac + bc
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR;
                double dblValI = _pDouble->getImg(j, iRow) * dblR;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR - _pDouble->getImg(j, iRow) * dblI;
                double dblValI = _pDouble->get(j, iRow) * dblI + _pDouble->getImg(j, iRow) * dblR;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }

    *_pOut = pOut;
    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    return 0;
}

int MultiplySparseByDouble(Sparse *_pSparse, Double*_pDouble, GenericType** _pOut)
{
    if (_pDouble->isScalar())
    {
        //SP * d -> SP
        Sparse* pOut = NULL;
        if (_pDouble->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            pOut = _pSparse->multiply(dbl);
        }
        else
        {
            pOut = _pSparse->multiply(_pDouble->get(0));
        }
        *_pOut = pOut;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D * sp -> D .* d
        Double* pD = NULL;

        if (_pSparse->isComplex())
        {
            std::complex<double> dbl(_pSparse->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pSparse->get(0, 0));
        }

        InternalType* pIT = GenericDotTimes(_pDouble, pD);
        *_pOut = pIT->getAs<GenericType>();
        delete pD;
        return 0;
    }

    if (_pSparse->getCols() != _pDouble->getRows())
    {
        return 1;
    }

    //try to be smart and only compute for non zero values

    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pSparse->outputValues(pValR, pValI);

    Double* pOut = new Double(_pSparse->getRows(), _pDouble->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow    = static_cast<int>(pRows[i]) - 1;
            int iCol    = static_cast<int>(pCols[i]) - 1;
            double dbl  = pValR[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblVal = _pDouble->get(iCol, j) * dbl;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblVal);
            }
        }
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        //a * (b ci) -> ab ac
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR;
                double dblValI = _pDouble->get(iCol, j) * dblI;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        //(a bi) * c -> ac + bc
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR;
                double dblValI = _pDouble->getImg(iCol, j) * dblR;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR - _pDouble->getImg(iCol, j) * dblI;
                double dblValI = _pDouble->get(iCol, j) * dblI + _pDouble->getImg(iCol, j) * dblR;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }

    *_pOut = pOut;
    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    return 0;
}

int DotMultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pOut)
{
    if (_pSparse1->isScalar() || _pSparse2->isScalar())
    {
        //SP .* sp or sp .* SP
        return MultiplySparseBySparse(_pSparse1, _pSparse2, _pOut);
    }

    if (_pSparse1->getRows() != _pSparse2->getRows() || _pSparse1->getCols() != _pSparse2->getCols())
    {
        return 1;
    }

    *_pOut = _pSparse1->dotMultiply(*_pSparse2);

    return 0;
}

int DotMultiplyDoubleBySparse(Double* _pDouble, Sparse* _pSparse, GenericType**  _pOut)
{
    if (_pDouble->isScalar())
    {
        return MultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
    }

    if (_pSparse->isScalar())
    {
        return MultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
    }

    if (_pSparse->getRows() != _pDouble->getRows() || _pSparse->getCols() != _pDouble->getCols())
    {
        return 1;
    }

    Sparse* pOut = new Sparse(_pDouble->getRows(), _pDouble->getCols(), _pSparse->isComplex() || _pDouble->isComplex());
    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            pOut->set(iRow, iCol, _pSparse->get(iRow, iCol) * _pDouble->get(iRow, iCol));
        }
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            std::complex<double> newVal(dbl.real() * _pDouble->get(iRow, iCol), dbl.imag() * _pDouble->get(iRow, iCol));
            pOut->set(iRow, iCol, newVal);
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            std::complex<double> newVal(dbl.real() * _pDouble->get(iRow, iCol), dbl.real() * _pDouble->getImg(iRow, iCol));
            pOut->set(iRow, iCol, newVal);
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            double dblR = _pDouble->get(iRow, iCol) * dbl.real() - _pDouble->getImg(iRow, iCol) * dbl.imag();
            double dblI = _pDouble->getImg(iRow, iCol) * dbl.real() + _pDouble->get(iRow, iCol) * dbl.imag();

            std::complex<double> newVal(dblR, dblI);
            pOut->set(iRow, iCol, newVal);
        }
    }

    *_pOut = pOut;
    delete[] pRows;

    return 0;
}

int DotMultiplySparseByDouble(Sparse* _pSparse, Double* _pDouble, GenericType** _pOut)
{
    return DotMultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
}

int DotMultiplyPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom** _pPolyOut)
{
    return DotMultiplyDoubleByPoly(_pDouble, _pPoly, _pPolyOut);
}

int DotMultiplyDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    int iSize = _pDouble->getSize();
    if (_pDouble->isScalar() == false &&
            _pPoly->isScalar() == false &&
            iSize != _pPoly->getSize())
    {
        return 1;
    }

    int* piRanks = new int[iSize];
    memset(piRanks, 0x00, iSize * sizeof(int));
    Polynom* pPolyTemp = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRanks);
    delete[] piRanks;
    pPolyTemp->setCoef(_pDouble);
    int iErr = DotMultiplyPolyByPoly(pPolyTemp, _pPoly, _pPolyOut);
    delete pPolyTemp;
    return iErr;
}

int DotMultiplyPolyByPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom** _pPolyOut)
{
    if (_pPoly1->isScalar() || _pPoly2->isScalar())
    {
        return MultiplyPolyByPoly(_pPoly1, _pPoly2, _pPolyOut);
    }
    else
    {
        if (_pPoly1->getSize() != _pPoly2->getSize())
        {
            return 1;
        }

        int* piRank = new int[_pPoly1->getSize()];
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            piRank[i] = _pPoly1->get(i).getRank() + _pPoly2->get(i).getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), piRank);

        if (_pPoly1->isComplex() && _pPoly2->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                (*_pPolyOut)->get(i).setZeros();

                iMultiComplexPolyByComplexPoly(
                    _pPoly1->get(i).get(), _pPoly1->get(i).getImg(), _pPoly1->get(i).getSize(),
                    _pPoly2->get(i).get(), _pPoly2->get(i).getImg(), _pPoly2->get(i).getSize(),
                    (*_pPolyOut)->get(i).get(), (*_pPolyOut)->get(i).getImg(), (*_pPolyOut)->get(i).getSize());

            }
        }
        else if (_pPoly1->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                (*_pPolyOut)->get(i).setZeros();

                iMultiComplexPolyByScilabPolynom(
                    _pPoly1->get(i).get(), _pPoly1->get(i).getImg(), _pPoly1->get(i).getSize(),
                    _pPoly2->get(i).get(), _pPoly2->get(i).getSize(),
                    (*_pPolyOut)->get(i).get(), (*_pPolyOut)->get(i).getImg(), (*_pPolyOut)->get(i).getSize());
            }
        }
        else if (_pPoly2->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                (*_pPolyOut)->get(i).setZeros();

                iMultiScilabPolynomByComplexPoly(
                    _pPoly1->get(i).get(), _pPoly1->get(i).getSize(),
                    _pPoly2->get(i).get(), _pPoly2->get(i).getImg(), _pPoly2->get(i).getSize(),
                    (*_pPolyOut)->get(i).get(), (*_pPolyOut)->get(i).getImg(), (*_pPolyOut)->get(i).getSize());
            }
        }
        else
        {
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                (*_pPolyOut)->get(i).setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    _pPoly1->get(i).get(), _pPoly1->get(i).getSize(),
                    _pPoly2->get(i).get(), _pPoly2->get(i).getSize(),
                    (*_pPolyOut)->get(i).get(), (*_pPolyOut)->get(i).getSize());
            }
        }
    }

    return 0;
}

