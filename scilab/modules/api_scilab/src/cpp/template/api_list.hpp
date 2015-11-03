/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include "gatewaystruct.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"

extern "C"
{
#include "api_scilab.h"
}

int API_PROTO(isDefined)(scilabEnv env, scilabVar var, int index)
{
    return scilab_isUndefined(env, var, index) ? 0 : 1;
}

int API_PROTO(isUndefined)(scilabEnv env, scilabVar var, int index)
{
    types::List* l = (types::List*)var;
    types::InternalType* item = l->get(index);
    if (item->isListUndefined())
    {
        return 1;
    }

    return 0;
}

scilabVar API_PROTO(createList)(scilabEnv env)
{
    return (scilabVar)new types::List();
}

scilabVar API_PROTO(createTList)(scilabEnv env)
{
    return (scilabVar)new types::TList();
}

scilabVar API_PROTO(createMList)(scilabEnv env)
{
    return (scilabVar)new types::MList();
}

scilabVar API_PROTO(getListItem)(scilabEnv env, scilabVar var, int index)
{
    types::List* l = (types::List*)var;
    return (scilabVar)l->get(index);
}

api_return API_PROTO(setListItem)(scilabEnv env, scilabVar var, int pos, scilabVar val)
{
    types::List* l = (types::List*)var;
    l->set(pos, (types::InternalType*)val);
    return API_OK;
}

scilabVar API_PROTO(getTListField)(scilabEnv env, scilabVar var, wchar_t* field)
{
    types::TList* l = (types::TList*)var;
    return (scilabVar)l->getField(field);

}

scilabVar API_PROTO(getMListField)(scilabEnv env, scilabVar var, wchar_t* field)
{
    types::MList* l = (types::MList*)var;
    return (scilabVar)l->getField(field);
}

api_return API_PROTO(setTListField)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar val)
{
    types::TList* l = (types::TList*)var;
    l->set(field, (types::InternalType*)val);
    return API_OK;
}

api_return API_PROTO(setMListField)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar val)
{
    types::MList* l = (types::MList*)var;
    l->set(field, (types::InternalType*)val);
    return API_OK;
}

api_return API_PROTO(appendToList)(scilabEnv env, scilabVar var, scilabVar val)
{
    types::List* l = (types::List*)var;
    l->append((types::InternalType*)val);
    return API_OK;
}

