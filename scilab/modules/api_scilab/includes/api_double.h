/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#ifndef __DOUBLE_API__
#define __DOUBLE_API__

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
    SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal);

    /**
     * Get complex double variable data
     * @param[in] _piAddress variable address
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real data
     * @param[out] _pdblImg return pointer to imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

    /**
     * Get double variable data
     * @param[in] _piAddress variable address
     * @param[out] _piRows return number of row
     * @param[out] _piCols return number of column
     * @param[out] _pdblZ return pointer to Z format data ( Real1, Img1, Real2, Img2, ... )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ);

    /**
     * Get double variable data
     * @param[in] _piAddress variable address
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piReal return pointer to real data as integer
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfDoubleAsInteger(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piReal);

    /**
     * Get double variable data
     * @param[in] _piAddress variable address
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piReal return pointer to real data as integer
     * @param[out] _piImg return pointer to imaginary data as integer
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getComplexMatrixOfDoubleAsInteger(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piReal, int** _piImg);

    /**
     * Allocate a double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal);

    /**
     * Allocate a complex double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblReal return pointer to real data
     * @param[out] _pdblImg return pointer to imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

    /**
     * Allocate a double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _piReal return pointer to integer real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piReal);

    /**
     * Allocate a complex double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _piReal return pointer to integer real data
     * @param[out] _piImg return pointer to integer imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocComplexMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piReal, int** _piImg);


    /**
     * Allocate a doublecomplex variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblData return pointer to doublecomplex data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex** _pdblData);

    /**
     * Create a double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal);

    /**
     * Create a complex double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @param[in] _pdblImg pointer to imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piReal pointer to integer real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piReal);

    /**
     * Create a complex double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piReal pointer to integer real data
     * @param[in] _piImg pointer to integer imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piReal, const int* _piImg);

    /**
     * Create a complex double variable
     * @param[in] _iVar variable number
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblData pointer to Z format data ( Real1, Img1, Real2, Img2, ... )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex* _pdblData);

    /**
     * Create a named double variable
     * @param[in] _pstName variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const double* _pdblReal);

    /**
     * Create a named complex double variable
     * @param[in] _pstName variable name
     * @param[in] _iRows mumber of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @param[in] _pdblImg pointer to imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedComplexMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a named complex double variable
     * @param[in] _pstName variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblData pointer to Z format data ( Real1, Img1, Real2, Img2, ... )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedComplexZMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const doublecomplex* _pdblData);

    /**
     * Get double named variable data
     * @param[in] _pstName variable name
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, double* _pdblReal);

    /**
     * Get double named variable data
     * @param[in] _pstName variable name
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return real data
     * @param[out] _pdblImg return imaginary data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readNamedComplexMatrixOfDouble(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

    /*shortcut functions*/


    /**
     * Check if the variable type is double
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isDoubleType(void* _pvCtx, int* _piAddress);

    /**
     * Check if the named variable type is double
     * @param[in] _pstName variable name
     * @return 1 for true and 0 for false
     */
    int isNamedDoubleType(void* _pvCtx, const char* _pstName);

    /**
     * Get scalar double value
     * @param[in] _piAddress variable address
     * @param[out] _pdblReal return real value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int getScalarDouble(void* _pvCtx, int* _piAddress, double* _pdblReal);

    /**
     * Get scalar complex double value
     * @param[in] _piAddress variable address
     * @param[out] _pdblReal return real part
     * @param[out] _pdblImg return imaginary part
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int getScalarComplexDouble(void* _pvCtx, int* _piAddress, double* _pdblReal, double* _pdblImg);

    /**
     * Get scalar double value from a named variable
     * @param[in] _pstName variable name
     * @param[out] _pdblReal return real value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int getNamedScalarDouble(void* _pvCtx, const char* _pstName, double* _pdblReal);

    /**
     * Get scalar complex double value from a named variable
     * @param[in] _pstName variable name
     * @param[out] _pdblReal return real part
     * @param[out] _pdblImg return imaginary part
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int getNamedScalarComplexDouble(void* _pvCtx, const char* _pstName, double* _pdblReal, double* _pdblImg);

    /**
     * Create a scalar double variable
     * @param[in] _iVar variable number
     * @param[in] _dblReal real value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int createScalarDouble(void* _pvCtx, int _iVar, double _dblReal);

    /**
     * Create a scalar complex double variable
     * @param[in] _iVar variable number
     * @param[in] _dblReal real part
     * @param[in] _dblImg imaginary part
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int createScalarComplexDouble(void* _pvCtx, int _iVar, double _dblReal, double _dblImg);

    /**
     * Create a named scalar double variable
     * @param[in] _pstName variable name
     * @param[in] _dblReal real value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int createNamedScalarDouble(void* _pvCtx, const char* _pstName, double _dblReal);

    /**
     * Create a named scalar complex double variable
     * @param[in] _pstName variable name
     * @param[in] _dblReal real part
     * @param[in] _dblImg imaginary part
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int createNamedScalarComplexDouble(void* _pvCtx, const char* _pstName, double _dblReal, double _dblImg);

#ifdef __cplusplus
}
#endif
#endif /* __DOUBLE_API__ */
