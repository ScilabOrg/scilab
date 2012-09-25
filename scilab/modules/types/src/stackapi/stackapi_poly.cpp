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
#include "stackapi_internal_poly.h"
#include "localization.h"

#include "MALLOC.h"
extern "C" {
#include "code2str.h"
};


static int stackapi_getCommonAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int stackapi_getCommonAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);

SciErr stackapi_getPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char *pstVarName	= NULL;

	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getPolyVariableName");
		return sciErr;
	}

	if(_piAddress[0] != sci_poly)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getPolyVariableName", _("polynomial matrix"));
		return sciErr;
	}

	*_piVarNameLen = 4;
	pstVarName = (char*)MALLOC(sizeof(char) * 5);
	code2str(&pstVarName, &_piAddress[4], *_piVarNameLen);
	for(int i = 0 ; i < *_piVarNameLen ; i++)
	{
		if(pstVarName[i] == ' ')
		{
			memset(pstVarName + i, 0x00, *_piVarNameLen - i);
			*_piVarNameLen = i;
			break;
		}
	}
	pstVarName[4] = 0;

	if(_pstVarName == NULL)
	{
		return sciErr;
	}

	strcpy(_pstVarName, pstVarName);
	
	return sciErr;
}

SciErr stackapi_getMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr stackapi_getComplexMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr stackapi_getCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType					= 0;
	int iSize					= 0;
	int *piOffset			= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "stackapi_getComplexMatrixOfPoly" : "stackapi_getMatrixOfPoly");
		return sciErr;
	}

	sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "stackapi_getComplexMatrixOfPoly" : "stackapi_getMatrixOfPoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}
	
	if(iType != sci_poly)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "stackapi_getComplexMatrixOfPoly" : "stackapi_getMatrixOfPoly", _("polynomial matrix"));
		return sciErr;
	}

	sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "stackapi_getComplexMatrixOfPoly" : "stackapi_getMatrixOfPoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	iSize	= *_piRows * *_piCols;

	if(_piNbCoef == NULL)
	{
		return sciErr;
	}

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	for(int i = 0 ; i < iSize ; i++)
	{
		_piNbCoef[i]	= piOffset[i + 1] - piOffset[i];
	}

	if(_pdblReal == NULL)
	{
		return sciErr;
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));
	for(int i = 0 ; i < iSize ; i++)
	{
		memcpy(_pdblReal[i], pdblReal + piOffset[i] - 1, sizeof(double) * _piNbCoef[i]);
	}

	if(stackapi_isVarComplex(_pvCtx, _piAddress) && _iComplex == 1)
	{
		pdblImg = pdblReal + piOffset[iSize] - 1;
		for(int i = 0 ; i < iSize ; i++)
		{
			memcpy(_pdblImg[i], pdblImg + piOffset[i] - 1, sizeof(double) * _piNbCoef[i]);
		}
	}
	return sciErr;
}

