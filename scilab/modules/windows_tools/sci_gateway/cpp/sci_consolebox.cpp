/*
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "WinConsole.h"
#include "sci_mode.h"
}

using namespace types;

Function::ReturnValue sci_consolebox(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() < 0 || in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "consolebox", 0, 1);
        return Function::Error;
    }

    //request mode
    if (in.size() == 0)
    {
        if (getScilabMode() == SCILAB_STD)
        {
            if (GetConsoleState())
            {
                out.push_back(new String(L"on"));
            }
            else
            {
                out.push_back(new String(L"off"));
            }
        }
        else
        {
            sciprint(_("Only on Windows Mode, not in Console Mode.\n"));
            out.push_back(new String(L"off"));
        }
    }
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
