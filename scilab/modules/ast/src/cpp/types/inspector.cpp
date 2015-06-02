/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>
#include <iostream>
#include <map>

#include "inspector.hxx"
#include "types.hxx"

namespace types
{
#ifndef NDEBUG
std::list<InternalType*> Inspector::m_vIT;

size_t Inspector::getItemCount()
{
    return m_vIT.size();
}

size_t Inspector::getUnreferencedItemCount()
{
    int iCount = 0;
    for (auto it : m_vIT)
    {
        if (it->getRef() == 0)
        {
            iCount++;
        }
    }
    return iCount;
}


void Inspector::addItem(InternalType* _pIT)
{
    types::GenericType* pGT = _pIT->getAs<types::GenericType>();
    m_vIT.push_back(_pIT);
}

void Inspector::removeItem(InternalType* _pIT)
{
    m_vIT.remove(_pIT);
}

InternalType* Inspector::getItem(size_t _iPos)
{
    if (m_vIT.size() > _iPos)
    {
        return *std::next(m_vIT.begin(), _iPos);
    }

    return NULL;
}

InternalType* Inspector::getUnreferencedItem(size_t _iPos)
{
    size_t iCount = 0;
    for (auto it : m_vIT)
    {
        if (it->getRef() == 0)
        {
            if (iCount == _iPos)
            {
                std::wcout << L"getUnreferencedItem : " << it << std::endl;
                return it;
            }
            iCount++;
        }
    }

    return NULL;
}

std::wstring Inspector::showItem(size_t _iPos)
{
    std::wstring st;
    InternalType* pIT = getItem(_iPos);
    if (pIT == NULL)
    {
        st = L"NULL";
    }
    else
    {
        st = pIT->getTypeStr();
    }
    return st;
}

std::wstring Inspector::showUnreferencedItem(size_t _iPos)
{
    std::wstring st;
    InternalType* pIT = getUnreferencedItem(_iPos);
    if (pIT == NULL)
    {
        st = L"NULL";
    }
    else
    {
        st = pIT->getTypeStr();
    }
    return st;
}

void Inspector::deleteItems()
{
    for (auto it : m_vIT)
    {
        delete it;
    }

    m_vIT.clear();
}

void Inspector::displayMemleak()
{
    std::map<std::wstring, size_t> statistics;

    if (m_vIT.size() != 0)
    {
        // construct the statistic map
        for (auto it : m_vIT)
        {
            statistics[it->getTypeStr()]++;
        }

        // display the result
        std::wcerr << L"Memory leaked, please file a bug on http://bugzilla.scilab.org" << std::endl;
        for (auto it = statistics.begin(), itEnd = statistics.end(); it != itEnd; ++it)
        {
            std::wcerr << L"    " << it->second << L" " << it->first;

            // list the not free-ed pointers
            std::wcerr << L" : ";
            bool isFirst = true;
            for (auto pi : m_vIT)
            {
                if (it->first == pi->getTypeStr())
                {
                    if (isFirst)
                    {
                        isFirst = false;
                    }
                    else
                    {
                        std::wcerr << " , ";
                    }

                    std::wcerr << pi;
                }
            }

            std::wcerr << std::endl;
        }
    }
}

#endif
}
