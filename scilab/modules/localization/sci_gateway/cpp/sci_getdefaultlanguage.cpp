/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "localization_gw.hxx"
#include "function.hxx"
#include "arrayof.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "defaultlanguage.h"
}

using namespace types;

Function::ReturnValue sci_getdefaultlanguage(typed_list &in, int _piRetCount, typed_list &out)
{
    if(in.size() != 0)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"getdefaultlanguage", 0);
        return Function::Error;
    }

    if(_piRetCount != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"getdefaultlanguage", 1);
        return Function::Error;
    }
    
    out.push_back(new String(SCILABDEFAULTLANGUAGE));

    return Function::OK;
}
