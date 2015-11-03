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
#include "int.hxx"

extern "C"
{
#include "api_scilab.h"
}

int API_PROTO(getIntegerPrecision)(scilabEnv env, scilabVar var)
{
    switch (((types::InternalType*)var)->getType())
    {
        case types::InternalType::ScilabInt8:
            return SCI_INT8;
        case types::InternalType::ScilabUInt8:
            return SCI_UINT8;
        case types::InternalType::ScilabInt16:
            return SCI_INT16;
        case types::InternalType::ScilabUInt16:
            return SCI_UINT16;
        case types::InternalType::ScilabInt32:
            return SCI_INT32;
        case types::InternalType::ScilabUInt32:
            return SCI_UINT32;
        case types::InternalType::ScilabInt64:
            return SCI_INT64;
        case types::InternalType::ScilabUInt64:
            return SCI_UINT64;
    }

    return 0;
}

int API_PROTO(isInt8)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt8() ? 1 : 0;
}

int API_PROTO(isInt16)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt16() ? 1 : 0;
}

int API_PROTO(isInt32)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt32() ? 1 : 0;
}

int API_PROTO(isInt64)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt64() ? 1 : 0;
}

int API_PROTO(isUnsignedInt8)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isUInt8() ? 1 : 0;
}

int API_PROTO(isUnsignedInt16)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt16() ? 1 : 0;
}

int API_PROTO(isUnsignedInt32)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isUInt32() ? 1 : 0;
}

int API_PROTO(isUnsignedInt64)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isUInt64() ? 1 : 0;
}

scilabVar API_PROTO(createInteger)(scilabEnv env, int prec, int dim, int* dims)
{
    switch (prec)
    {
        case SCI_INT8:
            return scilab_createInteger8(env, dim, dims);
        case SCI_INT16:
            return scilab_createInteger16(env, dim, dims);
        case SCI_INT32:
            return scilab_createInteger32(env, dim, dims);
        case SCI_INT64:
            return scilab_createInteger64(env, dim, dims);
        case SCI_UINT8:
            return scilab_createUnsignedInteger8(env, dim, dims);
        case SCI_UINT16:
            return scilab_createUnsignedInteger16(env, dim, dims);
        case SCI_UINT32:
            return scilab_createUnsignedInteger32(env, dim, dims);
        case SCI_UINT64:
            return scilab_createUnsignedInteger64(env, dim, dims);
    }

    return NULL;
}

scilabVar API_PROTO(createInteger8)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::Int8(dim, dims);
}

scilabVar API_PROTO(createInteger16)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::Int16(dim, dims);
}

scilabVar API_PROTO(createInteger32)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::Int32(dim, dims);
}

scilabVar API_PROTO(createInteger64)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::Int64(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger8)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::UInt8(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger16)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::UInt16(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger32)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::UInt32(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger64)(scilabEnv env, int dim, int* dims)
{
    return (scilabVar)new types::UInt64(dim, dims);
}

scilabVar API_PROTO(createInteger2d)(scilabEnv env, int prec, int row, int col)
{
    int dims[2] = {row, col};
    return scilab_createInteger(env, prec, 2, dims);
}

scilabVar API_PROTO(createInteger2d8)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_INT8, row, col);
}

scilabVar API_PROTO(createInteger2d16)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_INT16, row, col);
}

scilabVar API_PROTO(createInteger2d32)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_INT32, row, col);
}

scilabVar API_PROTO(createInteger2d64)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_INT64, row, col);
}

scilabVar API_PROTO(createUnsignedInteger2d8)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_UINT8, row, col);
}

scilabVar API_PROTO(createUnsignedInteger2d16)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_UINT16, row, col);
}

scilabVar API_PROTO(createUnsignedInteger2d32)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_UINT32, row, col);
}

scilabVar API_PROTO(createUnsignedInteger2d64)(scilabEnv env, int row, int col)
{
    return scilab_createInteger2d(env, SCI_UINT64, row, col);
}

