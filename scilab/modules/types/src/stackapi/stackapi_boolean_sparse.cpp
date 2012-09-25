/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be 
 * still available and supported in Scilab 6.
 */

#include "stackapi_scilab.h"
#include "stackapi_internal_common.h"
#include "stackapi_boolean_sparse.h"
#include "stackapi_internal_boolean_sparse.h"
#include "localization.h"


#include "MALLOC.h"

SciErr stackapi_getBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType = 0;

	if(	_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getBooleanSparseMatrix");
		return sciErr;
	}

	sciErr =  stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr || iType != sci_boolean_sparse)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "stackapi_getBooleanSparseMatrix", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "stackapi_getBooleanSparseMatrix", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	*_piNbItem = _piAddress[4];

	if(_piNbItemRow == NULL)
	{
		return sciErr;
	}
	*_piNbItemRow = _piAddress + 5;//4 for header + 1 for NbItem

	if(_piColPos == NULL)
	{
		return sciErr;
	}
	*_piColPos = *_piNbItemRow + *_piRows;

	return sciErr;
}

SciErr stackapi_allocBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos = Top - Rhs + _iVar;
	int iAddr   = *Lstk(iNewPos);
	int iPos    = 5 + _iRows + _iNbItem;
	int* piAddr = NULL;

    //return empty matrix
    if(_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        sciErr = stackapi_createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        }
        return sciErr;
    }

    int iMemSize = (int)( ( (double)iPos / 2 ) + 0.5);
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	sciErr = stackapi_fillBooleanSparseMatrix(_pvCtx, piAddr, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to create variable in Scilab memory"), "stackapi_allocBooleanSparseMatrix");
		return sciErr;
	}

	iPos += iAddr;
	stackapi_updateInterSCI(_iVar, '$', iAddr, iPos);
	stackapi_updateLstk(iNewPos, iPos, 0);
	return sciErr;
}

SciErr stackapi_fillBooleanSparseMatrix(void* _pvCtx, int *_piAddress, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_fillBooleanSparseMatrix");
		return sciErr;
	}

	_piAddress[0]		= sci_boolean_sparse;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= 0;

	_piAddress[4]		= _iNbItem;

	*_piNbItemRow	= _piAddress + 5;//4 for header + 1 for NbItem
	*_piColPos		= *_piNbItemRow + _iRows;
	return sciErr;
}

SciErr stackapi_createBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piNbItemRow    = NULL;
	int* piColPos       = NULL;

    if(_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        sciErr = stackapi_createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        }
        return sciErr;
    }

	sciErr = stackapi_allocBooleanSparseMatrix(_pvCtx, _iVar, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_BOOLEAN_SPARSE, _("%s: Unable to create variable in Scilab memory"), "createBooleanSparseMatrix");
		return sciErr;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
	return sciErr;
}
/*--------------------------------------------------------------------------*/
int stackapi_isBooleanSparseType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_boolean_sparse);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	sciErr = stackapi_getBooleanSparseMatrix(_pvCtx, _piAddress, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getAllocatedBooleanSparseMatrix", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbItemRow		= (int*)MALLOC(sizeof(int) * *_piRows);
	memcpy(*_piNbItemRow, piNbItemRow, sizeof(int) * *_piRows);

	*_piColPos			= (int*)MALLOC(sizeof(int) * *_piNbItem);
	memcpy(*_piColPos, piColPos, sizeof(int) * *_piNbItem);

	return 0;
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedBooleanSparse(int* _piNbItemRow, int* _piColPos)
{
	FREE(_piNbItemRow);
	FREE(_piColPos);
}
