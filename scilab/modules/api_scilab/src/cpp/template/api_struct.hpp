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

scilabVar API_PROTO(createStruct)(scilabEnv env, int dim, int* dims)
{
    types::Struct* s = new types::Struct(dim, dims);
    return (scilabVar)s;
}

scilabVar API_PROTO(createStruct2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
    types::Struct* s = new types::Struct(2, dims);
    return (scilabVar)s;
}

/*fields*/
int API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, wchar_t** fields)
{
    types::Struct* s = (types::Struct*)var;

    for (int i = 0; i < count; ++i)
    {
        s->addField(fields[i]);
    }

    return 0;
}

int API_PROTO(addField)(scilabEnv env, scilabVar var, wchar_t* field)
{
    types::Struct* s = (types::Struct*)var;
    s->addField(field);
    return 0;
}

int API_PROTO(getFields)(scilabEnv env, scilabVar var, wchar_t*** fields)
{
    types::Struct* s = (types::Struct*)var;
    types::String* str = s->getFieldNames();
    *fields = str->get();
    return str->getSize();
}

/*array*/
int API_PROTO(getStructArray)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar** vals)
{
    types::Struct* s = (types::Struct*)var;
    types::SingleStruct** ss = s->get();
    int size = s->getSize();
    *vals = new scilabVar[size];
    for (int i = 0; i < size; ++i)
    {
        (*vals)[i] = (scilabVar)ss[i]->get(field);
    }

    return 0;
}

int API_PROTO(setStructArray)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar* vals)
{
    types::Struct* s = (types::Struct*)var;
    types::SingleStruct** ss = s->get();
    int size = s->getSize();
    for (int i = 0; i < size; ++i)
    {
        ss[i]->set(field, (types::InternalType*)vals[i]);
    }

    return 0;
}

/*data*/
scilabVar API_PROTO(getStructData)(scilabEnv env, scilabVar var, wchar_t* field, int* dims)
{
    types::Struct* s = (types::Struct*)var;
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    return (scilabVar)ss->get(field);
}
scilabVar API_PROTO(getStructData2d)(scilabEnv env, scilabVar var, wchar_t* field, int row, int col)
{
    int dims[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    return (scilabVar)ss->get(field);
}

int API_PROTO(setStructData)(scilabEnv env, scilabVar var, wchar_t* field, int* dims, scilabVar data)
{
    types::Struct* s = (types::Struct*)var;
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    ss->set(field, (types::InternalType*)data);
    return 0;
}

int API_PROTO(setStructData2d)(scilabEnv env, scilabVar var, wchar_t* field, int row, int col, scilabVar data)
{
    int dims[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
    types::SingleStruct* ss = s->get(s->getIndex(dims));
    ss->set(field, (types::InternalType*)data);
    return 0;
}

