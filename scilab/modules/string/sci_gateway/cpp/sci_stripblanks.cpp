/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "string_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "types.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "stripblanks.hxx"
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "localization.h"
#include "Scierror.h"
};
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_stripblanks(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
#define FUNCNAME "stripblanks"
    bool bRemoveTab = false;

    // check input parameters
    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), FUNCNAME, 1, 2);
        return types::Function::Error;
    }

    // check output parameters
    if (_iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), FUNCNAME, 1);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isBool() == false || in[1]->getAs<types::Bool>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), FUNCNAME, 2);
            return types::Function::Error;
        }

        if (in[1]->getAs<types::Bool>()->get()[0] == 1)
        {
            bRemoveTab = true;
        }
    }

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabString:
        {
            types::String *pS = stripblanks(in[0]->getAs<types::String>(), bRemoveTab);
            if (pS == NULL)
            {
                Scierror(999, _("%s : No more memory.\n"), FUNCNAME);
                return types::Function::Error;
            }

            out.push_back(pS);
        }
        break;
        case types::InternalType::ScilabDouble://manage []
        {
            if (in[0]->getAs<types::Double>()->getSize() != 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), FUNCNAME, 1);
                return types::Function::Error;
            }

            out.push_back(types::Double::Empty());
        }
        break;
        default:
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), FUNCNAME, 1);
            return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
