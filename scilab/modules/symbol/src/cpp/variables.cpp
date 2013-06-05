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

#include "variables.hxx"

namespace symbol
{
VarBox::VarBox(int _iLevel, types::InternalType* _pIT, bool _bGlobal, bool _bGlobalVisible)
    : m_iLevel(_iLevel), m_pIT(_pIT), m_bGlobal(_bGlobal), m_bGlobalVisible(_bGlobalVisible)
{
    m_pIT->IncreaseRef();
}

void Variables::put(const Symbol& _key, types::InternalType& _pIT)
{
    put(_key, _pIT, m_iLevel);
}

void Variables::put(const Symbol& _key, types::InternalType& _iT, int _iLevel)
{
    VarMap::iterator itVar = m_vars.find(_key);
    VarBoxList* pL = NULL;
    VarBox* pBox = NULL;

    if (itVar != m_vars.end())
    {
        //item alreay exists

        //check scope level
        VarBoxList::reverse_iterator itBox = itVar->second->rbegin();

        bool bInserted = false;
        for (; itBox != itVar->second->rend() ; itBox++)
        {
            pBox = *itBox;
            if ((*itBox)->m_iLevel > _iLevel)
            {
                continue;
            }
            else if ((*itBox)->m_iLevel < _iLevel)
            {
                pBox = new VarBox(_iLevel, &_iT);
                itVar->second->insert(itBox.base(), pBox);
                bInserted = true;
                break;
            }
            else if ((*itBox)->m_iLevel == _iLevel)
            {
                if ((*itBox)->m_pIT == &_iT)
                {
                    return;
                }

                (*itBox)->m_pIT->DecreaseRef();
                if ((*itBox)->m_pIT->isDeletable())
                {
                    delete (*itBox)->m_pIT;
                }


                //increase ref
                _iT.IncreaseRef();
                //update value
                (*itBox)->m_pIT = &_iT;
                bInserted = true;
                break;
            }
        }

        if (bInserted == false)
        {
            //for case of first var ref is in higher scope
            pBox = new VarBox(_iLevel, &_iT);
            itVar->second->push_front(pBox);
        }
    }
    else
    {
        //new var
        pBox = new VarBox(_iLevel, &_iT);
        pL = new VarBoxList();
        pL->push_back(pBox);
        m_vars[_key] = pL;
    }

    return;
}

void Variables::putInPreviousScope(const Symbol& _key, types::InternalType& _iT)
{
    put(_key, _iT, Max(0, m_iLevel - 1));
}

types::InternalType* Variables::get(const Symbol& _key) const
{
    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        return itVar->second->back()->m_pIT;
    }
    else
    {
        return NULL;
    }
}

types::InternalType* Variables::getCurrentLevel(const Symbol& _key) const
{
    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end() && itVar->second->back()->m_iLevel == m_iLevel)
    {
        return itVar->second->back()->m_pIT;
    }
    else
    {
        return NULL;
    }
}

types::InternalType* Variables::getAllButCurrentLevel(const Symbol& _key) const
{
    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBoxList* pL = itVar->second;
        VarBoxList::iterator itBox = pL->begin();
        for (; itBox != pL->end() ; itBox++)
        {
            if ((*itBox)->m_iLevel < m_iLevel)
            {
                return (*itBox)->m_pIT;
            }
        }
    }

    return NULL;
}

bool Variables::remove(const Symbol& _key)
{
    VarMap::iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBox* pBox = itVar->second->back();
        if (pBox->m_iLevel != m_iLevel || pBox->m_pIT == NULL)
        {
            return false;
        }

        pBox->m_pIT->DecreaseRef();
        if (pBox->m_pIT->isDeletable())
        {
            delete pBox->m_pIT;
        }

        delete pBox;

        itVar->second->pop_back();

        //remove empty entries from variable map
        if (itVar->second->empty())
        {
            m_vars.erase(itVar);
        }
        return true;
    }
    else
    {
        return false;
    }
}
}