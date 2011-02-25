/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "localization_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "defaultlanguage.h"
#include "sci_warning.h"
#include "setgetlanguage.h"
}

using namespace types;

Function::ReturnValue sci_setlanguage(typed_list &in, int _piRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"setlanguage", 1);
        return Function::Error;
    }

    if(_piRetCount != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"setlanguage", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"setlanguage", 1);
        return Function::Error;
    }

    wchar_t *param = in[0]->getAs<types::String>()->get(0);
    wchar_t *newlanguage = convertlanguagealias(param);

    if ( !LanguageIsOK(param) && (newlanguage == NULL) )
    {
        if (getWarningMode())
        {
            if (newlanguage == NULL)
            {
                sciprintW(_W("%ls: Unsupported language '%ls'.\n"), L"setlanguage", param);
            }
            else
            {
                sciprintW(_W("%ls: Unsupported language '%ls'.\n"), L"setlanguage", newlanguage);
            }
        }

        out.push_back(new Bool(FALSE));
        return Function::OK;
    }



    if (newlanguage)
    {
        if (needtochangelanguage(newlanguage))
        {
            if (!setlanguage(newlanguage))
            {
                out.push_back(new Bool(FALSE));
            }
            else
            {
                out.push_back(new Bool(TRUE));
            }
        }
        else
        {
            out.push_back(new Bool(TRUE));
        }
    }
    else
    {
        if (getWarningMode())
        {
            sciprintW(_W("Unsupported language '%ls'.\n"), param);
            sciprintW(_W("Switching to default language : '%ls'.\n"), SCILABDEFAULTLANGUAGE);
        }
        setlanguage(SCILABDEFAULTLANGUAGE);
        out.push_back(new Bool(FALSE));
    }

    return Function::OK;
}
