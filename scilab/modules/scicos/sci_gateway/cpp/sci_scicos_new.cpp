/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>
#include <algorithm>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "string.hxx"
#include "function.hxx"

#include "view_scilab/Adapters.hxx"
#include "view_scilab/BlockAdapter.hxx"
#include "view_scilab/GraphicsAdapter.hxx"
#include "view_scilab/ModelAdapter.hxx"

#include "model/Block.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace org_scilab_modules_scicos;

static const std::string funame = "scicos_new";

template<class Adaptor, class Adaptee>
Adaptor* alloc_and_set(types::String* type_name, types::typed_list &in)
{
    Adaptee* o = new Adaptee("");
    Adaptor* adaptor = new Adaptor(o);

    // the first header entry is the type
    for (int i = 1; i < in.size(); i++)
    {
        if (!adaptor->set(type_name->get(i), in[i]))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: unable to set \"%s\".\n"), funame.data(), i, type_name->get(i));
            delete adaptor;
            adaptor = 0;
        }
    }

    return adaptor;
}

types::Function::ReturnValue sci_scicos_new(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }
    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::InternalType* type = in[0];
    if (type->getType() != types::InternalType::ScilabString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    types::String* type_name = type->getAs<types::String>();
    if (type_name->getRows() > 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Row vector expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    if (type_name->getCols() != in.size())
    {
        Scierror(999, _("%s: Wrong number of input argument: %d expected.\n"), funame.data(), type_name->getCols());
        return types::Function::Error;
    }

    /*
     * allocate the right adapter then try to set fields values
     */

    const view_scilab::Adapters::adapters_index_t adapter_index = view_scilab::Adapters::instance().lookup_by_typename(type_name->get(0));
    types::InternalType* status;
    switch (adapter_index)
    {
        case view_scilab::Adapters::BLOCK_ADAPTER:
            status = alloc_and_set<view_scilab::BlockAdapter, model::Block>(type_name, in);
            if (status == 0)
            {
                return types::Function::Error;
            }
            out.push_back(status);
            break;
        case view_scilab::Adapters::GRAPHIC_ADAPTER:
            status = alloc_and_set<view_scilab::GraphicsAdapter, model::Block>(type_name, in);
            if (status == 0)
            {
                return types::Function::Error;
            }
            out.push_back(status);
            break;
        case view_scilab::Adapters::MODEL_ADAPTER:
            status = alloc_and_set<view_scilab::ModelAdapter, model::Block>(type_name, in);
            if (status == 0)
            {
                return types::Function::Error;
            }
            out.push_back(status);
            break;
        default:
            Scierror(999, _("%s: Wrong value for input argument #%d:  \"%s\" type is not managed.\n"), funame.data(), 1, type_name->get(0));
            return types::Function::Error;
            break;
    }

    return types::Function::OK;
}
