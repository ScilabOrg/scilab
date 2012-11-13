/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "history_manager_gw.hxx"
#include "function.hxx"
#include "HistoryManager.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_resethistory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() == 0)
    {
        BOOL bOK = HistoryManager::getInstance()->reset();
        if(!bOK)
        {
            Scierror(999, _("%s: Reset history failed.\n"), "resethistory");
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "resethistory", 0);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

