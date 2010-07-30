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

                                    GenericType() : InternalType(), m_iRows(0), m_iCols(0), m_iSize(0) {}
        virtual                     ~GenericType() {}

    public :
        void                        whoAmI(void) const { std::cout << "types::GenericType"; }

        /*commun functions*/
        int                         cols_get() const;
        int                         rows_get() const;
        int                         size_get() const;

        std::string                 DimToString() const;

        /* GenericType */
        GenericType*                getAsGenericType(void) { return this; }
        GenericType const*                getAsGenericType(void)const { return this; }

        /* FIXME : should be : virtual GenericType*	get(int _iPos) = 0; */
        virtual GenericType const*        get_col_value(int _iPos) const{ return NULL; }

        bool                        isIdentity(void) const;
        virtual bool                isAssignable(void) const { return true; }

        virtual RealType            getType(void) const{ return RealGeneric; }

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string         getTypeStr() const {return string("generictype");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string         getShortTypeStr() const {return string("");}
    };
}
#endif /* !__TYPES_HXX__ */
