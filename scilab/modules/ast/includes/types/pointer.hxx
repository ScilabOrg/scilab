/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __POINTER_HXX__
#define __POINTER_HXX__

#include "user.hxx"

namespace types
{
class Pointer : public UserType
{
public :
    Pointer()
    {
        m_pvData = NULL;
    }

    Pointer(void* _pvData)
    {
        m_pvData = _pvData;
    }

    Pointer(int _rows, int _cols, void* _pvData)
    {
        m_iRows = _rows;
        m_iCols = _cols;
        m_pvData = _pvData;
    }
    virtual ~Pointer() {}

    bool hasToString()
    {
        return false;
    }

    std::wstring getTypeStr()
    {
        return L"pointer";
    }

    std::wstring getShortTypeStr()
    {
        return L"ptr";
    }

    bool isPointer(void)
    {
        return true;
    }

    bool toString(std::wostringstream& ostr)
    {
        return true;
    }

    InternalType* clone()
    {
        return new Pointer(m_pvData);
    }

    void* get()
    {
        return m_pvData;
    }

    void set(void* _pvData)
    {
        m_pvData = _pvData;
    }

    virtual bool isAssignable(void)
    {
        return true;
    }

protected :

    void* m_pvData;
};
}

#endif /* !__USER_HXX__ */
