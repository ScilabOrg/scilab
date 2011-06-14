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

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_warning.h"
#include "charEncoding.h"
#include "sciprint.h"
#include "getos.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

types::Function::ReturnValue sci_warning(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(77,_W("%ls: Wrong number of input argument(s): %d expected.\n"), L"warning", 1);
        return Function::Error;
    }

    if(_iRetCount != 1)
    {
        ScierrorW(78,_W("%ls: Wrong number of output argument(s): %d expected.\n"), L"warning", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: String expected.\n"), L"warning", 1);
        return Function::Error;
    }

    String *psInput = in[0]->getAs<types::String>();

    if (psInput->getSize() == 1)
    {
        /* "on" "off" "query" */
        if(wcscmp(psInput->get(0), L"on") == 0)
        {
            setWarningMode(TRUE);
            return Function::OK;
        }

        if(wcscmp(psInput->get(0), L"off") == 0)
        {
            setWarningMode(FALSE);
            return Function::OK;
        }

        if(wcscmp(psInput->get(0), L"query") == 0)
        {
            if(getWarningMode())
            {
                out.push_back(new String(L"on"));
            }
            else
            {
                out.push_back(new String(L"off"));
            }
            return Function::OK;
        }
    }

    if (getWarningMode())
    {
        for(int i = 0; i < psInput->getSize() ; ++i)
        {
            sciprintW(_W("WARNING: %ls\n"), psInput->get(i));
        }
        sciprintW(L"\n");
    }


	return Function::OK;
}
/*--------------------------------------------------------------------------*/
