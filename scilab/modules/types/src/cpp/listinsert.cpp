/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include <cstdio>

#include "listinsert.hxx"
#include "localization.h"

namespace types
{

    ListInsert::ListInsert(InternalType* _pIT)
    {
        _pIT->IncreaseRef();
        m_pInsert = _pIT;
    }

    ListInsert::~ListInsert()
    {
        if(m_pInsert != NULL)
        {
            m_pInsert->DecreaseRef();
            if(m_pInsert->isDeletable())
            {
                delete m_pInsert;
                m_pInsert = NULL;
            }
        }
    }

    void ListInsert::whoAmI()
    {
        std::cout << "types::ListInsert";
    }

    InternalType* ListInsert::clone(void)
    {
        return new ListInsert(m_pInsert);
    }

    InternalType::RealType ListInsert::getType(void)
    {
        return RealListInsertOperation;
    }

    std::wstring ListInsert::toString(int _iPrecision, int _iLineLen)
    {
        std::wostringstream ostr;

        //FIXME : Implement me.
        ostr << L"FIXME : Implement ListInsert::toString" << std::endl;
        return ostr.str();
    }

    InternalType* ListInsert::getInsert()
    {
        return m_pInsert;
    }
}
