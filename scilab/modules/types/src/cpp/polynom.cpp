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
#include "core_math.h"
#include "tostring_common.hxx"
#include "singlepoly.hxx"
#include "polynom.hxx"

using namespace std;

namespace types
{
    Polynom::Polynom()
    {
    }

    Polynom::Polynom(wstring _szVarName, int _iRows, int _iCols, int *_piRank)
    {
        int piDims[2]   = {_iRows, _iCols};
        m_szVarName	    = _szVarName;
        m_bComplex	    = false;
        SinglePoly** pPoly    = NULL;
        create(piDims, 2, &pPoly, NULL);
        for(int i = 0 ; i < getSize() ; i++)
        {
            double* pReal = NULL;
            m_pRealData[i] = new SinglePoly(&pReal, _piRank[i]);
        }
    }

    Polynom::Polynom(wstring _szVarName, int _iRows, int _iCols, const int *_piRank)
    {
        int piDims[2]   = {_iRows, _iCols};
        m_szVarName	    = _szVarName;
        m_bComplex	    = false;
        SinglePoly** pPoly     = NULL;

        create(piDims, 2, &pPoly, NULL);
        for(int i = 0 ; i < getSize() ; i++)
        {
            double* pReal = NULL;
            m_pRealData[i] = new SinglePoly(&pReal, _piRank[i]);
        }
    }