SciErr stackapi_createMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal)
{
	return stackapi_createCommonMatrixOfPoly(_pvCtx, _iVar, 0, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr stackapi_createComplexMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
	return stackapi_createCommonMatrixOfPoly(_pvCtx, _iVar, 1, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr stackapi_createCommonMatrixOfPoly(void* _pvCtx, int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr     = NULL;
	int iSize       = _iRows * _iCols;
	int iNewPos     = Top - Rhs + _iVar;
	int iAddr       = *Lstk(iNewPos);
	int iTotalLen   = 0;

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

	stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	sciErr = stackapi_fillCommonMatrixOfPoly(_pvCtx, piAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POLY, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createComplexMatrixOfPoly" : "createMatrixOfPoly");
		return sciErr;
	}

	stackapi_updateInterSCI(_iVar, '$', iAddr, iAddr + 4 + 4 + iSize + 1);
	stackapi_updateLstk(iNewPos, iAddr + 4 + 4 + iSize + 1, iTotalLen);

	return sciErr;
}

SciErr stackapi_fillCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piOffset			= NULL;
	int *piVarName		= NULL;
	int iSize					= _iRows * _iCols;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	//header
	_piAddress[0] = sci_poly;
	_piAddress[1] = _iRows;
	_piAddress[2] = _iCols;
	_piAddress[3] = _iComplex;


	//4 for header
	piVarName = _piAddress + 4;//4 for header
	if(strlen(_pstVarName) > 4)//4 characters max
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_TOO_LONG_VAR, _("%s: Formal variable name of polynomial can't exceed 4 characters"));
		return sciErr;
	}

	//fill variable name with space ( 40 sergescii )
	piVarName[0] = 40;
	piVarName[1] = 40;
	piVarName[2] = 40;
	piVarName[3] = 40;
	str2code(piVarName, &_pstVarName);

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	piOffset[0] = 1;
	for(int i = 0 ; i < iSize ; i++)
	{
		piOffset[i + 1] = piOffset[i] + _piNbCoef[i];
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));

	for(int i = 0 ; i < iSize ; i++)
	{
		memcpy(pdblReal + piOffset[i] - 1, _pdblReal[i], _piNbCoef[i] * sizeof(double));
	}

    pdblImg = pdblReal + piOffset[iSize] - 1;
	if(_iComplex == 1 && _pdblImg)
	{
		for(int i = 0 ; i < iSize ; i++)
		{
			memcpy(pdblImg + piOffset[i] - 1, _pdblImg[i], _piNbCoef[i] * sizeof(double));
		}
	}
    else
    {//fill with 0
		for(int i = 0 ; i < iSize ; i++)
		{
            memset(pdblImg + piOffset[i] - 1, 0x00, _piNbCoef[i] * sizeof(double));
        }
    }

	*_piTotalLen = (piOffset[iSize] - 1) * (_iComplex + 1) * 2;
	return sciErr;
}

/*shortcut functions */

/*--------------------------------------------------------------------------*/
int stackapi_isPolyType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_poly);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal)
{
	return stackapi_getCommonAllocatedSinglePoly(_pvCtx, _piAddress, 0, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedSingleComplexPoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return stackapi_getCommonAllocatedSinglePoly(_pvCtx, _piAddress, 1, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int stackapi_getCommonAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRows	= 0;
	int iCols	= 0;

	if(stackapi_isScalar(_pvCtx, _piAddress) == 0)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, _piNbCoef, NULL, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pdblReal = (double*)MALLOC(sizeof(double) * *_piNbCoef);

	if(_iComplex)
	{
		*_pdblImg	= (double*)MALLOC(sizeof(double) * *_piNbCoef);
	}

	sciErr = stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, _piNbCoef, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal)
{
	return stackapi_getCommonAllocatedMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedMatrixOfComplexPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
	return stackapi_getCommonAllocatedMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int stackapi_getCommonAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, NULL, NULL, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedMatrixOfComplexPoly" : "getAllocatedMatrixOfPoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbCoef = (int*)MALLOC(sizeof(int) * *_piRows * *_piCols);

	sciErr = stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, *_piNbCoef, NULL, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedMatrixOfComplexPoly" : "getAllocatedMatrixOfPoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pdblReal = (double**)MALLOC(sizeof(double*) * *_piRows * *_piCols);
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		(*_pdblReal)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
	}

    if(stackapi_isVarComplex(_pvCtx, _piAddress) && _pdblImg != NULL)
	{
		*_pdblImg	= (double**)MALLOC(sizeof(double*) * *_piRows * *_piCols);
		for(int i = 0 ; i < *_piRows * *_piCols ; i++)
		{
			(*_pdblImg)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
		}
	}

	sciErr = stackapi_getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, *_piNbCoef, *_pdblReal, _pdblImg == NULL ? NULL : *_pdblImg);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedSinglePoly(double* _pdblReal)
{
	FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedSingleComplexPoly(double* _pdblReal, double* _pdblImg)
{
	stackapi_freeAllocatedSinglePoly(_pdblReal);
	FREE(_pdblImg);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedMatrixOfPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	FREE(_piNbCoef);

	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		FREE(_pdblReal[i]);
	}
	FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedMatrixOfComplexPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	stackapi_freeAllocatedMatrixOfPoly(_iRows, _iCols, _piNbCoef, _pdblReal);

	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		FREE(_pdblImg[i]);
	}
	FREE(_pdblImg);
}
