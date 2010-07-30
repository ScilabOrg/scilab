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

namespace
{
    template<typename T> string toString(T const& m)
    {
        ostringstream ostr;
        ostr << "Values       : ";
        for (int i = 0; i < m.nonZeros(); i++)
        {
            ostr << m._valuePtr()[i] << " ";
        }
        ostr << "\nInner Indices: ";
        for (int i = 0; i < m.nonZeros(); i++)
        {
            ostr << m._innerIndexPtr()[i] << " ";
        }
        ostr << "\nOuter Indices: ";
        for (int i = 0; i < m.outerSize(); i++)
        {
            ostr << m._outerIndexPtr()[i] << " ";
        }
        ostr << endl << endl;
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


}

namespace types
{
    /*------------*/
    /*                          ~Sparse             */
    /*------------*/
    Sparse::~Sparse(){
        delete m_matrixReal;
        delete m_matrixCplx;
    }

    /*--------------------*/
    /*              Sparse          */
    /*  Empty constructor   */
    /*--------------------*/
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
    Sparse::Sparse(types::Double *_obj)
    {
        m_iCols    = _obj->cols_get();
        m_iRows    = _obj->rows_get();
        m_iSize    = m_iCols * m_iRows;
        if(_obj->isComplex())
        {
            m_matrixCplx= new CplxSparse_t ( _obj->rows_get(), _obj->cols_get() );
            m_matrixReal= 0;
            Finalizer<CplxSparse_t> f(*m_matrixCplx);
            for (int i = 0; i != _obj->rows_get(); ++i)
            {
                for (int j = 0; j != _obj->cols_get(); j++)
                {
                    double const real( _obj->real_get(i,j)), img( _obj->img_get(i,j));
                    if( real != 0. && img != 0.)
                    {
                        m_matrixCplx->insert(i,j) = std::complex<double>(real, img);
                    }
                }
            }
        }
        else
        {

           m_matrixCplx= 0;
            m_matrixReal= new RealSparse_t ( _obj->rows_get(), _obj->cols_get() );
            Finalizer<RealSparse_t> f(*m_matrixReal);
            for (int i = 0; i != _obj->rows_get(); ++i)
            {
                for (int j = 0; j != _obj->cols_get(); j++)
                {
                    double const real( _obj->real_get(i,j));
                    if( real != 0. )
                    {
                    m_matrixReal->insert(i,j) = real;
                    }
                }
            }
        }
    }

    /*--------------------*/
    /*              Sparse          */
    /*  constructor         */
    /*--------------------*/
    Sparse::Sparse(Double *_indices, Double *_vals)
    {
        double *endOfRow(_indices->real_get()+_indices->rows_get());

        CreateSparse(*_indices
                     , static_cast<int>(*std::max_element(_indices->real_get(), endOfRow))
                     , static_cast<int>(*std::max_element(endOfRow, endOfRow+_indices->rows_get()))
                     , *_vals);

        return;
    }

    /*--------------------*/
    /*              Sparse          */
    /*  constructor         */
    /*--------------------*/
    Sparse::Sparse(Double *_indices, Double *_vals, Double *_dims)
    {
        CreateSparse(*_indices
                     , static_cast<std::size_t>(_dims->real_get(0,0))
                     , static_cast<std::size_t>(_dims->real_get(0,1))
                     ,* _vals);

        return;
    }

