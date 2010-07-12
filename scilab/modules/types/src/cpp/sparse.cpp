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
#include <Eigen/Sparse>
#include <complex>
#include "types.hxx"
#include "sparse.hxx"
#include "tostring_common.hxx"

extern "C"
{
	#include "elem_common.h"
}

using namespace std;
using namespace Eigen;

namespace types
{
	/*------------*/
	/*	                        ~Sparse	        	*/
	/*------------*/
	Sparse::~Sparse(){}

	/*--------------------*/
	/*				Sparse			*/
	/*	Empty constructor	*/
	/*--------------------*/
	Sparse::Sparse(int _iRows, int _iCols, bool _bComplex)
	{
		if(_bComplex == false)
		{
			m_matrix = new SparseMatrix<double> (_iRows, _iCols);
			m_complexMatrix = NULL;
		}
		else
		{
			m_complexMatrix = new SparseMatrix<std::complex<double> > (_iRows, _iCols);
			m_matrix = NULL;		
		}
		m_bComplex = _bComplex;
		m_iCols	   = _iCols;
		m_iRows	   = _iRows;
		m_iSize    = m_iCols * m_iRows;
		m_iSizeMax = m_iSize;
		m_pdblReal = NULL;
		m_pdblImg  = NULL;

		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	Real constructor	*/
	/*--------------------*/
	Sparse::Sparse(types::Double *_obj)
	{

		if ( _obj->isComplex() ) 
		{
			m_complexMatrix = new SparseMatrix<std::complex<double> > ( _obj->rows_get(), _obj->cols_get() );
			m_matrix = NULL;
			m_bComplex = true;
			
			for (int i = 0; i < _obj->rows_get(); i++)
			{
				for (int j = 0; j < _obj->cols_get(); j++)
				{
					m_complexMatrix->insert(i,j) = std::complex<double>( _obj->real_get(i,j), _obj->img_get(i,j) );
				}
			}
			m_iCols    = m_complexMatrix->cols();
			m_iRows    = m_complexMatrix->rows();
			m_iSize    = m_iCols * m_iRows;
			m_iSizeMax = m_iSize;
			m_pdblReal = _obj->real_get();
			m_pdblImg  = _obj->img_get();			
		}
		else
		{
			m_matrix = new SparseMatrix<double> ( _obj->rows_get(), _obj->cols_get() );
			m_complexMatrix = NULL;			
			m_bComplex = false;

			for (int i = 0; i < _obj->rows_get(); i++)
			{
				for (int j = 0; j < _obj->cols_get(); j++)
				{
					m_matrix->insert(i,j) = _obj->real_get(i,j);
				}
			}
			m_iCols    = m_matrix->cols();
			m_iRows    = m_matrix->rows();
			m_iSize    = m_iCols * m_iRows;
			m_iSizeMax = m_iSize;
			m_pdblReal = _obj->real_get();
			m_pdblImg  = NULL;			

		}
			
		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor      	*/
	/*--------------------*/
	Sparse::Sparse(Double *_indices, Double *_vals)
	{
		CreateSparse(_indices, _vals, NULL);
		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor      	*/
	/*--------------------*/
	Sparse::Sparse(Double *_indices, Double *_vals, Double *_dims)
	{
		CreateSparse(_indices, _vals, _dims);
		return;	
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	helper constructor     	*/
	/*--------------------*/
	void Sparse::CreateSparse(Double *_indices, Double *_vals, Double *_dims)
	{
		int maxRow, maxCol;

		if (_dims != NULL)
		{	
			maxRow = _dims->real_get(0,0);
			maxCol = _dims->real_get(0,1);
		}
		else
		{
			maxRow = 1;
			maxCol = 1;
		}

		if ( _vals->isComplex() ) 
		{
			for (int i = 0; i < _indices->rows_get(); i++)
			{
				if (_indices->real_get(i,0) > maxRow)
					maxRow = _indices->real_get(i,0);
				if (_indices->real_get(i,1) > maxCol)
					maxCol = _indices->real_get(i,1);
			}
			m_complexMatrix = new SparseMatrix<std::complex<double> > ( maxRow, maxCol );
			m_bComplex = true;
			
			for (int i = 0; i < maxRow; i++)
			{
				m_complexMatrix->insert(_indices->real_get(i,0),_indices->real_get(i,1)) = _vals->img_get(i,0);
			}			

			m_iCols    = m_complexMatrix->cols();
			m_iRows    = m_complexMatrix->rows();
			m_iSize    = m_iCols * m_iRows;
			m_iSizeMax = m_iSize;
			m_pdblReal = new double[m_iSize];
			m_pdblImg  = new double[m_iSize];

			for (int i = 0; i < maxRow; i++) {
				for (int j = 0; j < maxCol; j++) {
					m_pdblReal[j * maxRow + i] = real( m_complexMatrix->coeff(i,j) );
					m_pdblImg[j * maxRow + i]  = imag( m_complexMatrix->coeff(i,j) );					
				}
			}	


		}
		else
		{
			for (int i = 0; i < _indices->rows_get(); i++)
			{
				if (_indices->real_get(i,0) > maxRow)
					maxRow = _indices->real_get(i,0);
				if (_indices->real_get(i,1) > maxCol)
					maxCol = _indices->real_get(i,1);
			}
			m_matrix = new SparseMatrix<double> ( maxRow, maxCol );
			m_bComplex = false;
			
			for (int i = 0; i < maxRow; i++)
			{
				m_matrix->insert(_indices->real_get(i,0),_indices->real_get(i,1)) = _vals->real_get(i,0);
			}			

			m_iCols    = m_matrix->cols();
			m_iRows    = m_matrix->rows();
			m_iSize    = m_iCols * m_iRows;
			m_iSizeMax = m_iSize;
			m_pdblReal = new double[m_iSize];
			m_pdblImg  = NULL;

			for (int i = 0; i < maxRow; i++) {
				for (int j = 0; j < maxCol; j++) {
					m_pdblReal[j * maxRow + i] = m_matrix->coeff(i,j);
				}
			}	

		}
		return;

	
	}

	bool Sparse::isComplex()
	{
		return m_bComplex;
	}

	void Sparse::complex_set(bool _bComplex)
	{
		if(_bComplex == false)
		{
			if(isComplex() == true)
			{
				m_bComplex = false;
				m_matrix = new SparseMatrix<double> (m_complexMatrix->rows(), m_complexMatrix->cols());
				m_complexMatrix = NULL;
				img_delete(!_bComplex);
				m_bComplex = false;

			}
		}
		else // _bComplex == true
		{
			if(isComplex() == false)
			{
				m_bComplex = true;
				m_complexMatrix = new SparseMatrix<std::complex<double> > ( m_matrix->rows(), m_matrix->cols() );
				m_matrix = NULL;
				m_pdblImg = new double[m_iSize];
				memset(m_pdblImg, 0x00, sizeof(double) * m_iSize);
				m_bComplex = true;
			
			}
		}
		return;
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Sparse::val_set(int _iRows, int _iCols, double _dblVal)
	{
		if( m_bComplex == true )
			return false;
		else
		{
			if(_iRows < m_matrix->rows() && _iCols < m_matrix->cols())
			{
				m_matrix->insert(_iRows, _iCols)       = _dblVal; 
				m_pdblReal[_iCols * m_iRows + _iRows] = _dblVal;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Sparse::val_set(int _iRows, int _iCols, double _dblReal, double _dblImg)
	{
		if( m_bComplex == false )
			return false;
		else
		{
			if(_iRows < m_complexMatrix->rows() && _iCols < m_complexMatrix->cols())
			{
				m_complexMatrix->insert(_iRows, _iCols) = std::complex<double>(_dblReal, _dblImg); 
				m_pdblReal[_iCols * m_iRows + _iRows]  = _dblReal;					
				m_pdblImg[_iCols * m_iRows + _iRows]   = _dblImg;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	/*----------------*/
	/*		real_get		*/
	/*----------------*/
	double Sparse::real_get(int _iRows, int _iCols)
	{
	        
		return m_matrix->coeff(_iRows, _iCols);
	}

	/*----------------*/
	/*		real_get		*/
	/*----------------*/
	std::complex<double> Sparse::img_get(int _iRows, int _iCols)
	{
		return m_complexMatrix->coeff(_iRows, _iCols);	
	}

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void Sparse::whoAmI()
	{
		std::cout << "types::Sparse";
	}

	/*--------------*/
	/*	getAsSparse		*/
	/*--------------*/
	Sparse* Sparse::getAsSparse(void)
	{
		return this;
	}

	/*--------------*/
	/*	clone    		*/
	/*--------------*/
	Sparse* Sparse::clone(void)
	{
		return this;
	}

	/*------------*/
	/*	getType		*/
	/*------------*/
	GenericType::RealType Sparse::getType(void)
	{
		return RealSparse;
	}

	/*------------*/
	/*	zero_set	*/
	/*------------*/
	bool Sparse::zero_set()
	{
		if(m_pdblReal != NULL)
		{
			for(int i = 0 ; i < m_iSize ; i++)
			{
				m_pdblReal[i] = 0;
			}
			m_matrix->setZero();
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_pdblImg != NULL)
			{
				memset(m_pdblImg, 0x00, m_iSize * sizeof(double));
				m_complexMatrix->setZero();
			}
			else
				return false;
		}

		return true;
	}

	string Sparse::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;

		if (m_bComplex == true) 
		{
			ostr << m_complexMatrix << endl << endl;
		}
		else
		{
			ostr << m_matrix << endl << endl;
		}
		//if(isComplex() == false)
/*		{
			//Comment tenir compte de la longueur des lignes dans le formatage de variable ?
			if(isIdentity())
			{
				ostr << "eye *" << endl << endl;
				if(isComplex() == false)
				{
					int iWidth = 0, iPrec = 0;
					bool bFP = false; // FloatingPoint
					GetDoubleFormat((m_pdblReal[0]), _iPrecision, &iWidth, &iPrec, &bFP);
					AddDoubleValue(&ostr, (m_pdblReal[0]), iWidth, iPrec);
					ostr << endl;
				}
				else
				{//complex value
					int iWidthR = 0, iWidthI = 0;
					int iPrecR = 0, iPrecI = 0;
					bool bFPR = false, bFPI = false; // FloatingPoint
					GetDoubleFormat(ZeroIsZero(m_pdblReal[0]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
					GetDoubleFormat(ZeroIsZero(m_pdblImg[0]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
					AddDoubleComplexValue(&ostr, ZeroIsZero(m_pdblReal[0]), ZeroIsZero(m_pdblImg[0]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
					ostr << endl;
				}
				ostr << endl;
			}
			else if(cols_get() == 0 || rows_get() == 0)
			{
				ostr << "   []";
				ostr << endl;
			}
			else if(cols_get() == 1 && rows_get() == 1)
			{//scalar
				if(isComplex() == false)
				{
					int iWidth = 0, iPrec = 0;
					bool bFP = false; // FloatingPoint
					GetDoubleFormat((m_pdblReal[0]), _iPrecision, &iWidth, &iPrec, &bFP);
					AddDoubleValue(&ostr, (m_pdblReal[0]), iWidth, iPrec);
					ostr << endl;
				}
				else
				{//complex value
					int iWidthR = 0, iWidthI = 0;
					int iPrecR = 0, iPrecI = 0;
					bool bFPR = false, bFPI = false; // FloatingPoint
					GetDoubleFormat(ZeroIsZero(m_pdblReal[0]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
					GetDoubleFormat(ZeroIsZero(m_pdblImg[0]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
					AddDoubleComplexValue(&ostr, ZeroIsZero(m_pdblReal[0]), ZeroIsZero(m_pdblImg[0]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
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
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]), _iPrecision, &iWidth, &iPrec, &bFP);
						AddDoubleValue(&ostr, ZeroIsZero(m_pdblReal[i]), iWidth, iPrec);
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
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
						GetDoubleFormat(ZeroIsZero(m_pdblImg[i]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
						AddDoubleComplexValue(&ostr, ZeroIsZero(m_pdblReal[i]), ZeroIsZero(m_pdblImg[i]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
						ostr << endl;
					}
				}
			}
			else if(rows_get() == 1)
			{//row vector
				ostringstream ostemp;
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
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]), _iPrecision, &iWidth, &iPrec, &bFP);
						iLen = iWidth + bFP + static_cast<int>(ostemp.str().size());
						if(iLen > _iLineLen)
						{//Max length, new line
							ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
							ostr << ostemp.str() << endl;
							ostemp.str("\x00");
							iLastVal = i;
						}

						if(ostemp.str().size() != 0)
						{
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}

						AddDoubleValue(&ostemp, ZeroIsZero(m_pdblReal[i]), iWidth, iPrec);
					}

					if(iLastVal != 0)
					{
						ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
						GetDoubleFormat(ZeroIsZero(m_pdblImg[i]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);

						iLen = static_cast<int>(ostemp.str().size());
						if(isZero(m_pdblImg[i]))
						{
							if(isZero(m_pdblReal[i]))
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
							if(isZero(m_pdblReal[i]))
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
							ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
							ostr << ostemp.str() << endl;
							ostemp.str("");
							iLastVal = i;
						}

						if(ostemp.str().size() != 0)
						{
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}

						AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[i]), ZeroIsZero(m_pdblImg[i]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
					}

					if(iLastVal != 0)
					{
						ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
					}
				}
				ostemp << endl;
				ostr << ostemp.str();
			}
			else // matrix
			{
				ostringstream ostemp;
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

							GetDoubleFormat(ZeroIsZero(m_pdblReal[iCols1 * rows_get() + iRows1]), _iPrecision, &iWidth, &iPrec, &bFP);
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
									GetDoubleFormat(ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), _iPrecision, &iWidth, &iPrec, &bFP);
									AddDoubleValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), piSize[iCols2], iPrec);
									ostemp << SPACE_BETWEEN_TWO_VALUES;
								}
								ostemp << endl;
							}
							iLen = 0;
							ostr << endl << "       column " << iLastCol + 1 << " to " << iCols1 << endl << endl;;
							ostr << ostemp.str();
							ostemp.str("");
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
							GetDoubleFormat(ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), _iPrecision, &iWidth, &iPrec, &bFP);
							AddDoubleValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), piSize[iCols2], iPrec);
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}
						ostemp << endl;
					}
					if(iLastCol != 0)
					{
						ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
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

							GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols1 * rows_get() + iRows1]), ZeroIsZero(m_pdblImg[iCols1 * rows_get() + iRows1]), _iPrecision,
								&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

							iTotalWidth += (iWidthR == 0 ? 0 : SIGN_LENGTH) + (iWidthI == 0 ? 0 : SIGN_LENGTH + 1);
							if(iTotalWidth > piSize[iCols1])
							{
								piSize[iCols1] = iTotalWidth;
							}
						}

						if(iLen + piSize[iCols1] > _iLineLen)
						{//find the limit, print this part
/*
							for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
							{
//								ostr  << "|%" << piSize[iCols2] << "%|";

								for(int x = 0 ; x < piSize[iCols2]-1 ; x++)
								{
									ostr << " ";
								}
								ostr << SPACE_BETWEEN_REAL_COMPLEX << SYMBOL_I << "|" << SPACE_BETWEEN_TWO_VALUES;
							}
*/
/*							for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
							{
								for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
								{
									int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
									int iPrecR = 0, iPrecI = 0;
									bool bFPR = false, bFPI = false; // FloatingPoint

									GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]), _iPrecision,
										&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

									AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]),
										piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
									ostemp << SPACE_BETWEEN_TWO_VALUES;
								}
								ostemp << endl;
							}
							iLen = 0;
							ostr << endl << "       column " << iLastCol + 1 << " to " << iCols1 << endl << endl;;
							ostr << ostemp.str();
							ostemp.str("");
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

							GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]), _iPrecision,
								&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

							AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]),
								piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}
						ostemp << endl;
					}

					if(iLastCol != 0)
					{
						ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
					}
					ostr << ostemp.str();
				}
			}
		}
	/*	
		else
		{
		ostr << "C'est complex ca :'(" << endl;
		}
	*/	
		return ostr.str();
	
	}

	/*--------------*/
	/*	real_clean	*/
	/*--------------*/
	void Sparse::real_delete()
	{
		if(m_pdblReal != NULL)
		{
			delete[] m_pdblReal;
			m_pdblReal = NULL;
		}
	}

	/*------------*/
	/*	img_clean	*/
	/*------------*/
	void Sparse::img_delete(bool _bSetReal)
	{
		if(m_pdblImg != NULL)
		{
			delete[] m_pdblImg;
			m_pdblImg = NULL;
			m_bComplex = !_bSetReal;
		}
	}

	/*------------*/
	/*	all_clean	*/
	/*------------*/
	void Sparse::all_delete(bool _bSetReal)
	{
		real_delete();
		img_delete(_bSetReal);
	}


	bool Sparse::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return true;
		}

		//alloc new data array
		double* pdblReal = NULL;
		double* pdblImg = NULL;

		if(isComplex())
		{
			SparseMatrix<std::complex<double> > *temp = m_complexMatrix;
			m_complexMatrix->resize(_iNewRows, _iNewCols);

			for (int i = 0; i < _iNewRows; i++) {
				for (int j = 0; j < _iNewCols; j++) {
					m_complexMatrix->insert(i,j) = temp->coeff(i,j); 
				}
			}	

			m_iSizeMax = static_cast<int>(_iNewRows * _iNewCols);

			pdblReal	= new double[m_iSizeMax];
			memset(pdblReal, 0x00, sizeof(double) * m_iSizeMax);
			pdblImg		= new double[m_iSizeMax];
			memset(pdblImg, 0x00, sizeof(double) * m_iSizeMax);

			for(int i = 0 ; i < rows_get() ; i++)
			{
				for(int j = 0 ; j < cols_get() ; j++)
				{
					pdblReal[j * _iNewRows + i] = m_pdblReal[j * rows_get() + i];
					pdblImg[j * _iNewRows + i]	= m_pdblImg[j * rows_get() + i];
				}
			}

			delete[] m_pdblReal;
			delete[] m_pdblImg;
			m_pdblReal	= pdblReal;
			m_pdblImg	= pdblImg;

		}
		else
		{
			SparseMatrix<double> *temp = m_matrix;
			m_matrix->resize(_iNewRows, _iNewCols);

			for (int i = 0; i < _iNewRows; i++) {
				for (int j = 0; j < _iNewCols; j++) {
					m_matrix->insert(i,j) = temp->coeff(i,j); 
				}
			}	

			m_iSizeMax = static_cast<int>(_iNewRows * _iNewCols * 1.1);
			
			pdblReal	= new double[m_iSizeMax];
			memset(pdblReal, 0x00, sizeof(double) * m_iSizeMax);

			for(int i = 0 ; i < rows_get() ; i++)
			{
				for(int j = 0 ; j < cols_get() ; j++)
				{
					pdblReal[j * _iNewRows + i] = m_pdblReal[j * rows_get() + i];
				}
			}

			delete[] m_pdblReal;
			m_pdblReal	= pdblReal;

		}

		return true;
	}

	bool Sparse::operator==(const InternalType& it)
	{
		if(const_cast<InternalType &>(it).getType() != RealSparse)
		{
			return false;
		}

		Sparse* pdbl = const_cast<InternalType &>(it).getAsSparse();

		if(pdbl->rows_get() != rows_get() || pdbl->cols_get() != cols_get())
		{
			return false;
		}

		if(pdbl->isComplex() != isComplex())
		{
			return false;
		}

		return true;
	}

	bool Sparse::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}


	bool Sparse::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
	{
		int iNewRows = rows_get();
		int iNewCols = cols_get();
		//check input size
		if(_bAsVector == false)
		{
			if(rows_get() < _piMaxDim[0] || cols_get() < _piMaxDim[1])
			{//compute new dimensions
				iNewRows = Max(_piMaxDim[0], rows_get());
				iNewCols = Max(_piMaxDim[1], cols_get());
			}
		}
		else
		{
			if(size_get() < _piMaxDim[0])
			{
				if(rows_get() == 1 || size_get() == 0)
				{
					iNewRows = 1;
					iNewCols = _piMaxDim[0];
				}
				else if(cols_get() == 1)
				{
					iNewRows = _piMaxDim[0];
					iNewCols = 1;
				}
				else
				{
					return false;
				}
			}
		}

		//check if the size of _poSource is compatible with the size of the variable
		if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
		{
			return false;
		}
		else if(_bAsVector == true && (iNewRows * iNewCols < _poSource->size_get()))
		{
			return false;
		}


		//check if the count of values is compatible with indexes
		if(_poSource->size_get() != 1 && _poSource->size_get() != _iSeqCount)
		{
			return false;
		}


		switch(_poSource->getType())
		{
		case InternalType::RealSparse :
			{
				Sparse *pIn = _poSource->getAsSparse();

				//Only resize after all tests !
				if(resize(iNewRows, iNewCols) == false)
				{
					return false;
				}

				//variable can receive new values.
				if(pIn->size_get() == 1)
				{//a(?) = x
					if(pIn->isComplex())
					{//a(?) = C
						double* pInR = pIn->real_get();
						double* pInI = pIn->img_get();

						complex_set(true);//do nothing if variable is already complex

						if(_bAsVector)
						{//a([]) = C
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[0];
								m_pdblImg[_piSeqCoord[i] - 1]		= pInI[0];
							}
						}
						else
						{//a([],[]) = C
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								m_pdblReal[iPos]	= pInR[0];
								m_pdblImg[iPos]		= pInI[0];
							}
						}
					}
					else
					{//a(?) = R
						double* pInR = pIn->real_get();

						if(_bAsVector)
						{//a([]) = R
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[0];
							}
						}
						else
						{//a([],[]) = R
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								m_pdblReal[iPos]	= pInR[0];
							}
						}
					}
				}
				else
				{//a(?) = [x]
					if(pIn->isComplex())
					{//a(?) = [C]
						double* pInR = pIn->real_get();
						double* pInI = pIn->img_get();

						complex_set(true);//do nothing if variable is already complex

						if(_bAsVector)
						{//a([]) = [C]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[i];
								m_pdblImg[_piSeqCoord[i] - 1]		= pInI[i];
							}
						}
						else
						{//a([],[]) = [C]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								m_pdblReal[iPos]	= pInR[i];
								m_pdblImg[iPos]		= pInI[i];
							}
						}
					}
					else
					{//a(?) = [R]
						double* pInR = pIn->real_get();

						if(_bAsVector)
						{//a([]) = [R]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[i];
							}
						}
						else
						{//a([],[]) = [R]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								int iTempR = i / pIn->cols_get();
								int iTempC = i % pIn->cols_get();
								int iNew_i = iTempR + iTempC * pIn->rows_get();

								m_pdblReal[iPos]	= pInR[iNew_i];
							}
						}
					}
				}
			break;
			}
		default :
			return false;
			break;
		}
		return true;
	}

	Sparse* Sparse::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse* _poSource, bool _bAsVector)
	{
		Sparse* pdbl	= NULL ; 
		
		if(_bAsVector)
		{
			if(_poSource->cols_get() == 1)
			{
				pdbl = new Sparse(_piMaxDim[0], 1, _poSource->isComplex());
			}
			else if(_poSource->rows_get() == 1)
			{
				pdbl = new Sparse(1, _piMaxDim[0], _poSource->isComplex());
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			pdbl = new Sparse(_piMaxDim[0], _piMaxDim[1], _poSource->isComplex());
		}

		pdbl->zero_set();
		if(pdbl->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
		{
			delete pdbl;
			return NULL;
		}

		return pdbl;
	}

	Sparse* Sparse::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
	{
		Sparse *pOut	= NULL;
		int iRowsOut	= 0;
		int iColsOut	= 0;

		//check input param

		if(	(_bAsVector && _piMaxDim[0] > size_get()) ||
            (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            (_bAsVector == false && _piMaxDim[1] > cols_get()))
		{
			return NULL;
		}

		if(_bAsVector)
		{//a([])
			if(rows_get() == 1)
			{
				iRowsOut	= 1;
				iColsOut	= _piDimSize[0];
			}
			else
			{
				iRowsOut	= _piDimSize[0];
				iColsOut	= 1;
			}
		}
		else
		{//a([],[])
			iRowsOut	= _piDimSize[0];
			iColsOut	= _piDimSize[1];
		}

		pOut							= new Sparse(iRowsOut, iColsOut, isComplex());
		double* pdblReal	= pOut->real_get();
		double* pdblImg		= pOut->img_get();


		if(_bAsVector)
		{
			if(isComplex())
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					pdblReal[i] = m_pdblReal[_piSeqCoord[i] - 1];
					pdblImg[i]	= m_pdblImg[_piSeqCoord[i] - 1];
				}
			}
			else
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					pdblReal[i] = m_pdblReal[_piSeqCoord[i] - 1];
				}
			}
		}
		else
		{
			if(isComplex())
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					int iOutIndex				= (i % iColsOut) * iRowsOut + (i / iColsOut);
					int iInIndex				= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
					pdblReal[iOutIndex] = m_pdblReal[iInIndex];
					pdblImg[iOutIndex]	= m_pdblImg[iInIndex];
				}
			}
			else
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					//convert vertical indexes to horizontal indexes
					int iOutIndex				= (i % iColsOut) * iRowsOut + (i / iColsOut);
					int iInIndex				= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
					pdblReal[iOutIndex] = m_pdblReal[iInIndex];
				}
			}
		}
		
		return pOut;
	}

	/*------------*/
	/*	real_set	*/
	/*------------*/
	bool Sparse::real_set(double *_pdblReal)
	{
		if(_pdblReal != NULL)
		{
			if(m_pdblReal == NULL)
			{
				m_pdblReal = new double[m_iSize];
			}

			memcpy(m_pdblReal, _pdblReal, m_iSize * sizeof(double));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	img_set		*/
	/*------------*/
	bool Sparse::img_set(double *_pdblImg)
	{
		if(_pdblImg != NULL)
		{
			if(m_pdblImg == NULL)
			{
				m_pdblImg = new double[m_iSize];
			}

			memcpy(m_pdblImg, _pdblImg, m_iSize * sizeof(double));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	double*	Sparse::real_get() const
	{
		return m_pdblReal;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	double*	Sparse::img_get() const
	{
		return m_pdblImg;
	}

}
 
