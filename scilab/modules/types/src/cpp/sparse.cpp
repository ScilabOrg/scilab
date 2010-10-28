/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
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
#include <iterator>
#include <algorithm>
#include "types.hxx"
#include "sparse.hxx"
#include "tostring_common.hxx"
#include "double.hxx"
#include "MatrixIterator.tcc"
extern "C"
{
#include "elem_common.h"
}

using namespace std;

namespace
{
    /* used for debuging output
     */
    template<typename Os, typename In, typename Sz> Os& writeData(wchar_t const* title, In beg, Sz n, Os& os)
    {
        os<<title;
        copy_n(beg, n, std::ostream_iterator<typename std::iterator_traits<In>::value_type, char>(os, L" "));
        os<< endl;
        return os;
    }

    template<typename T> wstring toString(T const& m)
    {
        std::wostringstream ostr;
        ostr<<L"(\t"<<m.rows()<<L",\t"<<m.cols()<<L")";
        if(!m.nonZeros())
        {
            ostr<<L" zero";
        }
        ostr<<L" sparse matrix\n\n";
        for (int k=0; k <m.outerSize(); ++k)
            for (typename T::InnerIterator it(m,k); it; ++it)
            {
                ostr<<L"(\t" << it.row()+1<< L",\t"<< it.col()+1<<L")\t"<<  it.value()<<L'\n';
            }

#if 0
        writeData(L"Values       : ", m._valuePtr(),m.nonZeros(), ostr);
        writeData(L"\nInner Indices: ", m._innerIndexPtr(),m.nonZeros(), ostr);
        writeData(L"\nOuter Indices: ", m._outerIndexPtr(),m.outerSize(), ostr);
        ostr << endl;
#endif
        return ostr.str();
    }
    /** utility function used to resize Eigen matrix.
        We create a new matrix of required size (filled with 0) and add the current matrix to it.
        @param original ref (non const!) to a ptr to an Eigen::Sparse matrix
        @param r nb of rows of the new matrix
        @param c nb of columns of the new matrix
    */
    template<typename T> bool resize(T*& original, std::size_t r, std::size_t c)
    {
        T* resized= new T(r, c);
        *resized += *original;
        delete original;
        original= resized;
        return true;
    }
    /** utility function to compare two Eigen::Sparse matrices to equality
     */
    template<typename T> bool equal(T const& s1, T const& s2)
    {
        bool res(true);
        for (int k=0; res && k != s1.outerSize(); ++k)
        {
            for (typename T::InnerIterator it1(s1,k), it2(s2, k); res && it1 && it2 ; ++it1, ++it2)
            {
                res = (it1.value() == it2.value()
                       && it1.row() == it2.row()
                       && it1.col() == it2.col());
            }
        }
        return res;
    }
    /**
       utility function to set non zero values of an Eigen::Sparse matrix to a fixed values
       @param s : sparse matrix to modify
       @param v : value to set (default to 1.)
    */
    template<typename T> bool setNonZero(T& s, typename Eigen::internal::traits<T>::Scalar v= 1.) {
        for (typename Eigen::internal::traits<T>::Index j=0; j<s.outerSize(); ++j)
        {
            for (typename T::InnerIterator it(s, j); it; ++it)
            {
                it.valueRef()= v;
            }
        }
        return true;
    }

}

namespace types
{
    Sparse::~Sparse()
    {
        delete matrixReal;
        delete matrixCplx;
    }

    Sparse::Sparse( Sparse const& src) : GenericType(src)
                                       , matrixReal(src.matrixReal ? new RealSparse_t(*src.matrixReal) : 0)
                                       , matrixCplx(src.matrixCplx ? new CplxSparse_t(*src.matrixCplx) : 0)
    {
    }

    Sparse::Sparse(int _iRows, int _iCols, bool cplx)
        : matrixReal(cplx ? 0: new RealSparse_t(_iRows, _iCols))
        , matrixCplx(cplx ? new CplxSparse_t(_iRows, _iCols) : 0)
    {
        m_iRows=_iRows;
        m_iCols=_iCols;
        m_iSize=_iRows * _iCols;
    }

