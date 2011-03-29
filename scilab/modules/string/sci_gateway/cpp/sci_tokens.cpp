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
#include "os_wcsdup.h"
#include "tokens.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}

#include <sciprint.h>

types::Function::ReturnValue sci_tokens(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutString   = NULL;
    types::String* pString      = NULL;
    types::String* pCharSample  = NULL;
    wchar_t* seps               = NULL;
    int sizeSeps                = 0;
    
    if(in.size() > 2 || in.size() == 0)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"tokens", 1, 2);
        return types::Function::Error;
    }    
    if(_iRetCount != 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"tokens", 1);
        return types::Function::Error;
    }
    
// first arg
	if(in[0]->isString() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"tokens", 1);
		return types::Function::Error;
	}
    pString = in[0]->getAs<types::String>();
    if(pString->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong size for input argument #%d.\n"),L"tokens", 1);
        return types::Function::Error;
    }
    if(wcslen(pString->get(0)) == 0)
    {
        types::Double* pOutDouble = types::Double::Empty();
        out.push_back(pOutDouble);
        return types::Function::OK;
    }

// second arg
    if(in.size() == 2) 
    {
    	if(in[1]->isString() == false)
	    {
		    ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"tokens", 2);
		    return types::Function::Error;
	    }
        pCharSample = in[1]->getAs<types::String>();
        
        if(pCharSample->getSize() == 0)
        {
            ScierrorW(999,_W("%ls: Wrong size for input argument #%d.\n"),L"tokens", 2);
            return types::Function::Error;
        }
        sizeSeps = pCharSample->getSize();
        seps = (wchar_t*)MALLOC((sizeSeps+1)*sizeof(wchar_t));
	    for (int i = 0; i < sizeSeps ; i++)
	    {
            int iLen = wcslen(pCharSample->get(i));
            if(iLen > 1 || iLen < 0)
            {
        		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: Char(s) expected.\n"),L"tokens", 2);
        		delete pOutString;
                return types::Function::Error;
            }
            seps[i] = pCharSample->get(i)[0];
        }
    }
    else // default delimiters are ' ' and Tabulation
    {
        sizeSeps = 2;
        seps = (wchar_t*)MALLOC((sizeSeps+1)*sizeof(wchar_t));
        seps[0] = L' ';
        seps[1] = L'\t';
    }
    seps[sizeSeps] = L'\0';

    // perfom operation
    int dimsArray[2] = {0,1};
    int dims = 2;
    
    wchar_t** Output_Strings = stringTokens(pString->get(0), seps, &dimsArray[0]);
    
    if(Output_Strings == NULL)
    {//return empty matrix
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else
    {
        pOutString  = new types::String(dims,dimsArray);
        pOutString->set(Output_Strings);
        
        for(int i = 0 ; i < dimsArray[0] ; i++)
        {
            FREE(Output_Strings[i]);
        }
        FREE(Output_Strings);
    }
    
    out.push_back(pOutString);
    return types::Function::OK;
}

