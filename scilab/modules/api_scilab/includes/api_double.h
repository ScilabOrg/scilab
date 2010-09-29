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

#ifndef __DOUBLE_API__
#define __DOUBLE_API__

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
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblZ return pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

/**
 * Create double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

/**
 * Get complex double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[ ] _pdblData pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex* _pdblData);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, const double* _pdblReal);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblData pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedComplexZMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, const doublecomplex* _pdblData);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @param[out] _pdblImg return imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

/*shortcut functions*/


/**
 * check if the variable type is double
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isDoubleType(void* _pvCtx, int* _piAddress);

/**
 * check if the variable type is double
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedDoubleType(void* _pvCtx, char* _pstName);

/**
 * Get scalar double variable data
 * @param[in] _piAddress variable address
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarDouble(void* _pvCtx, int* _piAddress, double* _pdblReal);

/**
 * Get scalar complex double variable data
 * @param[in] _piAddress variable address
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarComplexDouble(void* _pvCtx, int* _piAddress, double* _pdblReal, double* _pdblImg);

/**
 * Get scalar double variable data
 * @param[in] _pstName variable name
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarDouble(void* _pvCtx, char* _pstName, double* _pdblReal);

/**
 * Get scalar complex double variable data
 * @param[in] _pstName variable name
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarComplexDouble(void* _pvCtx, char* _pstName, double* _pdblReal, double* _pdblImg);

/**
 * Create scalar double
 * @param[in] _iVar variable number
 * @param[in] _dblReal value
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarDouble(void* _pvCtx, int _iVar, double _dblReal);

/**
 * Create scalar complex double variable
 * @param[in] _iVar variable number
 * @param[in] _dblReal real value
 * @param[in] _dblImg imaginary value
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarComplexDouble(void* _pvCtx, int _iVar, double _dblReal, double _dblImg);

/**
 * Create named scalar double
 * @param[in] _pstName variable name
 * @param[in] _dblReal value
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarDouble(void* _pvCtx, char* _pstName, double _dblReal);

/**
 * Create named scalar complex double variable
 * @param[in] _pstName variable name
 * @param[in] _dblReal real value
 * @param[in] _dblImg imaginary value
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarComplexDouble(void* _pvCtx, char* _pstName, double _dblReal, double _dblImg);

#ifdef __cplusplus
}
#endif
#endif /* __DOUBLE_API__ */
