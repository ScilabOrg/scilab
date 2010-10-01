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
#define EIGEN_SUPERLU_SUPPORT
#include <Eigen/Core>
#include <Eigen/Sparse>

#define CONST

using std::string;
namespace
{
    template<typename T> struct Finalizer
    {
        explicit Finalizer(T& toFinalize): o(toFinalize)
        {
        }
        ~Finalizer()
        {
            o.finalize();
        }
        T& o;
    };
}

namespace types
{
    class Sparse : public GenericType
    {
    public :
        virtual             ~Sparse();
        Sparse(Double CONST& src);
        Sparse(Double CONST& src, Double CONST& idx);
        Sparse(Double CONST& src, Double CONST& idx, Double CONST& dims);
        Sparse(int _iRows, int _iCols, bool cplx=false);
        Sparse(Sparse const& o);
        Sparse(Double CONST& xadj, Double CONST& adjncy, Double CONST& src, std::size_t r, std::size_t c);

        /*data management*/
        bool                                    val_set(int _iRows, int _iCols, double _dblReal);
        bool                                    val_set(int _iRows, int _iCols, std::complex<double> v);
        bool                                val_set(int _iRows, int _iCols, double _dblReal, double _dblImg);
        std::complex<double>                    val_get(int _iRows, int _iCols) const;

        bool one_set();

        double real_get(int r, int c) const;
        double img_get(int r, int c) const;
        std::complex<double> cplx_get(int r, int c) const;

        bool isComplex() const;
        /*zero set filler*/
        bool                        zero_set();

        /*Config management*/
        void                        whoAmI() const;
        bool                isExtract() const;
        Sparse*                     getAsSparse(void);
        Sparse const*                     getAsSparse(void) const;
        Sparse*             clone(void) const;
        Sparse*             clone(void) { return const_cast<Sparse const*>(this)->clone();}
        string                      toString(int _iPrecision, int _iLineLen) const;
        string                      toString(int _iPrecision, int _iLineLen)
        { return const_cast<Sparse const*>(this)->toString(_iPrecision,_iLineLen ); }
        bool                        resize(int _iNewRows, int _iNewCols);
        bool                        insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType CONST* _poSource, bool _bAsVector);
        static Sparse*              insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse CONST* _poSource, bool _bAsVector);
        Sparse*                     extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) CONST;


        bool                        operator==(const InternalType& it) const;
        bool                        operator!=(const InternalType& it) const;

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string         getTypeStr() const {return string("sparse");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string     getShortTypeStr() const {return string("constant");}
        Sparse* add(Sparse const& o) const;
        Sparse* substract(Sparse const& o) const;
        Sparse* multiply(Sparse const& o) const;

        Sparse* newTransposed()const;
        Sparse* newOnes()const;
        std::size_t nonZeros() const;
        std::size_t nonZeros(std::size_t i) const;
        double* outputCols(double* out) const;
        std::pair<double*, double*> outputValues(double* outReal, double* outImag)const;
        double* outputRowCol(double* out)const;
        void fillDouble(Double& dest) CONST;


    protected :
        RealType                    getType(void) CONST;

    private :
        typedef Eigen::SparseMatrix<double >   RealSparse_t;
        typedef Eigen::SparseMatrix<std::complex<double > >    CplxSparse_t;
        Sparse(RealSparse_t* realSp, CplxSparse_t* cplxSp);
        void CreateSparse(Double const& _indices, std::size_t _rowsMax, std::size_t _colsMax, Double const& _vals);
        RealSparse_t* m_matrixReal;
        CplxSparse_t* m_matrixCplx;

        template<typename DestIter>
        void create(int rows, int cols, Double CONST& src, DestIter o, std::size_t n);

        template<typename Src, typename RealSp, typename CplxSp, typename Sz>
        bool doInsert(Src CONST& src, RealSp& realSp, CplxSp& cplxSp, int const* coords, bool asVector, Sz n);

        void toComplex();
    };
}

#endif /* !__SPARSE_HH__ */
