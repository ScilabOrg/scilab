/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "getmode.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}


using namespace types;

int getMode(typed_list &in, int _iProcess, int _iRef)
{
    int iMode = 0;
    if(in[_iProcess]->isString())
    {
        String* pS = in[_iProcess]->getAs<String>();
        if(pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
        }

        switch(pS->get(0)[0])
        {
        case 'r' :
            iMode = 1;
            break;
        case 'c' :
            iMode = 2;
            break;
        case '*' :
            iMode = 0;
            break;
        case 'm' :
            iMode = -1;
            break;
        default :
            Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), "size", _iProcess + 1, "m" , "*" , "r", "c");
            iMode = -2;
            break;
        }
    }
    else if(in[1]->isDouble() && in[1]->getAs<Double>()->isComplex() == false)
    {
        Double* pD = in[_iProcess]->getAs<Double>();
        if(pD->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
            iMode = -2;
        }

        iMode = static_cast<int>(pD->getReal()[0]);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"), "size", 2);
        iMode = -2;
    }

    //special case for -1
    if(iMode == -1)
    {
        iMode = 0;
        if(in[_iRef]->getAsGenericType()->getRows() > 1)
        {
            iMode = 1;
        }
        else if(in[_iRef]->getAsGenericType()->getCols() > 1)
        {
            iMode = 2;
        }
    }

    return iMode;
}