    Sparse::Sparse(Double CONST& src)
    {
        create(src.rows_get(), src.cols_get(), src, RowWiseFullIterator(src.rows_get(), src.cols_get()), src.size_get());
    }
    Sparse::Sparse(Double CONST& src, Double CONST& idx)
    {
        double CONST* const endOfRow(idx.real_get() + idx.rows_get());
        create( static_cast<int>(*std::max_element(idx.real_get(), endOfRow))
                , static_cast<int>(*std::max_element(endOfRow, endOfRow+idx.rows_get()))
                , src, makeIteratorFromVar(idx), idx.size_get()/2 );
    }

    Sparse::Sparse(Double CONST& src, Double CONST& idx, Double CONST& dims)
    {
        create(static_cast<std::size_t>(dims.real_get(0,0))
               ,static_cast<std::size_t>(dims.real_get(0,1))
               , src, makeIteratorFromVar(idx), idx.size_get()/2);
    }
    Sparse::Sparse(RealSparse_t* realSp, CplxSparse_t* cplxSp):  matrixReal(realSp), matrixCplx(cplxSp)
    {
        if(realSp)
        {
            m_iCols= realSp->cols();
            m_iRows= realSp->rows();
        }
        else
        {
            m_iCols= cplxSp->cols();
            m_iRows= cplxSp->rows();
        }
        m_iSize= m_iCols * m_iRows;
    }

    Sparse::Sparse(Double CONST& xadj, Double CONST& adjncy, Double CONST& src, std::size_t r, std::size_t c)
    {
        Adjacency a(xadj.real_get(), adjncy.real_get());
        create(r, c, src, makeIteratorFromVar(a), src.size_get());
    }

    template<typename DestIter>
    void Sparse::create(int rows, int cols, Double CONST& src, DestIter o, std::size_t n)
    {
        m_iCols= cols;
        m_iRows= rows;
        m_iSize= cols*rows;
        if(src.isComplex())
        {
            matrixReal= 0;
            matrixCplx=  new CplxSparse_t( rows, cols);
            matrixCplx->reserve(n);
            Finalizer<CplxSparse_t> f(*matrixCplx);
            copy_n(makeMatrixIterator<std::complex<double> >(src, RowWiseFullIterator(src.rows_get(), src.cols_get())), n, makeMatrixIterator<std::complex<double> >(*matrixCplx, o));
        }
        else
        {
            matrixReal= new RealSparse_t(rows, cols);
            matrixReal->reserve(n);
            matrixCplx=  0;
            Finalizer<RealSparse_t> f(*matrixReal);
            copy_n(makeMatrixIterator<double >(src,  RowWiseFullIterator(src.rows_get(), src.cols_get())), n
                   , makeMatrixIterator<double>(*matrixReal, o));

        }
    }
    void Sparse::fillDouble(Double& dest) CONST
    {
        if(isComplex())
        {
            copy_n(makeMatrixIterator<std::complex<double> >(*matrixCplx, RowWiseFullIterator(rows_get(), cols_get())), size_get()
                   ,makeMatrixIterator<std::complex<double> >(dest, RowWiseFullIterator(dest.rows_get(), dest.cols_get())));
        }
        else
        {
            copy_n( makeMatrixIterator<double>(*matrixReal,  RowWiseFullIterator(rows_get(), cols_get())), size_get()
                    , makeMatrixIterator<double >(dest, RowWiseFullIterator(dest.rows_get(), dest.cols_get())));
        }
    }

    bool Sparse::val_set(int _iRows, int _iCols, double _dblVal)
    {
        return val_set(_iRows, _iCols, _dblVal, 0.);
    }

    bool Sparse::val_set(int _iRows, int _iCols, std::complex<double> v)
    {
        return val_set(_iRows, _iCols, v.real(), v.imag());
    }

