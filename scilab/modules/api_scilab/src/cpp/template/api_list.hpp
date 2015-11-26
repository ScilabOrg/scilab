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
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"isUndefined", _W("var must be a list variable"));
        return STATUS_ERROR;
    }
#endif
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
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"getListItem", _W("var must be a list variable"));
        return nullptr;
    }
#endif

    if (index < 0 || index >= l->getSize())
    {
#ifdef __API_SCILAB_SAFE__
        scilab_setInternalError(env, L"getListItem", _W("index out of bounds"));
#endif
        return nullptr;
    }

    return (scilabVar)l->get(index);
}

scilabStatus API_PROTO(setListItem)(scilabEnv env, scilabVar var, int index, const scilabVar val)
{
    types::List* l = (types::List*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"setListItem", _W("var must be a list variable"));
        return STATUS_ERROR;
    }
#endif
 
    bool ret = l->set(index, (types::InternalType*)val);
    return ret ? STATUS_OK : STATUS_ERROR;
}

int API_PROTO(getTListFieldNames)(scilabEnv env, scilabVar var, const wchar_t*** fieldnames)
{
    types::TList* l = (types::TList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"getTListFieldNames", _W("var must be a tlist variable"));
        return nullptr;
    }
#endif

    types::String* names = l->getFieldNames();
    *fieldnames = names->get();
    return names->getSize();
}

scilabVar API_PROTO(getTListField)(scilabEnv env, scilabVar var, const wchar_t* field)
{
    types::TList* l = (types::TList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"getTListField", _W("var must be a tlist variable"));
        return nullptr;
    }
#endif

    return (scilabVar)l->getField(field);

}

scilabStatus API_PROTO(setTListField)(scilabEnv env, scilabVar var, const wchar_t* field, const scilabVar val)
{
    types::TList* l = (types::TList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"setTListField", _W("var must be a tlist variable"));
        return STATUS_ERROR;
    }
#endif

    bool ret = l->set(field, (types::InternalType*)val);
    return ret ? STATUS_OK : STATUS_ERROR;
}

int API_PROTO(getMListFieldNames)(scilabEnv env, scilabVar var, const wchar_t*** fieldnames)
{
    types::MList* l = (types::MList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"getMListFieldNames", _W("var must be a tlist variable"));
        return nullptr;
    }
#endif

    types::String* names = l->getFieldNames();
    *fieldnames = names->get();
    return names->getSize();
}

scilabVar API_PROTO(getMListField)(scilabEnv env, scilabVar var, const wchar_t* field)
{
    types::MList* l = (types::MList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isMList() == false)
    {
        scilab_setInternalError(env, L"getMListField", _W("var must be a mlist variable"));
        return nullptr;
    }
#endif
    return (scilabVar)l->getField(field);
}

scilabStatus API_PROTO(setMListField)(scilabEnv env, scilabVar var, const wchar_t* field, const scilabVar val)
{
    types::MList* l = (types::MList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isMList() == false)
    {
        scilab_setInternalError(env, L"setMListField", _W("var must be a mlist variable"));
        return STATUS_ERROR;
    }
#endif

    bool ret = l->set(field, (types::InternalType*)val);
    return ret ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(appendToList)(scilabEnv env, scilabVar var, const scilabVar val)
{
    types::List* l = (types::List*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"appendToList", _W("var must be a list variable"));
        return STATUS_ERROR;
    }
#endif
    l->append((types::InternalType*)val);
    return STATUS_OK;
}

