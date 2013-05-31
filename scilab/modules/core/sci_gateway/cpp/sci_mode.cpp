/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
#include "configvariable.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_mode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    InternalType *pIT = NULL;

    /* Check the number of input argument */
    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "mode" , 0, 1);
        return Function::Error;
    }

    /* Check the number of output argument */
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "mode", 1);
        return Function::Error;
    }

    if (in.size() == 0)
    {
        out.push_back(new Double(ConfigVariable::getPromptMode()));
    }
    else
    {
        if (in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "mode", 1);
            return Function::Error;
        }

        types::Double* pDblIn = in[0]->getAs<types::Double>();

        if (pDblIn->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "mode", 1);
            return types::Function::Error;
        }

        int iScilabMode = (int)pDblIn->get(0);
        if (pDblIn->get(0) != (double)iScilabMode)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), "mode", 1);
            return types::Function::Error;
        }

        ConfigVariable::setPromptMode(iScilabMode);
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
