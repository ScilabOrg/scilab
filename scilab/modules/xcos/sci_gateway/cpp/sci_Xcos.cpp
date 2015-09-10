/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2012-2015 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <cwchar>

#include "Xcos.hxx"
#include "loadStatus.hxx"
#include "view_scilab/Adapters.hxx"
#include "view_scilab/DiagramAdapter.hxx"

#include "types.hxx"
#include "function.hxx"

#include "gw_xcos.hxx"

extern "C" {
#include "sci_malloc.h"
#include "getFullFilename.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace org_scilab_modules_xcos;
using namespace org_scilab_modules_scicos;

static int callXcos(char* fname, char* file, long diagramId);

/*--------------------------------------------------------------------------*/
static char funname[] = "xcos";

types::Function::ReturnValue sci_Xcos(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), funname, 0, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname, 1);
        return types::Function::Error;
    }

    /*
     * xcos() call
     */
    if (in.empty())
    {
        callXcos(funname, nullptr, ScicosID());
        return types::Function::OK;
    }

    /*
     * xcos("file.zcos") or xcos(["file.zcos" "foo.zcos"]) call
     */
    if (in.size() == 1 && in[0]->isString())
    {
        types::String* arg1 = in[0]->getAs<types::String>();

        for (int i = 0; i < arg1->getSize(); ++i)
        {
            char* c_str = wide_string_to_UTF8(arg1->get(0));
            char* file = getFullFilename(c_str);
            if (file == nullptr)
            {
                FREE(c_str);
                return types::Function::Error;
            }
            if (callXcos(funname, file, ScicosID()))
            {
                FREE(c_str);
                return types::Function::Error;
            }
            FREE(c_str);
        }

        return types::Function::OK;
    }

    /*
     * xcos(scs_m) call
     */
    if (in.size() == 1 && in[0]->isUserType())
    {
        view_scilab::Adapters::adapters_index_t index = view_scilab::Adapters::instance().lookup_by_typename(in[0]->getShortTypeStr());
        if (index != view_scilab::Adapters::DIAGRAM_ADAPTER)
        {
            Scierror(77, _("%s: Wrong type for input argument #%d: ""%s"" expected.\n"), funname, 1, "diagram");
            return types::Function::Error;
        }

        if (callXcos(funname, nullptr, in[0]->getAs<view_scilab::DiagramAdapter>()->getAdaptee()->id()))
        {
            return types::Function::Error;
        }
        return types::Function::OK;
    }

    // 	we finished the 1-argument handling ; short-cut return with a clear error message
    if (in.size() == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string or ""%s"" expected.\n"), funname, 1, "diagram");
        return types::Function::Error;
    }


    /*
     * xcos("file.zcos", scs_m) call ; load the file into an existing diagram
     */
    if (in.size() == 2 &&
            in[0]->isString() &&
            in[1]->isUserType())
    {

        if (in[0]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), funname, 1);
            return types::Function::Error;
        }

        view_scilab::Adapters::adapters_index_t index = view_scilab::Adapters::instance().lookup_by_typename(in[0]->getShortTypeStr());
        if (index != view_scilab::Adapters::DIAGRAM_ADAPTER)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: ""%s"" expected.\n"), funname, 2, "diagram");
            return types::Function::Error;
        }


        if (callXcos(funname, nullptr, in[0]->getAs<view_scilab::DiagramAdapter>()->getAdaptee()->id()))
        {
            return types::Function::Error;
        }
        return types::Function::OK;
    }
    /*
     * If not returned yet, display a generic error message.
     */
    Scierror(999, _("%s: Wrong type for input argument #%d: string or ""%s"" expected.\n"), funname, 1, "diagram");
    return types::Function::Error;
}
/*--------------------------------------------------------------------------*/

static int callXcos(char *fname, char* file, long diagramId)
{
    set_loaded_status(XCOS_CALLED);

    try
    {
        Xcos::xcos(getScilabJavaVM(), file, diagramId);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        std::cerr << exception.getJavaExceptionName() << std::endl;
        std::cerr << exception.getJavaDescription() << std::endl;
        std::cerr << exception.getJavaStackTrace() << std::endl;

        Scierror(999, "%s: %s\n", fname,
                 exception.getJavaDescription().c_str());

        if (file)
        {
            FREE(file);
        }
        return 1;
    }
    catch (GiwsException::JniException &exception)
    {
        std::cerr << exception.getJavaExceptionName() << std::endl;
        std::cerr << exception.getJavaDescription() << std::endl;
        std::cerr << exception.getJavaStackTrace() << std::endl;

        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());

        if (file)
        {
            FREE(file);
        }
        return 1;
    }

    if (file)
    {
        FREE(file);
    }
    return 0;
}

