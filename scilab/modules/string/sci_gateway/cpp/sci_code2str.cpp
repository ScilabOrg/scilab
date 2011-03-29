/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}


types::Function::ReturnValue sci_code2str(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOutDouble = NULL;
    
    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"code2str", 1);
        return types::Function::Error;
    }    
  /*  if(_iRetCount > 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"strstr", 1);
        return types::Function::Error;
    }
  */    
	if(in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"code2str", 1);
		return types::Function::Error;
	}

    wchar_t* pwstStr = in[0]->getAs<types::String>()->get(0);

    if(wcslen(pwstStr) == 0)
    {
        pOutDouble = types::Double::Empty();
    }
    
    pOutDouble = new types::Double(1);
   
    out.push_back(pOutDouble);
    return types::Function::OK;
}

