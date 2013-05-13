/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LISTEMPTY_HXX__
#define __LISTEMPTY_HXX__

#include "types.hxx"
#include "listoperation.hxx"
#include "dynlib_types.h"

namespace types
{
class TYPES_IMPEXP ListEmpty : public ListOperation
{
public :
    ListEmpty(): ListOperation() {};
    virtual                 ~ListEmpty();

    // FIXME : Should not return NULL;
    InternalType*           clone();

    RealType                getType(void);
    bool                    isListEmpty()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::wostringstream& ostr);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"listempty";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"";
    }
};
}


#endif /* !__LISTEMPTY_HXX__ */
