 /*  -*- C++ -*-
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef MATRIXITERATORS_TCC
#define MATRIXITERATORS_TCC

#include "MatrixIterator.hxx"


#include <complex>
#include <utility>
#include <iterator>

#include "double.hxx"
#include "sparse.hxx"

#include <Eigen/Core>
// cannot use const yet because of const-correctness issues
#define CONST

template<typename T>
struct UndefinedAccessorForType {};


template<> double get(types::Double CONST& d, int r, int c){ return d.real_get(r, c); }
template<> std::complex<double> get(types::Double CONST& d, int r, int c){ return std::complex<double>(d.real_get(r, c), d.img_get(r,c)); }

template<> double get(types::Sparse CONST& s, int r, int c){ return s.real_get(r, c); }
template<> std::complex<double> get(types::Sparse CONST& s, int r, int c){ return s.cplx_get(r, c); }

template<> double get(Eigen::SparseMatrix<double, 0, int> CONST&s, int r, int c){ return s.coeff(r, c);}
template<> std::complex<double> get(Eigen::SparseMatrix<double, 0, int> CONST&s, int r, int c){ return std::complex<double>(s.coeff(r, c), 0.);}

template<> double get(Eigen::SparseMatrix<std::complex<double>, 0, int> CONST&s, int r, int c){ return s.coeff(r, c).real();}
template<> std::complex<double> get(Eigen::SparseMatrix<std::complex<double>, 0, int> CONST&s, int r, int c){ return s.coeff(r, c);}

template<typename V, typename S> V get(S CONST&, int, int){ return UndefinedAccessorForType<S>(); }

template<> bool set(types::Double & d, int r, int c, double v){ return d.val_set(r, c, v); }
template<> bool set(types::Double & d, int r, int c, std::complex<double> v){ return  d.val_set(r, c, v.real(), v.imag());}

template<> bool set(types::Sparse & s, int r, int c, double v){ return s.val_set(r, c, v); }
template<> bool set(types::Sparse & s, int r, int c, std::complex<double> v){ return s.val_set(r, c, v); }

template<> bool set(Eigen::SparseMatrix<double, 0, int>& s, int r, int c, double v)
{
    if(v != 0.)
    {
        s.insert(r,c)=v;
    }
    return true;
}

template<> bool set(Eigen::SparseMatrix<double, 0, int>& s, int r, int c, std::complex<double> v)
{
    if( v.real() != 0.)
    {
        s.insert(r,c)=v.real();
    }
    return  true;
}
// should we make this a compile error ?
template<> bool set(Eigen::SparseMatrix<std::complex<double>, 0, int>& s, int r, int c, double v)
{
    if(v != 0.)
    {
        s.insert(r,c)=std::complex<double>(v);
    }
    return true;
}

namespace
{
    std::complex<double> const cplxZero(0., 0.);
}
template<> bool set(Eigen::SparseMatrix<std::complex<double>, 0, int>& s, int r, int c, std::complex<double> v)
{
    if(v != cplxZero)
    {
        s.insert(r,c)=v;
    }
    return true;
}

template<typename S, typename V> bool set(S &, int, int, V){ return UndefinedAccessorForType<S>(); }

template<typename S> int rows(S CONST&s) { return s.rows();}
template<typename S> int cols(S CONST&s) { return s.cols();}

template<> int rows(types::Double CONST&d) { return d.rows_get();}
template<> int cols(types::Double CONST&d) { return d.cols_get();}
template<> int rows(types::Sparse CONST&s) { return s.rows_get();}
template<> int cols(types::Sparse CONST&s) { return s.cols_get();}


template<typename V, typename S, typename Iter>
MatrixIterator<S,V, Iter> makeMatrixIterator(S& s, Iter i){ return MatrixIterator<S,V,Iter>(s, i);}

template<typename S> struct IteratorFromVar { };

template<> struct IteratorFromVar<types::Double> : Coords2DIterator
{
    IteratorFromVar(types::Double& d_):d(d_),r(0)
    { // check dimension ?
    }

    IteratorFromVar& operator++()
    {
        ++r;
        return *this;
    }
    IteratorFromVar operator++(int)
    {
        IteratorFromVar tmp(*this);
        ++r;
        return tmp;
    }
    Coords2D operator*()
    {
        return std::pair<int,int>(static_cast<int>(d.real_get(r,0)-1), d.real_get(r,1)-1);
    }
private:
    types::Double& d;
    int r;
};

/*
  iterator from adjacency matrices :
 */
template<> struct IteratorFromVar<Adjacency> : Coords2DIterator
{
    IteratorFromVar(Adjacency& a):xadj(a.xadj), adjncy(a.adjncy),c(1), nb(1)
    {
        update();
    }

    IteratorFromVar& operator++()
    {
        ++nb;
        update();
        ++adjncy;
        return *this;
    }
    IteratorFromVar operator++(int)
    {
        IteratorFromVar tmp(*this);
        ++nb;
        update();
        ++adjncy;
        return tmp;
    }
    std::pair<int, int> operator*()
    {
        return std::pair<int,int>(static_cast<int>(*adjncy)-1, c-1);
    }
private:
    void update()
    {
        for(; xadj[1] <= nb; ++c, ++xadj)
        {
        }
    }
    double const* xadj;
    double const* adjncy;
    int c;
    std::size_t nb;
};

template<typename S> IteratorFromVar<S> makeIteratorFromVar(S& s) { return IteratorFromVar<S>(s); }


template<typename In, typename Sz, typename Out>
Out copy_n(In i, Sz n, Out o){
    for(; n; --n, ++i, ++o)
    {
        *o= *i;
    }
    return o;
}
#endif
