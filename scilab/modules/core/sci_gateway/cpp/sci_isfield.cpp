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

#include "core_gw.hxx"
#include "function.hxx"
#include "struct.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "mlist.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

types::Function::ReturnValue sci_isfield(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "isfield", 1);
        return types::Function::Error;
    }

    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "isfield", 2);
        return types::Function::Error;
    }

    if (in[0]->isStruct() == false &&
            in[0]->isTList()  == false &&
            in[0]->isMList()  == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: struct array or tlist or mlist expected.\n"), "isfield", 1);
        return types::Function::Error;
    }

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "isfield", 2);
        return types::Function::Error;
    }

    types::String *pInString = in[1]->getAs<types::String>();
    wchar_t** wcsStr = pInString->get();
    types::Bool *pOutBool = new types::Bool(pInString->getRows(), pInString->getCols());

    switch (in[0]->getType())
    {
        case types::GenericType::ScilabStruct :
        {
            types::Struct* pStruct = in[0]->getAs<types::Struct>();
            for (int i = 0; i < pInString->getSize(); i++)
            {
                pOutBool->set(i, pStruct->exists(std::wstring(wcsStr[i])));
            }
            break;
        }
        case types::GenericType::ScilabTList :
        case types::GenericType::ScilabMList :
        {
            types::TList* pTL = in[0]->getAs<types::TList>();
            for (int i = 0; i < pInString->getSize(); i++)
            {
                pOutBool->set(i, pTL->exists(std::wstring(wcsStr[i])));
            }
            break;
        }
    }

    out.push_back(pOutBool);
    return types::Function::OK;
}
