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
using namespace Eigen;

namespace
{
    template<typename Os, typename In, typename Sz> Os& writeData(char const* title, In beg, Sz n, Os& os)
    {
        os<<title;
        copy_n(beg, std::ostream_iterator<typename std::iterator_traits<In>::value_type, char>(os, " "), n);
        os<< endl;
        return os;
    }
    template<typename T> string toString(T const& m)
    {
        ostringstream ostr;
        writeData("Values       : ", m._valuePtr(),m.nonZeros(), ostr);
        writeData("\nInner Indices: ", m._innerIndexPtr(),m.nonZeros(), ostr);
        writeData("\nOuter Indices: ", m._outerIndexPtr(),m.outerSize(), ostr);
        ostr << endl;
        return ostr.str();
    }

    template<typename T> bool resize(T** original, std::size_t r, std::size_t c)
    {
        T* resized= new T(r, c);
        *resized += **original;
        delete *original;
        *original= resized;
        return true;
    }

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

    template<typename T> bool setNonZero(T& s, typename ei_traits<T>::Scalar v= 1.) {
        for (typename Eigen::ei_traits<T>::Index j=0; j<s.outerSize(); ++j)
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
    /*------------*/
    /*                          ~Sparse             */
    /*------------*/
    Sparse::~Sparse()
    {
        delete m_matrixReal;
        delete m_matrixCplx;
    }

    Sparse::Sparse( Sparse const& src) : GenericType(src)
                                       , m_matrixReal(src.m_matrixReal ? new RealSparse_t(*src.m_matrixReal) : 0)
                                       , m_matrixCplx(src.m_matrixCplx ? new CplxSparse_t(*src.m_matrixCplx) : 0)
    {
    }

    Sparse::Sparse(int _iRows, int _iCols, bool cplx)
        : m_matrixReal(cplx ? 0: new RealSparse_t(_iRows, _iCols))
        , m_matrixCplx(cplx ? new CplxSparse_t(_iRows, _iCols) : 0)
    {
        m_iRows=_iRows;
        m_iCols=_iCols;
        m_iSize=_iRows * _iCols;
    }

    /*--------------------*/
    /*              Sparse          */
    /*  Real constructor    */
    /*--------------------*/

    Sparse::Sparse(Double CONST& src)
    {
        create(src.rows_get(), src.cols_get(), src, rowIter, src.size_get());
    }
    Sparse::Sparse(Double CONST& src, Double CONST& idx)
    {
        double CONST* const endOfRow(idx.real_get() + idx.rows_get());
        create( static_cast<int>(*std::max_element(idx.real_get(), endOfRow))
                , static_cast<int>(*std::max_element(endOfRow, endOfRow+idx.rows_get()))
                , src, makeIteratorFromVar(idx), idx.size_get() );
    }

    /*--------------------*/
    /*              Sparse          */
    /*  constructor         */
    /*--------------------*/
    Sparse::Sparse(Double CONST& src, Double CONST& idx, Double CONST& dims)
    {
        create(static_cast<std::size_t>(dims.real_get(0,0))
               ,static_cast<std::size_t>(dims.real_get(0,1))
               , src, makeIteratorFromVar(idx), idx.size_get());
    }

    /*--------------------*/
    /*              Sparse          */
    /*  helper constructor      */
    /*--------------------*/
    template<typename DestIter>
    void Sparse::create(int rows, int cols, Double CONST& src, DestIter o, std::size_t n)
    {
        m_iCols= cols;
        m_iRows= rows;
        m_iSize= cols*rows;

        if(src.isComplex())
        {
            m_matrixReal= 0;
            m_matrixCplx=  new CplxSparse_t( cols, rows);
            Finalizer<CplxSparse_t> f(*m_matrixCplx);
            copy_n(makeMatrixIterator<std::complex<double> >(src, rowIter), makeMatrixIterator<std::complex<double> >(*m_matrixCplx, o), n);
        }
        else
        {
            m_matrixReal= new RealSparse_t(cols, rows);
            m_matrixCplx=  0;
            Finalizer<RealSparse_t> f(*m_matrixReal);
            copy_n(makeMatrixIterator<double >(src, rowIter), makeMatrixIterator<double>(*m_matrixReal, o), n);

        }
    }
    /*----------------*/
    /*      val_set     */
    /*----------------*/
    bool Sparse::val_set(int _iRows, int _iCols, double _dblVal)
    {
        return val_set(_iRows, _iCols, _dblVal, 0.);
    }

    bool Sparse::val_set(int _iRows, int _iCols, std::complex<double> v)
    {
        return val_set(_iRows, _iCols, v.real(), v.imag());
    }

    /*----------------*/
    /*      val_set     */
    /*----------------*/
    bool Sparse::val_set(int _iRows, int _iCols, double _dblReal, double _dblImg)
    {
        if((_dblImg != 0. && m_matrixReal ) /* complex value in real matrix */
           || (_iRows >= rows_get() || _iCols >= cols_get())) /* index out of bounds */
        {
            return false;
        }
        else
        {
            if(m_matrixReal)
            {
                Finalizer<RealSparse_t> f(*m_matrixReal);
                m_matrixReal->insert(_iRows, _iCols) = _dblReal;
            }
            else
            {
                Finalizer<CplxSparse_t> f(*m_matrixCplx);
                m_matrixCplx->insert(_iRows, _iCols) = std::complex<double>(_dblReal, _dblImg);
            }
        }
        return true;
    }

    bool Sparse::isComplex() const
    {
        return static_cast<bool>(m_matrixCplx);
    }

    // TODO: should have both a bounds checking and a non-checking interface to elt access
    double  Sparse::real_get(int _iRows, int _iCols) const
    {
        return m_matrixReal
            ? m_matrixReal->coeff(_iRows, _iCols)
            : m_matrixCplx->coeff(_iRows, _iCols).real();
    }

    double  Sparse::img_get(int _iRows, int _iCols) const
    {
        return m_matrixCplx
            ? m_matrixCplx->coeff(_iRows, _iCols).imag()
            : 0. ;
    }



    /*----------------*/
    /*     cplx_get     */
    /*----------------*/
    std::complex<double> Sparse::cplx_get(int _iRows, int _iCols) const
    {
        return m_matrixCplx
            ? m_matrixCplx->coeff(_iRows, _iCols)
            : std::complex<double>(m_matrixReal->coeff(_iRows, _iCols), 0.);
    }

    /*--------------*/
    /*      whoIAm      */
    /*--------------*/
    void Sparse::whoAmI() const
    {
        std::cout << "types::Sparse";
    }

    /*--------------*/
    /*  getAsSparse     */
    /*--------------*/
    Sparse* Sparse::getAsSparse(void)
    {
        return this;
    }
    Sparse const* Sparse::getAsSparse(void)const
    {
        return this;
    }

    /*--------------*/
    /*  clone           */
    /*--------------*/
    Sparse* Sparse::clone(void) const
    {
        return new Sparse(*this);
    }

    /*------------*/
    /*  getType     */
    /*------------*/
    GenericType::RealType Sparse::getType(void) const
    {
        return RealSparse;
    }

    /*------------*/
    /*  zero_set    */
    /*------------*/
    bool Sparse::zero_set()
    {
        return (m_matrixReal ? m_matrixReal->setZero() : m_matrixCplx->setZero()), true ;
    }

    string Sparse::toString(int _iPrecision, int _iLineLen)const
    {
        return m_matrixReal ? ::toString(*m_matrixReal) : ::toString(*m_matrixCplx);
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
            res= m_matrixReal
                ? ::resize(&m_matrixReal, _iNewRows, _iNewCols)
                : ::resize(&m_matrixCplx, _iNewRows, _iNewCols);
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
                    ? equal(*m_matrixCplx, *otherSparse->m_matrixCplx)
                    : equal(*m_matrixReal, *otherSparse->m_matrixReal)));
    }

    bool Sparse::operator!=(const InternalType& it) const
    {
        return !(*this == it);
    }

    bool Sparse::one_set()
    {
        return isComplex()? setNonZero(*m_matrixCplx) : setNonZero(*m_matrixReal);
    }

    void Sparse::toComplex()
    {
        if(!isComplex())
        {
            try
            {
                m_matrixCplx= new CplxSparse_t(m_matrixReal->cast<std::complex<double> >());
                /*
                  m_matrixCplx= new CplxSparse_t(m_matrixReal->rows(), m_matrixReal->cols());
                  *m_matrixCplx=m_matrixReal->cast<std::complex<double> >();
                  */
                delete m_matrixReal;
            }
            catch(...)
            {
                delete m_matrixCplx;
                throw;
            }
        }
    }

    /*
     * if _bAsVector, then _piSeqCoord contains the positions when the matrix is considered a vector
     * i.e. p -> [p % rows, p/rows]
     * if _poSource->size_get() == 1 it is considered an infinite sequence
     * imo, we should generalize to always recycle _poSource when needed to match _iSeqCount
     *
     * TODO check what we want for invalid indices (<0) : is it ok to modify the marix up to the invalid indices
     * otherwise, is pre check is required DONE
     */
    bool Sparse::insert(int _iSeqCount, int CONST* _piSeqCoord, int CONST* _piMaxDim, GenericType CONST* src, bool _bAsVector)
    {
        int iNewRows = Max(_piMaxDim[0], rows_get());
        int iNewCols = Max(_piMaxDim[1], cols_get());
        int iPos     = 0;
        int row      = 0;
        int col      = 0;
        bool res(false);
        //check if the size of _poSource is compatible with the size of the variable
        if(((!_bAsVector) && (iNewRows < src->rows_get() || iNewCols < src->cols_get()))
           ||( _bAsVector && (iNewRows * iNewCols < src->size_get())))
        {
            return false;
        }
        //check if the count of values is compatible with indexes
        if(src->size_get() != 1 && src->size_get() != _iSeqCount)
        {
            return false;
        }
        // invalid index < 1
        if(*std::min_element(_piSeqCoord, _piSeqCoord+ (_bAsVector ? 1:2)* _iSeqCount) < 1)
        {
            return false;
        }
        try
        {

            switch(src->getType())
            {

            case InternalType::RealDouble:
            {
                res=doInsert(*src->getAsDouble(), *m_matrixReal, *m_matrixCplx, _piSeqCoord, _bAsVector, _iSeqCount);
                break;
            }
            case InternalType::RealSparse :
            {
                res=doInsert(*src->getAsSparse(), *m_matrixReal, *m_matrixCplx, _piSeqCoord, _bAsVector, _iSeqCount);
                break;
            }
            default: // TODO
            {}
            }
        }
        catch(...)
        {
            res= false;
        }

        return res;
    }
    /*
     * as insert but in a new Sparse of size _piMaxDim
     */
    Sparse* Sparse::insert_new(int _iSeqCount, int CONST* _piSeqCoord, int CONST* _piMaxDim, Sparse CONST* _poSource, bool _bAsVector)
    {
        Sparse& src(const_cast<Sparse&>(*_poSource)); /* const correctness issue :( */
        bool const cplx(src.isComplex());
        Sparse* pSp ( _bAsVector ?
                      ((src.cols_get() == 1)
                       ? new Sparse(_piMaxDim[0], 1, cplx)
                       :( (src.rows_get() == 1)
                          ? new Sparse(1, _piMaxDim[0], cplx)
                          : 0))
                      :  new Sparse(_piMaxDim[0], _piMaxDim[1], cplx));
        if( pSp && !pSp->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) )
        {
            delete pSp;
            pSp= 0;
        }
        return pSp;
    }
    /*
     * create a new Sparse of dims according to _piDimSize and fill it from currentSparse (along _piSeqCoord)
     */
    Sparse* Sparse::extract(int _iSeqCount, int CONST* _piSeqCoord, int CONST* _piMaxDim, int CONST* _piDimSize, bool _bAsVector) CONST
    {
        if( (_bAsVector && _piMaxDim[0] > size_get()) ||
            (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            (_bAsVector == false && _piMaxDim[1] > cols_get()))
        {
            return 0;
        }

        bool const cplx(isComplex());
        Sparse* pSp ( _bAsVector ?
                      ((const_cast<Sparse*>(this)->rows_get() == 1)
                       ?  new Sparse(1, _piDimSize[0], cplx)
                       :new Sparse(_piDimSize[0], 1, cplx))
                      :  new Sparse(_piDimSize[0], _piDimSize[1], cplx));
        if(!pSp->insert(_iSeqCount, _piSeqCoord, _piMaxDim, this, _bAsVector))
        {
            delete pSp;
            pSp= 0;
        }
        return pSp;
    }

    template<typename Src, typename RealSp, typename CplxSp, typename Sz>
    bool Sparse::doInsert(Src CONST& src, RealSp& realSp, CplxSp& cplxSp, int const* coords, bool asVector, Sz n)
    {
        if(!(src.isComplex() || isComplex()))
        {
            copy_n(makeMatrixIterator<double>(src, rowIter)
                   , makeMatrixIterator<double>(realSp, std::make_pair(coords, asVector)), n);
        }
        else
        {
            toComplex();
            copy_n(makeMatrixIterator<std::complex<double> >(src, rowIter)
                   , makeMatrixIterator<std::complex<double> >(cplxSp, std::make_pair(coords, asVector)), n);
        }
        return true;
    };
}

