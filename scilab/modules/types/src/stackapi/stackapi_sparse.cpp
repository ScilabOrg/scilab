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
#include "stackapi_internal_sparse.h"
#include "localization.h"

#include "MALLOC.h"

static int stackapi_getCommonAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

//internal sparse functions
SciErr stackapi_getSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return stackapi_getCommonSparseMatrix(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr stackapi_getComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return stackapi_getCommonSparseMatrix(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr stackapi_getCommonSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iPos	= 0;
	int iType	= 0;

	if(	_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "stackapi_getComplexSparseMatrix" : "stackapi_getSparseMatrix");
		return sciErr;
	}
	
	sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "stackapi_getComplexSparseMatrix" : "stackapi_getSparseMatrix", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iType != sci_sparse)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "stackapi_getComplexSparseMatrix" : "stackapi_getSparseMatrix", _("sparse matrix"));
		return sciErr;
	}

	if(stackapi_isVarComplex(_pvCtx, _piAddress) != _iComplex)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), _iComplex ? "stackapi_getComplexSparseMatrix" : "stackapi_getSparseMatrix");
		return sciErr;
	}


	sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "stackapi_getComplexSparseMatrix" : "stackapi_getSparseMatrix", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
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

	if(_pdblReal == NULL)
	{
		return sciErr;
	}

	iPos = (*_piNbItem + *_piRows) % 2 == 1 ? 0 : 1;
	*_pdblReal = (double*)(*_piColPos + *_piNbItem + iPos);
	if(_iComplex == 1 && _pdblImg != NULL)
	{
		*_pdblImg = *_pdblReal + *_piNbItem;
	}
	return sciErr;
}

SciErr stackapi_allocSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return stackapi_allocCommonSparseMatrix(_pvCtx, _iVar, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr stackapi_allocComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return stackapi_allocCommonSparseMatrix(_pvCtx, _iVar, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr stackapi_allocCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos     = Top - Rhs + _iVar;
	int iAddr       = *Lstk(iNewPos);
	int	iTotalSize  = 0;
	int iOffset     = 0;
	int* piAddr     = NULL;

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

    //header + offset
	int iMemSize = (5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2)) / 2;
	//+ items size
	iMemSize += _iNbItem * (_iComplex + 1); 
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = stackapi_fillCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalSize);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_SPARSE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "allocComplexSparseMatrix" : "allocSparseMatrix");
		return sciErr;
	}

	iOffset	= 5;//4 for header + 1 for NbItem
	iOffset		+= _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);

	stackapi_updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + iOffset));
	stackapi_updateLstk(iNewPos, sadr(iadr(iAddr) + iOffset), iTotalSize);
	return sciErr;
}

SciErr stackapi_fillCommonSparseMatrix(void* _pvCtx, int *_piAddress, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg, int* _piTotalSize)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_fillCommonSparseMatrix");
		return sciErr;
	}

	_piAddress[0]		= sci_sparse;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iComplex;

	_piAddress[4]		= _iNbItem;

	*_piNbItemRow	= _piAddress + 5;//4 for header + 1 for NbItem
	*_piColPos		= *_piNbItemRow + _iRows;
	*_pdblReal		= (double*)(*_piColPos + _iNbItem + !((_iRows + _iNbItem) % 2));

	if(_iComplex == 1)
	{
		*_pdblImg			= *_pdblReal + _iNbItem;
	}

	*_piTotalSize	= _iNbItem * (_iComplex + 1);
	return sciErr;
}

SciErr stackapi_createSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal)
{
	return stackapi_createCommonSparseMatrix(_pvCtx, _iVar, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr stackapi_createComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
	return stackapi_createCommonSparseMatrix(_pvCtx, _iVar, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr stackapi_createCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piNbItemRow    = NULL;
	int* piColPos       = NULL;
	int iOne            = 1;
	double* pdblReal    = NULL;
	double* pdblImg     = NULL;

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

	sciErr = stackapi_allocCommonSparseMatrix(_pvCtx, _iVar, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SPARSE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createComplexSparseMatrix" : "createSparseMatrix");
		return sciErr;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
	C2F(dcopy)(&_iNbItem, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
	if(_iComplex)
	{
		C2F(dcopy)(&_iNbItem, const_cast<double*>(_pdblImg), &iOne, pdblImg, &iOne);
	}
	return sciErr;
}

/* shortcut functions */

/*--------------------------------------------------------------------------*/
int stackapi_isSparseType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_sparse);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return stackapi_getCommonAllocatedSparseMatrix(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return stackapi_getCommonAllocatedSparseMatrix(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int stackapi_getCommonAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piNbItemRow	= NULL;
	int* piColPos		= NULL;
	int iOne		= 1;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	sciErr = stackapi_getCommonSparseMatrix(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedComplexSparseMatrix" : "getAllocatedSparseMatrix", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbItemRow = (int*)MALLOC(sizeof(int) * *_piRows);
	memcpy(*_piNbItemRow, piNbItemRow, sizeof(int) * *_piRows);

	*_piColPos = (int*)MALLOC(sizeof(int) * *_piNbItem);
	memcpy(*_piColPos, piColPos, sizeof(int) * *_piNbItem);

	*_pdblReal = (double*)MALLOC(sizeof(double) * *_piNbItem);
	C2F(dcopy)(_piNbItem, pdblReal, &iOne, *_pdblReal, &iOne);

	if(_iComplex)
	{
		*_pdblImg = (double*)MALLOC(sizeof(double) * *_piNbItem);
		C2F(dcopy)(_piNbItem, pdblImg, &iOne, *_pdblImg, &iOne);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal)
{
	FREE(_piNbItemRows);
	FREE(_piColPos);
	FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedComplexSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	stackapi_freeAllocatedSparseMatrix(_piNbItemRows, _piColPos, _pdblReal);
	FREE(_pdblImg);
}
/*--------------------------------------------------------------------------*/
