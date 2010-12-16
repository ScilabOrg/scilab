/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_HXX__
#define __TYPES_HXX__

#include <vector>
#include <iostream>
#include <string>

#ifdef RESULT_POOL_ALLOC // worse : not for lots of small vectors
#include <boost/pool/pool_alloc.hpp>
#endif

#include "internal.hxx"

using namespace std;

namespace types
{
    /*
    ** Type
    */
    class GenericType : public InternalType
    {

    protected :
        int                         m_iRows;
        int                         m_iCols;
        int                         m_iSize;
        int                         m_iSizeMax;

                                    GenericType() : InternalType(), m_iRows(0), m_iCols(0), m_iSize(0) {}
        virtual                     ~GenericType() {}

        bool                        hasAllIndexesOfRow(int _iRow, int* _piCoord, int _iCoordCount);
        bool                        hasAllIndexesOfCol(int _iCol, int* _piCoord, int _iCoordCount);

    public :
        void                        whoAmI(void) { std::cout << "types::GenericType"; }

        /*commun functions*/
        int                         cols_get();
        int                         rows_get();
        int                         size_get();

        wstring                     DimToString();

        /* GenericType */
        GenericType*                getAsGenericType(void) { return this; }

        /* FIXME : should be : virtual GenericType*	get(int _iPos) = 0; */
        virtual GenericType*        get_col_value(int _iPos) { return NULL; }

        bool                        isIdentity(void);
        virtual bool                isAssignable(void) { return true; }

        virtual RealType            getType(void) { return RealGeneric; }

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring             getTypeStr() {return L"generictype";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring             getShortTypeStr() {return L"";}

        virtual GenericType*        clone(void) = 0;

    };

    static bool isCoordIndex(int _iIndex, int* _piCoord, int _iCoordCount);
#ifdef RESULT_POOL_ALLOC
    typedef std::vector<types::InternalType*, boost::pool_allocator<types::InternalType*> > result_t;
#else
    typedef std::vector<types::InternalType* > result_t;
#endif

}
#endif /* !__TYPES_HXX__ */
