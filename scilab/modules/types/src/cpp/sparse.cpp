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
	Sparse::~Sparse(){
	    delete m_matrix;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	Empty constructor	*/
	/*--------------------*/
	Sparse::Sparse(int _iRows, int _iCols)
	{
		CreateSparse(0, _iRows, _iCols, 0);
		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor	*/
	/*--------------------*/
	Sparse::Sparse(types::Double *_obj)
	{
		m_bExtract = false;
		m_iCols	   = _obj->cols_get();
		m_iRows	   = _obj->rows_get();
		m_iSize    = m_iCols * m_iRows;
		m_matrix = new SparseMatrix<std::complex<double> > ( _obj->rows_get(), _obj->cols_get() );
		
		for (int i = 0; i != _obj->rows_get(); ++i)
		{
			for (int j = 0; j != _obj->cols_get(); j++)
			{
	                    double real( _obj->real_get(i,j)), img(_obj->img_get(i,j) );
	                    
			    if( (real != 0.) || (img != 0.))
	                    {
	                        m_matrix->insert(i,j) = std::complex<double>( real, img);
	                    }
	                }
                }
		m_matrix->finalize();
		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor	*/
	/*--------------------*/
	Sparse::Sparse(SparseMatrix<std::complex<double> > *_obj)
	{
		m_bExtract = false;
		m_iCols	   = _obj->cols();
		m_iRows	   = _obj->rows();
		m_iSize    = m_iCols * m_iRows;
		m_matrix = new SparseMatrix<std::complex<double> > ( *_obj );
		return;

	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor      	*/
	/*--------------------*/
	Sparse::Sparse(Double *_indices, Double *_vals)
	{
	        double *endOfRow(_indices->real_get()+_indices->rows_get());

		CreateSparse(_indices
		             , static_cast<int>(*std::max_element(_indices->real_get(), endOfRow)) 
		             , static_cast<int>(*std::max_element(endOfRow, endOfRow+_indices->rows_get())) 
		             , _vals);

		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	constructor      	*/
	/*--------------------*/
	Sparse::Sparse(Double *_indices, Double *_vals, Double *_dims)
	{
		CreateSparse(_indices
                     , static_cast<int>(_dims->real_get(0,0)) 
                     , static_cast<int>(_dims->real_get(0,1)) 
                     , _vals);

		return;	
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	helper constructor     	*/
	/*--------------------*/
	void Sparse::CreateSparse(Double *_indices, std::size_t _rowsMax, std::size_t _colsMax, Double *_vals)
	{
		m_bExtract = false;
		m_iCols	   = _colsMax;
		m_iRows	   = _rowsMax;
		m_iSize    = m_iCols * m_iRows;
    		m_matrix   = new SparseMatrix<std::complex<double> >(_rowsMax, _colsMax);

    		if(_vals)
    		{
        		for(int i(0); i != _indices->rows_get(); ++i)
        		{
            			m_matrix->insert(_indices->real_get(i, 0) - 1, _indices->real_get(i, 1) - 1) = 
							std::complex<double>(_vals->real_get(i,0),_vals->img_get(i,0));
        		}
			m_matrix->finalize();
    		}
		return;
	}

	bool Sparse::isExtract()
	{
		return m_bExtract;
	}

	void Sparse::extract_set(bool _bExtract)
	{
		m_bExtract = _bExtract;
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Sparse::val_set(int _iRows, int _iCols, double _dblVal)
	{
		val_set(_iRows, _iCols, _dblVal, 0.);
		return true;
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Sparse::val_set(int _iRows, int _iCols, double _dblReal, double _dblImg)
	{
		if(_iRows < m_matrix->rows() && _iCols < m_matrix->cols())
		{
			m_matrix->insert(_iRows, _iCols) = std::complex<double>(_dblReal, _dblImg);
		}
		else
		{
			return false;
		}
		m_matrix->finalize();
		return true;
	}

	/*----------------*/
	/*		val_get		*/
	/*----------------*/
	std::complex<double> Sparse::val_get(int _iRows, int _iCols)
	{
	        
		return m_matrix->coeff(_iRows, _iCols);
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
		m_matrix->setZero();
		return true;
	}

	string Sparse::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;

		if (isExtract()) 
		{			
			ostr << *m_matrix << endl << endl;
		}	
		else 
		{//full sparse matrix
			ostr << "Values       : ";
			for (int i = 0; i < m_matrix->nonZeros(); i++)
			{
				ostr << m_matrix->_valuePtr()[i] << " ";
			}  
			ostr << "\nInner Indices: ";
			for (int i = 0; i < m_matrix->nonZeros(); i++)
			{
				ostr << m_matrix->_innerIndexPtr()[i] << " ";
			}  

			ostr << "\nOuter Indices: ";
			for (int i = 0; i < m_matrix->outerSize(); i++)
			{
				ostr << m_matrix->_outerIndexPtr()[i] << " ";
			}
			ostr << endl << endl;  

		}
		return ostr.str();	
	}

	SparseMatrix<std::complex<double> >* Sparse::matrix_get() 
	{
		return m_matrix;
	}

	bool Sparse::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return false;
		}

		SparseMatrix<std::complex<double> > *temp = new SparseMatrix<std::complex<double> >(*m_matrix);
		m_matrix->resize(_iNewRows, _iNewCols);

		for (int i = 0; i < _iNewRows; i++) {
			for (int j = 0; j < _iNewCols; j++) {
				if ( temp->coeff(i,j) != std::complex<double>(0,0) )
					m_matrix->insert(i,j) = temp->coeff(i,j); 
			}
		}
		m_matrix->finalize();	
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

		for (int i = 0; i < m_matrix->nonZeros(); i++)
		{
			if (m_matrix->_valuePtr()[i] != pdbl->matrix_get()->_valuePtr()[i])
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
		int iPos     = 0;
		int row      = 0;
		int col      = 0;

		//check input size
		if(rows_get() < _piMaxDim[0] || cols_get() < _piMaxDim[1])
		{//compute new dimensions
			iNewRows = Max(_piMaxDim[0], rows_get());
			iNewCols = Max(_piMaxDim[1], cols_get());
		}

		//check if the size of _poSource is compatible with the size of the variable
		if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
		{
			return false;
		}

		//check if the count of values is compatible with indexes
		if(_poSource->size_get() != 1 && _poSource->size_get() != _iSeqCount)
		{
			return false;
		}

		if(_poSource->getType() == InternalType::RealDouble)
		{
			types::Double *pIn = _poSource->getAsDouble();
			double* pInR = pIn->real_get();
			double* pInI = pIn->img_get();
							
			if(pIn->isComplex())
			{
				pInR = pIn->real_get();
				pInI = pIn->img_get();

			}
			else
			{
				pInR = pIn->real_get();			
			}

			//variable can receive new values.
			for(int i = 0 ; i < _iSeqCount ; i++)
			{

				iPos   = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
				row  = iPos % rows_get();
				col  = iPos / rows_get();
				
				if (row < 0 || col < 0)
					return false;

				m_matrix->insert(row, col) = pInR[0];									
				row++;							

				if ( row == rows_get() )
				{
					row = 0;
					col++;
				}						
			}
		}
		else {
			return false;
		}
		m_matrix->finalize();
		return true;
	}

	Sparse* Sparse::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse* _poSource, bool _bAsVector)
	{

		Sparse* pdbl	= NULL ; 
		
		pdbl = new Sparse(_piMaxDim[0], _piMaxDim[1]);

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
		int iRows	= 0;
		int iCols	= 0;
		int iInIndex 	= 0;
		int row 	= 0;
		int col 	= 0;

		//check input param
		if( (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            	    (_bAsVector == false && _piMaxDim[1] > cols_get()))
		{
			return NULL;
		}

		iRows		= _piDimSize[0];
		iCols		= _piDimSize[1];
		pOut		= new Sparse(iRows, iCols);
		pOut->extract_set(true);		

		for(int i = 0 ; i < _iSeqCount ; i++)
		{
			//convert vertical indexes to horizontal indexes
			iInIndex  = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
			iRows     = iInIndex % rows_get();
			iCols     = iInIndex / rows_get();

			if (iRows < 0 || iCols < 0)
				return false;

			pOut->val_set(row, col, m_matrix->coeff(iRows, iCols).real(), m_matrix->coeff(iRows, iCols).imag());
			row++;
			if ( row == rows_get() )
			{
				row = 0;
				col++;
			}
		}

		return pOut;
	}
}
