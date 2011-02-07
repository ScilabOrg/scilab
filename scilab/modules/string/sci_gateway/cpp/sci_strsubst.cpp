/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "string_gw.hxx"
#include "function.hxx"
#include "arrayof.hxx"

extern "C"
{
#include "strsubst.h"
#include "localization.h"
#include "Scierror.h"
#include "pcre_error.h"
#include "pcre_private.h"
#include "freeArrayOfString.h"
}
/*-------------------------------------------------------------------------------------*/
#define WCHAR_R L'r'
#define WCHAR_S L's'
/*-------------------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_strsubst(typed_list &in, int _iRetCount, typed_list &out)
{
    bool bRegExp = false;
    if(in.size() < 3 || in.size() > 4)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"strsubst", 3, 4);
        return Function::Error;
    }

    if(in.size() > 3)
    {
        if(in[3]->isString() == false && in[3]->getAsString()->size_get() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"strsubst", 4);
            return Function::Error;
        }

        if(in[3]->getAsString()->string_get(0)[0] == WCHAR_R)
        {
            bRegExp = true;
        }
        else if(in[3]->getAsString()->string_get(0)[0] == WCHAR_S)
        {
            bRegExp = false;
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: 's' or 'r' expected.\n"), L"strsubst", 4);
            return Function::Error;
        }
    }

    if(in[2]->isString() == false || in[2]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"strsubst", 3);
        return Function::Error;
    }

    wchar_t* pwstReplace = in[2]->getAsString()->string_get()[0];

    if(in[1]->isString() == false || in[1]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"strsubst", 2);
        return Function::Error;
    }

    wchar_t* pwstSearch = in[1]->getAsString()->string_get()[0];

    if(in[0]->isDouble() && in[0]->getAs<Double>()->isEmpty())
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string matrix expected.\n"), L"strsubst", 1);
        return Function::Error;
    }

    String* pS = in[0]->getAsString();

    String* pOut = new String(pS->rows_get(), pS->cols_get());
    wchar_t** pwstOutput = NULL;

    if(bRegExp)
    {
        int iErr = 0;
        pwstOutput = wcssubst_reg(pS->string_get(), pS->size_get(), pwstSearch, pwstReplace, &iErr);
        if(iErr != NO_MATCH)
        {
            pcre_error("strsubst", iErr);
            delete pOut;
            return Function::Error;
        }
    }
    else
    {
        pwstOutput = wcssubst(pS->string_get(), pS->size_get(), pwstSearch, pwstReplace);
    }

    pOut->string_set(pwstOutput);
    freeArrayOfWideString(pwstOutput, pOut->size_get());
    out.push_back(pOut);
    return Function::OK;
}
/*-------------------------------------------------------------------------------------*/
