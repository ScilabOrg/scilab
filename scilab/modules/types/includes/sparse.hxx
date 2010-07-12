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

#ifndef __SPARSE_HH__
#define __SPARSE_HH__

#include "types.hxx"
#include "alltypes.hxx"
#include <Eigen/Core>
#include <Eigen/Sparse>

using std::string;
using namespace Eigen;

namespace types
{
    class Sparse : public GenericType
    {
    public :
	virtual			    ~Sparse();
				    Sparse(Double *_obj);
				    Sparse(Double *_indices, Double *_vals);
				    Sparse(Double *_indices, Double *_vals, Double *_dims);
	                            Sparse(int _iRows, int _iCols);
        void		            CreateSparse(Double *_indices, std::size_t _rowsMax, std::size_t _colsMax, Double *_vals);

        /*data management*/	
        bool                        			val_set(int _iRows, int _iCols, double _dblReal);
        bool                       			val_set(int _iRows, int _iCols, double _dblReal, double _dblImg);
	std::complex<double> 	   	        	val_get(int _iRows, int _iCols);
	SparseMatrix<std::complex<double> >* 		matrix_get();                    

        /*zero set filler*/
        bool                        zero_set();

        /*Config management*/
        void                        whoAmI();
	bool 			    isExtract();
	void 			    extract_set(bool _bExtract);
	

        Sparse*                     getAsSparse(void);
	Sparse* 		    clone(void);
        string                      toString(int _iPrecision, int _iLineLen);
        bool                        resize(int _iNewRows, int _iNewCols);
        bool                        insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static Sparse*              insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse* _poSource, bool _bAsVector);
        Sparse*                     extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);


        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string         getTypeStr() {return string("sparse");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string	    getShortTypeStr() {return string("constant");}
    protected :
        RealType                    getType(void);

    private :
	SparseMatrix<std::complex<double> >	*m_matrix;
	bool					m_bExtract;        
	
    };
}

#endif /* !__SPARSE_HH__ */
