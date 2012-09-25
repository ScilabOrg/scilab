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
#include "stackapi_internal_int.h"
#include "localization.h"

static int stackapi_getCommonScalarInteger(void* _pvCtx, int* _piAddress, int _iPrec, void** _pvData);

SciErr stackapi_getMatrixOfIntegerPrecision(void* _pvCtx, int* _piAddress, int* _piPrecision)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfIntegerPrecision");
		return sciErr;
	}

	if(_piAddress[0] != sci_ints)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "stackapi_getMatrixOfIntegerPrecision", _("int matrix"));
		return sciErr;
	}

	*_piPrecision = _piAddress[3];
	return sciErr;
}

SciErr stackapi_getMatrixOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned char** _pucData8)
{
	return stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT8, _piRows, _piCols, (void**)_pucData8);
}

SciErr stackapi_getMatrixOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned short** _pusData16)
{
	return stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT16, _piRows, _piCols, (void**)_pusData16);
}

SciErr stackapi_getMatrixOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned int** _puiData32)
{
	return stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT32, _piRows, _piCols, (void**)_puiData32);
}

SciErr stackapi_getMatrixOfInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char** _pcData8)
{
	return stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT8, _piRows, _piCols, (void**)_pcData8);
}

SciErr stackapi_getMatrixOfInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, short** _psData16)
{
	return stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT16, _piRows, _piCols, (void**)_psData16);
}

SciErr stackapi_getMatrixOfInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piData32)
{
	return stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT32, _piRows, _piCols, (void**)_piData32);
}

SciErr stackapi_getCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iPrec		= 0;

	if(_piAddress == NULL)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getMatrixOfInteger");
		return sciErr;
	}

	sciErr = stackapi_getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfInteger", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iPrec != _iPrecision)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "stackapi_getMatrixOfInteger", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	*_piRows = _piAddress[1];
	*_piCols = _piAddress[2];

	*_piData	= (void*)(_piAddress + 4);
	return sciErr;
}

SciErr stackapi_createMatrixOfUnsigedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned char* _pucData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned char *pucData8	    = NULL;
	int iSize                   = _iRows * _iCols;

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

    sciErr = stackapi_allocMatrixOfUnsignedInteger8(_pvCtx, _iVar, _iRows, _iCols, &pucData8);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfUnsigedInteger8");
		return sciErr;
	}

	memcpy(pucData8, _pucData8, sizeof(unsigned char) * iSize);
	return sciErr;
}

SciErr stackapi_createMatrixOfUnsigedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned short* _pusData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned short *psData16    = NULL;
	int iSize                   = _iRows * _iCols;

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

	sciErr = stackapi_allocMatrixOfUnsignedInteger16(_pvCtx, _iVar, _iRows, _iCols, &psData16);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfUnsigedInteger16");
		return sciErr;
	}

	memcpy(psData16, _pusData16, sizeof(unsigned short) * iSize);
	return sciErr;
}

SciErr stackapi_createMatrixOfUnsigedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned int* _puiData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned int *piData32  = NULL;
	int iSize               = _iRows * _iCols;

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

	sciErr = stackapi_allocMatrixOfUnsignedInteger32(_pvCtx, _iVar, _iRows, _iCols, &piData32);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfUnsigedInteger32");
		return sciErr;
	}

	memcpy(piData32, _puiData32, sizeof(unsigned int) * iSize);
	return sciErr;
}

SciErr stackapi_createMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* _pcData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char *pcData8   = NULL;
	int iSize       = _iRows * _iCols;

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

	sciErr = stackapi_allocMatrixOfInteger8(_pvCtx, _iVar, _iRows, _iCols, &pcData8);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfInteger8");
		return sciErr;
	}

	memcpy(pcData8, _pcData8, sizeof(char) * iSize);
	return sciErr;
}

SciErr stackapi_createMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, const short* _psData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	short *psData16 = NULL;
	int iSize       = _iRows * _iCols;

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

	sciErr = stackapi_allocMatrixOfInteger16(_pvCtx, _iVar, _iRows, _iCols, &psData16);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfInteger16");
		return sciErr;
	}

	memcpy(psData16, _psData16, sizeof(short) * iSize);
	return sciErr;
}

