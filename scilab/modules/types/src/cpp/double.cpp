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
#include "double.hxx"
#include "tostring_common.hxx"
#include "scilabexception.hxx"

extern "C"
{
#include <stdio.h>
#include "elem_common.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_swprintf.h"
}

using namespace std;

namespace types
{
    Double* Double::Empty()
    {
        return new Double(0,0);
    }

    bool Double::isEmpty()
    {
        if(getDims() == 2 && getRows() == 0 && getCols() == 0)
        {
            return true;
        }
        return false;
    }

	Double::~Double()
	{
		if(isDeletable() == true)
		{
			deleteAll();
		}
#ifndef NDEBUG
        Inspector::removeItem(this);
#endif
	}

	Double::Double(int _iRows, int _iCols, bool _bComplex)
	{
        int piDims[2]   = {_iRows, _iCols};
		double *pReal   = NULL;
		double *pImg    = NULL;
		if(_bComplex == false)
		{
			create(piDims, 2, &pReal, NULL);
		}
		else
		{
			create(piDims, 2, &pReal, &pImg);
		}
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
	}

	Double::Double(double _dblReal)
	{
        m_piDims[0] = 1;
        m_piDims[1] = 1;
        m_iDims = 2;
        m_iRows = 1;
        m_iCols = 1;
        m_iSize = 1;
        m_iSizeMax = m_iSize;
        m_pRealData = new double[1];
        m_pRealData[0] = _dblReal;

  //      int piDims[2] = {1, 1};
		//double *pdblVal;
		//create(piDims, 2, &pdblVal, NULL);
		//pdblVal[0] = _dblReal;
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
	}

	Double::Double(double _dblReal, double _dblImg)
	{
        int piDims[2] = {1, 1};
		double *pdblR;
		double *pdblI;
		create(piDims, 2, &pdblR, &pdblI);
		pdblR[0] = _dblReal;
		pdblI[0] = _dblImg;
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
	}

	Double::Double(int _iRows, int _iCols, double **_pdblReal)
	{
        int piDims[2] = {_iRows, _iCols};
		create(piDims, 2, _pdblReal, NULL);
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
	}

