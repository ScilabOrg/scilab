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
			m_matrix = SparseMatrix<double> (_iRows, _iCols);
		}
		else
		{
			m_complexMatrix = SparseMatrix<std::complex<double> > (_iRows, _iCols);
		}
		m_bComplex = _bComplex;
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
			m_complexMatrix = SparseMatrix<std::complex<double> > ( _obj->rows_get(), _obj->cols_get() );
			m_bComplex = true;
			
			for (int i = 0; i < _obj->rows_get(); i++)
			{
				for (int j = 0; j < _obj->cols_get(); j++)
				{
					m_complexMatrix.insert(i,j) = std::complex<double>( _obj->real_get(i,j), _obj->img_get(i,j) );
				}
			}			
		}
		else
		{
			m_matrix = SparseMatrix<double> ( _obj->rows_get(), _obj->cols_get() );
			m_bComplex = false;

			for (int i = 0; i < _obj->rows_get(); i++)
			{
				for (int j = 0; j < _obj->cols_get(); j++)
				{
					m_matrix.insert(i,j) = _obj->real_get(i,j);
				}
			}
		}
			
		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor      	*/
	/*--------------------*/
	Sparse::Sparse(Double *_obj, Double *_obj2)
	{
		int maxRow = 1;
		int maxCol = 1;

		if ( _obj2->isComplex() ) 
		{
			for (int i = 0; i < _obj->rows_get(); i++)
			{
				if (_obj->real_get(i,0) > maxRow)
					maxRow = _obj->real_get(i,0);
				if (_obj->real_get(i,1) > maxCol)
					maxCol = _obj->real_get(i,1);
			}
			m_complexMatrix = SparseMatrix<std::complex<double> > ( maxRow, maxCol );
			m_bComplex = true;
			
			for (int i = 0; i < maxRow; i++)
			{
				m_complexMatrix.insert(_obj->real_get(i,0),_obj->real_get(i,1)) = _obj2->img_get(i,0);
			}			
		}
		else
		{
			for (int i = 0; i < _obj->rows_get(); i++)
			{
				if (_obj->real_get(i,0) > maxRow)
					maxRow = _obj->real_get(i,0);
				if (_obj->real_get(i,1) > maxCol)
					maxCol = _obj->real_get(i,1);
			}
			m_matrix = SparseMatrix<double> ( maxRow, maxCol );
			m_bComplex = false;
			
			for (int i = 0; i < maxRow; i++)
			{
				m_matrix.insert(_obj->real_get(i,0),_obj->real_get(i,1)) = _obj2->real_get(i,0);
			}			
		}

	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor     	*/
	/*--------------------*/
	Sparse::Sparse(Double *_obj, Double *_obj2, Double *_obj3)
	{
		int maxRow = _obj3->real_get(0,0);
		int maxCol = _obj3->real_get(0,1);

		if ( _obj2->isComplex() ) 
		{
			for (int i = 0; i < _obj->rows_get(); i++)
			{
				if (_obj->real_get(i,0) > maxRow)
					maxRow = _obj->real_get(i,0);
				if (_obj->real_get(i,1) > maxCol)
					maxCol = _obj->real_get(i,1);
			}
			m_complexMatrix = SparseMatrix<std::complex<double> > ( maxRow, maxCol );
			m_bComplex = true;
			
			for (int i = 0; i < maxRow; i++)
			{
				m_complexMatrix.insert(_obj->real_get(i,0),_obj->real_get(i,1)) = _obj2->img_get(i,0);
			}			
		}
		else
		{
			for (int i = 0; i < _obj->rows_get(); i++)
			{
				if (_obj->real_get(i,0) > maxRow)
					maxRow = _obj->real_get(i,0);
				if (_obj->real_get(i,1) > maxCol)
					maxCol = _obj->real_get(i,1);
			}
			m_matrix = SparseMatrix<double> ( maxRow, maxCol );
			m_bComplex = false;
			
			for (int i = 0; i < maxRow; i++)
			{
				m_matrix.insert(_obj->real_get(i,0),_obj->real_get(i,1)) = _obj2->real_get(i,0);
			}			
		}

	
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
				m_matrix = SparseMatrix<double> (m_complexMatrix.rows(), m_complexMatrix.cols());
			}
		}
		else // _bComplex == true
		{
			if(isComplex() == false)
			{
				m_bComplex = true;
				m_complexMatrix = SparseMatrix<std::complex<double> >(m_matrix.rows(), m_matrix.cols());
			}
		}
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
			if(_iRows < m_matrix.rows() && _iCols < m_matrix.cols())
			{
				m_matrix.insert(_iRows, _iCols) = _dblVal; 
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
			if(_iRows < m_matrix.rows() && _iCols < m_matrix.cols())
			{
				m_complexMatrix.insert(_iRows, _iCols) = std::complex<double>(_dblReal, _dblImg); 
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
	        
		return m_matrix.coeff(_iRows, _iCols);
	}

	/*----------------*/
	/*		real_get		*/
	/*----------------*/
	std::complex<double> Sparse::img_get(int _iRows, int _iCols)
	{
		return m_complexMatrix.coeff(_iRows, _iCols);	
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
		if(m_bComplex == true)
		{
			m_complexMatrix.setZero();
		}
		else
		{
			m_matrix.setZero();
		}
		
		return true;
	}

	string Sparse::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		//if(isComplex() == false)
	/*	{
			*Comment tenir compte de la longueur des lignes dans le formatage de variable ?* 
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
*
							for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
							{
//								ostr  << "|%" << piSize[iCols2] << "%|";

								for(int x = 0 ; x < piSize[iCols2]-1 ; x++)
								{
									ostr << " ";
								}
								ostr << SPACE_BETWEEN_REAL_COMPLEX << SYMBOL_I << "|" << SPACE_BETWEEN_TWO_VALUES;
							}

							for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
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
		
	//	else
		{
		ostr << "C'est complex ca :'(" << endl;
		}
	*/	
		return ostr.str();
	
	}

	bool Sparse::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return true;
		}

		if(isComplex())
		{
			SparseMatrix<std::complex<double> > temp = m_complexMatrix;
			m_complexMatrix.resize(_iNewRows, _iNewCols);

			for (int i = 0; i < _iNewRows; i++) {
				for (int j = 0; j < _iNewCols; j++) {
					m_complexMatrix.insert(i,j) = temp.coeff(i,j); 
				}
			}	
		}
		else
		{
			SparseMatrix<double> temp = m_matrix;
			m_matrix.resize(_iNewRows, _iNewCols);

			for (int i = 0; i < _iNewRows; i++) {
				for (int j = 0; j < _iNewCols; j++) {
					m_matrix.insert(i,j) = temp.coeff(i,j); 
				}
			}	
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

}

