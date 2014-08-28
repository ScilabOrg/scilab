/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __USER_HXX__
#define __USER_HXX__

#include "types.hxx"
#include "overload.hxx"

namespace types
{
class UserType : public InternalType
{
public :
    UserType() {}
    virtual	                ~UserType() {}

    /*** non virtual function to prevent overriding in user derived class ***/

    inline ScilabType       getType(void)
    {
        return ScilabUserType;
    }

    inline ScilabId         getId(void)
    {
        return IdUserType;
    }

    bool                    isUserType(void)
    {
        return true;
    }

public :
    /*** User will be asked to implement the following methods      ***/
    /*** in order Scilab engine to manage correctly this user type  ***/

    std::wstring    getTypeStr() = 0;
    std::wstring    getShortTypeStr() = 0;
    InternalType*   clone() = 0;

public :
    /*** use can overload these methode ***/
    /*** all methode not overloaded will call scilab overload       ***/

    virtual bool    hasToString()
    {
        // hasToStrig return false so sclab will call overload %..._p
        // and toString methode is useless
        return false;
    }

    virtual InternalType* extract(typed_list* _pArgs)
    {
        return NULL;
    }
};
}

#endif /* !__USER_HXX__ */
