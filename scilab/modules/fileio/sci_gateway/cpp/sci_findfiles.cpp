/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "findfiles.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include "os_wcsdup.h"
#include "scicurdir.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
#define DEFAULT_FILESPEC L"*.*"

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_findfiles(typed_list &in, int _iRetCount, typed_list &out)
{
    wchar_t* pwstPath   = NULL;
    wchar_t* pwstSpec   = NULL;
    bool bAllocatedSpec = false;

    if(in.size() < 0 || in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"findfiles", 0, 2);
        return Function::Error;
    }

    if(in.size() == 0)
    {//default path and default file spec
        int ierr        = 0;

        pwstPath        = scigetcwdW(&ierr);
        pwstSpec        = os_wcsdup(DEFAULT_FILESPEC);
        bAllocatedSpec  = true;
    }
    else
    {//user path
        if(in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"findfiles", 1);
            return Function::Error;
        }

        pwstPath = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);

        if(in.size() == 2)
        {//user file spec
            if(in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"findfiles", 2);
                return Function::Error;
            }

            pwstSpec = in[1]->getAs<types::String>()->get()[0];
        }
        else
        {//default file spec
            pwstSpec        = os_wcsdup(DEFAULT_FILESPEC);
            bAllocatedSpec  = true;
        }
    }

    int iSize               = 0;
    wchar_t** pwstFilesList = NULL;

    pwstFilesList = findfilesW(pwstPath, pwstSpec, &iSize, FALSE);
    if(pwstFilesList)
    {
        String* pS = new String(iSize, 1);
        pS->set(pwstFilesList);
        freeArrayOfWideString(pwstFilesList, iSize);
        out.push_back(pS);
    }
    else
    {
        out.push_back(Double::Empty());
    }

    FREE(pwstPath);
    if(bAllocatedSpec)
    {
        FREE(pwstSpec);
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
