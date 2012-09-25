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

#ifndef __COMMON_STACKAPI__
#define __COMMON_STACKAPI__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_common.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * The error management structure
     *
     */

#define MESSAGE_STACK_SIZE 5

    typedef struct api_Err
    {
        int iErr; /**< The error ID */
        int iMsgCount; /**< Error level */
        char* pstMsg[MESSAGE_STACK_SIZE]; /**< The error message */
    } SciErr;

    typedef struct api_Ctx
    {
        char* pstName; /**< Function name */
    } StrCtx, *pStrCtx;

    extern StrCtx* stackapi_pvApiCtx;

#include "stackapi_scilab.h"
    /* generics functions */

/*Rhs*/
    int* stackapi_getNbInputArgument(void* _pvCtx);
#define stackapi_nbInputArgument(PVCTX) (*stackapi_getNbInputArgument(PVCTX))

#ifdef Rhs
#undef Rhs
#endif
#define Rhs (*stackapi_getNbInputArgument(stackapi_pvApiCtx))

/*Top*/
    int* stackapi_getNbArgumentOnStack(void* _pvCtx);
#define stackapi_nbArgumentOnStack(PVCTX) (*stackapi_getNbArgumentOnStack(PVCTX))

#ifdef Top
#undef Top
#endif
#define Top (*stackapi_getNbArgumentOnStack(stackapi_pvApiCtx))

/*Lhs*/
    int* stackapi_getNbOutputArgument(void* _pvCtx);
#define stackapi_nbOutputArgument(PVCTX) (*stackapi_getNbOutputArgument(PVCTX))
#ifdef Lhs
#undef Lhs
#endif
#define Lhs (*stackapi_getNbOutputArgument(stackapi_pvApiCtx))

/*PutLhs*/
    int* stackapi_assignOutputVariable(void* _pvCtx, int _iVal);
#define stackapi_AssignOutputVariable(PVCTX, x) (*stackapi_assignOutputVariable(PVCTX, x))
#ifdef LhsVar
#undef LhsVar
#endif
#define LhsVar(x) (*stackapi_assignOutputVariable(stackapi_pvApiCtx, x))

/*PutLhsVar*/
    int stackapi_returnArguments(void* _pvCtx);
#define stackapi_ReturnArguments(PVCTX) if (! stackapi_returnArguments(PVCTX)) { return 0; }
#ifdef PutLhsVar
#undef PutLhsVar
#endif
#define PutLhsVar() if (! stackapi_returnArguments(stackapi_pvApiCtx)) { return 0; }

    /**
     * Check input argument count
     * @param[in] _iMin min value
     * @param[in] _iMax : max value
     * @return if _iMin >= rhs >= _iMax
     */
    int stackapi_checkInputArgument(void* _pvCtx, int _iMin, int _iMax);
#define stackapi_CheckInputArgument(ctx, min, max) \
    if(stackapi_checkInputArgument(ctx, min, max) == 0) \
    { \
        return 0; \
    }
#ifdef CheckRhs
#undef CheckRhs
#endif
#define stackapi_CheckRhs(min, max) \
    if(stackapi_checkInputArgument(stackapi_pvApiCtx, min, max) == 0) \
    { \
        return 0; \
    }
    /**
     * Check input argument count
     * @param[in] _iMin min value
     * @return if rhs >= _iMin
     */
    int stackapi_checkInputArgumentAtLeast(void* _pvCtx, int _iMin);
#define stackapi_CheckInputArgumentAtLeast(ctx, min) \
    if(stackapi_checkInputArgumentAtLeast(ctx, min) == 0) \
    { \
        return 0; \
    }

    /**
     * Check input argument count
     * @param[in] _iMax : max value
     * @return if rhs <= max
     */
    int stackapi_checkInputArgumentAtMost(void* _pvCtx, int _iMax);
#define stackapi_CheckInputArgumentAtMost(ctx, max) \
    if(stackapi_checkInputArgumentAtMost(ctx, max) == 0) \
    { \
        return 0; \
    }

    /**
     * Check output argument count
     * @param[in] _iMin min value
     * @param[in] _iMax : max value
     * @return if _iMin >= lhs >= _iMax
     */
    int stackapi_checkOutputArgument(void* _pvCtx, int _iMin, int _iMax);
#define stackapi_CheckOutputArgument(ctx, min, max) \
    if(stackapi_checkOutputArgument(ctx, min, max) == 0) \
    { \
        return 0; \
    }
