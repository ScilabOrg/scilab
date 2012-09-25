/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __POINTER_STACKAPI__
#define __POINTER_STACKAPI__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_pointer.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Get pointer variable
 * @param[in] _piAddress variable address
 * @param[out] _pvPtr return pointer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_getPointer(void* _pvCtx, int* _piAddress, void** _pvPtr);

/**
 * Allocate a pointer variable
 * @param[in] _iVar variable number
 * @param[out] _pvPtr return pointer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_allocPointer(void* _pvCtx, int _iVar, void** _pvPtr);

/**
 * Create pointer variable
 * @param[in] _iVar variable number
 * @param[in] _pvPtr pointer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_createPointer(void* _pvCtx, int _iVar, void* _pvPtr);

/* shortcut functions */

/**
 * Check if the variable is of pointer type
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int stackapi_isPointerType(void* _pvCtx, int* _piAddress);

#ifdef __cplusplus
}
#endif
#endif /* __POINTER_STACKAPI__ */