    /*--------------------*/
    /*              Sparse          */
    /*  helper constructor      */
    /*--------------------*/
    void Sparse::CreateSparse(Double const& _indices, std::size_t _rowsMax, std::size_t _colsMax, Double const& _vals)
    {
        m_iCols    = _colsMax;
        m_iRows    = _rowsMax;
        m_iSize    = m_iCols * m_iRows;
        if(_vals.isComplex())
        {
            m_matrixCplx = new CplxSparse_t ( _colsMax, _rowsMax );
            m_matrixReal= 0;
            Finalizer<CplxSparse_t> f(*m_matrixCplx);
            double const* const real(_vals.real_get());
            double const* const img(_vals.img_get());
            for(int i(0); i != _indices.rows_get(); ++i)
            {
                m_matrixCplx->insert(_indices.real_get(i, 0) - 1, _indices.real_get(i, 1) - 1) =
                    std::complex<double>(real[i], img[i]);
            }
        }
        else
        {
            m_matrixCplx = 0;
            m_matrixReal= new RealSparse_t (  _colsMax, _rowsMax );
            Finalizer<RealSparse_t> f(*m_matrixReal);
            double const* const real(_vals.real_get());
            for(int i(0); i != _indices.rows_get(); ++i)
            {
                m_matrixReal->insert(_indices.real_get(i, 0) - 1, _indices.real_get(i, 1) - 1) = real[i];
            }
        }
        return;
    }
    /*----------------*/
    /*      val_set     */
    /*----------------*/
    bool Sparse::val_set(int _iRows, int _iCols, double _dblVal)
    {
        return val_set(_iRows, _iCols, _dblVal, 0.);
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

    // TODO: should have both a bounds checking and a non-checking interface to elet access
	double	Sparse::real_get(int _iRows, int _iCols) const
	{
        return m_matrixReal
            ? m_matrixReal->coeff(_iRows, _iCols)
            : m_matrixCplx->coeff(_iRows, _iCols).real();
	}

	double	Sparse::img_get(int _iRows, int _iCols) const
	{
        return m_matrixCplx
            ? m_matrixCplx->coeff(_iRows, _iCols).imag()
            : 0. ;
	}



    /*----------------*/
    /*      val_get     */
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
        m_iRows=  _iNewRows;
        m_iCols= _iNewCols;
        m_iSize=  _iNewRows * _iNewCols;

        return m_matrixReal
            ? ::resize(&m_matrixReal, _iNewRows, _iNewCols)
            : ::resize(&m_matrixCplx, _iNewRows, _iNewCols);
    }
// TODO decide if a complex matrix with 0 imag can be == to a real matrix
// not true for dense
    bool Sparse::operator==(const InternalType& it) const
    {
        Sparse const* otherSparse = dynamic_cast<Sparse const*>(&it);
        return (otherSparse
                && (otherSparse->rows_get() != rows_get())
                && (otherSparse->cols_get() != cols_get())
                && (otherSparse->isComplex() != isComplex())
                && (isComplex()
                    ? equal(*m_matrixCplx, *otherSparse->m_matrixCplx)
                    : equal(*m_matrixReal, *otherSparse->m_matrixReal)));
    }

    bool Sparse::operator!=(const InternalType& it) const
    {
        return !(*this == it);
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
    bool Sparse::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType const* _poSource, bool _bAsVector)
    {
        int iNewRows = Max(_piMaxDim[0], rows_get());
        int iNewCols = Max(_piMaxDim[1], cols_get());
        int iPos     = 0;
        int row      = 0;
        int col      = 0;
        bool res(false);
        //check if the size of _poSource is compatible with the size of the variable
        if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
        {
            return false;
        }
/*
        //check if the count of values is compatible with indexes
        if(_poSource->size_get() != 1 && _poSource->size_get() != _iSeqCount)
        {
            return false;
        }
*/
        // invalid indiex < 1
        if(*std::min_element(_piSeqCoord, _piSeqCoord+ (_bAsVector ? 1:2)* _iSeqCount) < 1)
        {
            return false;
        }
        toCoordsReader tcr(_piSeqCoord,  _bAsVector, _poSource->rows_get(), _poSource->cols_get());
        switch(_poSource->getType())
        {
        case InternalType::RealDouble:
        {
            Double const& src(*_poSource->getAsDouble());
            res= m_matrixReal
                ? doInsert<false>(src, tcr, _iSeqCount, iNewRows, iNewCols, *m_matrixReal)
                : doInsert<true>(src, tcr, _iSeqCount, iNewRows, iNewCols, *m_matrixCplx);
            break;
        }
        case InternalType::RealSparse :
        {
            Sparse const& src(*_poSource->getAsSparse());
            res= m_matrixReal
                ? doInsert<false>(src, tcr, _iSeqCount, iNewRows, iNewCols, *m_matrixReal)
                : doInsert<true>(src, tcr, _iSeqCount, iNewRows, iNewCols, *m_matrixCplx);
            break;
        }
        default: // TODO
        {}
        }
        return res;
    }

    Sparse* Sparse::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse const* _poSource, bool _bAsVector)
    {
        bool const cplx(_poSource->isComplex());
        Sparse* pSp ( _bAsVector ?
                      ((_poSource->cols_get() == 1)
                       ? new Sparse(_piMaxDim[0], 1, _poSource->isComplex())
                       :( (_poSource->rows_get() == 1)
                          ? new Sparse(1, _piMaxDim[0], _poSource->isComplex())
                          : 0))
                      :  new Sparse(_piMaxDim[0], _piMaxDim[1], _poSource->isComplex()));
        if( pSp && !pSp->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) )
        {
            delete pSp;
            pSp= 0;
        }
        return pSp;
    }

    Sparse* Sparse::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)const
    {
        Sparse* pSp ( _bAsVector ?
                      ((rows_get() == 1)
                       ?  new Sparse(1, _piMaxDim[0], isComplex())
                       :new Sparse(_piMaxDim[0], 1, isComplex()))
                      :  new Sparse(_piMaxDim[0], _piMaxDim[1], isComplex()));
        // TODO
        return pSp;
    }

    Sparse::Sparse(Sparse const& o)
        : GenericType(o)
        , m_matrixReal( o.m_matrixReal ? new RealSparse_t(*o.m_matrixReal) : 0)
        , m_matrixCplx( o.m_matrixCplx ? new CplxSparse_t(*o.m_matrixCplx) : 0)
    {
    }

}
