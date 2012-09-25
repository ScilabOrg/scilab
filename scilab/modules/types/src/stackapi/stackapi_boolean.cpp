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
#include "stackapi_internal_boolean.h"
#include "localization.h"

#include "Scierror.h"


/********************************/
/*   boolean matrix functions   */
/********************************/
SciErr stackapi_getMatrixOfBoolean(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType = 0;

	if(	_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfBoolean");
		return sciErr;
	}

	sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr || iType != sci_boolean)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "stackapi_getMatrixOfBoolean", _("boolean matrix"));
		return sciErr;
	}

	sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfBoolean", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(_piBool)
	{
		*_piBool = _piAddress + 3;
	}
	return sciErr;
}

SciErr stackapi_allocMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr	= NULL;
	int iNewPos = Top - Rhs + _iVar;
	int iAddr   = *Lstk(iNewPos);

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

    int iMemSize = (int)(((double)(_iRows * _iCols) / 2) + 2);
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	stackapi_fillMatrixOfBoolean(_pvCtx, piAddr, _iRows, _iCols, _piBool);

	stackapi_updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 3));
	stackapi_updateLstk(iNewPos, sadr(iadr(iAddr) + 3), (_iRows * _iCols) / (sizeof(double) / sizeof(int)));

	return sciErr;
}

SciErr stackapi_fillMatrixOfBoolean(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	_piAddress[0]	= sci_boolean;
	_piAddress[1] = Min(_iRows, _iRows * _iCols);
	_piAddress[2] = Min(_iCols, _iRows * _iCols);

	*_piBool		= _piAddress + 3;
	return sciErr;
}

SciErr stackapi_createMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piBool		= NULL;

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

    sciErr = stackapi_allocMatrixOfBoolean(_pvCtx, _iVar, _iRows, _iCols, &piBool);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfBoolean");
		return sciErr;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);
	return sciErr;
}

/*shortcut functions*/

/*--------------------------------------------------------------------------*/
int stackapi_isBooleanType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_boolean);
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarBoolean(void* _pvCtx, int* _piAddress, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRows	= 0;
	int iCols	= 0;

	int* piBool = NULL;

	sciErr = stackapi_getMatrixOfBoolean(_pvCtx, _piAddress, &iRows, &iCols, &piBool);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SCALAR_BOOLEAN, _("%s: Unable to get argument #%d"), "getScalarBoolean", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(stackapi_isScalar(_pvCtx, _piAddress) == 0)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SCALAR_BOOLEAN, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "getScalarBoolean", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(_piBool != NULL)
	{
		*_piBool	= piBool[0];
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarBoolean(void* _pvCtx, int _iVar, int _iBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piBool = NULL;

	sciErr = stackapi_allocMatrixOfBoolean(_pvCtx, _iVar, 1, 1, &piBool);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createScalarBoolean");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	piBool[0] = _iBool;
	return 0;
}
/*--------------------------------------------------------------------------*/

