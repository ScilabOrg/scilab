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
#include "poly.hxx"
#include "arrayof.hxx"

using namespace std;

namespace types
{
	MatrixPoly::MatrixPoly()
	{
	}

	MatrixPoly::MatrixPoly(wstring _szVarName, int _iRows, int _iCols, int *_piRank)
	{
		m_iRows			= _iRows;
		m_iCols			= _iCols;
		m_iSize			= m_iRows * m_iCols;
		m_szVarName	= _szVarName;
		m_bComplex	= false;

		m_poPolyMatrix = new Poly[_iRows * _iCols];
		for(int i = 0 ; i < m_iSize ; i++)
		{
			m_poPolyMatrix[i].createPoly(NULL, NULL, _piRank[i]);
		}
	}

	MatrixPoly::MatrixPoly(wstring _szVarName, int _iRows, int _iCols, const int *_piRank)
	{
		m_iRows			= _iRows;
		m_iCols			= _iCols;
		m_iSize			= m_iRows * m_iCols;
		m_szVarName	= _szVarName;
		m_bComplex	= false;

		m_poPolyMatrix = new Poly[_iRows * _iCols];
		for(int i = 0 ; i < m_iSize ; i++)
		{
			m_poPolyMatrix[i].createPoly(NULL, NULL, _piRank[i]);
		}
	}

	MatrixPoly::~MatrixPoly()
	{
		if(isDeletable() == true)
		{
			delete[] m_poPolyMatrix;
		}
	}

	Poly* MatrixPoly::getPoly(int _iRows, int _iCols)
	{
		if(m_poPolyMatrix == NULL || _iRows >= m_iRows || _iCols >= m_iCols)
		{
			return NULL;
		}
		else
		{
			return &m_poPolyMatrix[_iCols * m_iRows + _iRows];
		}
	}

	Poly* MatrixPoly::getPoly(int _iIdx)
	{
		if(m_poPolyMatrix == NULL || _iIdx >= m_iSize)
		{
			return NULL;
		}
		else
		{
			return &m_poPolyMatrix[_iIdx];
		}
	}

	bool MatrixPoly::poly_set(int _iRows, int _iCols, Double *_pdblCoef)
	{
		return poly_set(_iCols * m_iRows + _iRows, _pdblCoef);
	}

	bool MatrixPoly::poly_set(int _iIdx, Double *_pdblCoef)
	{
		if(_iIdx < m_iSize)
		{
			/*Get old Poly*/
			Poly *poPoly = getPoly(_iIdx);
			poPoly->setRank(_pdblCoef->getSize());
			poPoly->setCoef(_pdblCoef);
		}
		else
		{
			return false;
		}

		return true;
	}

