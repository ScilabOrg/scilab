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

#include <cstring>
#include <sstream>
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "listundefined.hxx"
#include "callable.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

#ifndef NDEBUG
#include "inspector.hxx"
#endif

extern "C"
{
#include "os_string.h"
}

namespace types
{
/**
** Constructor & Destructor (public)
*/
TList::TList() : List()
{
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

TList::~TList()
{
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
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
    if (getSize() < 1)
    {
        return false;
    }

    String* pS = getFieldNames();

    //first field is the tlist type
    for (int i = 1 ; i < pS->getSize() ; i++)
    {
        if (wcscmp(pS->get(i), _sKey.c_str()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool TList::invoke(typed_list & in, optional_list & /*opt*/, int _iRetCount, typed_list & out, ast::ConstVisitor & execFunc, const ast::Exp & /*e*/)
{
    if (in.size() == 0)
    {
        out.push_back(this);
        return true;
    }
    else if (in.size() == 1)
    {
        InternalType * arg = in[0];
        InternalType * _out = NULL;
        if (arg->isDouble() || arg->isInt() || arg->isBool() || arg->isImplicitList() || arg->isColon() || arg->isDollar())
        {
            _out = List::extract(&in);
            if (_out == NULL)
            {
                // invalid index
                return false;
            }

            List* pList = _out->getAs<types::List>();
            for (int i = 0; i < pList->getSize(); i++)
            {
                out.push_back(pList->get(i));
            }

            delete pList;
        }
        else if (arg->isString())
        {
            std::list<std::wstring> stFields;
            String * pString = arg->getAs<types::String>();
            for (int i = 0; i < pString->getSize(); ++i)
            {
                stFields.push_back(pString->get(i));
            }

            _out = extractStrings(stFields);
            if (_out == NULL)
            {
                // invalid index
                return false;
            }

            List* pList = _out->getAs<types::List>();
            for (int i = 0; i < pList->getSize(); i++)
            {
                out.push_back(pList->get(i));
            }

            delete pList;
        }
        else if (arg->isPoly())
        {
            Polynom* pPoly = arg->getAs<Polynom>();
            SinglePoly* pSinglePoly = pPoly->get(0);

            int iMaxDim = 0;
            double dblParse = -1 * pSinglePoly->get(0);
            int iSize = getSize();

            if (pSinglePoly->getRank() < 2 && dblParse >= 0 && dblParse < (double)iSize)
            {
                out.push_back(get(iSize - 1 - dblParse));
            }
            else
            {
                return false;
            }
        }

        if (out.empty() == false)
        {
            return true;
        }
    }
    Callable::ReturnValue ret;
    // Overload of extraction need
    // the tlist from where we extract
    this->IncreaseRef();
    in.push_back(this);

    std::wstring stType = getShortTypeStr();
    try
    {
        ret = Overload::call(L"%" + stType + L"_e", in, _iRetCount, out, &execFunc);
    }
    catch (ast::ScilabError &se)
    {
        try
        {
            //to compatibility with scilab 5 code.
            //tlist/mlist name are truncated to 8 first character
            if (stType.size() > 8)
            {
                std::wcout << (L"%" + stType.substr(0, 8) + L"_e") << std::endl;
                ret = Overload::call(L"%" + stType.substr(0, 8) + L"_e", in, 1, out, &execFunc);
            }
            else
            {
                throw se;
            }
        }
        catch (ast::ScilabError & /*se*/)
        {
            ret = Overload::call(L"%l_e", in, 1, out, &execFunc);
        }
    }

    // Remove this from "in" for keep "in" unchanged.
    this->DecreaseRef();
    in.pop_back();

    if (ret == Callable::Error)
    {
        throw ast::ScilabError();
    }

    return true;
}

bool TList::extract(const std::wstring & name, InternalType *& out)
{
    if (exists(name))
    {
        out = getField(name);
        return true;
    }

    return false;
}

InternalType* TList::getField(const std::wstring& _sKey)
{
    return List::get(getIndexFromString(_sKey));
}

int TList::getIndexFromString(const std::wstring& _sKey)
{
    if (getSize() < 1)
    {
        return -1;
    }

    String* pS = getFieldNames();
    //first field is the tlist type
    for (int i = 1 ; i < pS->getSize() ; i++)
    {
        if (wcscmp(pS->get(i), _sKey.c_str()) == 0)
        {
            return i;
        }
    }
    return -1;
}

InternalType* TList::extractStrings(const std::list<std::wstring>& _stFields)
{
    int i = 0;
    List* pLResult = new List();
    std::list<std::wstring>::const_iterator it;
    for (it = _stFields.begin() ; it != _stFields.end() ; it++)
    {
        if (exists(*it) == false)
        {
            return pLResult;
        }
    }

    for (it = _stFields.begin() ; it != _stFields.end() ; it++, i++)
    {
        InternalType* pIT = getField(*it);
        if (pIT == NULL)
        {
            delete pLResult;
            return NULL;
        }

        pLResult->set(i, pIT);
    }

    return pLResult;
}

std::wstring TList::getTypeStr()
{
    if (getSize() < 1)
    {
        return L"";
    }

    return getFieldNames()->get(0);
}

std::wstring TList::getShortTypeStr()
{
    return getTypeStr();
}

bool TList::set(const std::wstring& _sKey, InternalType* _pIT)
{
    return List::set(getIndexFromString(_sKey), _pIT);
}

bool TList::set(const int _iIndex, InternalType* _pIT)
{
    return List::set(_iIndex, _pIT);
}

String* TList::getFieldNames()
{
    return (*m_plData)[0]->getAs<types::String>();
}

/**
** toString to display TLists
*/
bool TList::toString(std::wostringstream& ostr)
{
    //call overload %type_p if exists
    types::typed_list in;
    types::typed_list out;
    ast::ExecVisitor exec;

    IncreaseRef();
    in.push_back(this);

    try
    {
        if (Overload::generateNameAndCall(L"p", in, 1, out, &exec) == Function::Error)
        {
            ConfigVariable::setError();
        }

        ostr.str(L"");
        DecreaseRef();
        return true;
    }
    catch (ast::ScilabMessage /* &e */)
    {
        // avoid error message about undefined overload %type_p
        ConfigVariable::resetError();
    }

    DecreaseRef();

    // special case for lss
    if (getSize() != 0 &&
            (*m_plData)[0]->isString() &&
            (*m_plData)[0]->getAs<types::String>()->getSize() > 0 &&
            wcscmp((*m_plData)[0]->getAs<types::String>()->get(0), L"lss") == 0)
    {
        wchar_t* wcsVarName = os_wcsdup(ostr.str().c_str());
        int iPosition = 1;
        const wchar_t * wcsDesc[7] = {L"  (state-space system:)", L"= A matrix =", L"= B matrix =", L"= C matrix =", L"= D matrix =", L"= X0 (initial state) =", L"= Time domain ="};
        for (auto val : *m_plData)
        {
            std::wostringstream nextVarName;
            ostr.str(L"");
            nextVarName << " " << wcsVarName << L"(" << iPosition << L")";
            ostr << std::endl << nextVarName.str() << wcsDesc[iPosition - 1] << std::endl << std::endl;
            scilabWriteW(ostr.str().c_str());
            VariableToString(val, nextVarName.str().c_str());
            iPosition++;
        }
        ostr.str(L"");
        free(wcsVarName);
        return true;
    }

    // call normal toString
    return List::toString(ostr);
}
} // end namespace types
