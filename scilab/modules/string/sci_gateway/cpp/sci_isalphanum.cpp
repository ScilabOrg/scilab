/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "string_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_isalphanum(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String pStrIn = NULL;
    types::Bool pBIn = NULL;
    wchar_t* wcstrIn = NULL;
    int iResultSize = 0;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "isalphanum", 1);
        return Function::Error;
    }

    if (out.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "isalphanum", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument#%d: A String expected.\n"), "isalphanum", 1);
        types::Function::Error;
    }

    pStrIn = in[0]->getAs<types::String>();

    if (pStrIn->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument#%d: A scalar String expected.\n"), "isalphanum", 1);
        types::Function::Error;
    }

    wcstrIn = pStrIn->get();
    BOOL* pbResult = isalphanumW(wcstrIn, &iResultSize);
    if (pbResult == NULL)
    {
        out.push(types::Double::Empty());
return types::Function:
               Ok;
    }

    pBIn = new types::Bool(1, iResultSize);
    pBIn->set(pbResult);
    out.push(pBIn);

return types::Function:
           Ok;
}
/*-------------------------------------------------------------------------------------*/
