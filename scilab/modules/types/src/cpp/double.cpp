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
#include "arrayof.hxx"
#include "tostring_common.hxx"

extern "C"
{
	#include "elem_common.h"
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
        if(m_iRows == 0 && m_iCols == 0)
        {
            return true;
        }
        return false;
    }

	Double::~Double()
	{
		if(isDeletable() == true)
		{
			delete_all();
		}
	}

	Double::Double(int _iRows, int _iCols, bool _bComplex)
	{
        int piDims[2] = {_iRows, _iCols};
		double *pReal = NULL;
		double *pImg	= NULL;
		if(_bComplex == false)
		{
			create(piDims, 2, &pReal, NULL);
		}
		else
		{
			create(piDims, 2, &pReal, &pImg);
		}
		return;
	}

	Double::Double(double _dblReal)
	{
        int piDims[2] = {1, 1};
		double *pdblVal;
		create(piDims, 2, &pdblVal, NULL);
		pdblVal[0] = _dblReal;
		return;
	}

	Double::Double(double _dblReal, double _dblImg)
	{
        int piDims[2] = {1, 1};
		double *pdblR;
		double *pdblI;
		create(piDims, 2, &pdblR, &pdblI);
		pdblR[0] = _dblReal;
		pdblI[0] = _dblImg;
		return;
	}

	Double::Double(int _iRows, int _iCols, double **_pdblReal)
	{
        int piDims[2] = {_iRows, _iCols};
		create(piDims, 2, _pdblReal, NULL);
		return;
	}

	Double::Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg)
	{
        int piDims[2] = {_iRows, _iCols};
		create(piDims, 2, _pdblReal, _pdblImg);
		return;
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
    }

	double*	Double::get_real() const
	{
		return get();
	}

	double Double::get_real(int _iRows, int _iCols)
	{
        return get(_iRows, _iCols);
	}

	bool Double::set_real(double *_pdblReal)
	{
		return set(_pdblReal);
	}

	bool Double::set_real(const double *_pdblReal)
	{
		return set(_pdblReal);
	}

    bool Double::set_real(int* _piReal)
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

	bool Double::set_real(int _iPos, double _dblReal)
	{
		return set(_iPos, _dblReal);
	}

	bool Double::set_real(int _iRows, int _iCols, double _dblReal)
	{
		return set(_iRows, _iCols, _dblReal);
	}

	void Double::whoAmI()
	{
		std::cout << "types::Double";
	}

	GenericType::RealType Double::getType(void)
	{
		return RealDouble;
	}

	bool Double::zero_set()
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

	bool Double::one_set()
	{
		if(m_pRealData != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pRealData[iIndex] = 1;
			}
		}
		else
        {
			return false;
        }

		if(m_bComplex == true)
		{
			if(m_pImgData != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_pImgData[iIndex] = 1;
				}
			}
			else
            {
				return false;
            }
		}
		return true;
	}

	wstring Double::toString(int _iPrecision, int _iLineLen)
	{
        /*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
		wostringstream ostr;
        int* piDims = new int[m_iDims];

        parseSubMatrix(ostr, piDims, m_iDims, m_iDims - 1, _iPrecision, _iLineLen);
		return ostr.str();
	}

    void Double::parseSubMatrix(wostringstream& ostr, int* _piDims, int _iDims, int _iDim, int _iPrecision, int _iLineLen)
    {
        if(_iDim == 1)
        {//we have reach 2-dim matrix
            subMatrixToString(ostr, _piDims, _iDims, _iPrecision, _iLineLen);
        }
        else
        {//draw, continue to dig
            for(int i = 0 ; i < m_piDims[_iDim] ; i++)
            {
                _piDims[_iDim] = i;
                parseSubMatrix(ostr, _piDims, _iDims, _iDim - 1, _iPrecision, _iLineLen);
            }
        }
    }

    void Double::subMatrixToString(wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
        if(_iDims > 2)
        {//more than 2 dims
            ostr << L"(:,:";
            for(int i = 2 ; i < _iDims ; i++)
            {
                ostr << L"," << (_piDims[i] + 1);
            }
            ostr << L")" << std::endl << std::endl;
        }

        if(isIdentity())
        {
            ostr << L"eye *" << endl << endl;
            if(isComplex() == false)
            {
                int iWidth = 0, iPrec = 0;
                bool bFP = false; // FloatingPoint
                GetDoubleFormat((m_pRealData[0]), _iPrecision, &iWidth, &iPrec, &bFP);
                AddDoubleValue(&ostr, (m_pRealData[0]), iWidth, iPrec);
                ostr << endl;
            }
            else
            {//complex value
                int iWidthR = 0, iWidthI = 0;
                int iPrecR = 0, iPrecI = 0;
                bool bFPR = false, bFPI = false; // FloatingPoint
                GetDoubleFormat(ZeroIsZero(m_pRealData[0]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
                GetDoubleFormat(ZeroIsZero(m_pImgData[0]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
                AddDoubleComplexValue(&ostr, ZeroIsZero(m_pRealData[0]), ZeroIsZero(m_pImgData[0]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                ostr << endl;
            }
            ostr << endl;
        }
        else if(isEmpty())
        {
            ostr << L"   []";
            ostr << endl;
        }
        else if(isScalar())
        {//scalar
            _piDims[0] = 0;
            _piDims[1] = 0;
            int iPos = get_index(_piDims);

            if(isComplex() == false)
            {
                int iWidth = 0, iPrec = 0;
                bool bFP = false; // FloatingPoint
                GetDoubleFormat((m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                AddDoubleValue(&ostr, (m_pRealData[iPos]), iWidth, iPrec);
                ostr << endl;
            }
            else
            {//complex value
                int iWidthR = 0, iWidthI = 0;
                int iPrecR = 0, iPrecI = 0;
                bool bFPR = false, bFPI = false; // FloatingPoint
                GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidthR, &iPrecR, &bFPR);
                GetDoubleFormat(ZeroIsZero(m_pImgData[iPos]), _iPrecision, &iWidthI, &iPrecI, &bFPI);
                AddDoubleComplexValue(&ostr, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                ostr << endl;
            }
        }
        else if(cols_get() == 1)
        {//column vector

            //1 test and two loops or 1 loop and many tests ? good question isn't it ?
            //First is harder to maintaint but execution is faster ( 1 test and 1 loop )
            //Second is slower, 1 loop but many tests.

            //1 test and two loops for me ( AE )
            if(isComplex() == false)
            {
                for(int i = 0 ; i < rows_get() ; i++)
                {
                    int iWidth = 0, iPrec = 0;
                    bool bFP = false; // FloatingPoint
                    _piDims[1] = 0;
                    _piDims[0] = i;
                    int iPos = get_index(_piDims);

                    GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                    AddDoubleValue(&ostr, ZeroIsZero(m_pRealData[iPos]), iWidth, iPrec);
                    ostr << endl;
                }
            }
            else
            {
                for(int i = 0 ; i < rows_get() ; i++)
                {//complex value
                    int iWidthR = 0, iWidthI = 0;
                    int iPrecR = 0, iPrecI = 0;
                    bool bFPR = false, bFPI = false; // FloatingPoint

                    _piDims[1] = 0;
                    _piDims[0] = i;
                    int iPos = get_index(_piDims);

                    GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidthR, &iPrecR, &bFPR);
                    GetDoubleFormat(ZeroIsZero(m_pImgData[iPos]), _iPrecision, &iWidthI, &iPrecI, &bFPI);
                    AddDoubleComplexValue(&ostr, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                    ostr << endl;
                }
            }
        }
        else if(rows_get() == 1)
        {//row vector
            wostringstream ostemp;
            int iLastVal = 0;

            //1 test and two loops or 1 loop and many tests ? good question isn't it ?
            //First is harder to maintaint but execution is faster ( 1 test and 1 loop )
            //Second is slower, 1 loop but many tests.

            //1 test and two loops for me ( AE )
            if(isComplex() == false)
            {
                for(int i = 0 ; i < cols_get() ; i++)
                {
                    int iWidth = 0, iPrec = 0;
                    bool bFP = false; // FloatingPoint
                    int iLen = 0;

                    _piDims[0] = 0;
                    _piDims[1] = i;
                    int iPos = get_index(_piDims);

                    GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
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

                    AddDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), iWidth, iPrec);
                }

                if(iLastVal != 0)
                {
                    ostr << endl << L"       column " << iLastVal + 1 << L" to " << cols_get() << endl << endl;
                }
            }
            else //complex case
            {
                for(int i = 0 ; i < cols_get() ; i++)
                {
                    int iWidthR = 0, iWidthI = 0;
                    int iPrecR = 0, iPrecI = 0;
                    bool bFPR = false, bFPI = false; // FloatingPoint
                    int iLen = 0;

                    _piDims[0] = 0;
                    _piDims[1] = i;
                    int iPos = get_index(_piDims);

                    GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidthR, &iPrecR, &bFPR);
                    GetDoubleFormat(ZeroIsZero(m_pImgData[iPos]), _iPrecision, &iWidthI, &iPrecI, &bFPI);

                    iLen = static_cast<int>(ostemp.str().size());
                    if(isZero(m_pImgData[iPos]))
                    {
                        if(isZero(m_pRealData[iPos]))
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
                        if(isZero(m_pRealData[iPos]))
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

                    AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
                }

                if(iLastVal != 0)
                {
                    ostr << endl << L"       column " << iLastVal + 1 << L" to " << cols_get() << endl << endl;
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
            int *piSize = new int[cols_get()];
            memset(piSize, 0x00, cols_get() * sizeof(int));

            //1 test and two loops or 1 loop and many tests ? good question isn't it ?
            //First is harder to maintaint but execution is faster ( 1 test and 1 loop )
            //Second is slower, 1 loop but many tests.

            //1 test and two loops for me ( AE )
            if(isComplex() == false)
            {
                //compute the row size for padding for each printed bloc.
                for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
                {
                    for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
                    {
                        int iWidth			= 0;
                        int iPrec				= 0;
                        bool bFP				= false; // FloatingPoint
                        int iCurrentLen = 0;

                        _piDims[0] = iRows1;
                        _piDims[1] = iCols1;

                        int iPos = get_index(_piDims);
                        GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                        iCurrentLen	= iWidth + bFP;

                        if(iCurrentLen > piSize[iCols1])
                        {
                            piSize[iCols1] = iCurrentLen;
                        }
                    }

                    if(iLen + piSize[iCols1] > _iLineLen)
                    {//find the limit, print this part
                        for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
                        {
                            for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                            {
                                int iWidth			= 0;
                                int iPrec				= 0;
                                bool bFP				= false; // FloatingPoint

                                _piDims[0] = iRows2;
                                _piDims[1] = iCols2;

                                int iPos = get_index(_piDims);
                                GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                                AddDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), piSize[iCols2], iPrec);
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

                for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
                {
                    for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
                    {
                        int iWidth			= 0;
                        int iPrec				= 0;
                        bool bFP				= false; // FloatingPoint
                        
                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;

                        int iPos = get_index(_piDims);
                        GetDoubleFormat(ZeroIsZero(m_pRealData[iPos]), _iPrecision, &iWidth, &iPrec, &bFP);
                        AddDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), piSize[iCols2], iPrec);
                        ostemp << SPACE_BETWEEN_TWO_VALUES;
                    }
                    ostemp << endl;
                }
                if(iLastCol != 0)
                {
                    ostr << endl << L"       column " << iLastCol + 1 << L" to " << cols_get() << endl << endl;
                }
                ostr << ostemp.str();
            }
            else //Complex case
            {
                //compute the row size for padding for each printed bloc.
                for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
                {
                    for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
                    {
                        int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
                        int iPrecR = 0, iPrecI = 0;
                        bool bFPR = false, bFPI = false; // FloatingPoint

                        _piDims[0] = iRows1;
                        _piDims[1] = iCols1;

                        int iPos = get_index(_piDims);
                        GetComplexFormat(	ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), _iPrecision,
                            &iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

                        iTotalWidth += (iWidthR == 0 ? 0 : SIGN_LENGTH) + (iWidthI == 0 ? 0 : SIGN_LENGTH + 1);
                        if(iTotalWidth > piSize[iCols1])
                        {
                            piSize[iCols1] = iTotalWidth;
                        }
                    }

                    if(iLen + piSize[iCols1] > _iLineLen)
                    {//find the limit, print this part
                        for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
                        {
                            for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                            {
                                int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
                                int iPrecR = 0, iPrecI = 0;
                                bool bFPR = false, bFPI = false; // FloatingPoint

                                _piDims[0] = iRows2;
                                _piDims[1] = iCols2;

                                int iPos = get_index(_piDims);
                                GetComplexFormat(	ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), _iPrecision,
                                    &iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

                                AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]),
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

                for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
                {
                    for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
                    {
                        int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
                        int iPrecR = 0, iPrecI = 0;
                        bool bFPR = false, bFPI = false; // FloatingPoint

                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;

                        int iPos = get_index(_piDims);
                        GetComplexFormat(	ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), _iPrecision,
                            &iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

                        AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]),
                            piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
                        ostemp << SPACE_BETWEEN_TWO_VALUES;
                    }
                    ostemp << endl;
                }

                if(iLastCol != 0)
                {
                    ostr << endl << L"       column " << iLastCol + 1 << L" to " << cols_get() << endl << endl;
                }
                ostr << ostemp.str();
            }
        }
    }

	InternalType* Double::clone()
	{
		Double *pReturn = new Double(m_iDims, m_piDims, m_bComplex);
		memcpy(pReturn->get_real(), m_pRealData, m_iSize * sizeof(double));

		if(m_bComplex)
		{
			pReturn->complex_set(true);
			memcpy(pReturn->get_img(), m_pImgData, m_iSize * sizeof(double));
		}
		return pReturn;
	}

    bool Double::fillFromCol(int _iCols, Double *_poSource)
    {
        //blas
        int iDestOffset     = _iCols * m_iRows;
        int iSize           = _poSource->size_get();
        double* pdblDest    = m_pRealData + iDestOffset;
        int iOne            = 1;
        dcopy_(&iSize, _poSource->get_real(), &iOne, pdblDest, &iOne);

        if(m_bComplex)
        {
            pdblDest    = m_pImgData + iDestOffset;
            dcopy_(&iSize, _poSource->get_img(), &iOne, pdblDest, &iOne);
        }
        return true;
    }

    bool Double::fillFromRow(int _iRows, Double *_poSource)
    {
        int iCols = _poSource->cols_get();

        if(m_bComplex)
        {
        }
        else
        {
            for(int i = 0 ; i < iCols ; i++)
            {
                int iDestOffset     = i * m_iRows + _iRows;
                int iOrigOffset     = i * _poSource->rows_get();
                int iSize           = _poSource->rows_get();
                double* pdblDest    = m_pRealData + iDestOffset;
                double* pdblSource  = _poSource->get_real() + iOrigOffset;
                int iOne            = 1;

                dcopy_(&iSize, pdblSource, &iOne, pdblDest, &iOne);
            }
        }
        return true;
    }

	bool Double::append(int _iRows, int _iCols, Double *_poSource)
	{
		int iRows = _poSource->rows_get();
		int iCols = _poSource->cols_get();

		//insert without resize
		if(iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
		{
			return false;
		}

		if(m_bComplex)
		{
			for(int iRow = 0 ; iRow < iRows ; iRow++)
			{
                int iDestOffset = _iCols * m_iRows + iRow;
                int iOrigOffset = iCols * _poSource->rows_get() + iRow;
                memcpy(m_pRealData + iDestOffset, _poSource->get_real() + iOrigOffset, iCols * sizeof(double));
                if(_poSource->isComplex())
                {
                    memcpy(m_pImgData + iDestOffset, _poSource->get_img() + iOrigOffset, iCols * sizeof(double));
                }
                else
                {
                    memset(m_pImgData + iDestOffset, 0x00, iCols * sizeof(double));
                }

				//for(int iCol = 0 ; iCol < iCols ; iCol++)
				//{
				//	set_real(_iRows + iRow, _iCols + iCol, _poSource->get_real(iRow, iCols));
				//	set_img(_iRows + iRow, _iCols + iCol, _poSource->get_img(iRow, iCol));
				//}
			}
		}
		else
		{
            if(iRows != 1)
            {
                for(int iCol = 0 ; iCol < iCols ; iCol++)
                {
                    int iDestOffset = (iCol + _iCols ) * m_iRows + _iRows;
                    int iOrigOffset = iCol * _poSource->rows_get();
                    memcpy(m_pRealData + iDestOffset, _poSource->get_real() + iOrigOffset, iRows * sizeof(double));
                }
            }
            else
            {
                for(int iCol = 0 ; iCol < iCols ; iCol++)
                {
                    set(_iRows, _iCols + iCol, _poSource->get_real(0, iCol));
                }
            }

            //for(int iRow = 0 ; iRow < iRows ; iRow++)
            //{
            //    for(int iCol = 0 ; iCol < iCols ; iCol++)
            //    {
            //        set_real(_iRows + iRow, _iCols + iCol, _poSource->get_real(iRow, iCol));
            //    }
            //}
		}
		return true;
	}

	bool Double::operator==(const InternalType& it)
	{
		if(const_cast<InternalType &>(it).getType() != RealDouble)
		{
			return false;
		}

		Double* pdbl = const_cast<InternalType &>(it).getAs<Double>();

        if(pdbl->get_dims() != get_dims())
        {
            return false;
        }

        for(int i = 0 ; i < get_dims() ; i++)
        {
            if(pdbl->get_dims_array()[i] != get_dims_array()[i])
            {
                return false;
            }
        }

		if(pdbl->isComplex() != isComplex())
		{
			return false;
		}

		double *pdblReal = pdbl->get_real();
		if(memcmp(m_pRealData, pdblReal, size_get() * sizeof(double)) != 0)
		{
			return false;
		}

		if(isComplex())
		{
			double *pdblImg = pdbl->get_img();
			if(memcmp(m_pImgData, pdblImg, size_get() * sizeof(double)) != 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Double::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

    double Double::get_null_value()
    {
        return 0;
    }
    
    Double* Double::create_empty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new Double(_iDims, _piDims, _bComplex);
    }
}
