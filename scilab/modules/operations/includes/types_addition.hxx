/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_ADD_H__
#define __TYPES_ADD_H__

#include "dynlib_operations.hxx"
#include "generic_operations.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

void fillAddFunction();

// D DC D1 D1C
//Double
int AddDoubleToDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double** _pDoubleOut);
types::Double* add_D_D(types::Double *_pL, types::Double *_pR);
types::Double* add_D_DC(types::Double *_pL, types::Double *_pR);
types::Double* add_D_D1(types::Double *_pL, types::Double *_pR);
types::Double* add_D_D1C(types::Double *_pL, types::Double *_pR);
types::Double* add_D_E(types::Double *_pL);

types::Double* add_DC_D(types::Double *_pL, types::Double *_pR);
types::Double* add_DC_DC(types::Double *_pL, types::Double *_pR);
types::Double* add_DC_D1(types::Double *_pL, types::Double *_pR);
types::Double* add_DC_D1C(types::Double *_pL, types::Double *_pR);
types::Double* add_DC_E(types::Double *_pL);

types::Double* add_D1_D(types::Double *_pL, types::Double *_pR);
types::Double* add_D1_DC(types::Double *_pL, types::Double *_pR);
types::Double* add_D1_D1(types::Double *_pL, types::Double *_pR);
types::Double* add_D1_D1C(types::Double *_pL, types::Double *_pR);
types::Double* add_D1_E(types::Double *_pL);

types::Double* add_D1C_D(types::Double *_pL, types::Double *_pR);
types::Double* add_D1C_DC(types::Double *_pL, types::Double *_pR);
types::Double* add_D1C_D1(types::Double *_pL, types::Double *_pR);
types::Double* add_D1C_D1C(types::Double *_pL, types::Double *_pR);
types::Double* add_D1C_E(types::Double *_pL, types::Double *_pR);


types::Double* add_E_D(types::Double *_pL, types::Double *_pR);
types::Double* add_E_DC(types::Double *_pL, types::Double *_pR);
types::Double* add_E_D1(types::Double *_pL, types::Double *_pR);
types::Double* add_E_D1C(types::Double *_pL, types::Double *_pR);
types::Double* add_E_E(types::Double *_pL, types::Double *_pR);

//add matrix + matrix ( double, int, bool )
//x + x with T major
//same type
template<typename T> inline static void add(T* l, T* r, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
    }
}

template<typename T, typename U> inline static void add(T* l, U* r, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
    }
}

//x + x with U major
template<typename T, typename U> inline static void add(T* l, U* r, int size, U* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
    }
}

//x + xC with U major ( complex are double )
template<typename T> inline static void add(T* l, T* r, T* rc, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
        oc[i] = rc[i];
    }
}

template<typename T, typename U> inline static void add(T* l, U* r, U* rc, int size, U* o, U* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
        oc[i] = rc[i];
    }
}

//xC + x with T major ( complex are double )
template<typename T, typename U> inline static void add(T* l, T* lc, U* r, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
        oc[i] = lc[i];
    }
}

// xC + xC with T major ( complex are double )
template<typename T> inline static void add(T* l, T* lc, T* r, T* rc, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r[i];
        oc[i] = lc[i] + rc[i];
    }
}

// x + [] and [] + x
template<typename T> inline static void add(T* l, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i];
    }
}

// xC + [] and [] + xC
template<typename T> inline static void add(T* l, T* lc, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i];
        oc[i] = lc[i];
    }
}

//add matrix + scalar ( double, int, bool )
//same tpye
template<typename T> inline static void add(T* l, T r, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r;
    }
}

//x + x1 with T major
template<typename T, typename U> inline static void add(T* l, U r, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r;
    }
}

//x + x1 with U major
template<typename T, typename U> inline static void add(T* l, U r, int size, U* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r;
    }
}

//xC + x1 with T major ( complex are double )
template<typename T, typename U> inline static void add(T* l, T* lc, U r, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r;
        oc[i] = lc[i];
    }
}

//x + x1C with U major ( complex are double )
template<typename T, typename U> inline static void add(T* l, U r, U rc, int size, U* o, U* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r;
        oc[i] = rc;
    }
}

//xC + x1C with T major ( complex are double )
template<typename T> inline static void add(T* l, T* lc, T r, T rc, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l[i] + r;
        oc[i] = lc[i] + rc;
    }
}

//add scalar + matrix ( double, int, bool )
//same type
template<typename T> inline static void add(T l, T* r, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l + r[i];
    }
}

//x1 + x with T major
template<typename T, typename U> inline static void add(T l, U* r, int size, T* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l + r[i];
    }
}

//x1 + x with U major
template<typename T, typename U> inline static void add(T l, U* r, int size, U* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l + r[i];
    }
}

//x1 + xC with U major ( complex are double )
template<typename T, typename U> inline static void add(T l, U* r, U* rc, int size, U* o, U* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l + r[i];
        oc[i] = rc[i];
    }
}

//x1C + x with T major ( complex are double )
template<typename T, typename U> inline static void add(T l, T lc, U* r, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l + r[i];
        oc[i] = lc;
    }
}

//x1C + xC ( complex are double )
template<typename T> inline static void add(T l, T lc, T* r, T* rc, int size, T* o, T* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = l + r[i];
        oc[i] = rc[i];
    }
}


//add scalar + scalar ( double, int, bool )
//x1 + x1 same type
template<typename T> inline static void add(T l, T r, T* o)
{
    *o = l + r;
}

//x1 + x1 with T major
template<typename T, typename U> inline static void add(T l, U r, T* o)
{
    *o = l + r;
}

//x1 + x1 with U major
template<typename T, typename U> inline static void add(T l, U r, U* o)
{
    *o = l + r;
}

//x1 + x1C same type
template<typename T> inline static void add(T l, T r, T rc, T* o, T* oc)
{
    *o = l + r;
    *oc = rc;
}

//x1 + x1C with U major ( complex are double )
template<typename T, typename U> inline static void add(T l, U r, U rc, U* o, U* oc)
{
    *o = l + r;
    *oc = rc;
}

//x1C + x1 with T major ( complex are double )
template<typename T, typename U> inline static void add(T l, T lc, U r, T* o, T* oc)
{
    *o = l + r;
    *oc = lc;
}

//x1C + x1C with T major ( complex are double )
template<typename T, typename U> inline static void add(T l, T lc, U r, U rc, T* o, T* oc)
{
    *o = l + r;
    *oc = lc + rc;
}

//x1 + [] and [] + x1
template<typename T> inline static void add(T l, T* o)
{
    *o = l;
}

//x1c + [] and [] + x1c
template<typename T> inline static void add(T l, T lc, T* o, T* oc)
{
    *o = l;
    *oc = lc;
}

//[] + []
inline static void add()
{
}


//Poly
int AddDoubleToPoly(types::Polynom *_pPoly, types::Double *_pDouble, types::Polynom ** _pPolyOut);
int AddPolyToPoly(types::Polynom* pPoly1, types::Polynom* _pPoly2, types::Polynom ** _pPolyOut);

//String
int AddStringToString(types::String *_pString1, types::String *_pString2, types::String** _pStringOut);

//Sparse
int AddSparseToSparse(types::Sparse *_pSparse1, types::Sparse *_pSparse2, types::Sparse** _pSparseOut);
int AddSparseToDouble(types::Sparse *_pSparse,  types::Double* _pDouble, types::GenericType** _pDoubleOut);
int AddDoubleToSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::GenericType** _pDoubleOut);

#endif /* __TYPES_ADD_H__ */
