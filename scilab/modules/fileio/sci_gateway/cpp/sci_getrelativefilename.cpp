/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "PATH_MAX.h"
#include "isdir.h"
#include "FileExist.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "getrelativefilename.h"
}
/*--------------------------------------------------------------------------*/

Function::ReturnValue sci_getrelativefilename(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    wchar_t* wcsAbsDir  = NULL;
    wchar_t* wcsAbsFile = NULL;
    wchar_t* wcsResult  = NULL;

    types::String* pStrDir  = NULL;
    types::String* pStrFile = NULL;
    types::String* pStrOut  = NULL;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getrelativefilename", 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "getrelativefilename", 1);
        return types::Function::Error;
    }

    pStrDir = in[0]->getAs<types::String>();

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "getrelativefilename", 2);
        return types::Function::Error;
    }

    pStrFile = in[1]->getAs<types::String>();

    if (pStrDir->getSize() != pStrFile->getSize())
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same size expected.\n"), "getrelativefilename", 1, 2);
        return types::Function::Error;
    }

    pStrOut = new types::String(pStrDir->getDims(), pStrDir->getDimsArray());

    for (int i = 0; i < pStrDir->getSize(); i++)
    {
        wcsAbsDir = expandPathVariableW(pStrDir->get(i));
        if (wcslen(wcsAbsDir) > PATH_MAX)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"), "getrelativefilename", 1, PATH_MAX);
            return types::Function::Error;
        }
        if (!isdirW(wcsAbsDir))
        {
            char* pstAbs = wide_string_to_UTF8(wcsAbsDir);
            Scierror(999, _("%s: Directory '%s' doesn't exists.\n"), "getrelativefilename", pstAbs);
            FREE(pstAbs);
            return types::Function::Error;
        }

        wcsAbsFile = expandPathVariableW(pStrFile->get(i));
        if (wcslen(wcsAbsFile) > PATH_MAX)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"), "getrelativefilename", 2, PATH_MAX);
            return types::Function::Error;
        }
        if (!FileExistW(wcsAbsFile))
        {
            char* pstAbs = wide_string_to_UTF8(wcsAbsFile);
            Scierror(999, _("%s: File '%s' doesn't exists.\n"), "getrelativefilename", pstAbs);
            FREE(pstAbs);
            return types::Function::Error;
        }

        wcsResult = getrelativefilenameW(wcsAbsDir, wcsAbsFile);

        FREE(wcsAbsDir);
        FREE(wcsAbsFile);

        pStrOut->set(i, wcsResult);
    }

    out.push_back(pStrOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
