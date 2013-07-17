/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

enum ScopeRange
{
    Local,
    NoLocal,
    All,
    UnknownRange
};

static ScopeRange getScopeFromOption(const wchar_t *_psScope)
{
    if (wcscmp(_psScope, L"all") == 0 || wcscmp(_psScope, L"a") == 0)
    {
        return All;
    }
    if (wcscmp(_psScope, L"local") == 0 || wcscmp(_psScope, L"l") == 0)
    {
        return Local;
    }
    if (wcscmp(_psScope, L"nolocal") == 0 || wcscmp(_psScope, L"n") == 0)
    {
        return NoLocal;
    }

    return UnknownRange;
}

types::Function::ReturnValue sci_exists(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStrIn   = NULL;
    types::Double* pDblOut  = NULL;
    double* pdblOut         = NULL;

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "exists", 1, 2);
        return types::Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "exists", 1);
        return types::Function::Error;
    }

    if (in.size() == 2 && (!in[1]->isString() || in[1]->getAs<types::String>()->getSize() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "exists", 2);
        return types::Function::Error;
    }

    const wchar_t *psScope = NULL;
    if (in.size() == 2)
    {
        psScope = in[1]->getAs<types::String>()->get(0);
    }
    else
    {
        // Default option is "all"
        psScope = L"all";
    }

    pStrIn  = in[0]->getAs<types::String>();
    pDblOut = new types::Double(pStrIn->getDims(), pStrIn->getDimsArray());
    pdblOut = pDblOut->get();

    types::InternalType *pitReturn = NULL;
    switch (getScopeFromOption(psScope))
    {
        case All:
            for (int i = 0; i < pStrIn->getSize(); i++)
            {
                symbol::Context::getInstance()->get(symbol::Symbol(pStrIn->get(i))) ? pdblOut[i] = 1 : pdblOut[i] = 0;
            }
            break;
        case Local:
            for (int i = 0; i < pStrIn->getSize(); i++)
            {
                symbol::Context::getInstance()->getCurrentLevel(symbol::Symbol(pStrIn->get(i))) ? pdblOut[i] = 1 : pdblOut[i] = 0;
            }
            break;
        case NoLocal:
            for (int i = 0; i < pStrIn->getSize(); i++)
            {
                symbol::Context::getInstance()->getAllButCurrentLevel(symbol::Symbol(pStrIn->get(i))) ? pdblOut[i] = 1 : pdblOut[i] = 0;
            }
            break;
        default :
            Scierror(36, _("%s: Wrong input argument %d.\n"), "exists", 2);
            return types::Function::Error;
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