    bool Sparse::val_set(int _iRows, int _iCols, double _dblReal, double _dblImg)
    {
        if((_dblImg != 0. && matrixReal ) /* complex value in real matrix */
           || (_iRows >= rows_get() || _iCols >= cols_get())) /* index out of bounds */
        {
            return false;
        }
        else
        {
            if(matrixReal)
            {
                if(_dblReal != 0.)
                {
                    Finalizer<RealSparse_t> f(*matrixReal);
                    matrixReal->insert(_iRows, _iCols) = _dblReal;
                }
            }
            else
            {
                if(_dblReal != 0. || _dblImg != 0.)
                {
                    Finalizer<CplxSparse_t> f(*matrixCplx);
                    matrixCplx->insert(_iRows, _iCols) = std::complex<double>(_dblReal, _dblImg);
                }
            }
        }
        return true;
    }

    bool Sparse::isComplex() const
    {
        return static_cast<bool>(matrixCplx);
    }

    // TODO: should have both a bounds checking and a non-checking interface to elt access
    double  Sparse::real_get(int _iRows, int _iCols) const
    {
        return matrixReal
            ? matrixReal->coeff(_iRows, _iCols)
            : matrixCplx->coeff(_iRows, _iCols).real();
    }

    double  Sparse::img_get(int _iRows, int _iCols) const
    {
        return matrixCplx
            ? matrixCplx->coeff(_iRows, _iCols).imag()
            : 0. ;
    }

    std::complex<double> Sparse::cplx_get(int _iRows, int _iCols) const
    {
        return matrixCplx
            ? matrixCplx->coeff(_iRows, _iCols)
            : std::complex<double>(matrixReal->coeff(_iRows, _iCols), 0.);
    }

    void Sparse::whoAmI() const
    {
        std::cout << "types::Sparse";
    }

    Sparse* Sparse::getAsSparse(void)
    {
        return this;
    }
    Sparse const* Sparse::getAsSparse(void)const
    {
        return this;
    }

    Sparse* Sparse::clone(void) const
    {
        return new Sparse(*this);
    }

    GenericType::RealType Sparse::getType(void) CONST
    {
        return RealSparse;
    }

    bool Sparse::zero_set()
    {
        return (matrixReal ? matrixReal->setZero() : matrixCplx->setZero()), true ;
    }

    // TODO: handle precision and line length
    wstring Sparse::toString(int _iPrecision, int _iLineLen)const
    {
        return matrixReal ? ::toString(*matrixReal) : ::toString(*matrixCplx);
    }

    bool Sparse::resize(int _iNewRows, int _iNewCols)
    {
        if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
        {//nothing to do: hence we do NOT fail
            return true;
        }
        bool res(false);
        try
        {
            res= matrixReal
                ? ::resize(matrixReal, _iNewRows, _iNewCols)
                : ::resize(matrixCplx, _iNewRows, _iNewCols);
            m_iRows=  _iNewRows;
            m_iCols= _iNewCols;
            m_iSize=  _iNewRows * _iNewCols;
        }
        catch(...)
        {
            res= false;
        }
        return res;
    }
    // TODO decide if a complex matrix with 0 imag can be == to a real matrix
    // not true for dense
    bool Sparse::operator==(const InternalType& it) const
    {
        Sparse* otherSparse = const_cast<Sparse*>(dynamic_cast<Sparse const*>(&it));/* types::GenericType is not const-correct :( */
        Sparse& cthis (const_cast<Sparse&>(*this));
        return (otherSparse
                && (otherSparse->rows_get() != cthis.rows_get())
                && (otherSparse->cols_get() != cthis.cols_get())
                && (otherSparse->isComplex() != isComplex())
                && (isComplex()
                    ? equal(*matrixCplx, *otherSparse->matrixCplx)
                    : equal(*matrixReal, *otherSparse->matrixReal)));
    }

    bool Sparse::operator!=(const InternalType& it) const
    {
        return !(*this == it);
    }

    bool Sparse::one_set()
    {
        return isComplex()? setNonZero(*matrixCplx) : setNonZero(*matrixReal);
    }

    void Sparse::toComplex()
    {
        if(!isComplex())
        {
            try
            {
                matrixCplx= new CplxSparse_t(matrixReal->cast<std::complex<double> >());
                delete matrixReal;
            }
            catch(...)
            {
                delete matrixCplx;
                throw;
            }
        }
    }

