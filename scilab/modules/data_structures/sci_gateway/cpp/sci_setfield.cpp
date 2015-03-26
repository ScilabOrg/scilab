/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine Elias
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "data_structures_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "struct.hxx"
#include "user.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

static types::Function::ReturnValue sci_setfieldStruct(types::typed_list &in, int _iRetCount, types::typed_list &out);
static types::Function::ReturnValue sci_setfieldUserType(types::typed_list &in, int _iRetCount, types::typed_list &out);

/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_setfield(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "setfield", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "setfield", 0);
        return types::Function::Error;
    }


    //special case for struct
    if (in[2]->isStruct())
    {
        return sci_setfieldStruct(in, _iRetCount, out);
    }

    //special case for UserType
    if (in[2]->isUserType())
    {
        return sci_setfieldUserType(in, _iRetCount, out);
    }

    types::InternalType* pIndex = in[0];
    types::InternalType* pData = in[1];
    types::List* pL = in[2]->getAs<types::List>();

    if (pL->isList() == false && pL->isMList() == false && pL->isTList() == false)
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: List expected.\n"), "setfield", 3);
        return types::Function::Error;
    }

    if (pIndex->isString())
    {
        //insertion by fieldname
        types::String* pS = pIndex->getAs<types::String>();
        if ((pL->isMList() == false && pL->isTList() == false) || pS->getSize() != 1)
        {
            Scierror(999, _("%s: Not implemented in scilab...\n"), "setfield");
            return types::Function::Error;
        }

        types::TList* pT = pL->getAs<types::TList>();

        std::wstring stField = pS->get(0);
        if (pT->set(stField, pData) == false)
        {
            Scierror(999, _("%s: Invalid index.\n"), "setfield");
            return types::Function::Error;
        }
    }
    else
    {
        //insertion by index
        types::typed_list Args;
        Args.push_back(pIndex);
        pL->insert(&Args, pData);
    }

    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/

static types::Function::ReturnValue sci_setfieldStruct(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pIndex = in[0];
    types::InternalType* pData = in[1];
    types::Struct* pSt = in[2]->getAs<types::Struct>();

    std::vector<types::InternalType*> vectResult;

    if (pIndex->isString())
    {
        types::String* pFields = pIndex->getAs<types::String>();
        std::vector<std::wstring> wstFields;

        types::SingleStruct* pSStr = NULL;
        for (int j = 0; j < pSt->getSize(); ++j)
        {
            pSStr = pSt->get(j);

            for (int i = 0; i < pFields->getSize(); ++i)
            {
                pSStr->set(pFields->get(i), pData);
            }
        }

    }
    else if (pIndex->isDouble())
    {
        types::Double* pDlIndex = pIndex->getAs<types::Double>();

        types::SingleStruct* pSStr = NULL;
        types::String* pStrFNames = pSt->getFieldNames();
        for (int j = 0; j < pSt->getSize(); ++j)
        {
            pSStr = pSt->get(j);

            for (int i = 0; i < pDlIndex->getSize(); ++i)
            {
                if (pDlIndex->get(i) < 1)
                {
                    Scierror(999, _("%s: Invalid index.\n"), "setfield");
                    return types::Function::Error;
                }
                else if (pDlIndex->get(i) <= 2)
                {
                    Scierror(999, _("%s: Invalid index.\n"), "setfield");
                }
                else if (pDlIndex->get(i) > (pStrFNames->getSize() + 2))
                {
                    Scierror(999, _("%s: Invalid index.\n"), "setfield");
                    return types::Function::Error;
                }
                else
                {
                    pSStr->set(pStrFNames->get(pDlIndex->get(i) - 2 - 1), pData); //-2 for the header and size
                }
            }
        }

    }
    else
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: Integer expected.\n"), "setfield", 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/

static types::Function::ReturnValue sci_setfieldUserType(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::UserType* pUT = in[2]->getAs<types::UserType>();

    if (in[0]->isDouble())
    {
        types::Double* pIndex = in[0]->getAs<types::Double>();

        types::typed_list input;
        input.push_back(in[0]);
        pUT->insert(&input, in[1]);
    }
    else
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: Integer expected.\n"), "getfield", 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/
