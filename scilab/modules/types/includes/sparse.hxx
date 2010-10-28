/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bernard Hugueney
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
//#define EIGEN_SUPERLU_SUPPORT
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <complex>
#define CONST

using std::string;
namespace
{
    /**
       utility struct to make a lexcially scoped finalizer.
       Calls the given member function (default to finalize())
       upon destruction (i.e. when going out of scope).
     */
    template<typename T, void(T::*Fun)()= &T::finalize> struct Finalizer
    {
        explicit Finalizer(T& toFinalize): o(toFinalize)
        {
        }
        ~Finalizer()
        {
            (o.*Fun)();
        }
        T& o;
    };
}

namespace types
{
/**
   Sparse is a wrapper over Eigen sparse matrices templates for either double or std::complex<double> values.
 */
    class Sparse : public GenericType
    {
    public :
        virtual             ~Sparse();
        /* @param src: Double matrix to copy into a new sparse matrix
        **/
        Sparse(Double CONST& src);
        /* @param src : Double matrix to copy into a new sparse matrix
           @param idx : Double matrix to use as indexes to get values from the src
        **/
        Sparse(Double CONST& src, Double CONST& idx);
        /* @param src : Double matrix to copy into a new sparse matrix
           @param idx : Double matrix to use as indexes to get values from the src
           @param dims : Double matrix containing the dimensions of the new matrix
        **/
        Sparse(Double CONST& src, Double CONST& idx, Double CONST& dims);
        /*
          @param rows : nb of rows of the new patrix
          @param rows : nb of columns of the new patrix
          @param cplx : if the new matrix contains complex numbers
        **/
        Sparse(int rows, int cols, bool cplx=false);
        Sparse(Sparse const& o);
        /* cf. adj2sp()
          @param xadj : adjacency matrix for the new matrix
          @param adjncy : adjacency matrix (row indexes) for the new matrix
          @param src : data for the new matrix
          @param r : nb of rows for the new matrix
          @param c : nb of columns for the new matrix
        **/
        Sparse(Double CONST& xadj, Double CONST& adjncy, Double CONST& src, std::size_t r, std::size_t c);

        /*data management member function defined for compatibility with the Double API*/
        bool val_set(int _iRows, int _iCols, double _dblReal);
        bool val_set(int _iRows, int _iCols, std::complex<double> v);
        bool val_set(int _iRows, int _iCols, double _dblReal, double _dblImg);
        /*
          set non zero values to 1.
        **/
        bool                                    one_set();
        /* get real value at coords (r,c)
        **/
        double real_get(int r, int c) const;
        /* get imaginary value at coords (r,c), 0. if matrix contains real numbers.
        **/
        double img_get(int r, int c) const;
        /* get complex value at coords (r,c) (imaginary part set to 0. if matrix contains real numbers.
        **/
        std::complex<double> cplx_get(int r, int c) const;
        /* return true if matrix contains complex numbers, false otherwise.
        **/
        bool isComplex() const;
        /* clear all the values of the matrix to 0. (or 0.+0.i if complex)
        **/
        bool zero_set();

        /*Config management*/
        void whoAmI() const;
        bool isExtract() const;
        Sparse* getAsSparse(void);
        Sparse const* getAsSparse(void) const;
        Sparse* clone(void) const;
        Sparse* clone(void) { return const_cast<Sparse const*>(this)->clone();}
        wstring  toString(int _iPrecision, int _iLineLen) const;
        wstring  toString(int _iPrecision, int _iLineLen)
        { return const_cast<Sparse const*>(this)->toString(_iPrecision,_iLineLen ); }
        bool                        resize(int _iNewRows, int _iNewCols);
        bool                        insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType CONST* _poSource, bool _bAsVector);
        template<typename SrcType>
        static Sparse*              insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, SrcType CONST* _poSource, bool _bAsVector);
        Sparse*                     extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) CONST;


        bool                        operator==(const InternalType& it) const;
        bool                        operator!=(const InternalType& it) const;

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string         getTypeStr() const {return string("sparse");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string     getShortTypeStr() const {return string("constant");}

        /* create a new sparse matrix containing the result of an addition
           @param o other matrix to add

         */
        Sparse* add(Sparse const& o) const;
        /* create a new sparse matrix containing the result of a substraction
           @param o other matrix to substract
           @return ptr to the new matrix, 0 in case of failure
         */
        Sparse* substract(Sparse const& o) const;
        /* create a new sparse matrix containing the result of a multiplication
           @param o other matrix to substract
           @return ptr to the new matrix, 0 in case of failure
         */
        Sparse* multiply(Sparse const& o) const;

        /* create a new sparse matrix containing the result of a transposition
           @return ptr to the new matrix, 0 in case of failure
         */
        Sparse* newTransposed()const;
        /** create a new sparse matrix containing the non zero values set to 1.
           equivalent but faster than calling one_set() on a new copy of the
           current matrix.
           @return ptr to the new matrix, 0 in case of failure
         */
        Sparse* newOnes()const;
        /** @return the nb of non zero values.
         */
        std::size_t nonZeros() const;
        /* @param i row of the current sparse matrix
           @return the nb of non zero values in row i
         */
        std::size_t nonZeros(std::size_t i) const;
        /**
           output 1-base column numbers of the non zero elements
           @param out : ptr used as an output iterator over double values
           @return past-the-end output iterator after ouput is done
         */
        double* outputCols(double* out) const;
        /**
           output real and imaginary values of the non zero elements
           @param outReal : ptr used as an output iterator over double values for real values
           @param outImag : ptr used as an output iterator over double values for imaginary values if any
           @return pair of past-the-end output iterators after ouput is done
         */
        std::pair<double*, double*> outputValues(double* outReal, double* outImag)const;
        /**
           ouput rows and afterwards columns of the non zero elements
           @param out : ptr used as an output iterator over double values
           @return past-the-end output iterators after ouput is done
         */
        double* outputRowCol(double* out)const;

        /**
           @param dest Double to be filled with values from the current sparse matrix.
         */
        void fillDouble(Double& dest) CONST;

    protected :
        RealType getType(void) CONST;

    private :
        typedef Eigen::SparseMatrix<double >   RealSparse_t;
        typedef Eigen::SparseMatrix<std::complex<double > >    CplxSparse_t;
        /**
           One and only one of the args should be 0.
           @param realSp ptr to an Eigen sparse matrix of double values
           @param cplxSp ptr to an Eigen sparse matrix of std::complex<double> elements
         */
        Sparse(RealSparse_t* realSp, CplxSparse_t* cplxSp);

        RealSparse_t* matrixReal;
        CplxSparse_t* matrixCplx;

        /** utility function used by constructors
            @param rows : nb of rows
            @param cols : nb of columns
            @param src : Double matrix data source
            @param : iterator (cf MatrixIterator.hxx) with indices
            @param n : nb of elements to copy from data source.
         */
        template<typename DestIter>
        void create(int rows, int cols, Double CONST& src, DestIter o, std::size_t n);

        /** utility function used by insert functions conceptually : sp[destTrav]= src[srcTrav]
            @param src : data source
            @param SrcTrav : iterator over the data source
            @param n : nb of elements to copy
            @param sp : sparse destination matrix
            @param destTrav : iterator over the data sink (i.e. sp)
         */
        template<typename Src, typename SrcTraversal, typename Sz, typename DestTraversal>
        static bool copyToSparse(Src CONST& src, SrcTraversal srcTrav, Sz n, Sparse& sp, DestTraversal destTrav);
        /**
           "in-place" cast into a sparse matrix of comlpex values
         */
        void toComplex();
    };
}

#endif /* !__SPARSE_HH__ */
