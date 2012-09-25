/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include <string.h>

#include "charEncoding.h"
#include "MALLOC.h"
#include "stackapi_scilab.h"
#include "stackapi_internal_common.h"
#include "stackapi_internal_string.h"
#include "localization.h"

extern "C"
{
#include "code2str.h"
#include "freeArrayOfString.h"
};
/*--------------------------------------------------------------------------*/

/*******************************/
/*   string matrix functions   */
/*******************************/

SciErr stackapi_getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piOffset = NULL;
	int *piData		= NULL;
	int iType			= 0;

	if(	_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfString");
		return sciErr;
	}

	sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iType != sci_strings)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "stackapi_getMatrixOfString", _("string matrix"));
		return sciErr;
	}

	sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(_piLength == NULL)
	{
		return sciErr;
	}

	piOffset = _piAddress + 4;

	//non cummulative length
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		_piLength[i] = piOffset[i + 1] - piOffset[i];
	}

	if(_pstStrings == NULL || *_pstStrings == NULL)
	{
		return sciErr;
	}

	piData = piOffset + *_piRows * *_piCols + 1;

	int sum = 0;
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfString");
			return sciErr;
		}
		code2str(&_pstStrings[i], piData + sum, _piLength[i]);
		sum += _piLength[i];
		_pstStrings[i][_piLength[i]] = 0;
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr stackapi_createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* const * _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iTotalLen		= 0;
	int *piAddr			= NULL;

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

	sciErr = stackapi_fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_STRING, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfString");
		return sciErr;
	}

	stackapi_updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 5 + _iRows * _iCols));
	stackapi_updateLstk(iNewPos, sadr(iadr(iAddr) + 5 + _iRows * _iCols + !((_iRows * _iCols) % 2)), (iTotalLen + 1) / (sizeof(double) / sizeof(int)));
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr stackapi_fillMatrixOfString(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, const char* const* _pstStrings, int* _piTotalLen)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piOffset = NULL;
    int* piData   = NULL;
    int iOffset   = 0;
    int iTotalSize = 0;
    int iMemSize = 0;
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(*Lstk(Top)));

    for (int i = 0; i < _iRows * _iCols; i++)
    {
        iTotalSize = iTotalSize + (int)strlen(_pstStrings[i]) + 1;
    }

    iMemSize = iTotalSize + 2;

    if (iMemSize > iFreeSpace)
    {
        stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    _piAddress[0] = sci_strings;
    _piAddress[1] = _iRows;
    _piAddress[2] = _iCols;
    _piAddress[3] = 0; //always 0

    piOffset  = _piAddress + 4;
    piOffset[0] = 1; //Always 1
    piData    = piOffset + _iRows * _iCols + 1;

    if(_pstStrings == NULL)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_fillMatrixOfString");
        return sciErr;
    }

    for(int i = 0 ; i < _iRows * _iCols ; i++)
    {
        if(_pstStrings[i] == NULL)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfString");
            return sciErr;
        }

        int iLen = (int)strlen(_pstStrings[i]);
        str2code(piData + iOffset, &_pstStrings[i]);
        iOffset += iLen;
        piData[iOffset] = 0;
        piOffset[i + 1] = piOffset[i] + iLen;
    }

    *_piTotalLen  = piOffset[_iRows * _iCols] - 1;
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr stackapi_getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType					= 0;
	char **pstStrings = NULL;
	int *piLenStrings = NULL;
	int strSize = 0;

	if(	_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfWideString");
		return sciErr;
	}

	sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iType != sci_strings)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "stackapi_getMatrixOfWideString", _("string matrix"));
		return sciErr;
	}

	sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if (_piwLength == NULL)
	{
		return sciErr;
	}

	strSize = (*_piRows * *_piCols);
	piLenStrings	= (int*)MALLOC(sizeof(int) * strSize);

	// get length UTF size
	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
		return sciErr;
	}

	pstStrings = (char**)MALLOC(sizeof(char*) * strSize);

	for(int i = 0; i < strSize; i++)
	{
		pstStrings[i] = (char*)MALLOC(sizeof(char)*(piLenStrings[i] + 1));
	}

	// get strings UTF format
	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
		freeArrayOfString(pstStrings,strSize);
		return sciErr;
	}

	for(int i = 0; i < (*_piRows * *_piCols); i++)
	{
		wchar_t* wString = to_wide_string(pstStrings[i]);
		if (wString)
		{
			_piwLength[i] = (int)wcslen(wString);
			FREE(wString);
			wString = NULL;
		}
		else
		{
            _piwLength[i] = 0;
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_WIDE_STRING, _("%s: Unable to convert to wide string #%d"), "stackapi_getMatrixOfWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
            if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
            freeArrayOfString(pstStrings,strSize);
            return sciErr;
		}
	}

	if ( (_pwstStrings == NULL) || (*_pwstStrings == NULL) )
	{
		if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
		freeArrayOfString(pstStrings,strSize);
		return sciErr;
	}
	
	for (int i = 0; i < (*_piRows * *_piCols); i++)
	{
		if (pstStrings[i])
		{
			wchar_t *wcstring = to_wide_string(pstStrings[i]);
			if (wcstring)
			{
				if (_pwstStrings[i])
				{
					wcscpy(_pwstStrings[i], wcstring);
					_piwLength[i] = (int)wcslen(_pwstStrings[i]);
				}
				else
				{
					_pwstStrings[i] = NULL;
					_piwLength[i] = 0;
				}
				FREE(wcstring);
				wcstring = NULL;
			}
			else
			{
				// case to_wide_string fails
				_pwstStrings[i] = NULL;
				_piwLength[i] = 0;
                stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_WIDE_STRING, _("%s: Unable to convert to wide string #%d"), "stackapi_getMatrixOfWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
                if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
                freeArrayOfString(pstStrings,strSize);
                return sciErr;
			}
		}
		else
		{
			// case to_wide_string fails
			_pwstStrings[i] = NULL;
			_piwLength[i] = 0;
		}
	}

	freeArrayOfString(pstStrings, strSize);
	if (piLenStrings) {FREE(piLenStrings); piLenStrings = NULL;}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr stackapi_createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const wchar_t* const* _pstwStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char **pStrings = NULL;

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

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );

	for (int i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pstwStrings[i]);
	}

	sciErr = stackapi_createMatrixOfString(_pvCtx, _iVar, _iRows, _iCols, pStrings);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_WIDE_STRING, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfWideString");
	}

	freeArrayOfString(pStrings, _iRows * _iCols);

	return sciErr;
}

