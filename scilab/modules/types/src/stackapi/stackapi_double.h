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

#ifndef __DOUBLE_STACKAPI__
#define __DOUBLE_STACKAPI__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_double.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "doublecomplex.h"

/*******************************/
/*   double matrix functions   */
/*******************************/
/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pdblReal return pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get complex double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pdblReal return pointer to real data
 * @param[out] _pdblImg return pointer to imaginary data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblZ return pointer to Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ);

/**
 * Allocate a double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pdblReal return pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal);

/**
 * Allocate a complex double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pdblReal return pointer to real data
 * @param[out] _pdblImg return pointer to imaginary data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

/**
 * Create a double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pdblReal pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal);

/**
 * Create a complex double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pdblReal pointer to real data
 * @param[in] _pdblImg pointer to imaginary data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

/**
 * Create a complex double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pdblData pointer to Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr stackapi_createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex* _pdblData);

/*shortcut functions*/


/**
 * Check if the variable type is double
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int stackapi_isDoubleType(void* _pvCtx, int* _piAddress);

/**
 * Get scalar double value
 * @param[in] _piAddress variable address
 * @param[out] _pdblReal return real value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int stackapi_getScalarDouble(void* _pvCtx, int* _piAddress, double* _pdblReal);

/**
 * Get scalar complex double value
 * @param[in] _piAddress variable address
 * @param[out] _pdblReal return real part
 * @param[out] _pdblImg return imaginary part
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int stackapi_getScalarComplexDouble(void* _pvCtx, int* _piAddress, double* _pdblReal, double* _pdblImg);

/**
 * Create a scalar double variable
 * @param[in] _iVar variable number
 * @param[in] _dblReal real value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int stackapi_createScalarDouble(void* _pvCtx, int _iVar, double _dblReal);

/**
 * Create a scalar complex double variable
 * @param[in] _iVar variable number
 * @param[in] _dblReal real part
 * @param[in] _dblImg imaginary part
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int stackapi_createScalarComplexDouble(void* _pvCtx, int _iVar, double _dblReal, double _dblImg);

#ifdef __cplusplus
}
#endif
#endif /* __DOUBLE_STACKAPI__ */
