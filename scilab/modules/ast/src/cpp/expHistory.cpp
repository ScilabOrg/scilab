/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "expHistory.hxx"
#include "types_tools.hxx"

/**
** Constructor & Destructor (public)
*/

void ExpHistory::initHistory()
{
    m_pArgs             = NULL;
    m_piArgsDimsArray   = NULL;
    m_pExp              = NULL;
    m_pParent           = NULL;
    m_pITCurrent        = NULL;
    m_bReinsertMe       = false;
    m_iArgsDims         = 0;
    m_iWhere            = -1;
    m_iLevel            = 0;
}

ExpHistory::ExpHistory()
{
    initHistory();
}


ExpHistory::ExpHistory(ExpHistory* _pEH, const ast::SimpleVar* _pExp)
{
    initHistory();

    m_pExp      = _pExp;
    m_pParent   = _pEH;
}

ExpHistory::ExpHistory(ExpHistory* _pParent, types::typed_list* _pArgs)
{
    initHistory();

    m_pArgs     = _pArgs;
    m_pParent   = _pParent;
}

ExpHistory::ExpHistory(ExpHistory* _pParent, const ast::SimpleVar* _pExp, types::typed_list* _pArgs, int _iLevel,  types::InternalType* _pITCurrent)
{
    initHistory();

    m_pArgs         = _pArgs;
    m_pExp          = _pExp;
    m_pParent       = _pParent;
    m_pITCurrent    = _pITCurrent;
    m_iLevel        = _iLevel;
}

ExpHistory::~ExpHistory()
{
    if (m_piArgsDimsArray)
    {
        delete m_piArgsDimsArray;
    }
}

/**
** Accessor (public)
*/

void ExpHistory::setExp(const ast::SimpleVar* _pExp)
{
    m_pExp = _pExp;
}

const ast::SimpleVar* ExpHistory::getExp()
{
    return m_pExp;
}

std::wstring ExpHistory::getExpAsString()
{
    std::wstring wcsExp = L"";
    if (m_pExp)
    {
        wcsExp = m_pExp->name_get().name_get();
    }

    return wcsExp;
}

void ExpHistory::setArgs(types::typed_list* _pArgs)
{
    m_pArgs = _pArgs;
}

void ExpHistory::computeArgs()
{
    if (m_pArgs)
    {
        m_iArgsDims = m_pArgs->size();

        // compute indexes
        if (m_iArgsDims == 1)
        {
            m_iArgsDims++;
        }

        m_piArgsDimsArray  = new int[m_iArgsDims];
        m_piArgsDimsArray[1] = 1;

        int* piCountDim = new int[m_pArgs->size()];
        types::typed_list* pNewArgs = new types::typed_list();

        int iCount = checkIndexesArguments(m_pITCurrent, m_pArgs, pNewArgs, m_piArgsDimsArray, piCountDim);

        m_pArgs = pNewArgs;
        delete piCountDim;
    }
}

types::typed_list* ExpHistory::getArgs()
{
    return m_pArgs;
}

int ExpHistory::getSizeFromArgs()
{
    int iSizeFromArgs = 0;

    if (m_pArgs)
    {
        iSizeFromArgs = 1;
        if (m_piArgsDimsArray == NULL)
        {
            computeArgs();
        }

        for (int i = 0; i < m_pArgs->size(); i++)
        {
            iSizeFromArgs *= m_piArgsDimsArray[i];
        }
    }

    return iSizeFromArgs;
}

int* ExpHistory::getArgsDimsArray()
{
    if (m_pArgs)
    {
        if (m_piArgsDimsArray == NULL)
        {
            computeArgs();
        }
    }

    return m_piArgsDimsArray;
}

int ExpHistory::getArgsDims()
{
    if (m_pArgs)
    {
        if (m_piArgsDimsArray == NULL)
        {
            computeArgs();
        }
    }

    return m_iArgsDims;
}

void ExpHistory::setCurrent(types::InternalType* _pITCurrent)
{
    m_pITCurrent = _pITCurrent;
}

types::InternalType* ExpHistory::getCurrent()
{
    return m_pITCurrent;
}

ExpHistory* ExpHistory::getParent()
{
    return m_pParent;
}

void ExpHistory::setReinsertion()
{
    if (m_pITCurrent->isHandle() == false)
    {
        m_bReinsertMe = true;
    }
}

void ExpHistory::resetReinsertion()
{
    m_bReinsertMe = false;
}

bool ExpHistory::reinsertMe()
{
    return m_bReinsertMe;
}

int ExpHistory::getWhereReinsert()
{
    return m_iWhere;
}

void ExpHistory::setWhereReinsert(int _iWhere)
{
    m_iWhere = _iWhere;
}

int ExpHistory::getLevel()
{
    return m_iLevel;
}

void ExpHistory::setLevel(int _iLevel)
{
    m_iLevel = _iLevel;
}