	Double::Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg)
	{
        int piDims[2] = {_iRows, _iCols};
		create(piDims, 2, _pdblReal, _pdblImg);
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
	}

	Double::Double(int _iDims, int* _piDims, bool _bComplex)
    {
		double *pReal   = NULL;
		double *pImg	= NULL;
		if(_bComplex == false)
		{
			create(_piDims, _iDims, &pReal, NULL);
		}
		else
		{
			create(_piDims, _iDims, &pReal, &pImg);
		}
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
    }

	double*	Double::getReal() const
	{
		return get();
	}

	double Double::getReal(int _iRows, int _iCols)
	{
        return get(_iRows, _iCols);
	}

    bool Double::setInt(int* _piReal)
    {
        bool ret = true;
        for(int i = 0 ; i < m_iSize ; i++)
        {
            ret = set(i, static_cast<double>(_piReal[i]));
            if(ret == false)
            {
                return false;
            }
        }
		return true;
    }

	void Double::whoAmI()
	{
		std::cout << "types::Double";
	}

	GenericType::RealType Double::getType(void)
	{
		return RealDouble;
	}

	bool Double::setZeros()
	{
		if(m_pRealData != NULL)
		{
			memset(m_pRealData, 0x00, m_iSize * sizeof(double));
		}
		else
        {
			return false;
        }

		if(m_bComplex == true)
		{
			if(m_pImgData != NULL)
			{
				memset(m_pImgData, 0x00, m_iSize * sizeof(double));
			}
			else
            {
				return false;
            }
		}
		return true;
	}

	bool Double::setOnes()
	{
		if(m_pRealData != NULL)
        {
            std::fill(m_pRealData, m_pRealData + m_iSize, 1);
			//for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			//{
			//	m_pRealData[iIndex] = 1;
			//}
		}
		else
        {
			return false;
        }

		if(m_bComplex == true)
		{
			if(m_pImgData != NULL)
			{
                std::fill(m_pImgData, m_pImgData + m_iSize, 1);
			}
			else
            {
				return false;
            }
		}
		return true;
	}

    void Double::subMatrixToString(wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
        if(isIdentity())
        {
            ostr << L"eye *" << endl << endl;
            if(isComplex() == false)
            {
                int iWidth = 0, iPrec = 0;
                bool bFP = false; // FloatingPoint
                getDoubleFormat((m_pRealData[0]), _iPrecision, &iWidth, &iPrec, &bFP);
                addDoubleValue(&ostr, (m_pRealData[0]), iWidth, iPrec);
                ostr << endl;
            }
            else
            {//complex value
                int iWidthR = 0, iWidthI = 0;
                int iPrecR = 0, iPrecI = 0;
                bool bFPR = false, bFPI = false; // FloatingPoint
                getDoubleFormat(ZeroIsZero(m_pRealData[0]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
                getDoubleFormat(ZeroIsZero(m_pImgData[0]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
                addDoubleComplexValue(&ostr, ZeroIsZero(m_pRealData[0]), ZeroIsZero(m_pImgData[0]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                ostr << endl;
            }
            ostr << endl;
        }
        else if(isEmpty())
        {
            ostr << L"[]";
            ostr << endl;
        }
        else if(isScalar())
        {//scalar
            _piDims[0] = 0;
            _piDims[1] = 0;
            int iPos = getIndex(_piDims);

            if(isComplex() == false)
            {
                int iWidth = 0, iPrec = 0;
                bool bFP = false; // FloatingPoint
                getDoubleFormat((m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                addDoubleValue(&ostr, (m_pRealData[iPos]), iWidth, iPrec);
                ostr << endl;
            }
            else
            {//complex value
                int iWidthR = 0, iWidthI = 0;
                int iPrecR = 0, iPrecI = 0;
                bool bFPR = false, bFPI = false; // FloatingPoint
                getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidthR, &iPrecR, &bFPR);
                getDoubleFormat(ZeroIsZero(m_pImgData[iPos]), _iPrecision, &iWidthI, &iPrecI, &bFPI);
                addDoubleComplexValue(&ostr, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
            }
        }
        else if(getCols() == 1)
        {//column vector

            if(isComplex() == false)
            {
                for(int i = 0 ; i < getRows() ; i++)
                {
                    int iWidth = 0, iPrec = 0;
                    bool bFP = false; // FloatingPoint
                    _piDims[1] = 0;
                    _piDims[0] = i;
                    int iPos = getIndex(_piDims);

                    getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                    addDoubleValue(&ostr, ZeroIsZero(m_pRealData[iPos]), iWidth, iPrec);
                    ostr << endl;
                }
            }
            else
            {
                for(int i = 0 ; i < getRows() ; i++)
                {//complex value
                    int iWidthR = 0, iWidthI = 0;
                    int iPrecR = 0, iPrecI = 0;
                    bool bFPR = false, bFPI = false; // FloatingPoint

                    _piDims[1] = 0;
                    _piDims[0] = i;
                    int iPos = getIndex(_piDims);

                    getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidthR, &iPrecR, &bFPR);
                    getDoubleFormat(ZeroIsZero(m_pImgData[iPos]), _iPrecision, &iWidthI, &iPrecI, &bFPI);
                    addDoubleComplexValue(&ostr, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                    ostr << endl;
                }
            }
        }
        else if(getRows() == 1)
        {//row vector
            wostringstream ostemp;
            int iLastVal = 0;

            if(isComplex() == false)
            {
                for(int i = 0 ; i < getCols() ; i++)
                {
                    int iWidth = 0, iPrec = 0;
                    bool bFP = false; // FloatingPoint
                    int iLen = 0;

                    _piDims[0] = 0;
                    _piDims[1] = i;
                    int iPos = getIndex(_piDims);

                    getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                    iLen = iWidth + bFP + static_cast<int>(ostemp.str().size());
                    if(iLen > _iLineLen)
                    {//Max length, new line
                        ostr << endl << L"       column " << iLastVal + 1 << L" to " << i << endl << endl;
                        ostr << ostemp.str() << endl;
                        ostemp.str(L"");
                        iLastVal = i;
                    }

                    if(ostemp.str().size() != 0)
                    {
                        ostemp << SPACE_BETWEEN_TWO_VALUES;
                    }

                    addDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), iWidth, iPrec);
                }

                if(iLastVal != 0)
                {
                    ostr << endl << L"       column " << iLastVal + 1 << L" to " << getCols() << endl << endl;
                }
            }
            else //complex case
            {
                for(int i = 0 ; i < getCols() ; i++)
                {
                    int iWidthR = 0, iWidthI = 0;
                    int iPrecR = 0, iPrecI = 0;
                    bool bFPR = false, bFPI = false; // FloatingPoint
                    int iLen = 0;

                    _piDims[0] = 0;
                    _piDims[1] = i;
                    int iPos = getIndex(_piDims);

                    getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidthR, &iPrecR, &bFPR);
                    getDoubleFormat(ZeroIsZero(m_pImgData[iPos]), _iPrecision, &iWidthI, &iPrecI, &bFPI);

                    iLen = static_cast<int>(ostemp.str().size());
                    if(isRealZero(m_pImgData[iPos]))
                    {
                        if(isRealZero(m_pRealData[iPos]))
                        {
                            iLen += 1; //"0"
                        }
                        else
                        {
                            iLen		+= iWidthR + bFPR;
                            iWidthI	= 0;
                        }
                    }
                    else
                    {
                        if(isRealZero(m_pRealData[iPos]))
                        {
                            iLen		+= iWidthI + bFPI;
                            iWidthR	= 0;
                        }
                        else
                        {
                            iLen += iWidthR + bFPR;
                            iLen += SIZE_BETWEEN_REAL_COMPLEX;
                            iLen += iWidthI + bFPI;
                        }
                    }

                    if(iLen > _iLineLen)
                    {//Max length, new line
                        ostr << endl << L"       column " << iLastVal + 1 << L" to " << i << endl << endl;
                        ostr << ostemp.str() << endl;
                        ostemp.str(L"");
                        iLastVal = i;
                    }

                    if(ostemp.str().size() != 0)
                    {
                        ostemp << SPACE_BETWEEN_TWO_VALUES;
                    }

                    addDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                }

                if(iLastVal != 0)
                {
                    ostr << endl << L"       column " << iLastVal + 1 << L" to " << getCols() << endl << endl;
                }
            }
            ostemp << endl;
            ostr << ostemp.str();
        }
        else // matrix
        {
            wostringstream ostemp;
            int iLen = 0;
            int iLastCol = 0;

            //Array with the max printed size of each col
            int *piSize = new int[getCols()];
            memset(piSize, 0x00, getCols() * sizeof(int));

            if(isComplex() == false)
            {
                //compute the row size for padding for each printed bloc.
                for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)
                {
                    for(int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
                    {
                        int iWidth			= 0;
                        int iPrec				= 0;
                        bool bFP				= false; // FloatingPoint
                        int iCurrentLen = 0;

                        _piDims[0] = iRows1;
                        _piDims[1] = iCols1;
                        int iPos = getIndex(_piDims);
                        getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                        iCurrentLen	= iWidth + bFP;

                        if(iCurrentLen > piSize[iCols1])
                        {
                            piSize[iCols1] = iCurrentLen;
                        }
                    }

                    if(iLen + piSize[iCols1] > _iLineLen)
                    {//find the limit, print this part
                        for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                        {
                            for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                            {
                                int iWidth			= 0;
                                int iPrec				= 0;
                                bool bFP				= false; // FloatingPoint

                                _piDims[0] = iRows2;
                                _piDims[1] = iCols2;

                                int iPos = getIndex(_piDims);
                                getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                                addDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), piSize[iCols2], iPrec);
                                ostemp << SPACE_BETWEEN_TWO_VALUES;
                            }
                            ostemp << endl;
                        }
                        iLen = 0;
                        ostr << endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << endl << endl;;
                        ostr << ostemp.str();
                        ostemp.str(L"");
                        iLastCol = iCols1;

                    }
                    iLen += piSize[iCols1] + SIGN_LENGTH + SIZE_BETWEEN_TWO_VALUES;
                }

                for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                {
                    for(int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
                    {
                        int iWidth			= 0;
                        int iPrec				= 0;
                        bool bFP				= false; // FloatingPoint

                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;

                        int iPos = getIndex(_piDims);
                        getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                        addDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), piSize[iCols2], iPrec);
                        ostemp << SPACE_BETWEEN_TWO_VALUES;
                    }
                    ostemp << endl;
                }
                if(iLastCol != 0)
                {
                    ostr << endl << L"       column " << iLastCol + 1 << L" to " << getCols() << endl << endl;
                }
                ostr << ostemp.str();
            }
            else //Complex case
            {
                //compute the row size for padding for each printed bloc.
                for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)
                {
                    for(int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
                    {
                        int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
                        int iPrecR = 0, iPrecI = 0;
                        bool bFPR = false, bFPI = false; // FloatingPoint

                        _piDims[0] = iRows1;
                        _piDims[1] = iCols1;

                        int iPos = getIndex(_piDims);
                        getComplexFormat(	ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), _iPrecision,
                            &iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

                        iTotalWidth += (iWidthR == 0 ? 0 : SIGN_LENGTH) + (iWidthI == 0 ? 0 : SIGN_LENGTH + 1);
                        if(iTotalWidth > piSize[iCols1])
                        {
                            piSize[iCols1] = iTotalWidth;
                        }
                    }

                    if(iLen + piSize[iCols1] > _iLineLen)
                    {//find the limit, print this part
                        for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                        {
                            for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                            {
                                int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
                                int iPrecR = 0, iPrecI = 0;
                                bool bFPR = false, bFPI = false; // FloatingPoint

                                _piDims[0] = iRows2;
                                _piDims[1] = iCols2;

                                int iPos = getIndex(_piDims);
                                getComplexFormat(	ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), _iPrecision,
                                    &iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

                                addDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]),
                                    piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
                                ostemp << SPACE_BETWEEN_TWO_VALUES;
                            }
                            ostemp << endl;
                        }
                        iLen = 0;
                        ostr << endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << endl << endl;;
                        ostr << ostemp.str();
                        ostemp.str(L"");
                        iLastCol = iCols1;

                    }
                    iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_VALUES;
                }

                for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                {
                    for(int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
                    {
                        int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
                        int iPrecR = 0, iPrecI = 0;
                        bool bFPR = false, bFPI = false; // FloatingPoint

                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;

                        int iPos = getIndex(_piDims);
                        getComplexFormat(	ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), _iPrecision,
                            &iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

                        addDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]),
                            piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
                        ostemp << SPACE_BETWEEN_TWO_VALUES;
                    }
                    ostemp << endl;
                }

                if(iLastCol != 0)
                {
                    ostr << endl << L"       column " << iLastCol + 1 << L" to " << getCols() << endl << endl;
                }
                ostr << ostemp.str();
            }
        }
    }

	InternalType* Double::clone()
	{
		Double *pReturn = new Double(m_iDims, m_piDims, m_bComplex);
		memcpy(pReturn->getReal(), m_pRealData, m_iSize * sizeof(double));

		if(m_bComplex)
		{
			pReturn->setComplex(true);
			memcpy(pReturn->getImg(), m_pImgData, m_iSize * sizeof(double));
		}
		return pReturn;
	}

    bool Double::fillFromCol(int _iCols, Double *_poSource)
    {
        //blas
        int iDestOffset     = _iCols * m_iRows;
        int iSize           = _poSource->getSize();
        double* pdblDest    = m_pRealData + iDestOffset;
        int iOne            = 1;
        dcopy_(&iSize, _poSource->getReal(), &iOne, pdblDest, &iOne);

        if(m_bComplex)
        {
            pdblDest    = m_pImgData + iDestOffset;
            dcopy_(&iSize, _poSource->getImg(), &iOne, pdblDest, &iOne);
        }
        return true;
    }

    bool Double::fillFromRow(int _iRows, Double *_poSource)
    {
        int iCols = _poSource->getCols();

        if(m_bComplex)
        {
        }
        else
        {
            for(int i = 0 ; i < iCols ; i++)
            {
                int iDestOffset     = i * m_iRows + _iRows;
                int iOrigOffset     = i * _poSource->getRows();
                int iSize           = _poSource->getRows();
                double* pdblDest    = m_pRealData + iDestOffset;
                double* pdblSource  = _poSource->getReal() + iOrigOffset;
                int iOne            = 1;

                dcopy_(&iSize, pdblSource, &iOne, pdblDest, &iOne);
            }
        }
        return true;
    }

	bool Double::operator==(const InternalType& it)
	{
		if(const_cast<InternalType &>(it).isDouble() == false)
		{
			return false;
		}

		Double* pdbl = const_cast<InternalType &>(it).getAs<Double>();

        if(pdbl->getDims() != getDims())
        {
            return false;
        }

        for(int i = 0 ; i < getDims() ; i++)
        {
            if(pdbl->getDimsArray()[i] != getDimsArray()[i])
            {
                return false;
            }
        }

		if(pdbl->isComplex() != isComplex())
		{
			return false;
		}

		double *pdblReal = pdbl->getReal();

        for(int i = 0 ; i < getSize() ; i++)
        {
            if(m_pRealData[i] != pdblReal[i])
            {
                return false;
            }
        }

		if(isComplex())
		{
            double *pdblImg = pdbl->getImg();
            for(int i = 0 ; i < getSize() ; i++)
            {
                if(m_pImgData[i] != pdblImg[i])
                {
                    return false;
                }
            }
        }

        //if(memcmp(m_pRealData, pdblReal, getSize() * sizeof(double)) != 0)
		//{
		//	return false;
		//}

		//if(isComplex())
		//{
		//	double *pdblImg = pdbl->getImg();
		//	if(memcmp(m_pImgData, pdblImg, getSize() * sizeof(double)) != 0)
		//	{
		//		return false;
		//	}
		//}
		return true;
	}

	bool Double::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

    double Double::getNullValue()
    {
        return 0;
    }

    Double* Double::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new Double(_iDims, _piDims, _bComplex);
    }

    double Double::copyValue(double _dblData)
    {
        return _dblData;
    }

    void Double::deleteAll()
    {
        delete[] m_pRealData;
        m_pRealData = NULL;
        deleteImg();
    }

    void Double::deleteImg()
    {
        if(isComplex())
        {
            delete[] m_pImgData;
            m_pImgData = NULL;
        }
    }

    double* Double::allocData(int _iSize)
    {
        double* pDbl = NULL;
        try
        {
            if(_iSize < 0)
            {
                m_pRealData = NULL;m_pImgData = NULL;
                char message[bsiz];
                sprintf(message, _("Can not allocate negative size (%d).\n"),  _iSize);
                ast::ScilabError se(message);
                se.SetErrorNumber(999);
                throw(se);
            }
            else
            {
                pDbl = new double[_iSize];
            }
        }
        catch (std::bad_alloc &e)
        {
            char message[bsiz];
            sprintf(message, _("Can not allocate %.2f MB memory.\n"),  (double) (_iSize * sizeof(double)) / 1.e6);
            ast::ScilabError se(message);
            se.SetErrorNumber(999);
            throw(se);
        }
        return pDbl;
    }

    bool Double::append(int _iRows, int _iCols, InternalType* _poSource)
    {
        Double* pD = _poSource->getAs<Double>();
        int iRows = pD->getRows();
        int iCols = pD->getCols();
        int iSize = iRows * iCols;

        //insert without resize
        if(iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
        {
            return false;
        }

        //Update complexity if necessary
        setComplex(isComplex() || pD->isComplex());

        int iInc = 1;
        int iOne = 1;

        //two cases : 
        //  - append to 0,0
        //  - real append in x,y

        if(_iRows == 0 && _iCols == 0)
        {//append to 0,0
            //std::cout << "append 0,0" << std::endl;
            if(iRows == 1 || iRows == getRows())
            {
                //std::cout << "full Rows or one row" << std::endl;
                if(iRows == 1)
                {
                    iInc = getRows();
                    //std::cout << "iInc : " << iInc << std::endl;
                }
                
                if(isComplex())
                {
                    C2F(dcopy)(&iSize, pD->get(), &iOne, get(), &iInc);
                    if(pD->isComplex())
                    {
                        C2F(dcopy)(&iSize, pD->getImg(), &iOne, getImg(), &iInc);
                    }
                    else
                    {
                        memset(getImg(), 0x00, iSize * sizeof(double));
                    }
                }
                else
                {
                    C2F(dcopy)(&iSize, pD->get(), &iOne, get(), &iInc);
                }
            }
            else
            {
                //std::cout << "part of row" << std::endl;
                if(isComplex())
                {
                    for(int i = 0 ; i < iCols ; i++)
                    {
                        int iOffset = i * getRows();
                        C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + iOffset, &iOne);
                        if(pD->isComplex())
                        {
                            C2F(dcopy)(&iRows, pD->getImg() + i * iRows, &iOne, getImg() + iOffset, &iOne);
                        }
                        else
                        {
                            memset(getImg() + iOffset, 0x00, iRows * sizeof(double));
                        }
                    }
                }
                else
                {
                    for(int i = 0 ; i < iCols ; i++)
                    {
                        C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + i * getRows(), &iOne);
                    }
                }
            }
        }
        else if(_iRows == 0 || (_iCols == 0 && (iCols == 1 || iRows == 1)))
        {//real append in x,y
            //std::cout << "real append in x,y" << std::endl;
            if(iRows == 1)
            {
                iInc = getRows();
                //std::cout << "iInc : " << iInc << std::endl;
            }

            int iOffset =  _iCols * getRows() + _iRows;
            C2F(dcopy)(&iSize, pD->get(), &iOne, get() + iOffset, &iInc);

            if(isComplex())
            {
                int iOffset =  _iCols * getRows() + _iRows;
                C2F(dcopy)(&iSize, pD->get(), &iOne, get() + iOffset, &iInc);
                if(pD->isComplex())
                {
                    C2F(dcopy)(&iSize, pD->getImg(), &iOne, getImg() + iOffset, &iInc);
                }
                else
                {
                    memset(getImg() + iOffset, 0x00, iSize * sizeof(double));
                }
            }
        }
        else 
        {
            //std::cout << "no optimisation" << std::endl;
            if(isComplex())
            {
                for(int i = 0 ; i < iCols ; i++)
                {
                    int iOffset = (_iCols + i) * getRows() + _iRows;
                    C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + iOffset, &iOne);
                    if(pD->isComplex())
                    {
                        C2F(dcopy)(&iRows, pD->getImg() + i * iRows, &iOne, getImg() + iOffset, &iOne);
                    }
                    else
                    {
                        memset(getImg() + iOffset, 0x00, iRows * sizeof(double));
                    }
                }
            }
            else
            {
                for(int i = 0 ; i < iCols ; i++)
                {
                    C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + (_iCols + i) * getRows() + _iRows, &iOne);
                }
            }
        }
        return true;
    }
}