api_return API_PROTO(getIntegerArray)(scilabEnv env, scilabVar var, void** vals)
{
    switch (((types::InternalType*)var)->getType())
    {
        case types::InternalType::ScilabInt8:
            return scilab_getInteger8Array(env, var, (char**)vals);
        case types::InternalType::ScilabInt16:
            return scilab_getInteger16Array(env, var, (short**)vals);
        case types::InternalType::ScilabInt32:
            return scilab_getInteger32Array(env, var, (int**)vals);
        case types::InternalType::ScilabInt64:
            return scilab_getInteger64Array(env, var, (long long**)vals);
        case types::InternalType::ScilabUInt8:
            return scilab_getUnsignedInteger8Array(env, var, (unsigned char**)vals);
        case types::InternalType::ScilabUInt16:
            return scilab_getUnsignedInteger16Array(env, var, (unsigned short**)vals);
        case types::InternalType::ScilabUInt32:
            return scilab_getUnsignedInteger32Array(env, var, (unsigned int**)vals);
        case types::InternalType::ScilabUInt64:
            return scilab_getUnsignedInteger64Array(env, var, (unsigned long long**)vals);
    }

    return API_ERROR;
}

api_return API_PROTO(getInteger8Array)(scilabEnv env, scilabVar var, char** vals)
{
    types::Int8* i = (types::Int8*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getInteger16Array)(scilabEnv env, scilabVar var, short** vals)
{
    types::Int16* i = (types::Int16*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getInteger32Array)(scilabEnv env, scilabVar var, int** vals)
{
    types::Int32* i = (types::Int32*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getInteger64Array)(scilabEnv env, scilabVar var, long long** vals)
{
    types::Int64* i = (types::Int64*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getUnsignedInteger8Array)(scilabEnv env, scilabVar var, unsigned char** vals)
{
    types::UInt8* i = (types::UInt8*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getUnsignedInteger16Array)(scilabEnv env, scilabVar var, unsigned short** vals)
{
    types::UInt16* i = (types::UInt16*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getUnsignedInteger32Array)(scilabEnv env, scilabVar var, unsigned int** vals)
{
    types::UInt32* i = (types::UInt32*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(getUnsignedInteger64Array)(scilabEnv env, scilabVar var, unsigned long long** vals)
{
    types::UInt64* i = (types::UInt64*)var;
    *vals = i->get();
    return API_OK;
}

api_return API_PROTO(setIntegerArray)(scilabEnv env, scilabVar var, void* vals)
{
    switch (((types::InternalType*)var)->getType())
    {
        case types::InternalType::ScilabInt8:
            return scilab_getInteger8Array(env, var, (char**)vals);
        case types::InternalType::ScilabInt16:
            return scilab_getInteger16Array(env, var, (short**)vals);
        case types::InternalType::ScilabInt32:
            return scilab_getInteger32Array(env, var, (int**)vals);
        case types::InternalType::ScilabInt64:
            return scilab_getInteger64Array(env, var, (long long**)vals);
        case types::InternalType::ScilabUInt8:
            return scilab_getUnsignedInteger8Array(env, var, (unsigned char**)vals);
        case types::InternalType::ScilabUInt16:
            return scilab_getUnsignedInteger16Array(env, var, (unsigned short**)vals);
        case types::InternalType::ScilabUInt32:
            return scilab_getUnsignedInteger32Array(env, var, (unsigned int**)vals);
        case types::InternalType::ScilabUInt64:
            return scilab_getUnsignedInteger64Array(env, var, (unsigned long long**)vals);
    }

    return API_ERROR;
}

api_return API_PROTO(setInteger8Array)(scilabEnv env, scilabVar var, char* vals)
{
    types::Int8* i = (types::Int8*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setInteger16Array)(scilabEnv env, scilabVar var, short* vals)
{
    types::Int16* i = (types::Int16*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setInteger32Array)(scilabEnv env, scilabVar var, int* vals)
{
    types::Int32* i = (types::Int32*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setInteger64Array)(scilabEnv env, scilabVar var, long long* vals)
{
    types::Int64* i = (types::Int64*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setUnsignedInteger8Array)(scilabEnv env, scilabVar var, unsigned char* vals)
{
    types::UInt8* i = (types::UInt8*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setUnsignedInteger16Array)(scilabEnv env, scilabVar var, unsigned short* vals)
{
    types::UInt16* i = (types::UInt16*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setUnsignedInteger32Array)(scilabEnv env, scilabVar var, unsigned int* vals)
{
    types::UInt32* i = (types::UInt32*)var;
    i->set(vals);
    return API_OK;
}

api_return API_PROTO(setUnsignedInteger64Array)(scilabEnv env, scilabVar var, unsigned long long* vals)
{
    types::UInt64* i = (types::UInt64*)var;
    i->set(vals);
    return API_OK;
}
