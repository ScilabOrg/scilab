/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_clear(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    symbol::Context* ctx = symbol::Context::getInstance();
    int iWrongType = 1;

    if (in.size() == 0)
    {
        //clear();
        ctx->removeAll();
        return Function::OK;
    }

    // First check if all arguments are Single Strings.
    int var = 0;
    for (auto pIT : in)
    {
        ++var;
        if (pIT->isString() == false)
        {
            Scierror(207, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "clear", var);
            return Function::Error;
        }
    }

    bool bShow = false;
    for (auto pIT : in)
    {
        types::String* pStr = pIT->getAs<types::String>();
        for (int i = 0; i < pStr->getSize(); i++)
        {
            symbol::Symbol sym = symbol::Symbol(pStr->get(i));
            if (ctx->isprotected(sym) == false)
            {
                ctx->remove(sym);
            }
            else
            {
                if (bShow == false)
                {
                    Scierror(999, _("Redefining permanent variable.\n"));
                    bShow = true;
                }
            }
        }
    }

    if (bShow)
    {
        return Function::Error;
    }

    return Function::OK;
}
