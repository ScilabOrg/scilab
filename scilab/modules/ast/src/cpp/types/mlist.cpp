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

#include <sstream>
#include "mlist.hxx"
#include "callable.hxx"
#include "overload.hxx"

#ifndef NDEBUG
#include "inspector.hxx"
#endif

namespace types
{
bool MList::invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, ast::ConstVisitor & execFunc, const ast::Exp & /*e*/)
{
    if (in.size() == 0)
    {
        out.push_back(this);
        return true;
    }
    else if (in.size() == 1)
    {
        InternalType * arg = in[0];
        InternalType* _out = NULL;
        if (arg->isString())
        {
            std::list<std::wstring> stFields;
            String * pString = arg->getAs<types::String>();
            for (int i = 0; i < pString->getSize(); ++i)
            {
                stFields.push_back(pString->get(i));
            }

            _out = extractStrings(stFields);

            List* pList = _out->getAs<types::List>();
            for (int i = 0; i < pList->getSize(); i++)
            {
                out.push_back(pList->get(i));
            }

            delete pList;
        }

        if (!out.empty())
        {
            return true;
        }
    }

    Callable::ReturnValue ret;
    // Overload of extraction need
    // the mlist from where we extract
    this->IncreaseRef();
    in.push_back(this);

    try
    {
        ret = Overload::call(L"%" + getShortTypeStr() + L"_e", in, 1, out, &execFunc);
    }
    catch (ast::ScilabError & /*se*/)
    {
        ret = Overload::call(L"%l_e", in, 1, out, &execFunc);
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
}
