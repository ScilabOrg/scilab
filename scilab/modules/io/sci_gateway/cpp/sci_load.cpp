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
#include "api_scilab.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "library.hxx"
#include "loadlib.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_load(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    InternalType* pIT = in[0];

    if (pIT->isString() == false)
    {
        return Function::Error;
    }

    String *pS = pIT->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        return Function::Error;
    }

    char* pstPath = wide_string_to_UTF8(pS->get(0));
    // get file extension
    std::string fileExt(pstPath);
    int lastPos = fileExt.find_last_of("/\\") + 1;
    fileExt = fileExt.substr(lastPos);
    lastPos = fileExt.find_last_of(".") + 1;
    fileExt = fileExt.substr(lastPos);

    if (!fileExt.compare("lib") || !fileExt.compare("xml"))
    {
        wchar_t* pwstPathLib = expandPathVariableW(to_wide_string(pstPath));
        Library* lib = loadlib(pwstPathLib);
        FREE(pstPath);
        FREE(pwstPathLib);

        if (lib == NULL)
        {
            return Function::Error;
        }
    }
    else if (isHDF5File(pstPath))
    {
        FREE(pstPath);
        //call overload
        std::wstring wstFuncName = L"%_sodload";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }
    else
    {
        FREE(pstPath);
        Scierror(999, _("%s: Wrong file extension.\n"), "load");
        return Function::Error;
    }

    return Function::OK;
}


/*--------------------------------------------------------------------------*/
