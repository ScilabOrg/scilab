#ifndef MATRIXITERATORS_HXX
#define MATRIXITERATORS_HXX

#include <complex>
#include <utility>
#include <iterator>

#include "double.hxx"
#include "sparse.hxx"


// cannot use const yet because of const-correctness issues
#define CONST

template<typename T>
struct UndefinedAccessorForType;

template<typename S, typename V> V get(S CONST&, int, int);
template<> double get(types::Double CONST& d, int r, int c);
template<> double get(types::Sparse CONST& s, int r, int c);
template<> std::complex<double> get(types::Double CONST& d, int r, int c);
template<> std::complex<double> get(types::Sparse CONST& s, int r, int c);

template<typename S, typename V> bool set(S CONST&, int, int, V);
template<> bool set(types::Double CONST& d, int r, int c, double v);
template<> bool set(types::Sparse CONST& s, int r, int c, double v);
template<> bool set(types::Double CONST& d, int r, int c, std::complex<double> v);
template<> bool set(types::Sparse CONST& s, int r, int c, std::complex<double> v);



template<typename S> int rows(S CONST&s);
template<typename S> int cols(S CONST&s);

template<> int rows(types::Double CONST&d);
template<> int cols(types::Double CONST&d);

template<typename S, typename V> struct Accessor;

template<typename S, typename V, typename Iter>
struct MatrixIterator ;

struct RowWiseFullIterator {};
RowWiseFullIterator rowIter;

template<typename S, typename V>
struct MatrixIterator<S, V, RowWiseFullIterator> ;
template<typename S, typename V  >
struct MatrixIterator<S, V, std::pair<int const*, bool> > ;

template<typename V, typename S, typename Iter>
MatrixIterator<S,V, Iter> makeMatrixIterator(S& s, Iter i);

template<typename S>
struct IteratorFromVar;

template<typename S> IteratorFromVar<S> makeIteratorFromVar(S& s);


template<typename In, typename Out, typename Sz>
Out copy_n(In i, Out o, Sz n);
#endif
