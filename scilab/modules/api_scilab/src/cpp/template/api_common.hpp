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
#include "double.hxx"
#include "context.hxx"
#include "list.hxx"

extern "C"
{
#include "api_scilab.h"
}

/* Scilab 6 API*/

//var
scilabVar API_PROTO(getVar)(const wchar_t* name)
{
    return (scilabVar)symbol::Context::getInstance()->get(symbol::Symbol(name));
}

scilabVar API_PROTO(getClone)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    return (scilabVar)it->clone();
}

//type
int API_PROTO(getType)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }

    switch (it->getType())
    {
        case types::InternalType::ScilabDouble:
            return sci_matrix;
            break;
        case types::InternalType::ScilabPolynom:
            return sci_poly;
            break;
        case types::InternalType::ScilabBool:
            return sci_boolean;
            break;
        case types::InternalType::ScilabSparse:
            return sci_sparse;
            break;
        case types::InternalType::ScilabSparseBool:
            return sci_boolean_sparse;
            break;
            //case types::InternalType::RealMatlabSparse :
            //    return sci_matlab_sparse;
            //    break;
        case types::InternalType::ScilabInt8:
        case types::InternalType::ScilabUInt8:
        case types::InternalType::ScilabInt16:
        case types::InternalType::ScilabUInt16:
        case types::InternalType::ScilabInt32:
        case types::InternalType::ScilabUInt32:
        case types::InternalType::ScilabInt64:
        case types::InternalType::ScilabUInt64:
            return sci_ints;
            break;
        case types::InternalType::ScilabHandle:
            return sci_handles;
            break;
        case types::InternalType::ScilabString:
            return sci_strings;
            break;
        case types::InternalType::ScilabMacroFile:
            return sci_u_function;
            break;
        case types::InternalType::ScilabMacro:
            return sci_c_function;
            break;
        case types::InternalType::ScilabList:
            return sci_list;
            break;
        case types::InternalType::ScilabCell:
            return sci_cell;
            break;
        case types::InternalType::ScilabTList:
            return sci_tlist;
            break;
        case types::InternalType::ScilabMList:
            return sci_mlist;
            break;
        case types::InternalType::ScilabStruct:
            // Scilab < 6 compatibility... Struct have type 17;
            return sci_struct;
            break;
        case types::InternalType::ScilabUserType:
            return sci_pointer;
            break;
        case types::InternalType::ScilabColon:
        case types::InternalType::ScilabImplicitList:
            return sci_implicit_poly;
            break;
        case types::InternalType::ScilabFunction:
            return sci_intrinsic_function;
            break;
        case types::InternalType::ScilabLibrary:
            return sci_lib;
            break;
        default:
            return 0;
    }

}

int API_PROTO(isDouble)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isDouble() ? 1 : 0;
}

int API_PROTO(isString)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isString() ? 1 : 0;
}

int API_PROTO(isBoolean)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isBool() ? 1 : 0;
}

int API_PROTO(isCell)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isCell() ? 1 : 0;
}

int API_PROTO(isInt)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isInt() ? 1 : 0;
}

int API_PROTO(isPoly)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isPoly() ? 1 : 0;
}

int API_PROTO(isList)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isList() ? 1 : 0;
}

int API_PROTO(isTList)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isTList() ? 1 : 0;
}

int API_PROTO(isMList)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isMList() ? 1 : 0;
}

int API_PROTO(isHandle)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    return it->isHandle() ? 1 : 0;
}

//complex
int API_PROTO(isComplex)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }
    
    if (it->isGenericType())
    {
        types::GenericType* gt = static_cast<types::GenericType*>(it);
        return gt->isComplex() ? 1 : 0;
    }

    return 0;
}

//dimension
int API_PROTO(getDim)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }

    if (it->isArrayOf())
    {
        types::GenericType* gt = static_cast<types::GenericType*>(it);
        return gt->getDims();
    }

    int type = it->getType();
    switch (type)
    {
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            return 1;
        }
    }

    return 0;
}

int API_PROTO(getDim2d)(scilabEnv env, scilabVar var, int* row, int* col)
{
    *row = 0;
    *col = 0;
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }

    if (it->isArrayOf())
    {
        types::GenericType* gt = static_cast<types::GenericType*>(it);
        if (gt->getDims() > 2)
        {
            return API_ERROR;
        }

        *row = gt->getRows();
        *col = gt->getCols();
        return gt->getSize();
    }

    return API_ERROR;
}

int API_PROTO(getDimArray)(scilabEnv env, scilabVar var, const int** dims)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }

    if (it->isArrayOf())
    {
        types::GenericType* gt = static_cast<types::GenericType*>(it);
        *dims = gt->getDimsArray();
        return gt->getSize();
    }


    int type = it->getType();
    switch (type)
    {
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            types::List* l = it->getAs<types::List>();
            return l->getSize();
        }
    }

    return 0;
}

int API_PROTO(getSize)(scilabEnv env, scilabVar var)
{
    types::InternalType* it = (types::InternalType*)var;
    if (it == nullptr)
    {
        return 0;
    }

    if (it->isArrayOf())
    {
        types::GenericType* gt = static_cast<types::GenericType*>(it);
        return gt->getSize();
    }

    int type = it->getType();
    switch (type)
    {
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            types::List* l = it->getAs<types::List>();
            return l->getSize();
        }
    }

    return 0;
}

int API_PROTO(isScalar)(scilabEnv env, scilabVar var)
{
    return scilab_getSize(env, var) == 1 ? 1 : 0;
}

int API_PROTO(isVector)(scilabEnv env, scilabVar var)
{
    const int* dims = nullptr;
    int dim = scilab_getDimArray(env, var, &dims);

    for (int i = 0; i < dim; ++i)
    {
        int ret = scilab_isNVector(env, var, i);
        if (ret)
        {
            return 1;
        }
    }

    return 0;
}

int API_PROTO(isMatrix)(scilabEnv env, scilabVar var)
{
    return scilab_getDim(env, var) == 2 ? 1 : 0;
}

int API_PROTO(isNVector)(scilabEnv env, scilabVar var, int n)
{
    const int* dims = nullptr;
    int dim = scilab_getDimArray(env, var, &dims);

    for (int i = 0; i < dim; ++i)
    {
        if (i == n)
        {
            continue;
        }

        if (dims[i] != 1)
        {
            return 0;
        }
    }

    return 1;
}

int API_PROTO(isRowVector)(scilabEnv env, scilabVar var)
{
    return scilab_isNVector(env, var, 0);
}

int API_PROTO(isColVector)(scilabEnv env, scilabVar var)
{
    return scilab_isNVector(env, var, 1);
}

int API_PROTO(isHypermat)(scilabEnv env, scilabVar var)
{
    return scilab_getDim(env, var) > 2 ? 1 : 0;
}
