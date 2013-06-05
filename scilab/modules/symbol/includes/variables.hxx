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

#ifndef __keyIABLES_HXX__
#define __keyIABLES_HXX__

#include <map>
#include <list>
#include "types.hxx"
#include "symbol.hxx"

namespace symbol
{
struct VarBox
{
    VarBox(int _iLevel, types::InternalType* _pIT, bool _bGlobal = false, bool _bGlobalVisible = false);

    int m_iLevel;
    types::InternalType* m_pIT;
    bool m_bGlobal;
    bool m_bGlobalVisible;
};

class Variables
{
private :
    //current scope level
    int m_iLevel;

    typedef std::list<VarBox* > VarBoxList;
    typedef std::map<Symbol, VarBoxList*> VarMap;
    VarMap m_vars;

public :
    Variables() : m_iLevel(-1) {}

    virtual ~Variables() {}

    void put(const Symbol& _key, types::InternalType& _pIT);
    //{
    //    put(_key, _pIT, m_iLevel);
    //    //VarMap::iterator it = m_keys.find(_key);
    //    //VarBoxList* pL = NULL;
    //    //VarBox* pBox = NULL;

    //    //if(it != m_keys.end())
    //    //{//item alreay exists
    //    //    //check scope level
    //    //    pBox = it->second->back();
    //    //    if(pBox->m_iLevel < m_iLevel)
    //    //    {
    //    //        //create a new Box and add it
    //    //        pBox = new VarBox(m_iLevel, _pIT);
    //    //        it->second->push_back(pBox);
    //    //    }
    //    //    else // ==   pBox->m_iLevel must not be > m_iLevel
    //    //    {
    //    //        // a = a -> nothing to do
    //    //        if(pBox->m_pIT == _pIT)
    //    //        {
    //    //            return;
    //    //        }

    //    //        pBox->m_pIT->DecreaseRef();
    //    //        if(pBox->m_pIT->isDeletable())
    //    //        {
    //    //            delete pBox->m_pIT;
    //    //        }


    //    //        //increase ref
    //    //        _pIT->IncreaseRef();
    //    //        //update value
    //    //        pBox->m_pIT = _pIT;
    //    //    }
    //    //}
    //}

    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
    //{
    //    VarMap::iterator it = m_vars.find(_key);
    //    VarBoxList* pL = NULL;
    //    VarBox* pBox = NULL;

    //    if(it != m_vars.end())
    //    {//item alreay exists

    //        //check scope level
    //        VarBoxList::reverse_iterator itBox = it->second->rbegin();

    //        bool bInserted = false;
    //        for(;itBox != it->second->rend() ; itBox++)
    //        {
    //            if((*itBox)->m_iLevel > _iLevel)
    //            {
    //                continue;
    //            }
    //            else if((*itBox)->m_iLevel < _iLevel)
    //            {
    //                pBox = new VarBox(_iLevel, &_iT);
    //                it->second->insert(itBox.base()++, pBox);
    //                bInserted = true;
    //                break;
    //            }
    //            else if((*itBox)->m_iLevel == _iLevel)
    //            {
    //                if((*itBox)->m_pIT == &_iT)
    //                {
    //                    return;
    //                }

    //                (*itBox)->m_pIT->DecreaseRef();
    //                if((*itBox)->m_pIT->isDeletable())
    //                {
    //                    delete (*itBox)->m_pIT;
    //                }


    //                //increase ref
    //                _iT.IncreaseRef();
    //                //update value
    //                (*itBox)->m_pIT = &_iT;
    //                bInserted = true;
    //                break;
    //            }
    //        }

    //        if(bInserted == false)
    //        {
    //            //for case of first var ref is in higher scope
    //            pBox = new VarBox(_iLevel, &_iT);
    //            it->second->push_front(pBox);
    //        }
    //    }
    //    else
    //    {//new var
    //        pBox = new VarBox(_iLevel, &_iT);
    //        pL = new VarBoxList();
    //        pL->push_back(pBox);
    //        m_vars[_key] = pL;
    //    }
    //
    //    return;
    //}

    void putInPreviousScope(const Symbol& _key, types::InternalType& _iT);
    //{
    //    put(_key, _iT, Max(0, m_iLevel - 1));
    //}

    types::InternalType* get(const Symbol& _key) const;
    //{
    //    VarMap::const_iterator it = m_vars.find(_key);
    //    if(it != m_vars.end())
    //    {
    //        return it->second->back()->m_pIT;
    //    }
    //    else
    //    {
    //        return NULL;
    //    }
    //}

    types::InternalType* getCurrentLevel(const Symbol& _key) const;
    //{
    //    VarMap::const_iterator it = m_vars.find(_key);
    //    if(it != m_vars.end() && it->second->back()->m_iLevel == m_iLevel)
    //    {
    //        return it->second->back()->m_pIT;
    //    }
    //    else
    //    {
    //        return NULL;
    //    }
    //}

    types::InternalType* getAllButCurrentLevel(const Symbol& _key) const;
    //{
    //    VarMap::const_iterator it = m_vars.find(_key);
    //    if(it != m_vars.end())
    //    {
    //        VarBoxList* pL = it->second;
    //        VarBoxList::iterator itBox = pL->begin();
    //        for(;itBox != pL->end() ; itBox++)
    //        {
    //            if((*itBox)->m_iLevel < m_iLevel)
    //            {
    //                return (*itBox)->m_pIT;
    //            }
    //        }
    //    }

    //    return NULL;
    //}

    bool remove(const Symbol& _key);
    //{
    //    VarMap::iterator it = m_vars.find(_key);
    //    if(it != m_vars.end())
    //    {
    //        VarBox* pBox = it->second->back();
    //        if(pBox->m_iLevel != m_iLevel || pBox->m_pIT == NULL)
    //        {
    //            return false;
    //        }

    //        pBox->m_pIT->DecreaseRef();
    //        if(pBox->m_pIT->isDeletable())
    //        {
    //            delete pBox->m_pIT;
    //        }

    //        delete pBox;

    //        it->second->pop_back();

    //        //remove empty entries from variable map
    //        if(it->second->empty())
    //        {
    //            m_vars.erase(it);
    //        }
    //        return true;
    //    }
    //    else
    //    {
    //        return false;
    //    }
    //}

    void IncreaseLevel()
    {
        m_iLevel++;
    }
    void DecreaseLevel()
    {
        m_iLevel--;
    }
};
}
#endif