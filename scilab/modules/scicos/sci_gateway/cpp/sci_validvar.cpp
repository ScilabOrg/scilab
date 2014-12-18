/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>

#include "gw_scicos.hxx"
#include "context.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

static const std::string funame = "validvar";

types::Function::ReturnValue sci_validvar(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool isValid = false;
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), funame.data(), 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }


    if (symbol::Context::getInstance()->isValidVariableName(pS->get(0)))
    {
        isValid = true;
    }

    out.push_back(new types::Bool(isValid ? 1 : 0));
    return types::Function::OK;
}