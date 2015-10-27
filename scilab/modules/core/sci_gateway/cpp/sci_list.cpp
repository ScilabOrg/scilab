/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "list.hxx"
#include "function.hxx"

types::Function::ReturnValue sci_list(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    types::List *pRetVal = new types::List();

    for (const auto & it : in)
    {
        pRetVal->append((it));
    }

    out.push_back(pRetVal);
    return types::Function::OK;
}
