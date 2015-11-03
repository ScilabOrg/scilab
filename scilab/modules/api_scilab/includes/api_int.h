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

#ifndef __INT_API__
#define __INT_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_int.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define scilab_getIntegerPrecision          API_PROTO(getIntegerPrecision)
#define scilab_isInt8                       API_PROTO(isInt8)
#define scilab_isInt16                      API_PROTO(isInt16)
#define scilab_isInt32                      API_PROTO(isInt32)
#define scilab_isInt64                      API_PROTO(isInt64)
#define scilab_isUnsignedInt8               API_PROTO(isUnsignedInt8)
#define scilab_isUnsignedInt16              API_PROTO(isUnsignedInt16)
#define scilab_isUnsignedInt32              API_PROTO(isUnsignedInt32)
#define scilab_isUnsignedInt64              API_PROTO(isUnsignedInt64)

#define scilab_createInteger                API_PROTO(createInteger)
#define scilab_createInteger8               API_PROTO(createInteger8)
#define scilab_createInteger16              API_PROTO(createInteger16)
#define scilab_createInteger32              API_PROTO(createInteger32)
#define scilab_createInteger64              API_PROTO(createInteger64)
#define scilab_createUnsignedInteger8       API_PROTO(createUnsignedInteger8)
#define scilab_createUnsignedInteger16      API_PROTO(createUnsignedInteger16)
#define scilab_createUnsignedInteger32      API_PROTO(createUnsignedInteger32)
#define scilab_createUnsignedInteger64      API_PROTO(createUnsignedInteger64)

#define scilab_createInteger2d              API_PROTO(createInteger2d)
#define scilab_createInteger2d8             API_PROTO(createInteger2d8)
#define scilab_createInteger2d16            API_PROTO(createInteger2d16)
#define scilab_createInteger2d32            API_PROTO(createInteger2d32)
#define scilab_createInteger2d64            API_PROTO(createInteger2d64)
#define scilab_createUnsignedInteger2d8     API_PROTO(createUnsignedInteger2d8)
#define scilab_createUnsignedInteger2d16    API_PROTO(createUnsignedInteger2d16)
#define scilab_createUnsignedInteger2d32    API_PROTO(createUnsignedInteger2d32)
#define scilab_createUnsignedInteger2d64    API_PROTO(createUnsignedInteger2d64)

#define scilab_getIntegerArray              API_PROTO(getIntegerArray)
#define scilab_getInteger8Array             API_PROTO(getInteger8Array)
#define scilab_getInteger16Array            API_PROTO(getInteger16Array)
#define scilab_getInteger32Array            API_PROTO(getInteger32Array)
#define scilab_getInteger64Array            API_PROTO(getInteger64Array)
#define scilab_getUnsignedInteger8Array     API_PROTO(getUnsignedInteger8Array)
#define scilab_getUnsignedInteger16Array    API_PROTO(getUnsignedInteger16Array)
#define scilab_getUnsignedInteger32Array    API_PROTO(getUnsignedInteger32Array)
#define scilab_getUnsignedInteger64Array    API_PROTO(getUnsignedInteger64Array)

#define scilab_setIntegerArray              API_PROTO(setIntegerArray)
#define scilab_setInteger8Array             API_PROTO(setInteger8Array)
#define scilab_setInteger16Array            API_PROTO(setInteger16Array)
#define scilab_setInteger32Array            API_PROTO(setInteger32Array)
#define scilab_setInteger64Array            API_PROTO(setInteger64Array)
#define scilab_setUnsignedInteger8Array     API_PROTO(setUnsignedInteger8Array)
#define scilab_setUnsignedInteger16Array    API_PROTO(setUnsignedInteger16Array)
#define scilab_setUnsignedInteger32Array    API_PROTO(setUnsignedInteger32Array)
#define scilab_setUnsignedInteger64Array    API_PROTO(setUnsignedInteger64Array)


/* Scilab 6 API*/
int API_PROTO(getIntegerPrecision)(scilabEnv env, scilabVar var);

int API_PROTO(isInt8)(scilabEnv env, scilabVar var);
int API_PROTO(isInt16)(scilabEnv env, scilabVar var);
int API_PROTO(isInt32)(scilabEnv env, scilabVar var);
int API_PROTO(isInt64)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt8)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt16)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt32)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt64)(scilabEnv env, scilabVar var);

scilabVar API_PROTO(createInteger)(scilabEnv env, int prec, int dim, int* dims);
scilabVar API_PROTO(createInteger8)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createInteger16)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createInteger32)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createInteger64)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createUnsignedInteger8)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createUnsignedInteger16)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createUnsignedInteger32)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createUnsignedInteger64)(scilabEnv env, int dim, int* dims);

scilabVar API_PROTO(createInteger2d)(scilabEnv env, int prec, int row, int col);
scilabVar API_PROTO(createInteger2d8)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createInteger2d16)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createInteger2d32)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createInteger2d64)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger2d8)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger2d16)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger2d32)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger2d64)(scilabEnv env, int row, int col);

api_return API_PROTO(getIntegerArray)(scilabEnv env, scilabVar var, void** vals);
api_return API_PROTO(getInteger8Array)(scilabEnv env, scilabVar var, char** vals);
api_return API_PROTO(getInteger16Array)(scilabEnv env, scilabVar var, short** vals);
api_return API_PROTO(getInteger32Array)(scilabEnv env, scilabVar var, int** vals);
api_return API_PROTO(getInteger64Array)(scilabEnv env, scilabVar var, long long** vals);
api_return API_PROTO(getUnsignedInteger8Array)(scilabEnv env, scilabVar var, unsigned char** vals);
api_return API_PROTO(getUnsignedInteger16Array)(scilabEnv env, scilabVar var, unsigned short** vals);
api_return API_PROTO(getUnsignedInteger32Array)(scilabEnv env, scilabVar var, unsigned int** vals);
api_return API_PROTO(getUnsignedInteger64Array)(scilabEnv env, scilabVar var, unsigned long long** vals);

api_return API_PROTO(setIntegerArray)(scilabEnv env, scilabVar var, void* vals);
api_return API_PROTO(setInteger8Array)(scilabEnv env, scilabVar var, char* vals);
api_return API_PROTO(setInteger16Array)(scilabEnv env, scilabVar var, short* vals);
api_return API_PROTO(setInteger32Array)(scilabEnv env, scilabVar var, int* vals);
api_return API_PROTO(setInteger64Array)(scilabEnv env, scilabVar var, long long* vals);
api_return API_PROTO(setUnsignedInteger8Array)(scilabEnv env, scilabVar var, unsigned char* vals);
api_return API_PROTO(setUnsignedInteger16Array)(scilabEnv env, scilabVar var, unsigned short* vals);
api_return API_PROTO(setUnsignedInteger32Array)(scilabEnv env, scilabVar var, unsigned int* vals);
api_return API_PROTO(setUnsignedInteger64Array)(scilabEnv env, scilabVar var, unsigned long long* vals);

#ifdef __cplusplus
}
#endif
#endif /* __INT_API__ */
