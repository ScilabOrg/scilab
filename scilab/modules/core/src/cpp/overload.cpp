/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
extern "C"
{
#include "stdarg.h"
#include "localization.h"
}

#include "overload.hxx"
#include "context.hxx"
#include "scilabexception.hxx"

symbol::symbol_t Overload::buildOverloadName(symbol::symbol_t const& _stFunctionName, types::typed_list &in, int _iRetCount)
{
    std::wstring const& name ( _stFunctionName);
    switch(in.size())
    {
    case 0 :
        return symbol::symbol_t (L"%_" + name);
    case 1:
        return symbol::symbol_t (L"%" + in[0]->getShortTypeStr() + L"_" + name);
    case 2:
        return symbol::symbol_t (L"%" + in[0]->getShortTypeStr() + L"_" + name + L"_" + in[1]->getShortTypeStr());
    default :
        throw ast::ScilabError(L"Don't know how to overload " + name, 999, *new Location());
    }
    return _stFunctionName;
}

types::Function::ReturnValue Overload::generateNameAndCall(symbol::symbol_t const& _stFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe)
{
    return call(buildOverloadName(_stFunctionName, in, _iRetCount), in, _iRetCount, out, _execMe);
}

types::Function::ReturnValue Overload::call(symbol::symbol_t const& _stOverloadingFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe)
{
    types::InternalType *pIT = symbol::Context::getInstance()->get(_stOverloadingFunctionName);

    if(pIT == NULL || pIT->isCallable() == false)
    {
        std::wstring const& name(_stOverloadingFunctionName);
        throw ast::ScilabError(_W("check or define function ") + name + _W(" for overloading.\n\n"), 999, *new Location());
    }

    return pIT->getAsCallable()->call(in, _iRetCount, out, _execMe);
}

std::wstring Overload::getNameFromOper(ast::OpExp::Oper _oper)
{
    switch (_oper)
    {
        /* standard operators */
    case OpExp::plus :
        return std::wstring(L"a");
    case OpExp::minus :
        return std::wstring(L"s");
    case OpExp::times :
        return std::wstring(L"m");
    case OpExp::rdivide :
        return std::wstring(L"r");
    case OpExp::ldivide :
        return std::wstring(L"l");
    case OpExp::power :
        return std::wstring(L"p");
        /* dot operators */
    case OpExp::dottimes :
        return std::wstring(L"x");
    case OpExp::dotrdivide :
        return std::wstring(L"d");
    case OpExp::dotldivide :
        return std::wstring(L"q");
    case OpExp::dotpower :
        return std::wstring(L"j");
        /* Kron operators */
    case OpExp::krontimes :
        return std::wstring(L"k");
    case OpExp::kronrdivide :
        return std::wstring(L"y");
    case OpExp::kronldivide :
        return std::wstring(L"z");
        /* Control Operators ??? */
    case OpExp::controltimes :
        return std::wstring(L"u");
    case OpExp::controlrdivide :
        return std::wstring(L"v");
    case OpExp::controlldivide :
        return std::wstring(L"w");
    case OpExp::eq :
        return std::wstring(L"o");
    case OpExp::ne :
        return std::wstring(L"n");
    case OpExp::lt :
        return std::wstring(L"1");
    case OpExp::le :
        return std::wstring(L"3");
    case OpExp::gt :
        return std::wstring(L"2");
    case OpExp::ge :
        return std::wstring(L"4");
    case OpExp::logicalAnd :
        return std::wstring(L"h");
    case OpExp::logicalOr :
        return std::wstring(L"g");
    case OpExp::logicalShortCutAnd :
        return std::wstring(L"h");
    case OpExp::logicalShortCutOr :
        return std::wstring(L"g");
    default :
        return std::wstring(L"???");
    }
}

wstring formatString(const wstring& wstFormat, ...)
{
    wchar_t pwstTemp[1024];
    va_list arglist;
    va_start(arglist, wstFormat);
    int iLen = vswprintf(pwstTemp, 1024, wstFormat.c_str(), arglist);
    va_end(arglist);

    return wstring(pwstTemp, iLen);
}