/*------------------*/
/*shortcut functions*/
/*------------------*/
int stackapi_isStringType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_strings);
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedSingleString(void* _pvCtx, int* _piAddress, char** _pstData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRows = 0;
	int iCols = 0;
	int iLen = 0;

	if(stackapi_isScalar(_pvCtx, _piAddress) == 0 || stackapi_isStringType(_pvCtx, _piAddress) == 0)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_STRING, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "getAllocatedSingleString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pstData = (char*)MALLOC(sizeof(char) * (iLen + 1)); //+1 for null termination

	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, _pstData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedSingleWideString(void* _pvCtx, int* _piAddress, wchar_t** _pwstData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRows = 0;
	int iCols = 0;
	int iLen = 0;

	if(stackapi_isScalar(_pvCtx, _piAddress) == 0 || stackapi_isStringType(_pvCtx, _piAddress) == 0)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_WIDE_STRING, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "getAllocatedSingleWideString", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = stackapi_getMatrixOfWideString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleWideString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pwstData = (wchar_t*)MALLOC(sizeof(wchar_t) * (iLen + 1)); //+1 for null termination

	sciErr = stackapi_getMatrixOfWideString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, _pwstData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleWideString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char*** _pstData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int i;
	int* piLen = 0;

	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, NULL, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	piLen = (int*)MALLOC(sizeof(int) * *_piRows * *_piCols);
	*_pstData = (char**)MALLOC(sizeof(char*) * *_piRows * *_piCols);

	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLen, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfString");
		stackapi_printError(&sciErr, 0);
        if (piLen) {FREE(piLen); piLen = NULL;}
		return sciErr.iErr;
	}

	for(i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		(*_pstData)[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+1 for null termination
	}

	sciErr = stackapi_getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLen, *_pstData);
    if (piLen) {FREE(piLen); piLen = NULL;}
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getAllocatedMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, wchar_t*** _pwstData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int i;
	int* piLen = 0;

	sciErr = stackapi_getMatrixOfWideString(_pvCtx, _piAddress, _piRows, _piCols, NULL, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfWideString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	piLen = (int*)MALLOC(sizeof(int) * *_piRows * *_piCols);
	*_pwstData = (wchar_t**)MALLOC(sizeof(wchar_t*) * *_piRows * *_piCols);

	sciErr = stackapi_getMatrixOfWideString(_pvCtx, _piAddress, _piRows, _piCols, piLen, NULL);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfWideString");
        if (piLen) {FREE(piLen); piLen = NULL;}
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		(*_pwstData)[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (piLen[i] + 1));//+1 for null termination
	}

	sciErr = stackapi_getMatrixOfWideString(_pvCtx, _piAddress, _piRows, _piCols, piLen, *_pwstData);

    if (piLen) {FREE(piLen); piLen = NULL;}

	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ALLOC_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfWideString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createSingleString(void* _pvCtx, int _iVar, const char* _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	
	sciErr = stackapi_createMatrixOfString(_pvCtx, _iVar, 1, 1,	&_pstStrings);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SINGLE_STRING, _("%s: Unable to get argument data"), "createSingleString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createSingleWideString(void* _pvCtx, int _iVar, const wchar_t* _pwstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	
	sciErr = stackapi_createMatrixOfWideString(_pvCtx, _iVar, 1, 1,	&_pwstStrings);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "createSingleWideString");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedSingleString(char* _pstData)
{
	FREE(_pstData);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedSingleWideString(wchar_t* _pwstData)
{
	FREE(_pwstData);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedMatrixOfString(int _iRows, int _iCols, char** _pstData)
{
	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		FREE(_pstData[i]);
	}
	FREE(_pstData);
}
/*--------------------------------------------------------------------------*/
void stackapi_freeAllocatedMatrixOfWideString(int _iRows, int _iCols, wchar_t** _pwstData)
{
	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		FREE(_pwstData[i]);
	}
	FREE(_pwstData);
}
/*--------------------------------------------------------------------------*/
