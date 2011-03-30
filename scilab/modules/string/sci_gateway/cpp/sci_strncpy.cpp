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


types::Function::ReturnValue sci_strncpy(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pString      = NULL;
    types::Double* pDouble      = NULL;
    types::String* pOutString   = NULL;
    
    if(in.size() != 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"strncpy", 2);
        return types::Function::Error;
    }    
    if(_iRetCount != 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"strncpy", 1);
        return types::Function::Error;
    }    
	if(in[0]->isString() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: Matrix of strings expected.\n"),L"strncpy", 1);
		return types::Function::Error;
	}

	if(in[1]->isDouble() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: Matrix of integers expected.\n"),L"strncpy", 2);
        return types::Function::Error;
	}

    pString = in[0]->getAs<types::String>();
    pDouble = in[1]->getAs<types::Double>();
    
    if(pDouble->getSize() == 0)
    {
        ScierrorW(999,_W("%ls: Wrong size for input argument #%d: Non-empty matrix expected.\n"), L"strncpy",2);
        return types::Function::Error;
    }
    
    //same dimension or 2nd arg scalar
    if(pString->getSize() != pDouble->getSize() && pDouble->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong size for input argument #%d.\n"),L"strncpy", 2);
        return types::Function::Error;
    }
    
    pOutString  = new types::String(pString->getDims(), pString->getDimsArray());
    
    int j = 0; /* Input parameter two is dimension one */
    for(int i=0 ; i < pString->getSize() ; i++)
    {
        wchar_t *wcOutput   = NULL;
		int sizeOfCopy      = 0;
        
        if(pDouble->isScalar() == false) 
        {
            j = i; /* Input parameter One & two have same dimension */
        }

        if(pDouble->get(j) < wcslen(pString->get(i)))
        {
            int iLen = pDouble->get(j);
            if(iLen < 0)
            {
                iLen = 0;
            }
            
            wcOutput = (wchar_t*)MALLOC(sizeof(wchar_t) * (iLen + 1));
            sizeOfCopy = iLen;
        }
        else
        {
            int iLen = wcslen(pString->get(i));
            wcOutput = (wchar_t*)MALLOC(sizeof(wchar_t) * (iLen + 1));
            sizeOfCopy = iLen;
        }
		
		if(wcOutput)
		{
			wcsncpy(wcOutput, pString->get(i), sizeOfCopy);
			wcOutput[sizeOfCopy] = L'\0';

			pOutString->set(i, wcOutput);
			FREE(wcOutput);
			wcOutput = NULL;
		}
		else
		{
            delete pOutString;
			ScierrorW(999, _W("%ls: No more memory.\n"),L"strncpy");
			return types::Function::Error;
		}
    }
    
    out.push_back(pOutString);
    return types::Function::OK;
}

