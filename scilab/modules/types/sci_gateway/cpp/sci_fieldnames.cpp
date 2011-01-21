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

#include "function.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "list.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_fieldnames(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"fieldnames", 1);
        return Function::Error;
    }


    // FIXME : iso-functionnal to Scilab < 6
    // Works on other types excepts {m,t}list and struct
    if (in[0]->isStruct() == false && in[0]->isMList() == false && in[0]->isTList() == false)
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    // STRUCT
    if (in[0]->isStruct() == true)
    {
        Struct *pInStruct = in[0]->getAs<Struct>();
        //String *pstOutput = new String(pInStruct->size_get());

        return Function::OK;
    }

    // TLIST or MLIST
    // We only need list capabilities so retrieve first argument as List.
    List *pInList = in[0]->getAs<List>();
    InternalType *pIT = pInList->get(0);

    if (pIT == NULL || pIT->isString() == false)
    {
        // FIXME : iso-functionnal to Scilab < 6
        // Works on other types excepts {m,t}list and struct
        out.push_back(Double::Empty());
        return Function::OK;
    }

    String *pAllFields = pIT->getAs<String>();
    wchar_t **pwcsAllStrings =  pAllFields->string_get();
    // shift to forget first value corresponding to type.
//    ++pwcsAllStrings;



    String *pNewString = new String(pAllFields->size_get() - 1, 1, pwcsAllStrings+1);

    out.push_back(pNewString);

    return Function::OK;
}
