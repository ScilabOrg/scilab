/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef __STRUCT_API__
#define __STRUCT_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_struct.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createStruct         API_PROTO(createStruct)
#define scilab_createStruct2d       API_PROTO(createStruct2d)
#define scilab_addFields            API_PROTO(addFields)
#define scilab_addField             API_PROTO(addField)
#define scilab_getFields            API_PROTO(getFields)
#define scilab_getStructArray       API_PROTO(getStructArray)
#define scilab_setStructArray       API_PROTO(setStructArray)
#define scilab_getStructData        API_PROTO(getStructData)
#define scilab_getStructData2d      API_PROTO(getStructData2d)
#define scilab_setStructData        API_PROTO(setStructData)
#define scilab_setStructData2d      API_PROTO(setStructData2d)

scilabVar API_PROTO(createStruct)(scilabEnv env, int dim, int* dims);
scilabVar API_PROTO(createStruct2d)(scilabEnv env, int row, int col);

/*fields*/
api_return API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, wchar_t** fields);
api_return API_PROTO(addField)(scilabEnv env, scilabVar var, wchar_t* field);
api_return API_PROTO(getFields)(scilabEnv env, scilabVar var, wchar_t*** fields);

/*array*/
api_return API_PROTO(getStructArray)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar** vals);
api_return API_PROTO(setStructArray)(scilabEnv env, scilabVar var, wchar_t* field, scilabVar* vals);

/*data*/
scilabVar API_PROTO(getStructData)(scilabEnv env, scilabVar var, wchar_t* field, int* dims);
scilabVar API_PROTO(getStructData2d)(scilabEnv env, scilabVar var, wchar_t* field, int row, int col);

api_return API_PROTO(setStructData)(scilabEnv env, scilabVar var, wchar_t* field, int* dims, scilabVar data);
api_return API_PROTO(setStructData2d)(scilabEnv env, scilabVar var, wchar_t* field, int row, int col, scilabVar data);

#ifdef __cplusplus
}
#endif
#endif /* __STRUCT_API__ */
