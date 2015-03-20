/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab enterprises - Anais AUBERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "core_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_mtlb_mode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mtlb_mode", 0, 1);
        return types::Function::Error;

    }

    if (in.size() == 0)
    {

        types::Bool *pBoolOut = new types::Bool(0);
        bool bO = false;
        bO = ConfigVariable::getMtlbMode();

        if (bO)
        {
            pBoolOut->setTrue();
        }
        else
        {
            pBoolOut->setFalse();
        }

        out.push_back(pBoolOut);
    }
    else
    {
        if (in[0]->isBool() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Boolean matrix expected.\n"), "mtlb_mode", 1);
            return types::Function::Error;
        }
        types::Bool *pBoolIn = in[0]->getAs<types::Bool>();
        bool bO = false;
        if (pBoolIn->isScalar() == false)
        {

            Scierror(999, _("%s: Incompatible sizes.\n"), "mtlb_mode");
            return types::Function::Error;

        }
        bO = pBoolIn->get(0);
        ConfigVariable::setMtlbMode(bO);

    }


    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
