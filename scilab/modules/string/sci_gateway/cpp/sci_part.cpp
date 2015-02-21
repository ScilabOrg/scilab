/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : Let  s[k]  stands for the  k  character of Input_StringMatrixings
  ( or the  white space character if  k >length(s) ).
  part  returns  c , a matrix of character Input_StringMatrixings, such that
  c(i,j)  is the Input_StringMatrixing  "s[v(1)]...s[v(n)]"  (  s=mp(i,j)  ).
                                                                          */
/*------------------------------------------------------------------------*/
#include "string_gw.hxx"
#include "funcmanager.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "partfunction.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_part(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "part", 2);
        return Function::Error;
    }

    if (_iRetCount != -1 && _iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "part", 1);
        return Function::Error;
    }

    //part([], ...
    if (in[0]->isDouble() && in[0]->getAs<Double>()->isEmpty())
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if (in[0]->isString() == false)
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_part";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    String* pS = in[0]->getAs<types::String>();

    if (in[1]->isDouble() == false)
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%"  + in[1]->getShortTypeStr() + L"_part";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }


    Double* pD = in[1]->getAs<Double>();
    if (pD->isVector() == false && pD->isEmpty() == false)
    {
        //non vector
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), "part", 2);
        return Function::Error;
    }

    int* piIndex = new int[pD->getSize()];
    for (int i = 0 ; i < pD->getSize() ; i++)
    {
        piIndex[i] = static_cast<int>(pD->getReal()[i]);
    }

    wchar_t** pwstOut = partfunctionW(pS->get(), pS->getRows(), pS->getCols(), piIndex, pD->getSize());
    delete[] piIndex;
    String* pOut = new String(pS->getRows(), pS->getCols());
    pOut->set(pwstOut);
    freeArrayOfWideString(pwstOut, pOut->getSize());
    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
