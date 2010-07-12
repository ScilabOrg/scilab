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
		if (m_matrix)
		{
		    delete m_matrix;
		}
		if(m_complexMatrix)
		{
		    delete m_complexMatrix;
		}

	}

	/*--------------------*/
	/*				Sparse			*/
	/*	Empty constructor	*/
	/*--------------------*/
	Sparse::Sparse(int _iRows, int _iCols, bool _bComplex)
	{
		m_bComplex= _bComplex;
		CreateSparse(0, _iRows, _iCols, 0);
		return;
	}

	/*--------------------*/
	/*				Sparse			*/
	/*	Real constructor	*/
	/*--------------------*/
	Sparse::Sparse(types::Double *_obj)
	{
		m_bExtract = false;
	        m_bComplex = _obj->isComplex();
		m_iCols	   = _obj->cols_get();
		m_iRows	   = _obj->rows_get();
		m_iSize    = m_iCols * m_iRows;

		if (m_bComplex)
		{
			m_matrix = 0;
			m_complexMatrix = new SparseMatrix<std::complex<double> > ( _obj->rows_get(), _obj->cols_get() );
	                //RandomSetter<SparseMatrix<std::complex<double> > >setter(*m_complexMatrix);
			
			for (int i = 0; i != _obj->rows_get(); ++i)
			{
				for (int j = 0; j != _obj->cols_get(); j++)
				{
		                    double real( _obj->real_get(i,j)), img(_obj->img_get(i,j) );
		                    
				    if( (real != 0.) || (img != 0.))
		                    {
		                        m_complexMatrix->insert(i,j) = std::complex<double>( real, img);
		                    }
		                }
	                }
			m_complexMatrix->finalize();
		}
		else
		{
			m_matrix = new SparseMatrix<double> ( _obj->rows_get(), _obj->cols_get() );
			m_complexMatrix = NULL;
            		//RandomSetter<SparseMatrix<double > > setter(*m_matrix);
			for (int i = 0; i != _obj->rows_get(); ++i)
			{
				for (int j = 0; j != _obj->cols_get(); j++)
				{
                    			double real( _obj->real_get(i,j));
                    
					if(real != 0.)
                    			{	
                        			m_matrix->insert(i,j) = real;
                    			}
                		}
            		}
			m_matrix->finalize();
		}			
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
        	m_bComplex = _vals ? _vals->isComplex() : m_bComplex ;
		m_iCols	= _colsMax;
		m_iRows	= _rowsMax;
		m_iSize = m_iCols * m_iRows;
        	
		if(m_bComplex)
        	{
            		m_matrix = 0;
            		m_complexMatrix = new SparseMatrix<std::complex<double> >(_rowsMax, _colsMax);
            		if(_vals)
            		{
                		//RandomSetter<SparseMatrix<std::complex<double> > >setter(*m_complexMatrix);
                		for(int i(0); i != _indices->rows_get(); ++i)
                		{
                    			m_complexMatrix->insert(_indices->real_get(i, 0), _indices->real_get(i, 1)) = 
								std::complex<double>(_vals->real_get(i,0),_vals->img_get(i,0));
                		}
				m_complexMatrix->finalize();
            		}
        	}
        	else
        	{
            		m_matrix = new SparseMatrix<double > (_rowsMax, _colsMax);
            		m_complexMatrix= 0;
            		if(_vals)
            		{
                		//RandomSetter<SparseMatrix<double > > setter(*m_matrix);
                		for(int i(0); i != _indices->rows_get(); ++i)
                		{
                    			m_matrix->insert(_indices->real_get(i, 0), _indices->real_get(i, 1))= _vals->real_get(i,0);
                		}
				m_matrix->finalize();
            		}
        	}
		return;
	}

	bool Sparse::isComplex()
	{
		return m_bComplex;
	}

	bool Sparse::isExtract()
	{
		return m_bExtract;
	}

	void Sparse::extract_set(bool _bExtract)
	{
		m_bExtract = _bExtract;
	}

	void Sparse::complex_set(bool _bComplex)
	{
		if(_bComplex == false)
		{
			if(isComplex() == true)
			{
				m_matrix = new SparseMatrix<double> (m_complexMatrix->rows(), m_complexMatrix->cols());
				m_complexMatrix = NULL;
			}
		}
		else // _bComplex == true
		{
			if(isComplex() == false)
			{
				m_complexMatrix = new SparseMatrix<std::complex<double> > ( m_matrix->rows(), m_matrix->cols() );
				m_matrix = NULL;
			}
		}
        	m_bComplex = _bComplex;
		return;
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Sparse::val_set(int _iRows, int _iCols, double _dblVal)
	{
		if( m_bComplex == true )
       		{
			return val_set(_iRows, _iCols, _dblVal, 0.);
        	}
		else
		{
			if(_iRows < m_matrix->rows() && _iCols < m_matrix->cols())
			{
				m_matrix->insert(_iRows, _iCols)       = _dblVal;
			}
			else
			{
				return false;
			}
		}
		m_matrix->finalize();
		return true;
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Sparse::val_set(int _iRows, int _iCols, double _dblReal, double _dblImg)
	{
		if( m_bComplex == false )
	        {
            		return (_dblImg == 0.) ? val_set(_iRows, _iCols, _dblReal) : false ;
        	}
		else
		{
			if(_iRows < m_complexMatrix->rows() && _iCols < m_complexMatrix->cols())
			{
				m_complexMatrix->insert(_iRows, _iCols) = std::complex<double>(_dblReal, _dblImg);
			}
			else
			{
				return false;
			}
		}
		m_complexMatrix->finalize();
		return true;
	}

	/*----------------*/
	/*		real_get		*/
	/*----------------*/
	double Sparse::real_get(int _iRows, int _iCols)
	{
	        
		return m_bComplex ? m_complexMatrix->coeff(_iRows, _iCols).real() : m_matrix->coeff(_iRows, _iCols);
	}

	/*----------------*/
	/*		real_get		*/
	/*----------------*/
	std::complex<double> Sparse::img_get(int _iRows, int _iCols)
	{
		return m_bComplex ? m_complexMatrix->coeff(_iRows, _iCols).imag() : 0.;
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
		if(m_matrix)
		{
			m_matrix->setZero();
		}
		if(m_complexMatrix)
		{
            		m_complexMatrix->setZero();
        	}
		return true;
	}

	string Sparse::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;

		if (m_bComplex == true) 
		{
			if (isExtract()) 
			{			
				ostr << *m_complexMatrix << endl << endl;
			}	
			else 
			{//full sparse matrix
				ostr << "Values       : ";
				for (int i = 0; i < m_complexMatrix->nonZeros(); i++)
				{
					ostr << m_complexMatrix->_valuePtr()[i] << " ";
				}  
				ostr << "\nInner Indices: ";
				for (int i = 0; i < m_complexMatrix->nonZeros(); i++)
				{
					ostr << m_complexMatrix->_innerIndexPtr()[i] << " ";
				}  

				ostr << "\nOuter Indices: ";
				for (int i = 0; i < m_complexMatrix->outerSize(); i++)
				{
					ostr << m_complexMatrix->_outerIndexPtr()[i] << " ";
				}
				ostr << endl << endl;  

			}
		}
		else
		{
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
		}
		return ostr.str();
	
	}

	SparseMatrix<double>* Sparse::realMatrix_get() 
	{
		return m_matrix;
	}
 
	SparseMatrix<std::complex<double> >* Sparse::imgMatrix_get()
	{
		return m_complexMatrix;
	} 	

	bool Sparse::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return false;
		}

		if(isComplex())
		{
			SparseMatrix<std::complex<double> > *temp = new SparseMatrix<std::complex<double> >(*m_complexMatrix);
			m_complexMatrix->resize(_iNewRows, _iNewCols);

			for (int i = 0; i < _iNewRows; i++) {
				for (int j = 0; j < _iNewCols; j++) {
					if ( temp->coeff(i,j) != std::complex<double>(0,0) )
						m_complexMatrix->insert(i,j) = temp->coeff(i,j); 
				}
			}
			m_complexMatrix->finalize();	
		}
		else
		{
			SparseMatrix<double> *temp = new SparseMatrix<double>(*m_matrix);
			m_matrix->resize(_iNewRows, _iNewCols);

			for (int i = 0; i < _iNewRows; i++) {
				for (int j = 0; j < _iNewCols; j++) {
					if ( temp->coeff(i,j) != 0 )
						m_matrix->insert(i,j) = temp->coeff(i,j); 
				}
			}
			m_matrix->finalize();	
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

		if(isComplex())
		{
			for (int i = 0; i < m_complexMatrix->nonZeros(); i++)
			{
				if (m_matrix->_valuePtr()[i] != pdbl->imgMatrix_get()->_valuePtr()[i])
					return false;
			}
		}
		else
		{
			for (int i = 0; i < m_matrix->nonZeros(); i++)
			{
				if (m_matrix->_valuePtr()[i] != pdbl->realMatrix_get()->_valuePtr()[i])
					return false;
			}
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

				complex_set(true);//do nothing if variable is already complex
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

				if (isComplex())
					m_complexMatrix->insert(row, col) = std::complex<double>(pInR[0], pInI[0]);
				else
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
		if (isComplex())
			m_complexMatrix->finalize();
		else
			m_matrix->finalize();
		return true;
	}

	Sparse* Sparse::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse* _poSource, bool _bAsVector)
	{

		Sparse* pdbl	= NULL ; 
		
		pdbl = new Sparse(_piMaxDim[0], _piMaxDim[1], _poSource->isComplex());

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
		pOut		= new Sparse(iRows, iCols, isComplex());
		pOut->extract_set(true);		

		for(int i = 0 ; i < _iSeqCount ; i++)
		{
			//convert vertical indexes to horizontal indexes
			iInIndex  = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
			iRows     = iInIndex % rows_get();
			iCols     = iInIndex / rows_get();

			if (iRows < 0 || iCols < 0)
				return false;

			if (isComplex())
			{
				pOut->val_set(row, col, m_complexMatrix->coeff(iRows, iCols).real(), m_complexMatrix->coeff(iRows, iCols).imag());
			}
			else
			{
				pOut->val_set(row, col, m_matrix->coeff(iRows, iCols));
			}
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
 
