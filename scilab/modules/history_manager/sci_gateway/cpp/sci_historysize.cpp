/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_historysize(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStr = NULL;
    types::Double* pDo = NULL;
    int ret  = 0;

    if (in.size() == 0)
    {
        int size = HistoryManager::getInstance()->getNumberOfLines() - 1;
        out.push_back(new types::Double((double)size));
    }
    else if (in.size() == 1)
    {
        if (in[0]->isString() == true)
        {
            if(in[0]->getAs<types::String>()->isScalar() == true)
            {
                pStr = in[0]->getAs<types::String>();
                if (wcscmp(pStr->get(0), L"max") == 0)
                {
                    ret = HistoryManager::getInstance()->getNumberOfLinesMax();
                    out.push_back(new types::Double((double)ret));
                    
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" expected.\n"), "historysize", 1, "max");
                    return types::Function::Error;
                }    
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "historysize", 1);
                return types::Function::Error;
            }
            
        }
        else if (in[0]->isDouble() == true)
        {
            if(in[0]->getAs<types::Double>()->isScalar() == true)
            {
                pDo = in[0]->getAs<types::Double>();
                
                if (!HistoryManager::getInstance()->setNumberOfLinesMax(pDo->get(0)))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "historysize", 1);
                    return types::Function::Error;
                }
                out.push_back(pDo);
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "historysize", 1);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "historysize", 1);
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "historymanager", 0, 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

