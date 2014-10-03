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
#include <sstream>
#include <math.h>
#include "singlepoly.hxx"
#include "double.hxx"
#include "tostring_common.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "log.h"
#include "exp.h"
#include "elem_common.h"
#include "matrix_addition.h"
}

using namespace std;

namespace types
{

SinglePoly::SinglePoly()
{
    double* pdblCoefR = NULL;
    int piDims[2] = {1, 1};
    create(piDims, 2, &pdblCoefR, NULL);
    pdblCoefR[0] = 0;
    m_szVarName = L"";
}

SinglePoly::SinglePoly(std::wstring _szVarName)
{
    double* pdblCoefR = NULL;
    int piDims[2] = {1, 1};
    create(piDims, 2, &pdblCoefR, NULL);
    pdblCoefR[0] = 0;
    m_szVarName = _szVarName;
}

SinglePoly::SinglePoly(std::wstring _szVarName, double** _pdblCoefR, int _iRank)
{
    int piDims[2] = {_iRank + 1, 1};
    m_szVarName = _szVarName;
    create(piDims, 2, _pdblCoefR, NULL);
}

SinglePoly::SinglePoly(const SinglePoly& rightSide)
{
    m_iSize = rightSide.m_iSize;
    m_iRows = rightSide.m_iRows;
    m_iCols = rightSide.m_iCols;
    m_iDims = rightSide.m_iDims;
    m_szVarName = rightSide.m_szVarName;

    memcpy(m_piDims, rightSide.m_piDims, m_iDims * sizeof(int));

    if (rightSide.m_pRealData)
    {
        m_pRealData = new double[m_iSize];
        memcpy(m_pRealData, rightSide.m_pRealData, m_iDims * sizeof(double));
    }
}

SinglePoly::~SinglePoly()
{
    deleteAll();
}

void SinglePoly::deleteAll()
{
    if (m_pRealData)
    {
        delete[] m_pRealData;
        m_pRealData = NULL;
    }
}

void SinglePoly::deleteImg()
{
}

double SinglePoly::getNullValue()
{
    return 0;
}

SinglePoly* SinglePoly::createEmpty(int /*_iDims*/, int* _piDims, bool _bComplex)
{
    double* pdblData = NULL;
    SinglePoly* pSP = new SinglePoly(m_szVarName, &pdblData, _piDims[0] - 1);
    return pSP;
}

double* SinglePoly::allocData(int _iSize)
{
    double* pDbl = NULL;
    try
    {
        if (_iSize < 0)
        {
            m_pRealData = NULL;
            m_pImgData = NULL;
            char message[bsiz];
            sprintf(message, _("Can not allocate negative size (%d).\n"),  _iSize);
            ast::ScilabError se(message);
            se.SetErrorNumber(999);
            throw (se);
        }
        else
        {
            pDbl = new double[_iSize];
        }
    }
    catch (std::bad_alloc &/*e*/)
    {
        char message[bsiz];
        sprintf(message, _("Can not allocate %.2f MB memory.\n"),  (double) (_iSize * sizeof(double)) / 1.e6);
        ast::ScilabError se(message);
        se.SetErrorNumber(999);
        throw (se);
    }

    return pDbl;
}

double SinglePoly::copyValue(double _dblData)
{
    return _dblData;
}

int SinglePoly::getRank()
{
    return m_iSize - 1;
}

bool SinglePoly::setRank(int _iRank, bool bSave)
{
    double *pR = NULL;
    double *pI = NULL;
    if (bSave == false)
    {
        if (getRank() != _iRank)
        {
            int piDims[2] = {_iRank + 1, 1};
            deleteAll();
            create(piDims, 2, &pR, NULL);
            return true;
        }

        return true;
    }
    else
    {
        double* pdblOldReal = m_pRealData;
        int iMinSize = Min(m_iSize, _iRank + 1);
        int piDims[2] = {_iRank + 1, 1};
        create(piDims, 2, &pR, NULL);
        memcpy(m_pRealData, pdblOldReal, iMinSize * sizeof(double));
        delete[] pdblOldReal;
        pdblOldReal = NULL;

        return true;
    }

    return false;
}

bool SinglePoly::setZeros()
{
    if (m_pRealData != NULL)
    {
        memset(m_pRealData, 0x00, m_iSize * sizeof(double));
    }
    else
    {
        return false;
    }

    return true;
}

bool SinglePoly::setCoef(double* _pdblCoefR)
{
    if (_pdblCoefR != NULL)
    {
        memcpy(m_pRealData, _pdblCoefR, m_iSize * sizeof(double));
    }

    return true;
}

void SinglePoly::whoAmI()
{
    std::cout << "types::SinglePoly";
}

bool SinglePoly::evaluate(double _dblInR, double _dblInI, SinglePoly* spImg, double *_pdblOutR, double *_pdblOutI)
{
    *_pdblOutR = 0;
    *_pdblOutI = 0;

    if (m_iSize == 0)
    {
        return true;
    }

    if (spImg)
    {
        double* pImgData = spImg->get();
        for (int i = 0 ; i < m_iSize ; i++)
        {
            //real part
            *_pdblOutR += m_pRealData[i] * pow(_dblInR, i);

            //only if variable is complex
            *_pdblOutR -= pImgData[i] * pow(_dblInI, i);
            //img part
            *_pdblOutI += m_pRealData[i] * pow(_dblInR, i);

            *_pdblOutI += m_pRealData[i] * pow(_dblInI, i);
        }
    }
    else
    {
        for (int i = 0 ; i < m_iSize ; i++)
        {
            *_pdblOutR += m_pRealData[i] * pow(_dblInR, i);
            *_pdblOutI += m_pRealData[i] * pow(_dblInI, i);
        }
    }

    return true;
}

void SinglePoly::updateRank(void)
{
    double dblEps = getRelativeMachinePrecision();
    int iNewRank = getRank();
    for (int i = getRank(); i > 0 ; i--)
    {
        if (fabs(m_pRealData[i]) <= dblEps)
        {
            iNewRank--;
        }
        else
        {
            break;
        }
    }

    if (iNewRank < getRank())
    {
        setRank(iNewRank, true);
    }
}

bool SinglePoly::toString(std::wostringstream& ostr)
{
    ostr << L"FIXME : implement SinglePoly::toString" << std::endl;
    return true;
}

void SinglePoly::toStringReal(wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
{
    toStringInternal(m_pRealData, _szVar, _pListExp, _pListCoef);
}

bool SinglePoly::subMatrixToString(wostringstream& /*ostr*/, int* /*_piDims*/, int /*_iDims*/)
{
    return false;
}

void SinglePoly::toStringInternal(double *_pdblVal, wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
{
    int iLineLen = ConfigVariable::getConsoleWidth();

    wostringstream ostemp;
    wostringstream ostemp2;

    ostemp << L" ";
    ostemp2 << L" ";

    int iLen = 0;
    int iLastFlush = 2;
    for (int i = 0 ; i < m_iSize ; i++)
    {
        if (isRealZero(_pdblVal[i]) == false)
        {
            DoubleFormat df;
            getDoubleFormat(_pdblVal[i], &df);

            if (iLen + df.iWidth + df.iSignLen >= iLineLen - 1)
            {
                iLastFlush = i;
                _pListExp->push_back(ostemp2.str());
                ostemp2.str(L""); //reset stream
                addSpaces(&ostemp2, 11); //take from scilab ... why not ...

                _pListCoef->push_back(ostemp.str());
                ostemp.str(L""); //reset stream
                addSpaces(&ostemp, 11); //take from scilab ... why not ...
            }

            bool bFirst = ostemp.str().size() == 1;

            // In scientific notation case bExp == true, so we have to print point (2.000D+10s)
            // In other case don't print point (2s)
            df.bPrintPoint = df.bExp;
            df.bPrintPlusSign = bFirst == false;
            df.bPrintOne = i == 0;
            addDoubleValue(&ostemp, _pdblVal[i], &df);

            if (i == 0)
            {
                iLen = static_cast<int>(ostemp.str().size());
            }
            else if (i == 1)
            {
                // add polynom name
                ostemp << _szVar;
                iLen = static_cast<int>(ostemp.str().size());
            }
            else
            {
                // add polynom name and exponent
                ostemp << _szVar;
                iLen = static_cast<int>(ostemp.str().size());
                addSpaces(&ostemp2, iLen - static_cast<int>(ostemp2.str().size()));
                ostemp2 << i;
                int iSize = static_cast<int>(ostemp2.str().size()) - iLen;
                addSpaces(&ostemp, iSize);
            }
        }
    }

    if (iLastFlush != 0)
    {
        if (ostemp.str() == L" ")
        {
            ostemp << L"  0";
            addSpaces(&ostemp2, static_cast<int>(ostemp.str().size()));
        }

        if (ostemp2.str() == L" ")
        {
            // -1 because ostemp2 have already a space
            addSpaces(&ostemp2, static_cast<int>(ostemp.str().size()) - 1);
        }

        _pListExp->push_back(ostemp2.str());
        _pListCoef->push_back(ostemp.str());
    }

    return;
}

SinglePoly* SinglePoly::clone()
{
    SinglePoly* pPoly = NULL;
    double *pR = NULL;
    pPoly = new SinglePoly(m_szVarName, &pR, getRank());
    pPoly->setCoef(m_pRealData);
    return pPoly;
}

//SinglePoly SinglePoly::conjugate()
//{
//    if (isComplex())
//    {
//        double *pR = NULL;
//        double *pI = NULL;
//        SinglePoly pPoly(&pR, &pI, getRank());
//
//        Transposition::conjugate(m_iSize, m_pRealData, pR, m_pImgData, pI);
//
//        return pPoly;
//    }
//    else
//    {
//        return *this;
//    }
//}

wstring SinglePoly::getVariableName()
{
    return m_szVarName;
}

void SinglePoly::setVariableName(wstring _szVarName)
{
    m_szVarName = _szVarName;
}

SinglePoly& operator+(const SinglePoly& _lhs, const SinglePoly& _rhs)
{
    SinglePoly& spLhs = const_cast<SinglePoly &>(_lhs);
    SinglePoly& spRhs = const_cast<SinglePoly &>(_rhs);

    if (spLhs.getVariableName() != spRhs.getVariableName())
    {
        std::wostringstream os;
        os << _W("variables don't have the same formal variable");
        //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
        throw ast::ScilabError(os.str());
    }

    int iRankL = spLhs.getRank();
    int iRankR = spRhs.getRank();
    int iRank  = std::max(iRankL, iRankR);

    double* pdblCoeff = NULL;
    SinglePoly* spOut = new SinglePoly(spLhs.getVariableName(), &pdblCoeff, iRank);

    iAddScilabPolynomToScilabPolynom(
        spLhs.get(), iRankL + 1,
        spRhs.get(), iRankR + 1,
        pdblCoeff, iRank + 1);

    return *spOut;
}

SinglePoly& operator+(const SinglePoly& _lhs, const double _rhs)
{
    return _rhs + _lhs;
}

SinglePoly& operator+(const double _lhs, const SinglePoly& _rhs)
{
    SinglePoly& spRhs = const_cast<SinglePoly &>(_rhs);
    SinglePoly* spOut = new SinglePoly(spRhs);

    spOut->get()[0] += _lhs;

    return *spOut;
}

SinglePoly& operator*(const SinglePoly& _lhs, const SinglePoly& _rhs)
{
    SinglePoly& spLhs = const_cast<SinglePoly &>(_lhs);
    SinglePoly& spRhs = const_cast<SinglePoly &>(_rhs);

    if (spLhs.getVariableName() != spRhs.getVariableName())
    {
        std::wostringstream os;
        os << _W("variables don't have the same formal variable");
        //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
        throw ast::ScilabError(os.str());
    }

    int iRankL = spLhs.getRank();
    int iRankR = spRhs.getRank();
    int iRankOut = iRankL + iRankR;

    double* pdblOutR = NULL;
    double* pdblLR = spLhs.get();
    double* pdblRR = spRhs.get();

    SinglePoly* pOut = new SinglePoly(&pdblOutR, iRankOut);
    memset(pdblOutR, 0x00, sizeof(double) * (iRankOut + 1));

    for (int i = 0 ; i < iRankL + 1 ; ++i)
    {
        for (int j = 0 ; j < iRankR + 1 ; ++j)
        {
            pdblOutR[i + j]  += pdblLR[i] * pdblRR[j];
        }
    }

    return *pOut;
}

SinglePoly& SinglePoly::operator=(const SinglePoly& rightSide)
{
    if (this != &rightSide)
    {
        deleteAll();

        m_iSize = rightSide.m_iSize;
        m_iRows = rightSide.m_iRows;
        m_iCols = rightSide.m_iCols;
        m_iDims = rightSide.m_iDims;
        m_szVarName = rightSide.m_szVarName;
        memcpy(m_piDims, rightSide.m_piDims, m_iDims * sizeof(int));

        if (rightSide.m_pRealData)
        {
            m_pRealData = new double[m_iSize];
            memcpy(m_pRealData, rightSide.m_pRealData, m_iDims * sizeof(double));
        }
    }

    return *this;
}

SinglePoly& SinglePoly::operator=(SinglePoly && rightSide)
{
    if (this != &rightSide)
    {
        deleteAll();

        m_iSize = rightSide.m_iSize;
        m_iRows = rightSide.m_iRows;
        m_iCols = rightSide.m_iCols;
        m_iDims = rightSide.m_iDims;
        m_szVarName = rightSide.m_szVarName;

        // m_piDims is a static allocation
        // swap this two array to avoid copy data
        // rightSide will be delete after assignation
        std::swap(m_piDims, rightSide.m_piDims);

        // move data from right to left side without copy
        m_pRealData = std::move(rightSide.m_pRealData);
        rightSide.m_pRealData = NULL;
    }

    return *this;
}

bool operator==(const SinglePoly& _lhs, const InternalType& it)
{
    if (const_cast<InternalType &>(it).isSinglePoly() == false)
    {
        return false;
    }

    SinglePoly& lhs = const_cast<SinglePoly &>(_lhs);
    SinglePoly& rhs = const_cast<SinglePoly &>((SinglePoly&)it);

    if (lhs.getVariableName() != rhs.getVariableName())
    {
        return false;
    }

    if (lhs.getRank() != rhs.getRank())
    {
        return false;
    }

    double *pdblRealRhs = rhs.get();
    double *pdblRealLhs = lhs.get();
    for (int i = 0 ; i < lhs.getSize() ; i++)
    {
        if (pdblRealLhs[i] != pdblRealRhs[i])
        {
            return false;
        }
    }

    return true;
}

bool operator==(const SinglePoly& _lhs, const double _rhs)
{
    SinglePoly& lhs = const_cast<SinglePoly &>(_lhs);
    if (lhs.getRank() > 0)
    {
        return false;
    }

    if (lhs.get(0) == _rhs)
    {
        return true;
    }

    return false;
}

bool operator==(const double _lhs, const SinglePoly& _rhs)
{
    return _rhs == _lhs;
}

bool operator!=(const SinglePoly& _lhs, const double _rhs)
{
    return !(_lhs == _rhs);
}

bool operator!=(const double _lhs, const SinglePoly& _rhs)
{
    return !(_rhs == _lhs);
}

bool operator!(const SinglePoly& _lhs)
{
    return _lhs == 0;
}

bool SinglePoly::operator!=(const InternalType& it)
{
    return !(*this == it);
}

}
