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

#ifndef __NEWSCOPE_HXX__
#define __NEWSCOPE_HXX__

#include "variables.hxx"

namespace symbol
{
class Scope
{
public :
    Scope(Variables* _vars, int _iLevel) : m_vars(_vars), m_iLevel(_iLevel) {}
    virtual ~Scope();
    //{
    //    SymbolMap::iterator it = m_symbol.begin();
    //    for(; it != m_symbol.end() ; it++)
    //    {
    //        m_vars->remove(it->first);
    //    }
    //}

    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
    void put(const Symbol& _key, types::InternalType& _iT);
    //{
    //    m_symbol[_key] = 0;
    //    m_vars->put(_key, _iT, m_iLevel);
    //}


private :
    typedef std::map<Symbol, int> SymbolMap;
    SymbolMap m_symbol;
    int m_iLevel;
    Variables* m_vars;
};

class Scopes
{
private :
    typedef std::list<Scope*> ScopesList;
    ScopesList m_scopes;
    int m_iLevel;
    Variables* m_vars;

public :

    Scopes(Variables* _vars) : m_vars(_vars), m_iLevel(-1) {}

    void scope_begin();
    //{
    //    m_iLevel++;
    //    m_scopes.push_back(new Scope(m_vars, m_iLevel));
    //}

    void scope_end();
    //{
    //    Scope* scope = m_scopes.back();
    //    delete scope;
    //    m_scopes.pop_back();
    //    m_iLevel--;
    //}


    /*setters*/
    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
    //{
    //    //add in scope
    //    m_scopes.back()->put(_key, _iT);
    //}

    void put(const Symbol& _key, types::InternalType& _iT);
    //{
    //    put(_key, _iT, m_iLevel);
    //}

    void putInPreviousScope(const Symbol& _key, types::InternalType& _iT);
    //{
    //    put(_key, _iT, m_iLevel - 1);
    //}

    /* getters */
    types::InternalType* get(const Symbol& key) const;
    //{
    //    return m_vars->get(key);
    //}

    types::InternalType* getCurrentLevel(const Symbol& key) const;
    //{
    //    return m_vars->getCurrentLevel(key);
    //}

    types::InternalType* getAllButCurrentLevel(const Symbol& key) const;
    //{
    //    return m_vars->getAllButCurrentLevel(key);
    //}

    /*remove*/
    bool remove(const Symbol& _key);
    //{
    //    return true;
    //}

    /*tools*/
    std::list<symbol::Symbol>& getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd);
    //{
    //    std::list<symbol::Symbol>* lst = new std::list<symbol::Symbol>;
    //    return *lst;
    //}

};
}

#endif /* !__NEWSCOPE_HXX__ */