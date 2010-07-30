/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LISTINSERT_HXX__
#define __LISTINSERT_HXX__

#include <list>
#include "types.hxx"
#include "listoperation.hxx"

namespace types
{
    class ListInsert : public ListOperation
    {
    public :
                                ListInsert(InternalType* _pIT);
        virtual                 ~ListInsert();

        ListInsert*             clone() const;

        ListInsert*             getAsListInsert(void);
        ListInsert const*             getAsListInsert(void) const;
        RealType                getType(void) const;

        void                    whoAmI() const;

        std::string             toString(int _iPrecision, int _iLineLen) const;
        InternalType*           insert_get();

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string     getTypeStr()const {return string("listinsert");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string     getShortTypeStr()const {return string("");}

    private :
        InternalType*           m_pInsert;
    };
}


#endif /* !__LISTINSERT_HXX__ */
