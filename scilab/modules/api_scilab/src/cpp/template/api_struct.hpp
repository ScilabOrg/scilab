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
#include "struct.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createStructMatrix)(scilabEnv env, int dim, int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createStructMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createStructMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Struct* s = new types::Struct(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStructMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createStructMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createStructMatrix2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Struct* s = new types::Struct(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStructMatrix2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

/*fields*/
api_return API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, wchar_t** fields)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"addFields", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif

    for (int i = 0; i < count; ++i)
    {
        s->addField(fields[i]);
    }

    return API_OK;
}

api_return API_PROTO(addField)(scilabEnv env, scilabVar var, wchar_t* field)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"addField", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif
    s->addField(field);
    return API_OK;
}

int API_PROTO(getFields)(scilabEnv env, scilabVar var, wchar_t*** fields)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getFields", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif
    types::String* str = s->getFieldNames();
    *fields = str->get();
    return str->getSize();
}

/*array*/
api_return API_PROTO(getStructArray)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar** vals)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getStructArray", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif
    types::SingleStruct** ss = s->get();
    int size = s->getSize();
    *vals = new scilabVar[size];
    for (int i = 0; i < size; ++i)
    {
        (*vals)[i] = (scilabVar)ss[i]->get(field);
    }

    return API_OK;
}

api_return API_PROTO(setStructArray)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar* vals)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"setStructArray", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif
    types::SingleStruct** ss = s->get();
    int size = s->getSize();
    for (int i = 0; i < size; ++i)
    {
        ss[i]->set(field, (types::InternalType*)vals[i]);
    }

    return API_OK;
}

/*data*/
scilabVar API_PROTO(getStructData)(scilabEnv env, scilabVar var, wchar_t* field, int* dims)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getStructData", _W("var must be a struct variable"));
        return nullptr;
    }
#endif

    types::SingleStruct* ss = s->get(s->getIndex(dims));
    return (scilabVar)ss->get(field);
}
scilabVar API_PROTO(getStructData2d)(scilabEnv env, scilabVar var, wchar_t* field, int row, int col)
{
    int dims[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getStructData2d", _W("var must be a struct variable"));
        return nullptr;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    return (scilabVar)ss->get(field);
}

api_return API_PROTO(setStructData)(scilabEnv env, scilabVar var, wchar_t* field, int* dims, scilabVar data)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"setStructData", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    ss->set(field, (types::InternalType*)data);
    return API_OK;
}

api_return API_PROTO(setStructData2d)(scilabEnv env, scilabVar var, wchar_t* field, int row, int col, scilabVar data)
{
    int dims[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"setStructData2d", _W("var must be a struct variable"));
        return API_ERROR;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    ss->set(field, (types::InternalType*)data);
    return API_OK;
}