#ifdef CheckLhs
#undef CheckLhs
#endif
#define stackapi_CheckLhs(min, max) \
    if(stackapi_checkOutputArgument(stackapi_pvApiCtx, min, max) == 0) \
    { \
        return 0; \
    }

    /**
     * Check output argument count
     * @param[in] _iMin min value
     * @return if lhs >= _iMin
     */
    int stackapi_checkOutputArgumentAtLeast(void* _pvCtx, int _iMin);
#define stackapi_CheckOutputArgumentAtLeast(ctx, min) \
    if(stackapi_checkOutputArgumentAtLeast(ctx, min) == 0) \
    { \
        return 0; \
    }

    /**
     * Check output argument count
     * @param[in] _iMax : max value
     * @return if lhs <= max
     */
    int stackapi_checkOutputArgumentAtMost(void* _pvCtx, int _iMax);
#define stackapi_CheckOutputArgumentAtMost(ctx, max) \
    if(stackapi_checkOutputArgumentAtMost(ctx, max) == 0) \
    { \
        return 0; \
    }

    int stackapi_callOverloadFunction(void* _pvCtx, int _iVar, char* _pstName, unsigned int _iNameLen);
#define stackapi_CallOverloadFunction(x) stackapi_callOverloadFunction(stackapi_pvApiCtx, x, fname, strlen(fname))
#ifdef OverLoad
#undef OverLoad
#endif
#define OverLoad(x) stackapi_callOverloadFunction(stackapi_pvApiCtx, x, fname, (unsigned int)strlen(fname))
    /**
     * Get the memory address of a variable from the variable position
     * @param[in] _iVar variable number
     * @param[out] _piAddress return variable address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress);

    /**
     * Get the name of a variable from the variable position
     * @param[in] _iVar variable number
     * @param[out] _pstName variable name
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getVarNameFromPosition(void* _pvCtx, int _iVar, char* _pstName);

    /**
     * Get the variable type
     * @param[in] _piAddress variable address
     * @param[out] _piType return variable type ( sci_matrix, sci_strings, ... )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getVarType(void* _pvCtx, int* _piAddress, int* _piType);

    /**
     * Check if the variable type is complex double, complex sparse double or complex polynomial
     * @param[in] _piAddress variable address
     * @return if complex 1 otherwise 0
     */
    int stackapi_isVarComplex(void* _pvCtx, int* _piAddress);

    /**
     * Get variable dimensions
     * @param[in] _piAddress variable address
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of cols
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getVarDimension(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols);

    /**
     * Check if a variable is a matrix form ( row x col )
     * @param[in] _piAddress variable address
     * @return if matrix form type variable 1 otherwise 0
     */
    int stackapi_isVarMatrixType(void* _pvCtx, int* _piAddress);

    /**
     * Get process mode from input variable
     * @param[in] _iPos variable position in function call
     * @param[in] _piAddRef variable address
     * @param[out] _piMode return process mode ( 0 -> All, 1 -> Row, 2 -> Col )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getProcessMode(void* _pvCtx, int _iPos, int* _piAddRef, int *_piMode);

    /**
     * Get dimension for variable, extract value from a single value
     * @param[in] _piAddress variable address ( double or int variable )
     * @param[out] _piVal return value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getDimFromVar(void* _pvCtx, int* _piAddress, int* _piVal);

    /**
     * Get Rhs value from variable address
     * @param[in] _piAddress varaible address
     * @return rhs value of the variable, if failed returns 0
     */
    int stackapi_getRhsFromAddress(void* _pvCtx, int* _piAddress);

    /**
     * Check if a variable is a row vector
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isRowVector(void* _pvCtx, int* _piAddress);

    /**
     * Check if a variable is a column vector
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isColumnVector(void* _pvCtx, int* _piAddress);

    /**
     * Check if a variable is a vector ( row or column )
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isVector(void* _pvCtx, int* _piAddress);

    /**
     * Check if a variable is a scalar
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isScalar(void* _pvCtx, int* _piAddress);

    /**
     * Check if a variable is a square matrix
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isSquareMatrix(void* _pvCtx, int* _piAddress);

    /**
     * Check matrix dimension
     * @param[in] _piAddress variable address
     * @param[in] _iRows accepted number of rows ( if -1, don't check )
     * @param[in] _iCols accepted number of columns ( if -1, don't check )
     * @return 1 for true and 0 for false
     */
    int stackapi_checkVarDimension(void* _pvCtx, int* _piAddress, int _iRows, int _iCols);

    /**
     * Check if a matrix is empty
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isEmptyMatrix(void* _pvCtx, int* _piAddress);

    /**
     * Create an empty matrix
     * @param[in] _iVar variable number
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int stackapi_createEmptyMatrix(void* _pvCtx, int _iVar);

#ifdef __cplusplus
}
#endif
#endif /* __COMMON_STACKAPI__ */
