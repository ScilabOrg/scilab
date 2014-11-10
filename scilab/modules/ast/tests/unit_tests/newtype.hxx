/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "user.hxx"

class NewType : public types::UserType
{
public:
    NewType() {}
    ~NewType() {}

    bool                    hasToString()
    {
        return true;
    }

    bool                    toString(std::wostringstream& ostr)
    {
        ostr << "This is my type." << std::endl;
    }

    std::wstring            getTypeStr()
    {
        return L"NewType";
    }

    std::wstring            getShortTypeStr()
    {
        return L"nt";
    }

    types::InternalType*    clone()
    {
        return new NewType();
    }

};
