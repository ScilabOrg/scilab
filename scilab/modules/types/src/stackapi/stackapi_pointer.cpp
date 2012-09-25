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
#include "localization.h"

#include "MALLOC.h"
SciErr stackapi_getPointer(void* _pvCtx, int* _piAddress, void** _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType = 0;
	double *pdblTmp = NULL;

	if(	_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getPointer");
		return sciErr;
	}

	sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POINTER, _("%s: Unable to get argument #%d"), "stackapi_getPointer", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}
	
	if(iType != sci_pointer)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "stackapi_getPointer", _("pointer"));
		return sciErr;
	}
	
	pdblTmp = (double*)(_piAddress + 4);

	*_pvPtr = (void*)((unsigned long int)(*pdblTmp));
	return sciErr;
}

SciErr stackapi_fillPointer(void* _pvCtx, int *_piAddress, void** _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_fillPointer");
		return sciErr;
	}

	_piAddress[0] = sci_pointer;
	_piAddress[1] = 1;
	_piAddress[2] = 1;
	_piAddress[3] = 0;

	*_pvPtr = _piAddress + 4;

	return sciErr;
}

SciErr stackapi_allocPointer(void* _pvCtx, int _iVar, void** _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int* piAddr			= NULL;
	void* pvPtr			= NULL;

	int iMemSize = 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = stackapi_fillPointer(_pvCtx, piAddr, &pvPtr);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_POINTER, _("%s: Unable to create variable in Scilab memory"), "allocPointer");
		return sciErr;
	}

	*_pvPtr = pvPtr;
	stackapi_updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
	stackapi_updateLstk(iNewPos, sadr(iadr(iAddr) + 4), 2);

	return sciErr;
}

SciErr stackapi_createPointer(void* _pvCtx, int _iVar, void* _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	void* pvPtr		= NULL;

	sciErr = stackapi_allocPointer(_pvCtx, _iVar, &pvPtr);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POINTER, _("%s: Unable to create variable in Scilab memory"), "createPointer");
		return sciErr;
	}

	((double*)pvPtr)[0] = (double) ((unsigned long int)_pvPtr);

	return sciErr;
}
/*--------------------------------------------------------------------------*/
int isPointerType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_pointer);
}
/*--------------------------------------------------------------------------*/

