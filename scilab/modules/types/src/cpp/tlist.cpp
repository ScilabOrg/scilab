/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include "arrayof.hxx"
#include "tlist.hxx"
#include "listundefined.hxx"

namespace types
{
    /**
    ** Constructor & Destructor (public)
    */
    TList::TList() : List()
    {
    }

    /**
    ** Private Copy Constructor and data Access
    */
    TList::TList(TList *_oTListCopyMe)
    {
        std::vector<InternalType *>::iterator itValues;
        m_plData = new std::vector<InternalType *>;

        for(int i = 0 ; i < _oTListCopyMe->getData()->size() ; i++)
        {
            InternalType* pIT = (*_oTListCopyMe->getData())[i];
            append(pIT);
        }

        m_iSize = static_cast<int>(m_plData->size());
    }

    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    InternalType* TList::clone()
    {
        return new TList(this);
    }

    bool TList::exists(const std::wstring& _sKey)
    {
        if(getSize() < 1)
        {
            return false;
        }

        String* pS = (*m_plData)[0]->getAs<types::String>();

        //first field is the tlist type
        for(int i = 1 ; i < pS->getSize() ; i++)
        {
            if(std::wstring(pS->get(i)) == _sKey)
            {
                return true;
            }
        }
        return false;
    }

    InternalType* TList::get(const std::wstring& _sKey)
    {
        return get(getIndexFromString(_sKey));
    }

    InternalType* TList::get(const int _iIndex)
    {
        if(getSize() < 2)
        {
            return NULL;
        }

        if(_iIndex > 0 && _iIndex < (*m_plData)[0]->getAs<types::String>()->getSize())
        {
            return (*m_plData)[_iIndex];
        }
        return NULL;
    }

    int TList::getIndexFromString(const std::wstring _sKey)
    {
        if(getSize() < 1)
        {
            return -1;
        }

        String* pS = (*m_plData)[0]->getAs<types::String>();
        //first field is the tlist type
        for(int i = 1 ; i < pS->getSize() ; i++)
        {
            if(std::wstring(pS->get(i)) == _sKey)
            {
                return i;
            }
        }
        return -1;
    }

    std::vector<InternalType*> TList::extractStrings(std::list<std::wstring> _stFields)
    {
        std::vector<InternalType*> Result;

        std::list<std::wstring>::const_iterator it;
        for(it = _stFields.begin() ; it != _stFields.end() ; it++)
        {
            if(exists(*it) == false)
            {
                return Result;
            }
        }

        for(it = _stFields.begin() ; it != _stFields.end() ; it++)
        {
            Result.push_back(get(*it));
        }
        return Result;
    }

    std::wstring TList::getTypeStr()
    {
        if(getSize() < 1)
        {
            return L"";
        }

        return (*m_plData)[0]->getAs<types::String>()->get(0);
    }

    std::wstring TList::getShortTypeStr()
    {
        return getTypeStr();
    }

    bool TList::set(const std::wstring& _sKey, InternalType* _pIT)
    {
        return set(getIndexFromString(_sKey), _pIT);
    }

    bool TList::set(const int _iIndex, InternalType* _pIT)
    {
        if(_iIndex < 0)
        {
            return false;
        }

        while(m_plData->size() <_iIndex)
        {//incease list size and fill with "Undefined"
            m_plData->push_back(new ListUndefined());
            m_iSize = getSize();
        }

        //manage ref on the old value
        InternalType* pOld = (*m_plData)[_iIndex];
        if(pOld)
        {
            pOld->DecreaseRef();
            if(pOld->isDeletable())
            {
                delete pOld;
            }
        }

        _pIT->IncreaseRef();
        (*m_plData)[_iIndex] = _pIT;
        return true;
    }
}