SciErr stackapi_createMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piData32   = NULL;
	int iSize       = _iRows * _iCols;

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

	sciErr = stackapi_allocMatrixOfInteger32(_pvCtx, _iVar, _iRows, _iCols, &piData32);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfInteger32");
		return sciErr;
	}

	memcpy(piData32, _piData32, sizeof(int) * iSize);
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr stackapi_createMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, const ong long* _pllData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	long long  *pllData64   = NULL;
	int iSize               = _iRows * _iCols;

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

	sciErr = allocMatrixOfInteger64(_iVar, _iRows, _iCols, &pllData64);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "stackapi_createMatrixOfInteger64");
		return sciErr;
	}

	memcpy(pllData64, _pllData64, sizeof(long long) * iSize);
	return sciErr;
}
#endif

SciErr stackapi_allocMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pcData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr		= NULL;
	char *pcData8	= NULL;
	int iNewPos		= Top - Rhs + _iVar;

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

	sciErr = stackapi_allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT8, _iRows, _iCols, (void**)&pcData8);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger8");
		return sciErr;
	}

	*_pcData8 = pcData8;
	return sciErr;
}

SciErr stackapi_allocMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short** _psData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr     = NULL;
	short *psData16 = NULL;
	int iNewPos	    = Top - Rhs + _iVar;

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

	sciErr = stackapi_allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT16, _iRows, _iCols, (void**)&psData16);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger16");
		return sciErr;
	}

	*_psData16	= psData16;
	return sciErr;
}

SciErr stackapi_allocMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr		= NULL;
	int *piData32	= NULL;
	int iNewPos		= Top - Rhs + _iVar;

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

	sciErr = stackapi_allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT32, _iRows, _iCols, (void**)&piData32);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger32");
		return sciErr;
	}

	*_piData32	= piData32;
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr stackapi_allocMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _piData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr             = NULL;
	long long *pllData64    = NULL;
	int iNewPos             = Top - Rhs + _iVar;

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

	stackapi_getNewVarAddressFromPosition(iNewPos, &piAddr);

	sciErr = stackapi_allocCommonMatrixOfInteger(_iVar, piAddr, SCI_INT64, _iRows, _iCols, (void**)&piData32);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger64");
		return sciErr;
	}

	*_pllData64	= pllData64;
	return sciErr;
}
#endif

SciErr stackapi_allocMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char** _pucData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr             = NULL;
	unsigned char *pucData8 = NULL;
	int iNewPos             = Top - Rhs + _iVar;

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

	sciErr = stackapi_allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT8, _iRows, _iCols, (void**)&pucData8);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger8");
		return sciErr;
	}

	*_pucData8		= pucData8;
	return sciErr;
}

SciErr stackapi_allocMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short** _pusData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr                 = NULL;
	unsigned short *pusData16   = NULL;
	int iNewPos                 = Top - Rhs + _iVar;

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

	sciErr = stackapi_allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT16, _iRows, _iCols, (void**)&pusData16);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger16");
		return sciErr;
	}

	*_pusData16	= pusData16;
	return sciErr;
}

SciErr stackapi_allocMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int** _puiData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr             = NULL;
	unsigned int *puiData32 = NULL;
	int iNewPos             = Top - Rhs + _iVar;

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

	sciErr = stackapi_allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT32, _iRows, _iCols, (void**)&puiData32);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger32");
		return sciErr;
	}

	*_puiData32	= puiData32;
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr stackapi_allocMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned long long** _pullData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr                     = NULL;
	unsigned long long *pullData64  = NULL;
	int iNewPos                     = Top - Rhs + _iVar;

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

	stackapi_getNewVarAddressFromPosition(iNewPos, &piAddr);

	sciErr = stackapi_allocCommonMatrixOfInteger(_iVar, piAddr, SCI_UINT64, _iRows, _iCols, (void**)&puiData32);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger64");
		return sciErr;
	}

	*_pullData64	= pullData64;
	return sciErr;
}
#endif

SciErr stackapi_allocCommonMatrixOfInteger(void* _pvCtx, int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos     = Top - Rhs + _iVar;
	int iAddr       = *Lstk(iNewPos);
	int iRate       = (sizeof(double) / (_iPrecision % 10));
	int iSize       = _iRows * _iCols;
	int iDouble     = iSize / iRate;
	int iMod        = (iSize % iRate) == 0 ? 0 : 1;
	int iTotalSize  = iDouble + iMod;

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

	int iMemSize    = iTotalSize + 2;
	int iFreeSpace  = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	stackapi_fillCommonMatrixOfInteger(_pvCtx, _piAddress, _iPrecision, _iRows, _iCols, pvData);
	stackapi_updateInterSCI(_iVar, '$', iAddr, iAddr + 4);
	stackapi_updateLstk(iNewPos, iAddr + 4, iTotalSize );
	return sciErr;
}

