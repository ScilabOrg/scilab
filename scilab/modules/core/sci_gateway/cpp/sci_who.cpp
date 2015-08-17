/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include <sstream>
#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "scilabWrite.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_who(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    wchar_t* wcsWhat = L"";
    bool bSorted = false;
    types::String* pStrOut = NULL;
    types::Double* pDblOut = NULL;
    std::list<std::wstring> lstVar;
    int size = 0;

    if (in.size() < 0 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "who", 0, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "who", 0, 2);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        std::wstringstream wstream;
        symbol::Context::getInstance()->print(wstream, bSorted);
        wstream << std::endl;
        scilabForcedWriteW(wstream.str().c_str());
        return types::Function::OK;
    }

    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "who", 2);
            return types::Function::Error;
        }

        types::String* pStrSorted = in[1]->getAs<types::String>();

        if (pStrSorted->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "who", 2);
            return types::Function::Error;
        }

        if (wcscmp(pStrSorted->get(0), L"sorted") == 0)
        {
            bSorted = true;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'sorted' expected.\n"), "who", 2);
            return types::Function::Error;
        }
    }

    if (in.size() > 0)
    {
        if (in[0]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "who", 1);
            return types::Function::Error;
        }

        types::String* pStrWhat = in[0]->getAs<types::String>();

        if (pStrWhat->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "who", 1);
            return types::Function::Error;
        }

        wcsWhat = pStrWhat->get(0);
    }

    if (wcscmp(wcsWhat, L"local") == 0 || wcscmp(wcsWhat, L"get") == 0)
    {
        size = symbol::Context::getInstance()->getVarsNameForWho(lstVar, bSorted);
    }
    else if (wcscmp(wcsWhat, L"global") == 0)
    {
        size = symbol::Context::getInstance()->getGlobalNameForWho(lstVar, bSorted);
    }
    else if (bSorted == false && wcscmp(wcsWhat, L"sorted") == 0)
    {
        bSorted = true;
        std::wstringstream wstream;
        symbol::Context::getInstance()->print(wstream, bSorted);
        wstream << std::endl;
        scilabForcedWriteW(wstream.str().c_str());
        return types::Function::OK;
    }
    else
    {
        if (bSorted)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'local', 'get' or 'global' expected.\n"), "who", 1);
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'local', 'get', 'global' or 'sorted' expected.\n"), "who", 1);
        }

        return types::Function::Error;
    }

    if (lstVar.empty())
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }

        return types::Function::OK;
    }

    pStrOut = new types::String(size, 1);
    int i = 0;
    for (auto it : lstVar)
    {
        pStrOut->set(i++, it.c_str());
    }

    out.push_back(pStrOut);

    if (_iRetCount == 2)
    {
        pDblOut = new types::Double(pStrOut->getDims(), pStrOut->getDimsArray());
        memset(pDblOut->get(), 0x00, pDblOut->getSize() * sizeof(double));
        out.push_back(pDblOut);
    }

    return types::Function::OK;
}