    /*
     * if asVector, then _piSeqCoord contains the positions when the matrix is considered a vector
     * i.e. p -> [p % rows, p/rows]
     * if _poSource->size_get() == 1 it is considered an infinite sequence
     * imo, we should generalize to always recycle _poSource when needed to match _iSeqCount
     *
     * TODO check what we want for invalid indices (<0) : is it ok to modify the marix up to the invalid indices
     * otherwise, is pre check is required DONE
     *
     *
     * nbCoords : nb of Coords to
     * coords : either 1D or 2D coords
     * maxCoords: max of coords
     * src : source of values (either constant values if size()==1 or rowwise traversal from first elt
     * asVector : if coords is 1D
     */
    bool Sparse::insert(int nbCoords, int CONST* coords, int CONST * maxCoords, GenericType CONST* src, bool asVector)
    {
        int iNewRows = Max(maxCoords[0], rows_get());
        int iNewCols = Max(maxCoords[1], cols_get());
        int iPos     = 0;
        int row      = 0;
        int col      = 0;
        bool res(false);
        // invalid index < 1
        if(*std::min_element(coords, coords+ (asVector ? 1:2)* nbCoords) < 1)
        {
            return false;
        }
        switch(src->getType())
        {
        case InternalType::RealDouble:
        {
            res= asVector
                ? copyToSparse(*src->getAsDouble(), RowWiseFullIterator(src->rows_get(), src->cols_get()), nbCoords
                               , *this, Coords<true>(coords, rows_get()))
                : copyToSparse(*src->getAsDouble(), RowWiseFullIterator(Coords2D(src->rows_get(), src->cols_get())), nbCoords
                               , *this, Coords<false>(coords));
            break;
        }
        case  InternalType::RealSparse:
        {
            res= asVector
                ? copyToSparse(*src->getAsSparse(),  RowWiseFullIterator(src->rows_get(), src->cols_get()), nbCoords
                               , *this, Coords<true>(coords, rows_get()))
                : copyToSparse(*src->getAsSparse(), RowWiseFullIterator(src->rows_get(), src->cols_get()), nbCoords
                               , *this, Coords<false>(coords));
            break;
        }
        default :
        {
            res= false;
        }
        }
        return res;
    }
    /*
     * as insert but in a new Sparse of size maxCoords
     */
    template<typename SrcType>
    Sparse* Sparse::insert_new(int nbCoords, int CONST* coords, int CONST* maxCoords, SrcType CONST* src, bool asVector)
    {
        bool const cplx(src->isComplex());
        Sparse* pSp;
        if(asVector)
        {
            if(src->cols_get() == 1)
            {
                pSp= new Sparse(maxCoords[0], 1, cplx);
            }
            else
            {
                pSp= (src->rows_get() == 1) ? new Sparse(1, maxCoords[0], cplx) : 0;
            }
        }
        else
        {
            pSp=  new Sparse(maxCoords[0], maxCoords[1], cplx);
        }
        if( pSp && !pSp->insert(nbCoords, coords, maxCoords, src, asVector) )
        {
            delete pSp;
            pSp= 0;
        }
        return pSp;
    }
    template<> /* static dispatching function needed because isComplex is not part of GenericType interface */
    Sparse* Sparse::insert_new(int nbCoords, int CONST* coords, int CONST* maxCoords, GenericType CONST* src, bool asVector)
    {
        Sparse* res(0);
        switch(src->getType())
        {
        case InternalType::RealDouble:
        {
            res= Sparse::insert_new(nbCoords, coords, maxCoords, src->getAsDouble(), asVector);
            break;
        }
        case InternalType::RealSparse:
        {
            res= Sparse::insert_new(nbCoords, coords, maxCoords, src->getAsDouble(), asVector);
            break;
        }
        default :
        {
            res= 0;
        }
        }
        return res;
    }
    template Sparse* Sparse::insert_new<Sparse>(int, int*, int*, Sparse*, bool);
    /*
     * create a new Sparse of dims according to resSize and fill it from currentSparse (along coords)
     */
    Sparse* Sparse::extract(int nbCoords, int CONST* coords, int CONST* maxCoords, int CONST* resSize, bool asVector) CONST
    {
        if( (asVector && maxCoords[0] > size_get()) ||
            (asVector == false && maxCoords[0] > rows_get()) ||
            (asVector == false && maxCoords[1] > cols_get()))
        {
            return 0;
        }

        bool const cplx(isComplex());
        Sparse* pSp (0);
        if(asVector)
        {
            pSp= (rows_get() == 1) ?  new Sparse(1, resSize[0], cplx) : new Sparse(resSize[0], 1, cplx);
        }
        else
        {
            pSp = new Sparse(resSize[0], resSize[1], cplx);
        }
//        std::cerr<<"extracted sparse:"<<pSp->rows_get()<<", "<<pSp->cols_get()<<"seqCount="<<nbCoords<<"maxDim="<<maxCoords[0] <<","<< maxCoords[1]<<std::endl;
        if(! (asVector
              ? copyToSparse(*this,  Coords<true>(coords, rows_get()), nbCoords
                             , *pSp, RowWiseFullIterator(pSp->rows_get(), pSp->cols_get()))
              : copyToSparse(*this,  Coords<false>(coords), nbCoords
                             , *pSp, RowWiseFullIterator(pSp->rows_get(), pSp->cols_get()))))
        {
            delete pSp;
            pSp= 0;
        }
        return pSp;
    }
    /*
      coords are Scilab 1-based
      extract std::make_pair(coords, asVector), rowIter
    */
    template<typename Src, typename SrcTraversal, typename Sz, typename DestTraversal>
    bool Sparse::copyToSparse(Src CONST& src, SrcTraversal srcTrav, Sz n, Sparse& sp, DestTraversal destTrav)
    {
        if(!(src.isComplex() || sp.isComplex()))
        {
            copy_n(makeMatrixIterator<double>(src, srcTrav), n
                   , makeMatrixIterator<double>(*sp.matrixReal, destTrav));
        }
        else
        {
            sp.toComplex();
            copy_n(makeMatrixIterator<std::complex<double> >(src, srcTrav), n
                   , makeMatrixIterator<std::complex<double> >(*sp.matrixCplx, destTrav));
        }
        return true;
    }

