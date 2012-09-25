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

#ifndef __LIST_STACKAPI__
#define __LIST_STACKAPI__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_list.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "doublecomplex.h"
#include "version.h"

#if SCI_VERSION_MAJOR > 5
	#define __SCILAB_INT64__
#endif

    /**********************/
    /*   list functions   */
    /**********************/

    /**
     * Get the number of items in a list
     * @param[in] _piAddress list address
     * @param[out] _piNbItem return number of items
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem);

    /**
     * Get the address of an item in a list
     * @param[in] _piAddress list address
     * @param[in] _iItemNum item number
     * @param[out] _piItemAddress return item address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress);

    /**
     * Get a list from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved list in the parent list
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get a tlist from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved tlist in the parent list
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get an mlist from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved mlist in the parent list
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Create a list
     * @param[in] _iVar variable number
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

    /**
     * Create a tlist
     * @param[in] _iVar variable number
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

    /**
     * Create an mlist
     * @param[in] _iVar variable number
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

    /**
     * Create a list in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created list in the parent list
     * @param[in] _iNbItem number of items in the created list
     * @param[out] _piAddress return new list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create a tlist in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created tlist in the parent list
     * @param[in] _iNbItem number of items in the created tlist
     * @param[out] _piAddress return new tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create an mlist in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created mlist in the parent list
     * @param[in] _iNbItem number of items in the created mlist
     * @param[out] _piAddress return new mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /*********************
     * double functions  *
     *********************/

    /**
     * Get double variable data from a list item
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal);

    /**
     * Get complex double variable data from a list item
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

    /**
     * Allocate a double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal);

    /**
     * Allocate a complex double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

    /**
     * Create a double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal);

    /**
     * Create a complex double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real parts
     * @param[in] _pdblImg pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a complex double variable (Z-representation) in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblData pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const doublecomplex* _pdblData);

    /*********************
     * strings functions *
     *********************/

    /**
     * Get a string variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piLength return strings length
     * @param[out] _pstStrings return array of char
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

    /**
     * Create a string variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pstStrings array of null-terminated strings
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings);

    /*********************
     * boolean functions *
     *********************/

    /**
     * Get a boolean variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piBool return pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool);

    /**
     * Allocate a boolean variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _piBool return pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);

    /**
     * Create a boolean variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piBool pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piBool);

    /*************************
     * polynomials functions *
     *************************/

    /**
     * Get a polynomial variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return pointer to real polynomials coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

    /**
     * Get a complex polynomial variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return pointer to real polynomials coefficients
     * @param[out] _pdblImg return pointer to imaginary polynomials coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

    /**
     * Create a polynomial variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal);

    /**
     * Create a complex polynomial variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real polynomial coefficients
     * @param[in] _pdblImg pointer to imaginary polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);

    /*********************
     * integer functions *
     *********************/

    /**
     * Create an 8-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pcData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* _pcData);

    /**
     * Create an 8-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pucData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfUnsigedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned char* _pucData);

    /**
     * Create a 16-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _psData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const short* _psData);

    /**
     * Create a 16-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pusData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfUnsigedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned short* _pusData);

    /**
     * Create a 32-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData);

    /**
     * Create a 32-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _puiData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfUnsigedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned int* _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Create a 64-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pllData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const long long* _pllData);

    /**
     * Create a 64-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pullData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createMatrixOfUnsigedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned long long* _pullData);

#endif

    /**
     * Allocate an 8-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pcData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData);

    /**
     * Allocate an 8-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pucData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData);

    /**
     * Allocate a 16-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _psData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData);

    /**
     * Allocate a 16-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pusData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData);

    /**
     * Allocate a 32-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _piData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData);

    /**
     * Allocate a 32-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _puiData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Allocate a 64-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData);

    /**
     * Allocate a 64-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pullData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_allocMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData);

#endif

    /**
     * Get an 8-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pcData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData);

    /**
     * Get an 8-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pucData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData);

    /**
     * Get a 16-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _psData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData);

    /**
     * Get a 16-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pusData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData);

    /**
     * Get a 32-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData);

    /**
     * Get a 32-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _puiData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Get a 64-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long** _pllData);

    /**
     * Get a 64-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getMatrixOfUnsignedInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long** _pullData);

#endif

    /********************
     * sparse functions *
     ********************/

    /**
     * Create a double sparse variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of item column positions ( 1 indexed )
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal);

    /**
     * Create a complex sparse variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of item column positions ( 1 indexed )
     * @param[in] _pdblReal pointer to real parts
     * @param[in] _pdblImg pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

    /**
     * Get a double sparse variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of item column positions ( 1 indexed )
     * @param[out] _pdblReal return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

    /**
     * Get a complex sparse variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of item column positions ( 1 indexed )
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

    /****************************
     * boolean sparse functions *
     ****************************/

    /**
     * Create a boolean sparse variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of column positions of "true" elements ( 1 indexed )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);

    /**
     * Get a boolean sparse variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of column positions of "true" elements ( 1 indexed )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

    /*********************
     * pointer functions *
     *********************/

    /**
     * Get a pointer from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _pvPtr return pointer value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr);

    /**
     * Create a pointer in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pvPtr return pointer value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr stackapi_createPointerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, void* _pvPtr);

    /**
     * Check if the variable type is list
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isListType(void* _pvCtx, int* _piAddress);

    /**
     * Check if the variable type is tlist
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isTListType(void* _pvCtx, int* _piAddress);


    /**
     * Check if the variable type is mlist
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int stackapi_isMListType(void* _pvCtx, int* _piAddress);

#ifdef __cplusplus
}
#endif
#endif /* __LIST_STACKAPI__ */

