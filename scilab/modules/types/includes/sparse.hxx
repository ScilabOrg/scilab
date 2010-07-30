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

    // TODO make iterator returned by asVectorBegin<double>(Tconst&) and asVectorBegin<std::complex<double> >(Tconst&) functions
    template<typename T>
    struct valuesReader
    {
        explicit valuesReader(T const& src_):src(src), r(0),c(0)
        {
        }
        void operator++()
        {
            if(++r > src.rows_get())
            {
                if(++c > src.cols_get())// TODO check that wrap around is more convenient than fail
                {
                    c=0;
                }
                r=0;
            }
        }
        T const& src;
        int r,c;
    };

    template<bool cplx, typename T>
    struct valuesGetter : valuesReader<T>
    {
        valuesGetter(T const& t): valuesReader<T>(t)
        {
        }
        double get() const
        {
            std::cerr<<"getting "<<this->src.real_get(this->r, this->c)<<std::endl;
            return this->src.real_get(this->r, this->c);
        }
    };

    template<typename T>
    struct valuesGetter<true,T> : valuesReader<T>
    {
        valuesGetter(T const& t): valuesReader<T>(t)
        {
        }
        std::complex<double> get() const
        {
            return this->src.cplx_get(this->r, this->c);
        }
    };
    template<>
    struct valuesGetter<true, types::Double> : valuesReader<types::Double>
    {
        valuesGetter(types::Double const& t): valuesReader<types::Double>(t)
        {
        }
        std::complex<double> get() const
        {
            return std::complex<double>(this->src.real_get(this->r, this->c), this->src.img_get(this->r, this->c));
        }
    };

    struct toCoordsReader
    {
        explicit toCoordsReader(int* coords, bool asVector, int rows, int cols)
  : it(coords), v(asVector), r(rows), c(cols), s(rows*cols)
        {
        }
        std::pair<int, int> operator()()
        {
            std::pair<int, int> res(*it, *it);
            ++it;
            if(v)
            {
                res.first/= r;
                res.second%= r;
            }
            else
            {
                res.second= *it;
                ++it;
            }
            return res;
        }
        int const* it;
        bool const v;
        int const r, c, s;
    };
}

namespace types
{
    class Sparse : public GenericType
    {
    public :
	virtual			    ~Sparse();
				    Sparse(Double *_obj);
				    Sparse(Double *_indices, Double *_vals);
				    Sparse(Double *_indices, Double *_vals, Double *_dims);
        Sparse(int _iRows, int _iCols, bool cplx=false);

        /*data management*/
        bool                        			val_set(int _iRows, int _iCols, double _dblReal);
        bool                       			val_set(int _iRows, int _iCols, double _dblReal, double _dblImg);
	std::complex<double> 	   	        	val_get(int _iRows, int _iCols) const;
	SparseMatrix<std::complex<double> >* 		matrix_get();
	SparseMatrix<std::complex<double> >const* 		matrix_get()const;

        double real_get(int r, int c) const;
        double img_get(int r, int c) const;
        std::complex<double> cplx_get(int r, int c) const;

        bool isComplex() const;
        /*zero set filler*/
        bool                        zero_set();

        /*Config management*/
        void                        whoAmI() const;
	bool 			    isExtract() const;
        Sparse*                     getAsSparse(void);
        Sparse const*                     getAsSparse(void) const;
	Sparse* 		    clone(void) const;
        string                      toString(int _iPrecision, int _iLineLen) const;
        bool                        resize(int _iNewRows, int _iNewCols);
        bool                        insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType const* _poSource, bool _bAsVector);
        static Sparse*              insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Sparse const* _poSource, bool _bAsVector);
        Sparse*                     extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) const;


        bool                        operator==(const InternalType& it) const;
        bool                        operator!=(const InternalType& it) const;

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string         getTypeStr() const {return string("sparse");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string	    getShortTypeStr() const {return string("constant");}
    protected :
        RealType                    getType(void) const;

    private :
        Sparse(Sparse const& o);
        void CreateSparse(Double const& _indices, std::size_t _rowsMax, std::size_t _colsMax, Double const& _vals);
        typedef SparseMatrix<double >	RealSparse_t;
        typedef SparseMatrix<std::complex<double > >	CplxSparse_t;
        RealSparse_t* m_matrixReal;
        CplxSparse_t* m_matrixCplx;

    template<bool cplx, typename Src, typename Tcr, typename Sz, typename Dest>
    bool doInsert(Src const& src, Tcr& tcr, Sz const n, Sz const maxR, Sz const maxC, Dest& dest)
    {
        if(resize(maxR, maxC) == false)
        {
            return false;
        }
        valuesGetter<cplx, Src> vr(src);
        Finalizer<Dest> f(dest);
        for(Sz i(0); i != n; ++i, ++vr)
        {
            std::pair<int, int> c(tcr());
            std::cerr<<"inserting at"<<c.first<<" "<<c.second<<std::endl;
            dest.insert(c.first, c.second)= vr.get();
        }
        return true;
    }

    };
}

#endif /* !__SPARSE_HH__ */
