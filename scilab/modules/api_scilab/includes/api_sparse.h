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

#ifndef __SAPRSE_API__
#define __SAPRSE_API__

#ifdef __cplusplus
extern "C" {
#endif


/*****************************/
/*   sparse matrix functions */
/*****************************/

/**
 * Get sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[in] _iNbItem Number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Get sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[in] _iNbItem Number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * alloc sparse variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr allocSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * alloc sparse variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return pointer on data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr allocComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * Create sparse variable
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * Create sparse variable
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

/**
 * Create named sparse variable
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createNamedSparseMatrix(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * Create named sparse variable
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createNamedComplexSparseMatrix(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

/**
 * Read named sparse variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readNamedSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * Read named sparse variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readNamedComplexSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

/* shortcut functions */

/**
 * check if the variable type is a sparse
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isSparseType(void* _pvCtx, int* _piAddress);

/**
 * check if the variable type is a sparse
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedSparseType(void* _pvCtx, char* _pstName);

/**
 * Get sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _iNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return array of real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Get sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _iNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return array of real data
 * @param[out] _pdblImg return array of imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * Get named sparse variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _iNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return array of real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedAllocatedSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Get named sparse variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _iNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return array of real data
 * @param[out] _pdblImg return array of imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedAllocatedComplexSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);


/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal sparse real data
 */
void freeAllocatedSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal sparse real data
 * @param[in] _pdblImg sparse imaginary data
 */
void freeAllocatedComplexSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal, double* _pdblImg);

#ifdef __cplusplus
}
#endif
#endif /* __SAPRSE_API__ */
