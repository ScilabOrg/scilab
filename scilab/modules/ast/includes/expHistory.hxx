/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterpprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __EXPHISTORY_HXX__
#define __EXPHISTORY_HXX__

#include <list>
#include <vector>
#include "types.hxx"
#include "ast.hxx"
#include "simplevar.hxx"

class ExpHistory
{
public :
    ExpHistory();
    ExpHistory(ExpHistory*, const ast::SimpleVar*);
    ExpHistory(ExpHistory*, types::typed_list*);
    ExpHistory(ExpHistory*, const ast::SimpleVar*, types::typed_list*, int, types::InternalType*);

    ~ExpHistory();

    void                    initHistory();

    // expression "a"
    void                    setExp(const ast::SimpleVar*);
    const ast::SimpleVar*   getExp();
    std::wstring            getExpAsString();

    // arguments x of a(x)
    void                    setArgs(types::typed_list*);
    types::typed_list*      getArgs();
    int                     getSizeFromArgs();
    int                     getArgsDims();
    int*                    getArgsDimsArray();


    // current InternalType a
    void                    setCurrent(types::InternalType*);
    types::InternalType*    getCurrent();

    // parent of me
    ExpHistory*             getParent();

    // use to know if we have to reinsert current in there parent and where
    void                    setReinsertion();
    void                    resetReinsertion();
    bool                    reinsertMe();
    int                     getWhereReinsert();
    void                    setWhereReinsert(int);

    // use to know where we are in expression
    void                    setLevel(int);
    int                     getLevel();

private :

    void                    computeArgs();

    types::typed_list*      m_pArgs;
    int*                    m_piArgsDimsArray;
    int                     m_iArgsDims;

    const ast::SimpleVar*   m_pExp;
    ExpHistory*             m_pParent;
    types::InternalType*    m_pITCurrent;

    bool                    m_bReinsertMe;
    int                     m_iWhere;
    int                     m_iLevel;
};

#endif /* __EXPHISTORY_HXX__ */