SciErr stackapi_fillCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

    _piAddress[0]   = sci_ints;
	_piAddress[1]   = Min(_iRows, _iRows * _iCols);
	_piAddress[2]   = Min(_iCols, _iRows * _iCols);
	_piAddress[3]   = _iPrecision;
	*_pvData        = (void*)(_piAddress + 4);

	return sciErr;
}

/* shortcut functions */

int stackapi_isIntegerType(void* _pvCtx, int* _piAddress)
{
	return stackapi_checkVarType(_pvCtx, _piAddress, sci_ints);
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarInteger8(void* _pvCtx, int* _piAddress, char* _pcData)
{
	char* pcData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT8, (void**)&pcData);
	if(iRet)
	{
		return iRet;
	}

	*_pcData = pcData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarInteger16(void* _pvCtx, int* _piAddress, short* _psData)
{
	short* psData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT16, (void**)&psData);
	if(iRet)
	{
		return iRet;
	}

	*_psData = psData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarInteger32(void* _pvCtx, int* _piAddress, int* _piData)
{
	int* piData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT32, (void**)&piData);
	if(iRet)
	{
		return iRet;
	}

	*_piData = piData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int stackapi_getScalarInteger64(void* _pvCtx, int* _piAddress, long long* _pllData)
{
	long long* pllData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT64, (void**)&pllData);
	if(iRet)
	{
		return iRet;
	}

	*_pllData = pllData[0];
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int stackapi_getScalarUnsignedInteger8(void* _pvCtx, int* _piAddress, unsigned char* _pucData)
{
	unsigned char* pucData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT8, (void**)&pucData);
	if(iRet)
	{
		return iRet;
	}

	*_pucData = pucData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarUnsignedInteger16(void* _pvCtx, int* _piAddress, unsigned short* _pusData)
{
	unsigned short* pusData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT16, (void**)&pusData);
	if(iRet)
	{
		return iRet;
	}

	*_pusData = pusData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarUnsignedInteger32(void* _pvCtx, int* _piAddress, unsigned int* _puiData)
{
	unsigned int* puiData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT32, (void**)&puiData);
	if(iRet)
	{
		return iRet;
	}

	*_puiData = puiData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int stackapi_getScalarUnsignedInteger64(void* _pvCtx, int* _piAddress, unsigned long long* _pullData)
{
	unsigned long long* pullData = NULL;

	int iRet = stackapi_getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT64, (void**)&pullData);
	if(iRet)
	{
		return iRet;
	}

	*_pullData = pullData[0];
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
static int stackapi_getCommonScalarInteger(void* _pvCtx, int* _piAddress, int _iPrec, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRows	= 0;
	int iCols	= 0;

	if(stackapi_isScalar(_pvCtx, _piAddress) == 0)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SCALAR_INTEGER, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "getScalarInteger", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = stackapi_getCommonMatrixOfInteger(_pvCtx, _piAddress, _iPrec, &iRows, &iCols, _pvData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SCALAR_INTEGER, _("%s: Unable to get argument #%d"), "getScalarInteger", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarInteger8(void* _pvCtx, int _iVar, char _cData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfInteger8(_pvCtx, _iVar, 1, 1, &_cData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger8");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarInteger16(void* _pvCtx, int _iVar, short _sData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfInteger16(_pvCtx, _iVar, 1, 1, &_sData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger16");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarInteger32(void* _pvCtx, int _iVar, int _iData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfInteger32(_pvCtx, _iVar, 1, 1, &_iData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger32");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int stackapi_createScalarInteger64(void* _pvCtx, int _iVar, long long _llData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfInteger64(_pvCtx, _iVar, 1, 1, _llData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger64");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int stackapi_createScalarUnsignedInteger8(void* _pvCtx, int _iVar, unsigned char _ucData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfUnsigedInteger8(_pvCtx, _iVar, 1, 1, &_ucData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger8");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarUnsignedInteger16(void* _pvCtx, int _iVar, unsigned short _usData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfUnsigedInteger16(_pvCtx, _iVar, 1, 1, &_usData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger16");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarUnsignedInteger32(void* _pvCtx, int _iVar, unsigned int _uiData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfUnsigedInteger32(_pvCtx, _iVar, 1, 1, &_uiData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger32");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int stackapi_createScalarUnsignedInteger64(void* _pvCtx, int _iVar, unsigned long long _ullData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = stackapi_createMatrixOfUnsigedInteger64(_pvCtx, _iVar, 1, 1, &_ullData);
	if(sciErr.iErr)
	{
		stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger64");
		stackapi_printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
