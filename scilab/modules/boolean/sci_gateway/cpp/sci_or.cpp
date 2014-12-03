/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2012 - Allan CORNET
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "boolean_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "types.hxx"
#include "bool.hxx"

extern "C"
{
#include <string.h>
#include "Scierror.h"
#include "localization.h"
#include "vect_or.h"
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : or */
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_or(types::typed_list &in, int _iRetCount, types::typed_list &out)
{

    int m1 = 0, n1 = 0;
    int opt = 0;
    int *pBoolValuesOne = NULL;
    int *pBoolResult = NULL;

    int *piAddressVarOne = NULL;

    int pBdims = 0;
    int *pBdimsArray = NULL;
    int *pBdimsArrayOut = NULL;

    if ((in.size() < 1) || (in.size() > 2))
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "or", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "or", 1);
        return types::Function::Error;
    }

    if (in[0]->isBool() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_or";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    if (in.size() == 2)
    {

        //
        if (in[1]->getAs<types::GenericType>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "or", 2);
            return types::Function::Error;
        }

        if (in[1]->isString())
        {
            char *pStr =  wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
            size_t len = strlen(pStr);
            switch (pStr[0])
            {
                case 'r':
                {
                    opt = OR_BY_ROWS;
                }
                break;
                case '*':
                {
                    opt = OR_ON_ALL;
                }
                break;
                case 'c':
                {
                    opt = OR_BY_COLUMNS;
                }
                break;
                default:
                {
                    Scierror(44, _("%s: Wrong value for input argument #%d.\n"), "or", 2);
                    return types::Function::Error;
                }
                break;
            }
            delete(pStr);
            if (len != 1)
            {
                Scierror(44, _("%s: Wrong value for input argument #%d.\n"), "or", 2);
                return types::Function::Error;
            }

        }
        else if (in[1]->isDouble())
        {
            Double *pdblIn = in[1]->getAs<types::Double>();

            if (pdblIn->isComplex())
            {
                Scierror(44, _("%s: Wrong value for input argument #%d.\n"), "or", 2);
                return types::Function::Error;

            }
            opt = pdblIn->get(0);
            if (opt != pdblIn->get(0))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "or", 2);
                return types::Function::Error;
            }
            /* if ((opt > OR_BY_COLUMNS) || (opt < OR_BY_ROWS))
             {
                 Scierror(44, _("%s: Wrong value for input argument #%d.\n"), "or", 2);
                 return types::Function::Error;
             }*/
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "or", 2);
            return types::Function::Error;
        }
    }


    types::Bool *pboolIn = in[0]->getAs<types::Bool>();
    pBdims = pboolIn->getDims();
    pBdimsArray = pboolIn->getDimsArray();
    pBoolValuesOne = pboolIn->get();

    if (opt > pBdims)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "or", 2);
        return types::Function::Error;
    }

    pBdimsArrayOut = new int[pBdims];
    for (int i = 0 ; i < pBdims; i++)
    {
        pBdimsArrayOut[i] =  pBdimsArray[i];
    }

    if (opt != 0)
    {
        pBdimsArrayOut[opt - 1] = 1;
    }
    else
    {
        for (int i = 0 ; i < pBdims; i++)
        {
            pBdimsArrayOut[i] =  1;
        }
    }

    int siz = 1;
    for (int i = 0; i < pBdims; i++)
    {
        siz *= pBdimsArrayOut[i];

    }
    pBoolResult = new int[siz];
    types::Bool *pboolOut = new types::Bool(pBdims, pBdimsArrayOut);

    vect_or(pBoolValuesOne, pBdimsArray, pBdims, pBoolResult, opt);


    pboolOut->set(pBoolResult);

    delete(pBdimsArrayOut);

    out.push_back(pboolOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
