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

#include "newScope.hxx"

namespace symbol
{
Scope::~Scope()
{
    SymbolMap::iterator it = m_symbol.begin();
    for (; it != m_symbol.end() ; it++)
    {
        m_vars->remove(it->first);
    }
}

void Scope::put(const Symbol& _key, types::InternalType& _iT)
{
    m_symbol[_key] = 0;
    m_vars->put(_key, _iT, m_iLevel);
}

void Scope::put(const Symbol& _key, types::InternalType& _iT, int _iLevel)
{
    m_symbol[_key] = 0;
    m_vars->put(_key, _iT, _iLevel);
}

void Scopes::scope_begin()
{
    m_iLevel++;
    m_scopes.push_back(new Scope(&m_vars, m_iLevel));
    m_vars.IncreaseLevel();
}

void Scopes::scope_end()
{
    Scope* scope = m_scopes.back();
    delete scope;
    m_scopes.pop_back();
    m_iLevel--;
    m_vars.DecreaseLevel();
}

void Scopes::put(const Symbol& _key, types::InternalType& _iT, int _iLevel)
{
    //add in scope
    m_scopes.back()->put(_key, _iT, _iLevel);
}

void Scopes::put(const Symbol& _key, types::InternalType& _iT)
{
    put(_key, _iT, m_iLevel);
}

void Scopes::putInPreviousScope(const Symbol& _key, types::InternalType& _iT)
{
    put(_key, _iT, m_iLevel - 1);
}

/* getters */
types::InternalType* Scopes::get(const Symbol& key) const
{
    return m_vars.get(key);
}

types::InternalType* Scopes::getCurrentLevel(const Symbol& key) const
{
    return m_vars.getCurrentLevel(key);
}

types::InternalType* Scopes::getAllButCurrentLevel(const Symbol& key) const
{
    return m_vars.getAllButCurrentLevel(key);
}

/*remove*/
bool Scopes::remove(const Symbol& _key)
{
    //m_scopes->(_key);
    return true;
}

/*tools*/
std::list<symbol::Symbol>& Scopes::getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd)
{
    return m_vars.getFunctionList(_stModuleName, _bFromEnd);
}
}
