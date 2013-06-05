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

    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
    void put(const Symbol& _key, types::InternalType& _iT);

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
    Variables m_vars;

public :

    Scopes() : m_iLevel(-1) {}

    void scope_begin();
    void scope_end();

    /*setters*/
    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
    void put(const Symbol& _key, types::InternalType& _iT);
    void putInPreviousScope(const Symbol& _key, types::InternalType& _iT);

    /* getters */
    types::InternalType* get(const Symbol& key) const;
    types::InternalType* getCurrentLevel(const Symbol& key) const;
    types::InternalType* getAllButCurrentLevel(const Symbol& key) const;

    /*remove*/
    bool remove(const Symbol& _key);

    /*tools*/
    std::list<symbol::Symbol>& getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd);
};
}

#endif /* !__NEWSCOPE_HXX__ */