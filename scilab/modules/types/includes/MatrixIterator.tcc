#ifndef MATRIXITERATORS_TCC /* -*- C++ -*- */
#define MATRIXITERATORS_TCC

#include "MatrixIterator.hxx"


#include <complex>
#include <utility>
#include <iterator>

#include "double.hxx"
#include "sparse.hxx"

// cannot use const yet because of const-correctness issues
#define CONST

template<typename T>
struct UndefinedAccessorForType {};

template<typename S, typename V> V get(S CONST&, int, int){ return UndefinedAccessorForType<S>(); }
template<> double get(types::Double CONST& d, int r, int c){ return d.real_get(r, c); }
template<> double get(types::Sparse CONST& s, int r, int c){ return s.real_get(r, c); }
template<> std::complex<double> get(types::Double CONST& d, int r, int c){ return std::complex<double>(d.real_get(r, c), d.img_get(r,c)); }
template<> std::complex<double> get(types::Sparse CONST& s, int r, int c){ return s.cplx_get(r, c); }

template<typename S, typename V> bool set(S CONST&, int, int, V){ return UndefinedAccessorForType<S>(); }
template<> bool set(types::Double CONST& d, int r, int c, double v){ return d.val_set(r, c, v); }
template<> bool set(types::Sparse CONST& s, int r, int c, double v){ return s.val_set(r, c, v); }
template<> bool set(types::Double CONST& d, int r, int c, std::complex<double> v){ return d.val_set(r, c, v.real(), v.imag()); }
template<> bool set(types::Sparse CONST& s, int r, int c, std::complex<double> v){ return s.val_set(r, c, v); }


template<typename S> int rows(S CONST&s) { return s.rows();}
template<typename S> int cols(S CONST&s) { return s.cols();}

template<> int rows(types::Double CONST&d) { return d.rows_get();}
template<> int cols(types::Double CONST&d) { return d.cols_get();}
template<> int rows(types::Sparse CONST&s) { return s.rows_get();}
template<> int cols(types::Sparse CONST&s) { return s.cols_get();}

template<typename S, typename V> struct Accessor
{
    Accessor(S& s_, int r_, int c_): s(s_), r(r_), c(c_) {}
    operator V() CONST
    {
        return get<S,V>(s, r, c);
    }
    Accessor& operator=(V const& v)
    {
        ::set<S,V>(s, r, c, v);
        return *this;
    }
private:
    S& s;
    int r, c;
};

template<typename V, int _Options, typename _Index>
struct Accessor<Eigen::SparseMatrix<V, _Options, _Index>, V >
{
    Accessor( Eigen::SparseMatrix<V, _Options, _Index>& s_, int r_, int c_): s(s_), r(r_), c(c_) {}
    operator V() CONST
    {
        return s.coeff(r, c);
    }
    Accessor& operator=(V const& v)
    {
        s.insert(r, c)= v;
        return *this;
    }
private:
    Eigen::SparseMatrix<V, _Options, _Index>& s;
    int r, c;
};

template<int _Options, typename _Index>
struct Accessor<Eigen::SparseMatrix<double, _Options, _Index>, std::complex<double> >
{
    Accessor(Eigen::SparseMatrix<double, _Options, _Index>& s_, int r_, int c_): s(s_), r(r_), c(c_) {}
    operator std::complex<double>() CONST
    {
        return std::complex<double>(s.coeff(r, c), 0.);
    }
    Accessor& operator=(std::complex<double> const& v)
    {
        s.insert(r, c)= v.real();
        return *this;
    }
private:
    Eigen::SparseMatrix<double, _Options, _Index>& s;
    int r, c;
};

template<int _Options, typename _Index>
struct Accessor<Eigen::SparseMatrix<std::complex<double>, _Options, _Index>, double >
{
    Accessor(Eigen::SparseMatrix<std::complex<double> >& s_, int r_, int c_): s(s_), r(r_), c(c_) {}
    operator double() CONST
    {
        return s.coeff(r, c).real();
    }
    Accessor& operator=(double const& v)
    {
        s.insert(r, c)= std::complex<double>(v, 0.);
        return *this;
    }
private:
    Eigen::SparseMatrix<std::complex<double> >& s;
    int r, c;
};

template<typename S, typename V, typename Iter>
struct MatrixIterator : std::iterator<std::forward_iterator_tag, V>
{
    MatrixIterator(S& s_, Iter i_): s(s_), i(i_)
    {
    }
    MatrixIterator& operator++()
    {
        ++i;
        return *this;
    }
    MatrixIterator operator++(int)
    {
        MatrixIterator tmp(*this);
        ++i;
        return tmp;
    }
    Accessor<S,V> operator*()
    {
        return Accessor<S,V>(s, (*i).first, (*i).second);
    }
private:
    S& s;
    Iter i;
};


template<typename S, typename V>
struct MatrixIterator<S, V, RowWiseFullIterator> : std::iterator<std::forward_iterator_tag, V>
{
    MatrixIterator(S& s_, RowWiseFullIterator): s(s_), r(0), c(0)
    {
    }
    MatrixIterator& operator++()
    {
        if(++r == rows(s))
        {
            r=0;
            if(++c == cols(s))
            {/* wrap around */
                r=c=0;
            }
        }
        return *this;
    }
    MatrixIterator operator++(int)
    {
        MatrixIterator tmp(*this);
        ++(*this);
        return tmp;
    }
    Accessor<S,V> operator*()
    {
        return Accessor<S,V>(s, r, c);
    }
private:
    S& s;
    int r, c;
};
template<typename S, typename V  >
struct MatrixIterator<S, V, std::pair<int const*, bool> > : std::iterator<std::forward_iterator_tag, V>
{
    MatrixIterator(S& s_, std::pair<int const*, bool> cv): s(s_), c(cv.first), asVector(cv.second)
    {
    }
    MatrixIterator& operator++()
    {
        c+= asVector ? 1 : 2;
        return *this;
    }
    MatrixIterator operator++(int)
    {
        MatrixIterator tmp(*this);
        ++(*this);
        return tmp;
    }
    Accessor<S,V> operator*()
    {
        return asVector ?  Accessor<S,V>(s, c[0] % rows(s), c[0] / rows(s) ) : Accessor<S,V>(s, c[0], c[1]);
    }
private:
    S& s;
    int const* c;
    bool asVector;
};

template<typename V, typename S, typename Iter>
MatrixIterator<S,V, Iter> makeMatrixIterator(S& s, Iter i){ return MatrixIterator<S,V,Iter>(s, i);}

template<typename S>
struct IteratorFromVar{
};

template<>
struct IteratorFromVar<types::Double>:std::iterator<std::forward_iterator_tag, std::pair<int, int> >
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
    std::pair<int, int> operator*()
    {
        return std::pair<int,int>(static_cast<int>(d.real_get(r,0)), d.real_get(r,1));
    }
private:
    types::Double& d;
    int r;
};

template<typename S> IteratorFromVar<S> makeIteratorFromVar(S& s) { return IteratorFromVar<S>(s); }


template<typename In, typename Out, typename Sz>
Out copy_n(In i, Out o, Sz n){
    for(; n; --n, ++i, ++o)
    {
        *o= *i;
    }
    return o;
}
#endif