    Sparse* Sparse::add(Sparse const& o) const
    {
        if((m_iRows != o.m_iRows) ||(m_iCols != o.m_iCols))
        {
            return 0;
        }
        RealSparse_t* realSp(0);
        CplxSparse_t* cplxSp(0);
        if(!(isComplex() || o.isComplex()))
        {
            realSp= new RealSparse_t(*matrixReal + *(o.matrixReal));
        }
        else
        {
            if( isComplex())
            {
                cplxSp=  o.isComplex()
                    ? new CplxSparse_t(*matrixCplx + *(o.matrixCplx))
                    : new CplxSparse_t(*matrixCplx + o.matrixReal->cast<std::complex<double> >());
            }
            else
            {
                cplxSp= new CplxSparse_t(matrixReal->cast<std::complex<double> >() + *(o.matrixCplx));
            }
        }
        return new Sparse(realSp, cplxSp);
    }
    Sparse* Sparse::substract(Sparse const& o) const
    {
        if((m_iRows != o.m_iRows) ||(m_iCols != o.m_iCols))
        {
            return 0;
        }
        RealSparse_t* realSp(0);
        CplxSparse_t* cplxSp(0);
        if(!(isComplex() || o.isComplex()))
        {
            realSp= new RealSparse_t(*matrixReal - *(o.matrixReal));
        }
        else
        {
            if( isComplex())
            {
                cplxSp= o.isComplex()
                    ? new CplxSparse_t(*matrixCplx - *(o.matrixCplx))
                    : new CplxSparse_t(*matrixCplx - o.matrixReal->cast<std::complex<double> >());
            }
            else
            {
                cplxSp= new CplxSparse_t(matrixReal->cast<std::complex<double> >() - *(o.matrixCplx));
            }
        }
        return new Sparse(realSp, cplxSp);
    }
    Sparse* Sparse::multiply(Sparse const& o) const
    {
        if((m_iRows != o.m_iCols) ||(m_iCols != o.m_iRows))
        {
            return 0;
        }
        RealSparse_t* realSp(0);
        CplxSparse_t* cplxSp(0);
        if(!(isComplex() || o.isComplex()))
        {
            realSp= new RealSparse_t(*matrixReal * *(o.matrixReal));
        }
        else
        {
            cplxSp= isComplex()
                ?( o.isComplex()
                   ? new CplxSparse_t(*matrixCplx * *(o.matrixCplx))
                   : new CplxSparse_t(*matrixCplx * o.matrixReal->cast<std::complex<double> >()))
                : new CplxSparse_t(matrixReal->cast<std::complex<double> >() * *(o.matrixCplx));
        }
        return new Sparse(realSp, cplxSp);
    }

