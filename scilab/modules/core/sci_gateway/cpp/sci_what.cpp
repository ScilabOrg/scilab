/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "string.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "commandwords.h"
#include "getfunctionsname.h"
#include "sciprint.h"
}

types::Function::ReturnValue sci_what(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutStringFct = NULL;
    char **pOutStringFctTmp = NULL;
    int sizeFct = 0;
    types::String* pOutStringCmd = NULL;
    char **pOutStringCmdTmp = NULL;
    int sizeCmd = 0;

    // Check input arguments
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strrchr", 0);
        return types::Function::Error;
    }

    // Check output arguments
    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "strtod", 1, 2);
        return types::Function::Error;
    }

    // Retrieve list of functions and list of keywords
    pOutStringFctTmp = getFunctionsName(&sizeFct);
    pOutStringCmdTmp = getcommandkeywords(&sizeCmd);

    if (_iRetCount == 1)
    {
        // Print functions
        sciprint("\n");
        sciprint(_("Internal Functions:\n"));
        sciprint("\n");
        for (int i = 0; i < sizeFct; i++)
        {
            sciprint("%+24s ", pOutStringFctTmp[i]);
            if ((i + 1) % 4 == 0)
            {
                sciprint("\n");
            }
        }
        sciprint("\n");
        // Print keywords
        sciprint("\n");
        sciprint(_("Commands:\n"));
        sciprint("\n");
        for (int i = 0; i < sizeCmd; i++)
        {
            sciprint("%+24s ", pOutStringCmdTmp[i]);
            if ((i + 1) % 4 == 0)
            {
                sciprint("\n");
            }
        }
        sciprint("\n");
    }

    if (_iRetCount == 2)
    {
        // Scilab functions in first output argument
        pOutStringFct = new types::String(sizeFct, 1);
        for (int i=0; i<sizeFct; i++)
        {
            pOutStringFct->set(i, pOutStringFctTmp[i]);
        }
        out.push_back(pOutStringFct);

        // Scilab commands in second output argument
        pOutStringCmd = new types::String(sizeCmd, 1);
        for (int i=0; i<sizeCmd; i++)
        {
            pOutStringCmd->set(i, pOutStringCmdTmp[i]);
        }
        out.push_back(pOutStringCmd);
    }

    return types::Function::OK;
}
