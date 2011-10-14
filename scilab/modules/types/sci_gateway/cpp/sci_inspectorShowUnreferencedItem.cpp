/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>
#include "types_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "inspector.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "os_swprintf.h"
}

using namespace types;

Function::ReturnValue sci_inspectorShowUnreferencedItem(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() > 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"inspectorShowUnreferencedItem", 0, 1);    
        return Function::Error;
    }

    if(in.size() == 0)
    {
        int iCount = Inspector::getUnreferencedItemCount();
        String* pS = new String(iCount, 1);
        for(int i = 0 ; i < iCount ; i++)
        {
            pS->set(i, Inspector::showUnreferencedItem(i).c_str());
        }
        out.push_back(pS);
    }
    else
    {
        if(in[0]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"inspectorShowItem", 1);
            return Function::Error;
        }

        Double *pD = in[0]->getAs<Double>();
        if(pD->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A scalar expected.\n"), L"inspectorShowItem", 1);
            Function::Error;
        }

        int iPos = (int)pD->get(0) - 1;
        out.push_back(new String(Inspector::showUnreferencedItem(iPos).c_str()));
    }
    return Function::OK;
}
