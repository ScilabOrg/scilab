/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Cong WU
* Copyright (C) 2008 - 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/* desc : search position of a character string in an other string
using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include "function.hxx"
#include "context.hxx"
#include "types.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "localization.h"
#include "pcre.h"
#include "pcre_private.h"
#include "pcre_error.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_strdup.h"
}
/*------------------------------------------------------------------------*/
#define WCHAR_S L's'
#define WCHAR_R L'r'
#define WSTR_ONCE L'o'
/*------------------------------------------------------------------------*/
Function::ReturnValue sci_regexp(typed_list &in, int _iRetCount, typed_list &out)
{
	wchar_t wcType          = WCHAR_S;
    wchar_t* pwstInput      = NULL;
    wchar_t* pwstPattern    = NULL;

    int iPcreStatus         = 0;
    int iStart              = 0;
    int iStep               = 0;
    int iEnd                = 0;
    int* piStart            = NULL;
    int* piEnd              = NULL;
    int iOccurs             = 0;

    if(in.size() < 2 || in.size() > 3)
    {
        ScierrorW(999,_W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"regexp", 2, 3);
        return Function::Error;
    }

    // check output parameters
    if(_iRetCount < 1 || _iRetCount > 3)
    {
        ScierrorW(999,_W("%ls: Wrong number of output arguments: %d expected.\n"), L"regexp", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), L"regexp", 1);
        return Function::Error;
    }
    pwstInput = in[0]->getAsString()->string_get(0);

    if(in[1]->isString() == false || in[1]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), L"regexp", 2);
        return Function::Error;
    }
    pwstPattern = in[1]->getAsString()->string_get(0);

    if(in.size() == 3)
    {
        if(in[2]->isString() == false || in[2]->getAsString()->size_get() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), L"regexp", 3);
            return Function::Error;
        }

        if(in[2]->getAsString()->string_get(0)[0] != WSTR_ONCE)
        {
           ScierrorW(999,_W("%ls: Wrong type for input argument #%d: '%ls' expected.\n"), L"regexp", 3, L"o");
           return Function::Error;
        }
        wcType = WSTR_ONCE;
    }

    //input is empty
    if(wcslen(pwstInput) == 0)
    {
        Double* pStart = new Double(0,0);
        out.push_back(pStart);
        if(_iRetCount > 1)
        {
            Double* pEnd = new Double(0,0);
            out.push_back(pEnd);

            if(_iRetCount > 2)
            {
                String* pS = new String(0,0);
                out.push_back(pS);
            }
        }
        return Function::OK;
    }

    piStart     = new int[wcslen(pwstInput)];
    piEnd       = new int[wcslen(pwstInput)];

    do
    {
        iPcreStatus = wide_pcre_private(pwstInput + iStep, pwstPattern, &iStart, &iEnd);
        if(iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd)
        {
            piStart[iOccurs]    = iStart + iStep;
            piEnd[iOccurs++]    = iEnd + iStep;
            iStep               += iEnd;
        }
        else if(iPcreStatus != NO_MATCH)
        {
            pcre_error("regexp", iPcreStatus);
            delete[] piStart;
            delete[] piEnd;
            return Function::Error;
        }
    }while(iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd && wcType != WSTR_ONCE);
 

    Double* pStart = new Double(1, iOccurs);
    double* pdblStart = pStart->real_get();

    for(int i = 0 ; i < iOccurs ; i++)
    {
        pdblStart[i] = piStart[i] + 1; //one indexed
    }

    out.push_back(pStart);

    if(_iRetCount > 1)
    {
        Double* pEnd = new Double(1, iOccurs);
        double* pdblEnd = pEnd->real_get();
        for(int i = 0 ; i < iOccurs ; i++)
        {
            pdblEnd[i]   = piEnd[i];
        }
        out.push_back(pEnd);
    }

    if(_iRetCount == 3)
    {
        String *pS = NULL;
        if(iOccurs == 0)
        {
            pS = new String(1,1);
            pS->string_set(0, L"");
        }
        else
        {
            pS = new String(iOccurs, 1);
            for(int i = 0 ; i < iOccurs ; i++)
            {
                wchar_t* pwstTemp = new wchar_t[piEnd[i] - piStart[i] + 1];
                wcsncpy(pwstTemp, pwstInput + piStart[i], piEnd[i] - piStart[i]);
                pwstTemp[piEnd[i] - piStart[i]] = 0;
                pS->string_set(i, 0, pwstTemp);
                delete[] pwstTemp;
            }
        }
        out.push_back(pS);
    }
    delete[] piStart;
    delete[] piEnd;
    return Function::OK;
}
/*-----------------------------------------------------------------------------------*/
