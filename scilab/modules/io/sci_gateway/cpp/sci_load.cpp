/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "io_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "library.hxx"
#include "loadlib.hxx"


extern "C"
{
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
#include "FileExist.h"
#include "Scierror.h"
}

using namespace types;
static const std::string fname("laod");
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_load(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    InternalType* pIT = in[0];
    if (pIT->getId() != InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname.data(), 1);
        return Function::Error;
    }

    String *pS = pIT->getAs<types::String>();
    wchar_t* pwstPathLib = expandPathVariableW(pS->get(0));
    char* pstPath = wide_string_to_UTF8(pwstPathLib);
    if (FileExist(pstPath))
    {
        if (isHDF5File(pstPath))
        {
            FREE(pstPath);
            FREE(pwstPathLib);

            //call overload
            std::wstring wstFuncName = L"%_sodload";
            ast::ExecVisitor exec;
            Callable::ReturnValue Ret = Callable::Error;
            Ret = Overload::call(wstFuncName, in, _iRetCount, out, &exec);
            return Ret;
        }
        else
        {
            int err = 0;
            Library* lib = loadlib(pS->get(0), &err);
            FREE(pstPath);

            switch (err)
            {
                case 0:
                    //no error
                    break;
                case 1:
                {
                    char* pstPath = wide_string_to_UTF8(pS->get(0));
                    Scierror(999, _("%s: %s is not a valid module file.\n"), fname.data(), pstPath);
                    FREE(pstPath);
                    return Function::Error;
                }
                case 2:
                {
                    Scierror(999, "%s: %s", fname.data(), _("Redefining permanent variable.\n"));
                    return Function::Error;
                }
                default:
                {
                    //nothing
                }
            }

            FREE(pwstPathLib);
            lib->killMe();
        }
    }
    else
    {
        Scierror(999, _("%s: Unable to open file: \"%ls\".\n"), fname.data(), pwstPathLib);
        FREE(pstPath);
        FREE(pwstPathLib);
        return Function::Error;
    }

    return Function::OK;
}


/*--------------------------------------------------------------------------*/
