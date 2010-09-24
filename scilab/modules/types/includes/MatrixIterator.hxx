 /*
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

/*
  In order to reuse code for the various Matrix Classes, we need some uniform API to access elements.
  We cannot use runtime polymorphism (with dynamic dispatching) because of the runtime cost so we have to
  use compile-time polymorphism with templates.

  The provided free function templates get<>() and set<>() provide such an uniform API.

  In order to perform element-wise operations on Matrices (copy, partial or total assignments, etc.),
  we provide an iterator. To enable reading (with get<>()) or writiting (with set<>()) we provide an Accessor<> proxy.

  As is it common to iterate over a sub-matrix according to indices given by a Scilab variable (Double) we provide
  an iterator created from such a variable (IteratorFromVar).
 */

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
