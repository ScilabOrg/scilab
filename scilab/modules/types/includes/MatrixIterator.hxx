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

/**
   This free function overloads perform read access into a 2D container, using 0-based indices.
   @param s the 2D structure used to fetch a value of type V.
   @param r : the row (0 based)
   @param c : the column (0 based)
   @return : the value of type V at row r and column c of structure s
*/
template<typename S, typename V> V get(S CONST& s, int r, int c);

template<> double get(types::Double CONST& d, int r, int c);
template<> std::complex<double> get(types::Double CONST& d, int r, int c);

template<> double get(types::Sparse CONST& s, int r, int c);
template<> std::complex<double> get(types::Sparse CONST& s, int r, int c);

template<> double get(Eigen::SparseMatrix<double, 0, int>CONST& s, int r, int c);
template<> std::complex<double> get(Eigen::SparseMatrix<double, 0, int>CONST& s, int r, int c);

template<> double get(Eigen::SparseMatrix<std::complex<double>, 0, int>CONST& s, int r, int c);
template<> std::complex<double> get(Eigen::SparseMatrix<std::complex<double>, 0, int>CONST& s, int r, int c);


/**
   This free function overloads perform write access into a 2D container, using 0-based indices.
   @param s the 2D structure used to fetch a value of type V.
   @param r : the row (0 based)
   @param c : the column (0 based)
   @param v : the value of type V to set at row r and column c of structure s
   @return : true iff everything went ok (should throw otherwise anyway).
*/
template<typename S, typename V> bool set(S CONST&, int, int, V);

template<> bool set(types::Double& d, int r, int c, double v);
template<> bool set(types::Double& d, int r, int c, std::complex<double> v);

template<> bool set(types::Sparse& s, int r, int c, double v);
template<> bool set(types::Sparse& s, int r, int c, std::complex<double> v);

template<> bool set(Eigen::SparseMatrix<double, 0, int>& s, int r, int c, double v);
template<> bool set(Eigen::SparseMatrix<double, 0, int>& s, int r, int c, std::complex<double> v);

template<> bool set(Eigen::SparseMatrix<std::complex<double>, 0, int>& s, int r, int c, double v);
template<> bool set(Eigen::SparseMatrix<std::complex<double>, 0, int>& s, int r, int c, std::complex<double> v);

/**
  These free function overloads handle nb of rows size queries for 2D containers
   wrapping the corresponding member function.
   @param s : 2D structure to query
   @return : nb of rows
*/
template<typename S> int rows(S CONST&s);
template<> int rows(types::Double CONST&d);

/**
  These free function overloads handle nb of cols size queries for 2D containers
   wrapping the corresponding member function.
   @param s : 2D structure to query
   @return : nb of cols
*/
template<typename S> int cols(S CONST&s);
template<> int cols(types::Double CONST&d);

/* this proxy struct provides read and write access (using set and get)
   with the usual operators (operator*() and operator=() )*/
template<typename S, typename V> struct Accessor{
/**
   @param s_ : 2D structure to access
   @param r_ : row to access
   @param c_ ; column to access
*/
    Accessor(S& s_, int r_, int c_): s(s_), r(r_), c(c_) {}
/**
   read accessor as a casting operator
   @return : value of s at (r,c)
 */
    operator V() CONST
    {
        std::cerr<<"reading "<<get<S,V>(s, r, c)<<" @("<<r<<","<<c<<")\n";
        return ::get<S,V>(s, r, c);
    }
/**
   write accessor as an assignment operator
   @param v : value to set at (r,c) in s.
*/
    Accessor& operator=(V const& v)
    {
        std::cerr<<"writing "<<v<<" @("<<r<<","<<c<<")\n";
        ::set<S,V>(s, r, c, v);
        return *this;
    }
private:
    S& s;
    int r, c;
};

/* convenient typedef for pairs of (row, column) int values used as 2D coords */
typedef std::pair<int, int> Coords2D;
/* convenient typedef for iterator over pairs of (row, column) int values used as 2D coords */
typedef std::iterator<std::forward_iterator_tag, Coords2D > Coords2DIterator;
/**
   Iterator over coords making a full row-wise traversal wrapping around when reaching
   the end of the 2D container.
 */
struct RowWiseFullIterator : Coords2DIterator
{
/**
   @param cMax : size of the 2D structure
 */
    RowWiseFullIterator(Coords2D cMax): c(0,0), cMax(cMax)
    {
    }
/**
   @param rm : nb of rows of the 2D structure
   @param cm : nb of column of the 2D structure
 */
    RowWiseFullIterator(int rm, int cm): c(0,0), cMax(rm, cm)
    {
    }
    RowWiseFullIterator& operator++()
    {
        if(++c.first == cMax.first)
        {
            c.first= 0;
            if(++c.second == cMax.second)
            {/* wrap around */
                c.first= c.second= 0;
            }
        }
        return *this;
    }
    RowWiseFullIterator operator++(int)
    {
        RowWiseFullIterator tmp(*this);
        ++(*this);
        return tmp;
    }
    std::pair<int, int> operator*() const
    {
        return c;
    }
private:
    Coords2D c;
    Coords2D const cMax;
};

/**
 * Template for iterator over 2D coords from an int*.
 * Could handle wrap around with a length arg (i.e. to recycle values instead of raising
 * "error 15 Submatrix incorrectly defined."
 */
template<bool AsVector=false> struct Coords : Coords2DIterator
{
    Coords(int CONST* coords, int unused=0):coords(coords)
    {
    }
    Coords& operator++()
    {
        coords+= 2;
    }
    Coords& operator++(int)
    {
        Coords tmp(*this);
        ++(*this);
        return tmp;
    }
    Coords2D operator*()const
    {
        return Coords2D(coords[0]-1, coords[1]-1);
    }
private:
    int const* coords;
};
/**
   explicit specialization for 2D from 1D int* sequences
   (The 2D strcture is considered as a vector)
 */
template<> struct Coords<true> : Coords2DIterator
{
    Coords(int CONST* coords, int rMax):coords(coords), rMax(rMax)
    {
    }
    Coords& operator++()
    {
        ++coords;
    }
    Coords operator++(int)
    {
        Coords tmp(*this);
        ++(*this);
        return tmp;
    }
    Coords2D operator*()const
    {
        return Coords2D((coords[0]-1) % rMax, (coords[0]-1) / rMax);
    }
private:
    int const* coords;
    int const rMax;
};
/* This 'iterator' class allows traverses the 2D containers, either
Rowwise full traversal
or with 2D coords from another matrix
or with 1D coords from another vector (1x) matrix
to respect Double insert() API, we take int* and a bool
*/
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

template<typename V, typename S, typename Iter>
MatrixIterator<S,V, Iter> makeMatrixIterator(S& s, Iter i);

template<typename S> struct IteratorFromVar;

template<typename S> IteratorFromVar<S> makeIteratorFromVar(S& s);

struct Adjacency
{
    Adjacency(double const* x, double const*a): xadj(x), adjncy(a){}
    double const* xadj;
    double const* adjncy;
};




template<typename In, typename Sz, typename Out>
Out copy_n(In i, Sz n, Out o);
#endif
