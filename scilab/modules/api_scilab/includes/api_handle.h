/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#ifndef __HANDLE_API__
#define __HANDLE_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_handle.h. Include api_scilab.h instead.
#endif
#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Get handle variable
     * @param[in] _piAddress variable address
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pllHandle return pointer to data
     * @return if the operation succeeded (0) or not ( !0 )
     */
    SciErr getMatrixOfHandle(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, long long** _pllHandle);

    /**
     * Allocate handle variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pllHandle return pointer to data
     * @return if the operation succeeded (0) or not ( !0 )
     */
    SciErr allocMatrixOfHandle(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _pllHandle);

    /**
     * Create handle variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pllHandle pointer to data
     * @return if the operation succeeded (0) or not ( !0 )
     */
    SciErr createMatrixOfHandle(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long* _pllHandle);

    /*shortcut functions*/

    /**
     * Check if the variable type is handle
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isHandleType(void* _pvCtx, int* _piAddress);

    /**
     * Get scalar handle variable data
     * @param[in] _piAddress variable address
     * @param[out] _pllHandle return handle value
     * @return if the operation succeeded (0) or not ( !0 )
     */
    int getScalarHandle(void* _pvCtx, int* _piAddress, long long* _pllHandle);

    /**
     * Create scalar handle
     * @param[in] _iVar variable number
     * @param[in] _llHandle value
     * @return if the operation succeeded (0) or not ( !0 )
     */
    int createScalarHandle(void* _pvCtx, int _iVar, long long _llHandle);

#ifdef __cplusplus
}
#endif
#endif /* !__HANDLE_API__ */