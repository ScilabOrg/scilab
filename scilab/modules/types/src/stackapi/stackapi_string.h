/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __STRING_STACKAPI__
#define __STRING_STACKAPI__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_string.h. Include api_scilab.h instead.
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************/
/*   string matrix functions   */
/*******************************/

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piLength return strings lengths
 * @param[out] _pstStrings return array of char
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr stackapi_getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
 * Get wide strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piwLength return strings length
 * @param[out] _pwstStrings return array of wide char
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr stackapi_getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings);

/**
 * Create strings variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr stackapi_createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* const* _pstStrings);

/**
* Create wide strings variable data
* @param[in] _iVar variable number
* @param[in] _iRows Number of rows
* @param[in] _iCols Number of columns
* @param[in] _pwstStrings array of strings ( null terminated )
* @return if the operation succeeded (0) or not ( !0 )
*/
SciErr stackapi_createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const wchar_t* const* _pwstStrings);

/*shortcut functions*/

/**
 * check if the variable type is string
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int stackapi_isStringType(void* _pvCtx, int* _piAddress);

/**
 * Get single string variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstData return allocated string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int stackapi_getAllocatedSingleString(void* _pvCtx, int* _piAddress, char** _pstData);

/**
 * Get single wide string variable data
 * @param[in] _piAddress variable address
 * @param[out] _pwstData return allocated wide string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int stackapi_getAllocatedSingleWideString(void* _pvCtx, int* _piAddress, wchar_t** _pwstData);

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pstData return allocated array of string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int stackapi_getAllocatedMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char*** _pstData);

/**
 * Get wide strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pwstData return allocated array of wide string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int stackapi_getAllocatedMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, wchar_t*** _pwstData);

/**
 * Create single string variable data
 * @param[in] _iVar variable number
 * @param[in] _pstStrings string ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int stackapi_createSingleString(void* _pvCtx, int _iVar, const char* _pstStrings);

/**
 * Create single wide string variable data
 * @param[in] _iVar variable number
 * @param[in] _pwstStrings wide string ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int stackapi_createSingleWideString(void* _pvCtx, int _iVar, const wchar_t* _pwstStrings);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _pstData string data
 */
void stackapi_freeAllocatedSingleString(char* _pstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _pwstData wide string data
 */
void stackapi_freeAllocatedSingleWideString(wchar_t* _pwstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pstData matrix of string data
 */
void stackapi_freeAllocatedMatrixOfString(int _iRows, int _iCols, char** _pstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pwstData matrix of wide string data
 */
void stackapi_freeAllocatedMatrixOfWideString(int _iRows, int _iCols, wchar_t** _pwstData);

#ifdef __cplusplus
}
#endif

#endif /* __STRING_STACKAPI__ */
