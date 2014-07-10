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

#include "types_power.hxx"
#include "types_multiplication.hxx"
#include "scilabexception.hxx"

extern "C"
{
#include "operation_f.h"
#include "matrix_power.h"
#include "localization.h"
#include "charEncoding.h"
}

InternalType *GenericPower(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE ^ DOUBLE
    ** DOUBLE ** DOUBLE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = PowerDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY ^ DOUBLE
    ** POLY ** DOUBLE
    */
    if (_pLeftOperand->isPoly() && _pRightOperand->isDouble())
    {
        Polynom *pL   = _pLeftOperand->getAs<Polynom>();
        Double *pR    = _pRightOperand->getAs<Double>();

        int iResult = PowerPolyByDouble(pL, pR, &pResult);
        switch (iResult)
        {
            case 1 :
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::ScilabError(_W("Invalid exponent.\n"));
            default:
                //OK
                break;
        }

        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

}

InternalType *GenericDotPower(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE .^ DOUBLE
    ** DOUBLE .** DOUBLE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Double *pR  = _pRightOperand->getAs<Double>();

        int iResult = DotPowerDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY .^ DOUBLE
    ** POLY .** DOUBLE
    */
    if (TypeL == GenericType::ScilabPolynom && TypeR == GenericType::ScilabDouble)
    {
        Polynom *pL   = _pLeftOperand->getAs<Polynom>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = PowerPolyByDouble(pL, pR, &pResult);
        switch (iResult)
        {
            case 1 :
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::ScilabError(_W("Invalid exponent.\n"));
            default:
                //OK
                break;
        }
        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

}

int PowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->isScalar();
    bool bScalar2   = _pDouble2->isScalar();

    int iComplex = 1;

    if (bScalar1 && bScalar2)
    {
        //s ^ s
        *_pDoubleOut = new Double(1, 1, true);

        if (bComplex1 == false && bComplex2 == false)
        {
            iPowerRealScalarByRealScalar(_pDouble1->get(0), _pDouble2->get(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), &iComplex);
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iPowerRealScalarByComplexScalar(_pDouble1->get(0), _pDouble2->get(0), _pDouble2->getImg(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iPowerComplexScalarByRealScalar(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iPowerComplexScalarByComplexScalar(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(0), _pDouble2->getImg(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        if (iComplex == 0)
        {
            (*_pDoubleOut)->setComplex(false);
        }

        return 0;
    }
    else if (bScalar1 && _pDouble2->getDims() == 2)
    {
        //s ^ []
        *_pDoubleOut = new Double(_pDouble2->getRows(), _pDouble2->getCols(), true);

        if (bComplex1 == false && bComplex2 == false)
        {
            iPowerRealScalarByRealMatrix(
                _pDouble1->get(0),
                _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), &iComplex);
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iPowerRealScalarByComplexMatrix(
                _pDouble1->get(0),
                _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iPowerComplexScalarByRealMatrix(
                _pDouble1->get(0), _pDouble1->getImg(0),
                _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iPowerComplexScalarByComplexMatrix(
                _pDouble1->get(0), _pDouble1->getImg(0),
                _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        if (iComplex == 0)
        {
            (*_pDoubleOut)->setComplex(false);
        }

        return 0;
    }

    if (bScalar2 && _pDouble1->getDims() == 2 && _pDouble1->isVector() )
    {
        //_pDouble1 is a vector and _pDouble is a scalar
        *_pDoubleOut = new Double(_pDouble1->getRows(), _pDouble1->getCols() , true);

        if (bComplex1 == false && bComplex2 == false)
        {
            for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
            {
                iPowerRealScalarByRealScalar(
                    _pDouble1->get(i),
                    _pDouble2->get(0),
                    &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
            {
                iPowerRealScalarByComplexScalar(
                    _pDouble1->get(i),
                    _pDouble2->get(0), _pDouble2->getImg(0),
                    &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
            {
                iPowerComplexScalarByRealScalar(
                    _pDouble1->get(i), _pDouble1->getImg(i),
                    _pDouble2->get(0),
                    &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
            {
                iPowerComplexScalarByComplexScalar(
                    _pDouble1->get(i), _pDouble1->getImg(i),
                    _pDouble2->get(0), _pDouble2->getImg(0),
                    &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
            }
        }

        if (iComplex == 0)
        {
            (*_pDoubleOut)->setComplex(false);
        }

        return 0;
    }

    if (bScalar2 && ( _pDouble1->getRows() == _pDouble1->getCols()))
    {
        //power of a square matrix by a scalar exponent.
        int iRet = 0;
        if (bComplex2)
        {
            //mange by overloading
            return 0;
        }

        *_pDoubleOut = new Double(_pDouble1->getRows(), _pDouble1->getCols() , true);
        if (bComplex1 == false)
        {
            iRet = iPowerRealSquareMatrixByRealScalar(
                       _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
                       _pDouble2->get(0),
                       (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), &iComplex);
        }
        else if (bComplex1 == true)
        {
            iRet = iPowerComplexSquareMatrixByRealScalar(
                       _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                       _pDouble2->get(0),
                       (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        // call overload
        if (iRet == -1)
        {
            delete *_pDoubleOut;
            *_pDoubleOut = NULL;
            return 0;
        }

        if (iComplex == 0)
        {
            (*_pDoubleOut)->setComplex(false);
        }
    }
    return 0;
}

int PowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, InternalType** _pOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bComplex2  = _pDouble->isComplex();
    bool bScalar1   = _pPoly->isScalar();
    bool bScalar2   = _pDouble->isScalar();

    int iComplex = 1;

    if (bComplex2)
    {
        //invalid exponent.
        return 2;
    }

    if (_pDouble->isEmpty())
    {
        //p ** []
        *_pOut = Double::Empty();
        return 0;
    }

    if (bScalar1)
    {
        //p ^ x or p ^ X
        int iRank   = 0;
        int* piRank = new int[_pDouble->getSize()];

        _pPoly->getRank(&iRank);
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            int iInputRank = (int)_pDouble->get(i);
            if (iInputRank < 0)
            {
                //call overload
                _pOut = NULL;
                delete[] piRank;
                return 0;
            }

            piRank[i] = ((iRank - 1) * iInputRank) + 1;
        }

        Polynom* pOut = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pDouble->getCols(), piRank);
        pOut->setComplex(bComplex1);

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            Double* pCoeffOut = pOut->get(i)->getCoef();

            int iCurrentRank    = 0;
            int iLoop           = (int)_pDouble->get(i);

            //initialize Out to 1
            pCoeffOut->set(0, 1);
            //get a copy of p
            Polynom* pP = _pPoly->clone()->getAs<Polynom>();
            pP->setComplex(_pPoly->isComplex());

            while (iLoop)
            {
                if (iLoop % 2)
                {
                    int iRank = pP->getMaxRank() - 1;
                    if (bComplex1)
                    {
                        C2F(wpmul1)(pCoeffOut->get(), pCoeffOut->getImg(), &iCurrentRank, pP->getCoef()->get(), pP->getCoef()->getImg(), &iRank, pCoeffOut->get(), pCoeffOut->getImg());
                    }
                    else
                    {
                        C2F(dpmul1)(pCoeffOut->get(), &iCurrentRank, pP->getCoef()->get(), &iRank, pCoeffOut->get());
                    }
                    iCurrentRank += iRank;
                }

                iLoop /= 2;
                if (iLoop)
                {
                    //p = p * p
                    Polynom* pTemp = NULL;
                    MultiplyPolyByPoly(pP, pP, &pTemp);
                    delete pP;
                    pP = pTemp;
                }
            }
        }
        *_pOut = pOut;
    }
    return 0;
}

int DotPowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    int iResultComplex = 0;

    if (_pDouble1->isEmpty() || _pDouble2->isEmpty())
    {
        *_pDoubleOut = Double::Empty();
    }
    else if (_pDouble1->isIdentity())
    {
        return 1;
    }
    else if (_pDouble2->isIdentity())
    {
        *_pDoubleOut = dynamic_cast<Double*>(_pDouble1->clone());
    }
    else if (_pDouble1->isScalar())
    {
        //a .^ (b or B)
        *_pDoubleOut = new Double(_pDouble2->getDims() , _pDouble2->getDimsArray(), true);

        if (_pDouble1->isComplex())
        {
            double dblR1 = _pDouble1->get(0);
            double dblI1 = _pDouble1->getImg(0);

            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        dblR1, dblI1,
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        dblR1, dblI1,
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
        }
        else
        {
            double dblR1 = _pDouble1->get(0);
            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        dblR1,
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    int iComplex = 1;
                    iPowerRealScalarByRealScalar(
                        dblR1,
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                    iResultComplex |= iComplex;
                }
            }
        }
    }
    else if (_pDouble2->isScalar())
    {
        //A .^ b
        *_pDoubleOut = new Double(_pDouble1->getDims() , _pDouble1->getDimsArray(), true);
        if (_pDouble1->isComplex())
        {
            double dblR2 = _pDouble2->get(0);
            if (_pDouble2->isComplex())
            {
                double dblI2 = _pDouble2->getImg(0);
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        dblR2, dblI2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                double dblR2 = _pDouble2->get(0);
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        dblR2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
        }
        else
        {
            double dblR2 = _pDouble2->get(0);
            if (_pDouble2->isComplex())
            {
                double dblI2 = _pDouble2->getImg(0);
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        _pDouble1->get(i),
                        dblR2, dblI2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    int iComplex = 1;
                    iPowerRealScalarByRealScalar(
                        _pDouble1->get(i),
                        dblR2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                    iResultComplex |= iComplex;
                }
            }
        }
    }
    else
    {
        //A .^ B
        //check dimension compatibilities ( same number of dimension and same size for each dimension
        int iDims1      = _pDouble1->getDims();
        int* piDims1    = _pDouble1->getDimsArray();
        int iDims2      = _pDouble2->getDims();
        int* piDims2    = _pDouble2->getDimsArray();

        if (iDims1 != iDims2)
        {
            return 1;
        }

        for (int i = 0 ; i < iDims1 ; i++)
        {
            if (piDims1[i] != piDims2[i])
            {
                return 1;
            }
        }

        (*_pDoubleOut) = new Double(iDims2, piDims2, true);

        if (_pDouble1->isComplex())
        {
            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
        }
        else
        {
            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        _pDouble1->get(i),
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    int iComplex = 1;
                    iPowerRealScalarByRealScalar(
                        _pDouble1->get(i),
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                    iResultComplex |= iComplex;
                }
            }
        }
    }

    if (iResultComplex == 0)
    {
        (*_pDoubleOut)->setComplex(false);
    }
    return 0;
}

