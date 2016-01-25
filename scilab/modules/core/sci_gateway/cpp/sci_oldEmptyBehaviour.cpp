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
#include "scilabWrite.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "charEncoding.h"
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "Sciwarning.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_oldEmptyBehaviour(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String *psInput = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "oldEmptyBehaviour", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "oldEmptyBehaviour", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "oldEmptyBehaviour", 1);
        return types::Function::Error;
    }

    psInput = in[0]->getAs<types::String>();
    if (psInput->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Single string expected.\n"), "oldEmptyBehaviour", 1);
        return types::Function::Error;
    }

    /* "on" "off" "query" */
    if (wcscmp(psInput->get(0), L"on") == 0)
    {
        ConfigVariable::setOldEmptyBehaviour(TRUE);
        return types::Function::OK;
    }

    if (wcscmp(psInput->get(0), L"off") == 0)
    {
        ConfigVariable::setOldEmptyBehaviour(FALSE);
        return types::Function::OK;
    }

    if (wcscmp(psInput->get(0), L"query") == 0)
    {
        if (ConfigVariable::getOldEmptyBehaviour())
        {
            // OldEmptyBehaviour is on
            out.push_back(new types::String(L"on"));
        }
        else
        {
            // OldEmptyBehaviour is off
            out.push_back(new types::String(L"off"));
        }
        return types::Function::OK;
    }

    // String input is different than "on", "off" or "query"

    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {#%s}.\n"), "oldEmptyBehaviour", 1, "\"on\", \"off\", \"query\"");
    return types::Function::Error;
}
/*--------------------------------------------------------------------------*/