    Sparse* Sparse::newTransposed() const
    {
        return new Sparse( matrixReal ? new RealSparse_t(matrixReal->transpose()) : 0
                           ,matrixCplx ? new CplxSparse_t(matrixCplx->transpose()) : 0);
    }
    struct BoolCast
    {
        BoolCast(std::complex<double> const& c): b(c.real() ||c.imag()){}
        operator bool () const
        {
            return b;
        }
        operator double() const
        {
            return b ? 1. : 0.;
        }
        bool b;
    };
    Sparse* Sparse::newOnes() const
    { // result is never cplx
        return new Sparse( matrixReal
                           ? new RealSparse_t(matrixReal->cast<bool>().cast<double>())
                           : new RealSparse_t(matrixCplx->cast<BoolCast>().cast<double>())
                           , 0);
    }
    std::size_t Sparse::nonZeros() const
    {
        return  matrixReal ? matrixReal->nonZeros() : matrixCplx->nonZeros() ;
    }
    std::size_t Sparse::nonZeros(std::size_t r) const
    {
        return  matrixReal ? matrixReal->innerNonZeros(r) : matrixCplx->innerNonZeros(r) ;
    }

    namespace
    {
        template<typename S> struct GetReal: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.value();
            }
        };
        template<> struct GetReal< Eigen::SparseMatrix<std::complex<double > > >
        : std::unary_function<Eigen::SparseMatrix<std::complex<double > > ::InnerIterator, double>
        {
            double operator()( Eigen::SparseMatrix<std::complex<double > > ::InnerIterator it) const
            {
                return it.value().real();
            }
        };
        template<typename S> struct GetImag: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.value().imag();
            }
        };
        template<typename S> struct GetRow: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.row()+1;
            }
        };
        template<typename S> struct GetCol: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.col()+1;
            }
        };

        template<typename S, typename Out, typename F> Out sparseTransform(S& s, Out o, F f)
        {
            for (std::size_t k(0); k < s.outerSize(); ++k)
            {
                for (typename S::InnerIterator it(s,k); it; ++it, ++o)
                {
                    *o = f(it);
                }
            }
            return o;
        }
    }

    std::pair<double*, double*> Sparse::outputValues(double* outReal, double* outImag)const
    {
        return matrixReal
            ? std::make_pair(sparseTransform(*matrixReal, outReal, GetReal<RealSparse_t>()), outImag)
            : std::make_pair(sparseTransform(*matrixCplx, outReal, GetReal<CplxSparse_t>())
                             , sparseTransform(*matrixCplx, outImag, GetImag<CplxSparse_t>()));
    }

    double* Sparse::outputRowCol(double* out)const
    {
        return matrixReal
            ? sparseTransform(*matrixReal, sparseTransform(*matrixReal, out, GetRow<RealSparse_t>()), GetCol<RealSparse_t>())
            : sparseTransform(*matrixCplx, sparseTransform(*matrixCplx, out, GetRow<CplxSparse_t>()), GetCol<CplxSparse_t>());
    }
    double* Sparse::outputCols(double* out) const
    {
        return std::transform(out, isComplex()
                              ?  copy_n(matrixCplx->_innerIndexPtr(), nonZeros(), out)
                              : copy_n(matrixReal->_innerIndexPtr(), nonZeros(), out)
                              , out, std::bind2nd(std::plus<double>(), 1.));

    }
}

