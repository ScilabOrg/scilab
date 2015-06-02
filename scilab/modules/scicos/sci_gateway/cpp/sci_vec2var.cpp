/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <vector>
#include <string>
#include <cwchar>
#include <cstring>

#include "gw_scicos.hxx"

#include "vec2var.hxx"

#include "types.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

static const std::string vec2varName = "vec2var";

types::Function::ReturnValue sci_vec2var(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }

    if (!in[0]->isDouble())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real row vector expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }
    types::Double* input = in[0]->getAs<types::Double>();

    if (input->getCols() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }
    if (input->getRows() < 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, 2, 1);
        return types::Function::Error;
    }

    std::vector<double> inputVec (input->get(), input->get() + input->getRows());
    types::InternalType* res;
    if (!vec2var(inputVec, res))
    {
        return types::Function::Error;
    }

    out.push_back(res);
    return types::Function::OK;
}
