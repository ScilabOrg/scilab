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
	                            Sparse(int _iRows, int _iCols, bool _bComplex = false);
        void		            CreateSparse(Double *_indices, Double *_vals, Double *_dims);

        /*data management*/
        bool                        val_set(int _iRows, int _iCols, double _dblReal);
        bool                        val_set(int _iRows, int _iCols, double _dblReal, double _dblImg);
	double			    real_get(int _iRows, int _iCols);
	std::complex<double> 	    img_get(int _iRows, int _iCols);
        double*                     img_get() const;
        double*                     real_get() const;
        bool                        real_set(double *_pdblReal);
        bool                        img_set(double *_pdblImg);


        /*zero set filler*/
        bool                        zero_set();

        /*Config management*/
        void                        whoAmI();
        bool                        isComplex();
	bool 			    isExtract();
        void                        complex_set(bool _bComplex);
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

        /*clean values array*/
        void                        real_delete();
        void                        img_delete(bool _bSetReal = false);
        void                        all_delete(bool _bSetReal = false);

    private :
        SparseMatrix<double>   	          	*m_matrix;
	SparseMatrix<std::complex<double> >	*m_complexMatrix;
        double*                       	        m_pdblReal;
        double*                                 m_pdblImg;
        bool                                    m_bComplex;
	bool					m_bExtract;        
	int                                     m_iSizeMax;
	
    };
}

#endif /* !__SPARSE_HH__ */
