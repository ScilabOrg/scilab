/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "string.hxx"
#include "bool.hxx"
#include "symbol.hxx"
#include "context.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_clearfun(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected."), "clearfun", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(77, _("%s: Wrong number of output argument(s): %d expected."), "clearfun", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "clearfun", 1);
        return types::Function::Error;
    }

    types::String* pStrIn = in[0]->getAs<types::String>();

    if (pStrIn->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "clearfun", 1);
        return types::Function::Error;
    }

    symbol::Symbol symbolFunc = symbol::Symbol(pStrIn->get(0));
    types::InternalType* pITFunc = symbol::Context::getInstance()->get(symbolFunc);

    // is a macro, macro file or not a function
    if (pITFunc == NULL)
    {
        out.push_back(new types::Bool(0));
        return types::Function::OK;
    }

    symbol::Context::getInstance()->remove(symbolFunc);

    out.push_back(new types::Bool(1));
    return types::Function::OK;
}
