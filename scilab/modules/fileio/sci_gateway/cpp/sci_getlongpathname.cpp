/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
#include "bool.hxx"

extern "C"
{
#include "getlongpathname.h"
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getlongpathname(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "getlongpathname" , 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "getlongpathname", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "getlongpathname", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();

    types::String* pOut1 = new types::String(pS->getRows(), pS->getCols());
    types::Bool* pOut2 = new types::Bool(pS->getRows(), pS->getCols());
    int* pBool = pOut2->get();
    int size = pS->getSize();
    wchar_t** p = pS->get();
    for (int i = 0 ; i < size; i++)
    {
        wchar_t* tmp = getlongpathnameW(p[i], (BOOL*)&pBool[i]);
        pOut1->set(i, tmp);
        FREE(tmp);
    }

    out.push_back(pOut1);
    if (_iRetCount == 2)
    {
        out.push_back(pOut2);
    }
    else
    {
        delete pOut2;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