	bool MatrixPoly::getRank(int *_piRank)
	{
		if(_piRank == NULL || m_poPolyMatrix == NULL)
		{
			return false;
		}

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_piRank[i] = m_poPolyMatrix[i].getRank();
		}
		return true;
	}

	MatrixPoly* MatrixPoly::getAsPoly()
	{
		return this;
	}

	GenericType::RealType MatrixPoly::getType()
	{
		return RealPoly;
	}

	void MatrixPoly::whoAmI(void)
	{
		std::cout << "types::Poly";
	}

	bool MatrixPoly::isComplex(void)
	{
		return m_bComplex;
	}

	wstring MatrixPoly::getVariableName()
	{
		return m_szVarName;
	}

	void MatrixPoly::setComplex(bool _bComplex)
	{
		if(_bComplex != m_bComplex)
		{
			for(int i = 0 ; i < m_iSize ; i++)
			{
				m_poPolyMatrix[i].setComplex(_bComplex);
			}
			m_bComplex = _bComplex;
		}
	}

	MatrixPoly& MatrixPoly::operator= (MatrixPoly& poPoly)
	{
		m_iRows			= (&poPoly)->getRows();
		m_iCols			= (&poPoly)->getCols();
		m_iSize			= m_iRows * m_iCols;
		m_szVarName	= (&poPoly)->getVariableName();

		int *piRank = new int[m_iSize];
		m_bComplex	= false;

		(&poPoly)->getRank(piRank);

		m_poPolyMatrix = new Poly[m_iRows * m_iCols];
		for(int i = 0 ; i < m_iSize ; i++)
		{
			m_poPolyMatrix[i].createPoly(NULL, NULL, piRank[i]);
			if(m_poPolyMatrix[i].isComplex())
				m_bComplex = true;
			Double *pCoef = (&poPoly)->getPoly(i)->getCoef();
			m_poPolyMatrix[i].setCoef(pCoef);

		}

		return *this;
	}

	Double* MatrixPoly::evaluate(Double* _pdblValue)
	{
/*
		for(int iPolyR = 0 ; iPolyR < m_iRows ; iPolyR++)
		{
			for(int iPolyC = 0 ; iPolyC < m_iCols ; iPolyC++)
			{
				Poly *pPoly = getPoly(iPolyR, iPolyC);
				pPoly->evaluate(pInR, pInI, &pOutR, &pOutI);

			}
		}
*/
		double *pR	= _pdblValue->getReal();
		double *pI	= _pdblValue->getImg();
		int iRows		= _pdblValue->getRows();
		int iCols		= _pdblValue->getCols();

		double *pReturnR	= NULL;
		double *pReturnI	= NULL;
		Double *pReturn		= new Double(m_iRows * iRows, m_iCols * iCols, &pReturnR, &pReturnI);
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
			for(int iPolyCol = 0 ; iPolyCol < m_iCols ; iPolyCol++)
			{
				for(int iRow = 0 ; iRow < iRows ; iRow++)
				{
					for(int iPolyRow = 0 ; iPolyRow < m_iRows ; iPolyRow++)
					{
						double OutR	= 0;
						double OutI	= 0;

						Poly *pPoly = getPoly(iPolyRow, iPolyCol);
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

	void MatrixPoly::updateRank(void)
	{
		for(int i = 0 ; i < m_iSize ; i++)
		{
			Poly *pPoly = getPoly(i);
			pPoly->updateRank();
		}
	}

	int MatrixPoly::getMaxRank(void)
	{
		int *piRank = new int[getSize()];
		getRank(piRank);
		int iMaxRank = 0;
		for(int i = 0 ; i < getSize() ; i++)
		{
			iMaxRank = Max(iMaxRank, piRank[i]);
		}
		return iMaxRank;
	}

	Double* MatrixPoly::getCoef(void)
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
				Poly *pPoly	= getPoly(i);
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

	void MatrixPoly::setCoef(Double *_pCoef)
	{
		int iMaxRank = getMaxRank();

		setComplex(_pCoef->isComplex());
		double *pR = _pCoef->getReal();
		double *pI = _pCoef->getImg();
		for(int i = 0 ; i < getSize() ; i++)
		{
			Double *pTemp = new Double(1, iMaxRank, _pCoef->isComplex());
			Poly *pPoly = getPoly(i);
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

	wstring	MatrixPoly::toString(int _iPrecison, int _iLineLen)
	{
		wostringstream ostr;
		wostringstream osExp;
		wostringstream osCoef;

		list<wstring>::const_iterator it_Exp;
		list<wstring>::const_iterator it_Coef;
		list<wstring> listExpR, listCoefR, listExpI, listCoefI;

		if(m_iRows == 1 && m_iCols == 1)
		{
			if(m_bComplex)
			{
				ostr << L"Real part" << endl << endl << endl;
				getPoly(0)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
				for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
				{
					ostr << *it_Exp << endl << *it_Coef << endl;
				}

				ostr << L"Imaginary part" << endl << endl << endl ;
				getPoly(0)->toStringImg(_iPrecison, _iLineLen, getVariableName(), &listExpI, &listCoefI);
				for(it_Coef = listCoefI.begin(), it_Exp = listExpI.begin() ; it_Coef != listCoefI.end() ; it_Coef++,it_Exp++)
				{
					ostr << *it_Exp << endl << *it_Coef << endl;
				}
			}
			else
			{
				getPoly(0)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);

				for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
				{
					ostr << *it_Exp << endl << *it_Coef << endl;
				}
			}
		}
		else if(m_iRows == 1)
		{
			if(m_bComplex)
			{
				ostr << L"Real part" << endl << endl;
				ostr << getRowString(_iPrecison, _iLineLen, false);
				ostr << L"Imaginary part" << endl << endl;
				ostr << getRowString(_iPrecison, _iLineLen, true);
			}
			else
			{
				ostr << getRowString(_iPrecison, _iLineLen, false);
			}
		}
		else if(m_iCols == 1)
		{
			if(m_bComplex)
			{
				ostr << L"Real part" << endl << endl;
				ostr << getColString(_iPrecison, _iLineLen, false);
				ostr << L"Imaginary part" << endl << endl;
				ostr << getColString(_iPrecison, _iLineLen, true);
			}
			else
			{
				ostr << getColString(_iPrecison, _iLineLen, false);
			}
		}
		else
		{//Matrix
			if(m_bComplex)
			{
				ostr << L"Real part" << endl << endl;
				ostr << getMatrixString(_iPrecison, _iLineLen, false);
				ostr << L"Imaginary part" << endl << endl;
				ostr << getMatrixString(_iPrecison, _iLineLen, true);
			}
			else
			{
				ostr << getMatrixString(_iPrecison, _iLineLen, false);
			}
		}
		ostr << endl;
		return ostr.str();
	}

	wstring MatrixPoly::getMatrixString(int _iPrecison, int _iLineLen, bool _bComplex)
	{
		wostringstream ostr;
		wostringstream osExp;
		wostringstream osCoef;

		list<wstring>::const_iterator it_Exp;
		list<wstring>::const_iterator it_Coef;
		list<wstring> listExpR, listCoefR, listExpI, listCoefI;

		int iLen        = 0;
		int iLastCol    = 0;

		wstring szExp, szCoef;

		int *piMaxLen = new int[m_iCols];
		memset(piMaxLen, 0x00, sizeof(int) * m_iCols);

		//find the largest row for each col
		for(int iCols1 = 0 ; iCols1 < m_iCols ; iCols1++)
		{
			for(int iRows1 = 0 ; iRows1 < m_iRows ; iRows1++)
			{
				// FIXME : iLen shadow previous declaration
                int iLen = 0;
				if(_bComplex)
				{
					getPoly(iRows1, iCols1)->toStringImg(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
				}
				else
				{
					getPoly(iRows1, iCols1)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
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
				for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
				{
					bool bMultiLine = false;
					for(int iCols2 = iLastCol ; iCols2 < iCols1; iCols2++)
					{
						if(_bComplex)
						{
							getPoly(iRows2, iCols2)->toStringImg(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
						}
						else
						{
							getPoly(iRows2, iCols2)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
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
		}//for(int iCols1 = 0 ; iCols1 < m_iCols ; iCols1++)

		if(iLastCol + 1 == m_iCols)
		{
			ostr << endl << L"         Column " << m_iCols << endl << endl;
		}
		else
		{
			ostr << endl << L"         Column " << iLastCol + 1 << L" to " << m_iCols << endl << endl;
		}
		//print the end
		for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
		{
			for(int iCols2 = iLastCol ; iCols2 < m_iCols ; iCols2++)
			{
				if(_bComplex)
				{
					getPoly(iRows2, iCols2)->toStringImg(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
				}
				else
				{
					getPoly(iRows2, iCols2)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
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

	wstring MatrixPoly::getRowString(int _iPrecison, int _iLineLen, bool _bComplex)
	{
		int iLen        = 0;
		int iLastFlush  = 0;

		wostringstream ostr;
		wostringstream osExp;
		wostringstream osCoef;

		list<wstring>::const_iterator it_Exp;
		list<wstring>::const_iterator it_Coef;
		list<wstring> listExpR, listCoefR, listExpI, listCoefI;

		for(int i = 0 ; i < m_iSize ; i++)
		{
			wstring szExp, szCoef;

			if(_bComplex)
			{
				getPoly(i)->toStringImg(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
			}
			else
			{
				getPoly(i)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
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
			if(iLastFlush + 1 == m_iSize)
			{
				ostr << endl << L"         Column " << m_iSize << endl << endl;
			}
			else
			{
				ostr << endl << L"         Column " << iLastFlush + 1 << L" to " << m_iSize << endl << endl;
			}
		}
		ostr << osExp.str() << endl;
		ostr << osCoef.str() << endl;
		return ostr.str();
	}

	wstring MatrixPoly::getColString(int _iPrecison, int _iLineLen, bool _bComplex)
	{
		wostringstream ostr;
		wostringstream osExp;
		wostringstream osCoef;

		list<wstring>::const_iterator it_Exp;
		list<wstring>::const_iterator it_Coef;
		list<wstring> listExpR, listCoefR, listExpI, listCoefI;

		for(int i = 0 ; i < m_iSize ; i++)
		{
			wstring szExp, szCoef;

			if(_bComplex)
			{
				getPoly(i)->toStringImg(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
			}
			else
			{
				getPoly(i)->toStringReal(_iPrecison, _iLineLen, getVariableName(), &listExpR, &listCoefR);
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

	InternalType* MatrixPoly::insert(int _iRows, int _iCols, MatrixPoly *_poSource)
	{
		int iRows = _poSource->getRows();
		int iCols = _poSource->getCols();

		if(_iRows + iRows > m_iRows || _iCols + iCols > m_iCols)
		{
			return NULL;
		}

		for(int iRow = 0 ; iRow < iRows ; iRow++)
		{
			for(int iCol = 0 ; iCol < iCols ; iCol++)
			{
				poly_set(_iRows + iRow, _iCols + iCol, _poSource->getPoly(iRow, iCol)->getCoef());
			}
		}
		return this;
	}
	Double* MatrixPoly::extractCoef(int _iRank)
	{
		Double *pdbl	= new Double(m_iRows, m_iCols, m_bComplex);
		double *pReal	= pdbl->getReal();
		double *pImg	= pdbl->getImg();

		for(int i = 0 ; i < m_iSize ; i++)
		{
			Poly *pPoly = getPoly(i);

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
	bool MatrixPoly::insertCoef(int _iRank, Double* _pCoef)
	{
		double *pReal	= _pCoef->getReal();
		double *pImg	= _pCoef->getImg();

		for(int i = 0 ; i < m_iSize ; i++)
		{
			Poly *pPoly = getPoly(i);
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

	bool MatrixPoly::operator==(const InternalType& it)
	{
		if(const_cast<InternalType &>(it).getType() != RealPoly)
		{
			return false;
		}

		MatrixPoly* pM = const_cast<InternalType &>(it).getAsPoly();

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
			Poly* p1 = getPoly(i);
			Poly* p2 = pM->getPoly(i);

			if(*p1 != *p2)
			{
				return false;
			}
		}
		return true;
	}

	bool MatrixPoly::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

	GenericType* MatrixPoly::getColumnValues(int _iPos)
	{
		MatrixPoly* pMP = NULL;
		if(_iPos < getCols())
		{
			int *piRank = new int[getRows()];
			for(int i = 0 ; i < getRows() ; i++)
			{
				piRank[i] = getPoly(i, _iPos)->getRank();
			}

			pMP = new MatrixPoly(getVariableName(), getRows(), 1, piRank);
			for(int i = 0 ; i < getRows() ; i++)
			{
				pMP->poly_set(i, 0, getPoly(i, _iPos)->getCoef());
			}

			delete[] piRank;
		}

		return pMP;
	}
}

