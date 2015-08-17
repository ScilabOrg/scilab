/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#include "dynamic_link_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "dynamic_link.h"
#include "localization.h"
#include "Scierror.h"
}

bool isLink(wchar_t* _pwstEntryPoint, int* _piLib);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_c_link(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iLib = -1;
    char* pstFunctionName = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "c_link", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "c_link", 1, 2);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        types::Double* pDId = in[1]->getAs<types::Double>();
        if (pDId == NULL || pDId->isScalar() == false)
        {
            Scierror(999 , _("%s : second argument must be a unique id of a shared library.\n"), "c_link");
            return types::Function::Error;
        }

        iLib = (int)pDId->get(0);
    }


    if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "c_link", 1);
        return types::Function::Error;
    }

    types::String* pSLibName = in[0]->getAs<types::String>();
    bool bFind = isLink(pSLibName->get(0), &iLib);

    out.push_back(new types::Bool(bFind));
    if (_iRetCount == 2)
    {
        out.push_back(new types::Double(iLib));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
bool isLink(wchar_t* _pwstEntryPoint, int* _piLib)
{
    ConfigVariable::EntryPointStr* pEP = ConfigVariable::getEntryPoint(_pwstEntryPoint, *_piLib);

    if (pEP == NULL)
    {
        return false;
    }

    *_piLib = pEP->iLibIndex;
    return true;
}
/*--------------------------------------------------------------------------*/
