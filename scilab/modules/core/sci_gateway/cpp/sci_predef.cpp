/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
}

static const std::string fname("predef");

types::Function::ReturnValue sci_predef(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    types::InternalType* pIT = in[0];

    if (pIT->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected."), fname.data(),  1);
        return types::Function::Error;
    }

    types::String* pS = pIT->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected."), fname.data(), 1);
        return types::Function::Error;
    }

    std::wstring opt(pS->get()[0]);

    if (opt == L"all" || opt == L"a")
    {
        symbol::Context::getInstance()->protect();
    }
    else if (opt == L"clear" || opt == L"c")
    {
        symbol::Context::getInstance()->unprotect();
    }
    else if (opt == L"names" || opt == L"n")
    {
        std::list<std::wstring>* lst = symbol::Context::getInstance()->protectedVars();

        //create output string
        types::String* pS = new types::String(lst->size(), 1);
        int i = 0;
        wchar_t** ws = pS->get();
        for (auto var : *lst)
        {
            pS->set(i++, var.c_str());
        }
        delete lst;

        out.push_back(pS);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname.data(), "all", "clear", "names");
        return types::Function::Error;
    }

    return types::Function::OK;
}