    Polynom::Polynom(std::wstring _szVarName, int _iDims, int* _piDims, const int *_piRank)
    {
        m_szVarName	= _szVarName;
        m_bComplex	= false;
        SinglePoly** pPoly = NULL;
        create(_piDims, _iDims, &pPoly, NULL);

        if(_piRank != 0)
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                double* pReal = NULL;
                m_pRealData[i] = new SinglePoly(&pReal, _piRank[i]);
            }
        }
        else
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                double* pReal = NULL;
                m_pRealData[i] = new SinglePoly(&pReal, 0);
            }
        }
    }

    Polynom::~Polynom()
    {
        if(isDeletable() == true)
        {
            deleteAll();
        }
    }

    bool Polynom::setCoef(int _iRows, int _iCols, Double *_pdblCoef)
    {
        int piDims[] = {_iRows, _iCols};
        int iPos = getIndex(piDims);
        return setCoef(iPos, _pdblCoef);
    }

    bool Polynom::setCoef(int _iIdx, Double *_pdblCoef)
    {
        if(_iIdx < getSize())
        {
            /*Get old SinglePoly*/
            SinglePoly *poPoly = get(_iIdx);
            poPoly->setRank(_pdblCoef->getSize());
            poPoly->setCoef(_pdblCoef);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool Polynom::getRank(int *_piRank)
    {
        if(_piRank == NULL || m_pRealData == NULL)
        {
            return false;
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            _piRank[i] = m_pRealData[i]->getRank();
        }
        return true;
    }

    GenericType::RealType Polynom::getType()
    {
        return RealPoly;
    }

    void Polynom::whoAmI(void)
    {
        std::cout << "types::SinglePoly";
    }

    wstring Polynom::getVariableName()
    {
        return m_szVarName;
    }

    void Polynom::setComplex(bool _bComplex)
    {
        if(_bComplex != m_bComplex)
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                get(i)->setComplex(_bComplex);
            }
            m_bComplex = _bComplex;
        }
    }

    InternalType* Polynom::clone()
    {
        int* piRank = new int[getSize()];
        getRank(piRank);

        Polynom* pMP = new Polynom(getVariableName(), getDims(), getDimsArray(), piRank);
        pMP->setComplex(isComplex());
        for(int i = 0 ; i < getSize() ; i++)
        {
            pMP->set(i, get(i));
        }

        delete piRank;
        return pMP;
    }

    Double* Polynom::evaluate(Double* _pdblValue)
    {
        double *pR	= _pdblValue->getReal();
        double *pI	= _pdblValue->getImg();
        int iRows		= _pdblValue->getRows();
        int iCols		= _pdblValue->getCols();

        double *pReturnR	= NULL;
        double *pReturnI	= NULL;
        Double *pReturn		= new Double(getRows() * iRows, getCols() * iCols, &pReturnR, &pReturnI);
        if(pI != NULL)
        {
            pReturn->setComplex(true);
        }
        else
        {
            pReturn->setComplex(false);
        }

        int i = 0;
        //all lines of the matrix remplacement
        for(int iCol = 0 ; iCol < iCols ; iCol++)
        {
            for(int iPolyCol = 0 ; iPolyCol < getCols() ; iPolyCol++)
            {
                for(int iRow = 0 ; iRow < iRows ; iRow++)
                {
                    for(int iPolyRow = 0 ; iPolyRow < getRows() ; iPolyRow++)
                    {
                        double OutR	= 0;
                        double OutI	= 0;

                        SinglePoly *pPoly = get(iPolyRow, iPolyCol);
                        if(pReturn->isComplex())
                        {
                            pPoly->evaluate(pR[iCol * iRows + iRow], pI[iCol * iRows + iRow], &OutR, &OutI);
                            pReturnR[i]	= OutR;
                            pReturnI[i]	= OutI;
                        }
                        else
                        {
                            pPoly->evaluate(pR[iCol * iRows + iRow], 0, &OutR, &OutI);
                            pReturnR[i]	= OutR;
                        }
                        i++;
                    }
                }
            }
        }
        return pReturn;
    }

    void Polynom::updateRank(void)
    {
        for(int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = get(i);
            pPoly->updateRank();
        }
    }

    int Polynom::getMaxRank(void)
    {
        int *piRank = new int[getSize()];
        getRank(piRank);
        int iMaxRank = 0;
        for(int i = 0 ; i < getSize() ; i++)
        {
            iMaxRank = Max(iMaxRank, piRank[i]);
        }
        delete[] piRank;
        return iMaxRank;
    }

    Double* Polynom::getCoef(void)
    {
        int iMaxRank = getMaxRank();
        Double *pCoef = new Double(getRows(), getCols() * iMaxRank, false);
        if(isComplex())
        {
            pCoef->setComplex(true);
        }

        double *pCoefR	= pCoef->getReal();
        double *pCoefI	= pCoef->getImg();

        for(int iRank = 0 ; iRank < iMaxRank ; iRank++)
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                SinglePoly *pPoly	= get(i);
                if(iRank > pPoly->getRank())
                {
                    pCoefR[iRank * getSize() + i] = 0;
                    if(isComplex())
                    {
                        pCoefI[iRank * getSize() + i] = 0;
                    }
                }
                else
                {
                    double *pR	= pPoly->getCoef()->getReal();
                    double *pI	= pPoly->getCoef()->getImg();

                    pCoefR[iRank * getSize() + i] = pR[iRank];
                    if(isComplex())
                    {
                        pCoefI[iRank * getSize() + i] = pI[iRank];
                    }
                }
            }
        }
        return pCoef;
    }

    void Polynom::setCoef(Double *_pCoef)
    {
        int iMaxRank = getMaxRank();

        setComplex(_pCoef->isComplex());
        double *pR = _pCoef->getReal();
        double *pI = _pCoef->getImg();
        for(int i = 0 ; i < getSize() ; i++)
        {
            Double *pTemp = new Double(1, iMaxRank, _pCoef->isComplex());
            SinglePoly *pPoly = get(i);
            for(int iRank = 0 ; iRank < iMaxRank ; iRank++)
            {
                pTemp->getReal()[iRank] = pR[iRank * getSize() + i];
            }
            if(isComplex())
            {
                for(int iRank = 0 ; iRank < iMaxRank ; iRank++)
                {
                    pTemp->getImg()[iRank] = pI[iRank * getSize() + i];
                }
            }

            pPoly->setCoef(pTemp);
        }
    }

    void Polynom::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
        wostringstream osExp;
        wostringstream osCoef;

        list<wstring>::const_iterator it_Exp;
        list<wstring>::const_iterator it_Coef;
        list<wstring> listExpR, listCoefR, listExpI, listCoefI;

        if(isScalar())
        {
            if(isComplex())
            {
                ostr << L"Real part" << endl << endl << endl;
                get(0)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
                {
                    ostr << *it_Exp << endl << *it_Coef << endl;
                }

                ostr << L"Imaginary part" << endl << endl << endl ;
                get(0)->toStringImg(_iPrecision, _iLineLen, getVariableName(), &listExpI, &listCoefI);
                for(it_Coef = listCoefI.begin(), it_Exp = listExpI.begin() ; it_Coef != listCoefI.end() ; it_Coef++,it_Exp++)
                {
                    ostr << *it_Exp << endl << *it_Coef << endl;
                }
            }
            else
            {
                get(0)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);

                for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
                {
                    ostr << *it_Exp << endl << *it_Coef << endl;
                }
            }
        }
        else if(getRows() == 1)
        {
            if(isComplex())
            {
                ostr << L"Real part" << endl << endl;
                ostr << getRowString(_piDims, _iDims, _iPrecision, _iLineLen, false);
                ostr << L"Imaginary part" << endl << endl;
                ostr << getRowString(_piDims, _iDims, _iPrecision, _iLineLen, true);
            }
            else
            {
                ostr << getRowString(_piDims, _iDims, _iPrecision, _iLineLen, false);
            }
        }
        else if(getCols() == 1)
        {
            if(isComplex())
            {
                ostr << L"Real part" << endl << endl;
                ostr << getColString(_piDims, _iDims, _iPrecision, _iLineLen, false);
                ostr << L"Imaginary part" << endl << endl;
                ostr << getColString(_piDims, _iDims, _iPrecision, _iLineLen, true);
            }
            else
            {
                ostr << getColString(_piDims, _iDims, _iPrecision, _iLineLen, false);
            }
        }
        else
        {//Matrix
            if(isComplex())
            {
                ostr << L"Real part" << endl << endl;
                ostr << getMatrixString(_piDims, _iDims, _iPrecision, _iLineLen, false);
                ostr << L"Imaginary part" << endl << endl;
                ostr << getMatrixString(_piDims, _iDims, _iPrecision, _iLineLen, true);
            }
            else
            {
                ostr << getMatrixString(_piDims, _iDims, _iPrecision, _iLineLen, false);
            }
        }
        ostr << endl;
    }

    wstring Polynom::getMatrixString(int* _piDims, int _iDims, int _iPrecision, int _iLineLen, bool _bComplex)
    {
        wostringstream ostr;
        wostringstream osExp;
        wostringstream osCoef;

        list<wstring>::const_iterator it_Exp;
        list<wstring>::const_iterator it_Coef;
        list<wstring> listExpR, listCoefR, listExpI, listCoefI;

        int iLen        = 0;
        int iLastCol    = 0;
        bool bWordWarp  = false;

        wstring szExp, szCoef;

        int *piMaxLen = new int[getCols()];
        memset(piMaxLen, 0x00, sizeof(int) * getCols());

        //find the largest row for each col
        for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)
        {
            for(int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
            {
                // FIXME : iLen shadow previous declaration
                int iLen = 0;
                _piDims[0] = iRows1;
                _piDims[1] = iCols1;
                int iPos = getIndex(_piDims);
                if(_bComplex)
                {
                    get(iPos)->toStringImg(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                }
                else
                {
                    get(iPos)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                }

                if(listExpR.size() > 1)
                {
                    for(it_Exp = listExpR.begin() ; it_Exp != listExpR.end() ; it_Exp++)
                    {
                        iLen += static_cast<int>((*it_Exp).size());
                    }
                }
                else
                {
                    if(listExpR.front().size() != 0)
                    {
                        iLen = static_cast<int>(listExpR.front().size());
                    }
                    else
                    {
                        iLen = static_cast<int>(listCoefR.front().size());
                    }
                }
                piMaxLen[iCols1] = Min(Max(piMaxLen[iCols1], iLen), _iLineLen);
                listExpR.clear();
                listCoefR.clear();
            }

            //We know the length of the column

            if(static_cast<int>(iLen + piMaxLen[iCols1]) >= _iLineLen && iLen != 0)
            {//if the max length exceeded
                wostringstream ostemp;
                bWordWarp = true;
                for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                {
                    bool bMultiLine = false;
                    for(int iCols2 = iLastCol ; iCols2 < iCols1; iCols2++)
                    {
                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;

                        int iPos = getIndex(_piDims);
                        if(_bComplex)
                        {
                            get(iPos)->toStringImg(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                        }
                        else
                        {
                            get(iPos)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                        }

                        if(listCoefR.size() > 1)
                        {
                            for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
                            {
                                osExp << *it_Exp;
                                addSpaces(&osExp, piMaxLen[iCols2] - static_cast<int>((*it_Exp).size()));
                                osExp << endl;

                                osExp << *it_Coef;
                                addSpaces(&osExp, piMaxLen[iCols2] - static_cast<int>((*it_Coef).size()));
                                osExp << endl;
                                bMultiLine = true;
                            }
                        }
                        else
                        {
                            osExp << listExpR.front();
                            addSpaces(&osExp, piMaxLen[iCols2] - static_cast<int>(listExpR.front().size()));
                            osCoef << listCoefR.front();
                            addSpaces(&osCoef, piMaxLen[iCols2] - static_cast<int>(listCoefR.front().size()));
                            bMultiLine = false;
                        }
                        listExpR.clear();
                        listCoefR.clear();
                    }

                    if(bMultiLine == false)
                    {
                        osExp << endl;
                        osCoef << endl;
                    }
                    ostemp << osExp.str();
                    ostemp << osCoef.str() << endl;
                    osExp.str(L"");
                    osCoef.str(L"");

                }
                iLen	= piMaxLen[iCols1];

                //write "column x to y"
                if(iLastCol + 1 == iCols1)
                {
                    ostr << endl << L"         Column " << iCols1 << endl << endl;
                }
                else
                {
                    ostr << endl << L"         Column " << iLastCol + 1 << L" to " << iCols1 << endl << endl;
                }

                ostr << ostemp.str() << endl;

                iLastCol = iCols1;
            }
            else //if((int)(iLen + piMaxLen[iCols1]) <= _iLineLen)
            {
                iLen += piMaxLen[iCols1];
            }
        }//for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)

        if(bWordWarp)
        {
            if(iLastCol + 1 == getCols())
            {
                ostr << endl << L"         Column " << getCols() << endl << endl;
            }
            else
            {
                ostr << endl << L"         Column " << iLastCol + 1 << L" to " << getCols() << endl << endl;
            }
        }

        //print the end
        for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
        {
            for(int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
            {
                _piDims[0] = iRows2;
                _piDims[1] = iCols2;

                int iPos = getIndex(_piDims);
                if(_bComplex)
                {
                    get(iPos)->toStringImg(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                }
                else
                {
                    get(iPos)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
                }

                if(listCoefR.size() > 1)
                {
                    for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
                    {//normally useless ...
                        osExp << *it_Exp;
                        addSpaces(&osExp, piMaxLen[iCols2] - static_cast<int>((*it_Exp).size()));
                        osExp << endl;

                        osExp << *it_Coef;
                        addSpaces(&osExp, piMaxLen[iCols2] - static_cast<int>((*it_Coef).size()));
                        osExp << endl;
                    }
                }
                else
                {
                    if(listExpR.front().size() != 0)
                    {
                        osExp << listExpR.front();
                    }

                    addSpaces(&osExp, piMaxLen[iCols2] - static_cast<int>(listExpR.front().size()));
                    osCoef << listCoefR.front();
                    addSpaces(&osCoef, piMaxLen[iCols2] - static_cast<int>(listCoefR.front().size()));
                }
                listExpR.clear();
                listCoefR.clear();
            }

            if(osExp.str().size() != 0)
            {
                osExp << endl;
            }
            osCoef << endl;
            ostr << osExp.str();
            ostr << osCoef.str() << endl;
            osExp.str(L"");
            osCoef.str(L"");
        }
        return ostr.str();
    }

    wstring Polynom::getRowString(int* _piDims, int _iDims, int _iPrecision, int _iLineLen, bool _bComplex)
    {
        int iLen        = 0;
        int iLastFlush  = 0;

        wostringstream ostr;
        wostringstream osExp;
        wostringstream osCoef;

        list<wstring>::const_iterator it_Exp;
        list<wstring>::const_iterator it_Coef;
        list<wstring> listExpR, listCoefR, listExpI, listCoefI;

        for(int i = 0 ; i < getCols() ; i++)
        {
            wstring szExp, szCoef;

            _piDims[1] = 0;
            _piDims[0] = i;
            int iPos = getIndex(_piDims);
            if(_bComplex)
            {
                get(iPos)->toStringImg(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
            }
            else
            {
                get(iPos)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
            }

            if(iLen != 0 && static_cast<int>(iLen + listExpR.front().size()) > _iLineLen)
            {//flush strean
                if(i == iLastFlush + 1)
                {
                    ostr << endl << L"         Column " << i << endl << endl;
                }
                else
                {
                    ostr << endl << L"         Column " << iLastFlush + 1 /* 2 is better than 1, no ? */<< L" to " << i << endl << endl;
                }

                iLastFlush	= i;
                iLen				= 0;
                ostr << osExp.str() << endl;
                ostr << osCoef.str() << endl;
                osExp.str(L"");
                osCoef.str(L"");
            }

            if(listCoefR.size() > 1)
            {
                for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
                {
                    osExp << *it_Exp << endl << *it_Coef << endl;
                }
            }
            else
            {
                osExp << listExpR.front();
                osCoef << listCoefR.front();
            }

            if(osExp.str().size() != 0)
            {
                iLen = static_cast<int>(osExp.str().size());
            }
            else
            {
                iLen = static_cast<int>(osCoef.str().size());
            }

            listCoefR.clear();
            listExpR.clear();
        }

        if(iLastFlush != 0)
        {//last line of a multiline output
            if(iLastFlush + 1 == getSize())
            {
                ostr << endl << L"         Column " << getSize() << endl << endl;
            }
            else
            {
                ostr << endl << L"         Column " << iLastFlush + 1 << L" to " << getSize() << endl << endl;
            }
        }
        ostr << osExp.str() << endl;
        ostr << osCoef.str() << endl;
        return ostr.str();
    }

    wstring Polynom::getColString(int* _piDims, int _iDims, int _iPrecision, int _iLineLen, bool _bComplex)
    {
        wostringstream ostr;
        wostringstream osExp;
        wostringstream osCoef;

        list<wstring>::const_iterator it_Exp;
        list<wstring>::const_iterator it_Coef;
        list<wstring> listExpR, listCoefR, listExpI, listCoefI;

        for(int i = 0 ; i < getRows() ; i++)
        {
            wstring szExp, szCoef;

            _piDims[0] = i;
            _piDims[1] = 0;
            int iPos = getIndex(_piDims);
            if(_bComplex)
            {
                get(iPos)->toStringImg(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
            }
            else
            {
                get(iPos)->toStringReal(_iPrecision, _iLineLen, getVariableName(), &listExpR, &listCoefR);
            }

            for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
            {
                ostr << *it_Exp << endl << *it_Coef << endl;
            }
            ostr << endl;
            listCoefR.clear();
            listExpR.clear();
        }
        return ostr.str();
    }

    Double* Polynom::extractCoef(int _iRank)
    {
        Double *pdbl	= new Double(getRows(), getCols(), m_bComplex);
        double *pReal	= pdbl->getReal();
        double *pImg	= pdbl->getImg();

        for(int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = get(i);

            if(pPoly->getRank() <= _iRank)
            {
                pReal[i] = 0;
                if(m_bComplex)
                {
                    pImg[i]		= 0;
                }
            }
            else
            {
                pReal[i]		= pPoly->getCoef()->getReal()[_iRank];
                if(m_bComplex)
                {
                    pImg[i]		= pPoly->getCoef()->getImg()[_iRank];
                }
            }
        }

        return pdbl;
    }
    bool Polynom::insertCoef(int _iRank, Double* _pCoef)
    {
        double *pReal	= _pCoef->getReal();
        double *pImg	= _pCoef->getImg();

        for(int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = get(i);
            if(pPoly->getRank() <= _iRank)
            {
                return false;
            }

            pPoly->getCoef()->getReal()[_iRank] = pReal[i];
            if(m_bComplex)
            {
                pPoly->getCoef()->getImg()[_iRank] = pImg[i];
            }
        }
        return true;
    }

    bool Polynom::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isPoly() == false)
        {
            return false;
        }

        Polynom* pM = const_cast<InternalType &>(it).getAs<types::Polynom>();

        if(pM->getRows() != getRows() || pM->getCols() != getCols())
        {
            return false;
        }

        if(pM->isComplex() != isComplex())
        {
            return false;
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly* p1 = get(i);
            SinglePoly* p2 = pM->get(i);

            if(*p1 != *p2)
            {
                return false;
            }
        }
        return true;
    }

    bool Polynom::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    SinglePoly* Polynom::getNullValue()
    {
        double* pR = NULL;
        SinglePoly* pData = new SinglePoly(&pR, 1);
        pR[0] = 0;
        return pData;
    }

    Polynom* Polynom::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new Polynom(getVariableName(), _iDims, _piDims, NULL);
    }

    SinglePoly* Polynom::copyValue(SinglePoly* _pData)
    {
        if(_pData == NULL)
        {
            return NULL;
        }
        return _pData->clone();
    }

    void Polynom::deleteAll()
    {
        delete[] m_pRealData;
        m_pRealData = NULL;
        deleteImg();
    }

    void Polynom::deleteImg()
    {
    }

    SinglePoly** Polynom::allocData(int _iSize)
    {
        SinglePoly** pData = new SinglePoly*[_iSize];
        for(int i = 0 ; i < _iSize ; i++)
        {
            double* pReal;
            pData[i] = new SinglePoly(&pReal, 1);
            pReal[0] = 0;
        }
        return pData;
    }
}

