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

#include "function.hxx"
#include "int.hxx"

extern "C"
{
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "localization.h"
#include "MALLOC.h"
#include "call_scilab.h"
#include "api_scilab.h"
#include "api_internal_int.h"
#include "api_internal_common.h"
#include "api_oldstack.h"
}
using namespace types;

SciErr getMatrixOfIntegerPrecision(void* _pvCtx, int* _piAddress, int* _piPrecision)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfIntegerPrecision");
		return sciErr;
	}

	if(!((InternalType*)_piAddress)->isInt())
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfIntegerPrecision", _("int matrix"));
		return sciErr;
	}

    switch(((InternalType*)_piAddress)->getType())
    {
    case GenericType::RealInt8 :
        *_piPrecision = SCI_INT8;
        break;
    case GenericType::RealUInt8 :
        *_piPrecision = SCI_UINT8;
        break;
    case GenericType::RealInt16 :
        *_piPrecision = SCI_INT16;
        break;
    case GenericType::RealUInt16 :
        *_piPrecision = SCI_UINT16;
        break;
    case GenericType::RealInt32 :
        *_piPrecision = SCI_INT32;
        break;
    case GenericType::RealUInt32 :
        *_piPrecision = SCI_UINT32;
        break;
    case GenericType::RealInt64 :
        *_piPrecision = SCI_INT64;
        break;
    case GenericType::RealUInt64 :
        *_piPrecision = SCI_UINT64;
        break;
    }
	return sciErr;
}

SciErr getMatrixOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned char** _pucData8)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT8, _piRows, _piCols, (void**)_pucData8);
}

SciErr getMatrixOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned short** _pusData16)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT16, _piRows, _piCols, (void**)_pusData16);
}

SciErr getMatrixOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned int** _puiData32)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT32, _piRows, _piCols, (void**)_puiData32);
}

#ifdef __SCILAB_INT64__
SciErr getMatrixOfUnsignedInteger64(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned long long** _pullData64)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT64, _piRows, _piCols, (void**)_pullData64);
}
#endif

SciErr getMatrixOfInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char** _pcData8)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT8, _piRows, _piCols, (void**)_pcData8);
}

SciErr getMatrixOfInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, short** _psData16)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT16, _piRows, _piCols, (void**)_psData16);
}

SciErr getMatrixOfInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piData32)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT32, _piRows, _piCols, (void**)_piData32);
}

#ifdef __SCILAB_INT64__
SciErr getMatrixOfInteger64(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, long long** _pllData64)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT64, _piRows, _piCols, (void**)_pllData64);
}
#endif

SciErr getCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iPrec		= 0;

	if(_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfInteger");
		return sciErr;
	}

	sciErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "getMatrixOfInteger", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iPrec != _iPrecision)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "getMatrixOfInteger", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "getMatrixOfInteger", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

    switch(((InternalType*)_piAddress)->getType())
    {
    case GenericType::RealInt8 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::Int8>()->get();
        break;
    case GenericType::RealUInt8 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::UInt8>()->get();
        break;
    case GenericType::RealInt16 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::Int16>()->get();
        break;
    case GenericType::RealUInt16 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::UInt16>()->get();
        break;
    case GenericType::RealInt32 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::Int32>()->get();
        break;
    case GenericType::RealUInt32 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::UInt32>()->get();
        break;
    case GenericType::RealInt64 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::Int64>()->get();
        break;
    case GenericType::RealUInt64 :
        *_piData	= (void*)((InternalType*)_piAddress)->getAs<types::UInt64>()->get();
        break;
    }
	return sciErr;
}

SciErr createMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned char* _pucData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned char *pucData8		= NULL;
	int iSize									= _iRows * _iCols;

	sciErr = allocMatrixOfUnsignedInteger8(_pvCtx, _iVar, _iRows, _iCols, &pucData8);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger8");
		return sciErr;
	}

	memcpy(pucData8, _pucData8, sizeof(unsigned char) * iSize);
	return sciErr;
}

SciErr createMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned short* _pusData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned short *psData16	= NULL;
	int iSize									= _iRows * _iCols;

	sciErr = allocMatrixOfUnsignedInteger16(_pvCtx, _iVar, _iRows, _iCols, &psData16);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger16");
		return sciErr;
	}

	memcpy(psData16, _pusData16, sizeof(unsigned short) * iSize);
	return sciErr;
}

SciErr createMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned int* _puiData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned int *puiData32	= NULL;
	int iSize			= _iRows * _iCols;

	sciErr = allocMatrixOfUnsignedInteger32(_pvCtx, _iVar, _iRows, _iCols, &puiData32);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger32");
		return sciErr;
	}

	memcpy(puiData32, _puiData32, sizeof(unsigned int) * iSize);
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr createMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned long long* _pullData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	unsigned long long *pullData64	= NULL;
	int iSize			= _iRows * _iCols;

	sciErr = allocMatrixOfUnsignedInteger64(_pvCtx, _iVar, _iRows, _iCols, &pullData64);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger64");
		return sciErr;
	}

	memcpy(pullData64, _pullData64, sizeof(unsigned long long) * iSize);
	return sciErr;
}
#endif

SciErr createMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* _pcData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char *pcData8	= NULL;
	int iSize			= _iRows * _iCols;

	sciErr = allocMatrixOfInteger8(_pvCtx, _iVar, _iRows, _iCols, &pcData8);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger8");
		return sciErr;
	}

	memcpy(pcData8, _pcData8, sizeof(char) * iSize);
	return sciErr;
}

SciErr createMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, const short* _psData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	short *psData16	= NULL;
	int iSize			= _iRows * _iCols;

	sciErr = allocMatrixOfInteger16(_pvCtx, _iVar, _iRows, _iCols, &psData16);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger16");
		return sciErr;
	}

	memcpy(psData16, _psData16, sizeof(short) * iSize);
	return sciErr;
}

SciErr createMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piData32	= NULL;
	int iSize			= _iRows * _iCols;

	sciErr = allocMatrixOfInteger32(_pvCtx, _iVar, _iRows, _iCols, &piData32);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger32");
		return sciErr;
	}

	memcpy(piData32, _piData32, sizeof(int) * iSize);
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr createMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, const long long* _pllData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	long long  *pllData64	= NULL;
	int iSize							= _iRows * _iCols;

	sciErr = allocMatrixOfInteger64(_pvCtx, _iVar, _iRows, _iCols, &pllData64);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger64");
		return sciErr;
	}

	memcpy(pllData64, _pllData64, sizeof(long long) * iSize);
	return sciErr;
}
#endif

SciErr allocMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pcData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr		= NULL;
	char *pcData8	= NULL;
	int iNewPos		= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT8, _iRows, _iCols, (void**)&pcData8);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger8");
		return sciErr;
	}

	*_pcData8		= pcData8;
	return sciErr;
}

SciErr allocMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short** _psData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr			= NULL;
	short *psData16	= NULL;
	int iNewPos			= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT16, _iRows, _iCols, (void**)&psData16);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger16");
		return sciErr;
	}

	*_psData16	= psData16;
	return sciErr;
}

SciErr allocMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr		= NULL;
	int *piData32	= NULL;
	int iNewPos		= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT32, _iRows, _iCols, (void**)&piData32);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger32");
		return sciErr;
	}

	*_piData32	= piData32;
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr allocMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _pllData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr						= NULL;
	long long *pllData64	= NULL;
	int iNewPos						= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT64, _iRows, _iCols, (void**)&pllData64);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger64");
		return sciErr;
	}

	*_pllData64	= pllData64;
	return sciErr;
}
#endif

SciErr allocMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char** _pucData8)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr							= NULL;
	unsigned char *pucData8	= NULL;
	int iNewPos						= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT8, _iRows, _iCols, (void**)&pucData8);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger8");
		return sciErr;
	}

	*_pucData8		= pucData8;
	return sciErr;
}

SciErr allocMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short** _pusData16)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr								= NULL;
	unsigned short *pusData16	= NULL;
	int iNewPos								= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT16, _iRows, _iCols, (void**)&pusData16);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger16");
		return sciErr;
	}

	*_pusData16	= pusData16;
	return sciErr;
}

SciErr allocMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int** _puiData32)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr							= NULL;
	unsigned int *puiData32	= NULL;
	int iNewPos						= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT32, _iRows, _iCols, (void**)&puiData32);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger32");
		return sciErr;
	}

	*_puiData32	= puiData32;
	return sciErr;
}

#ifdef __SCILAB_INT64__
SciErr allocMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned long long** _pullData64)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr											= NULL;
	unsigned long long *pullData64	= NULL;
	int iNewPos											= api_Top((int*)_pvCtx) - api_Rhs((int*)_pvCtx) + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT64, _iRows, _iCols, (void**)&pullData64);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger64");
		return sciErr;
	}

	*_pullData64	= pullData64;
	return sciErr;
}
#endif

SciErr allocCommonMatrixOfInteger(void* _pvCtx, int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

    if(_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "allocMatrixOfInteger");
        return sciErr;
    }

    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    InternalType** out = pStr->m_pOut;

    InternalType *pIT;
    switch(_iPrecision)
    {
    case SCI_INT8 :
        pIT = new Int8(_iRows, _iCols, (char**)_pvData);
        break;
    case SCI_UINT8 :
        pIT = new UInt8(_iRows, _iCols, (unsigned char**)_pvData);
        break;
    case SCI_INT16 :
        pIT = new Int16(_iRows, _iCols, (short**)_pvData);
        break;
    case SCI_UINT16 :
        pIT = new UInt16(_iRows, _iCols, (unsigned short**)_pvData);
        break;
    case SCI_INT32 :
        pIT = new Int32(_iRows, _iCols, (int**)_pvData);
        break;
    case SCI_UINT32 :
        pIT = new UInt32(_iRows, _iCols, (unsigned int**)_pvData);
        break;
    case SCI_INT64 :
        pIT = new Int64(_iRows, _iCols, (long long**)_pvData);
        break;
    case SCI_UINT64 :
        pIT = new UInt64(_iRows, _iCols, (unsigned long long**)_pvData);
        break;
    }

    if(pIT == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), "allocMatrixOfInteger");
        return sciErr;
    }

    int rhs = _iVar - api_Rhs((int*)_pvCtx);
    out[rhs - 1] = pIT;
    return sciErr;
}

SciErr fillCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	_piAddress[0]		= sci_ints;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iPrecision;


	*_pvData		= (void*)(_piAddress + 4);

	return sciErr;
}

SciErr createNamedMatrixOfUnsignedInteger8(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned char* _pucData8)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT8, _iRows, _iCols, _pucData8);
}

SciErr createNamedMatrixOfUnsignedInteger16(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned short* _pusData16)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT16, _iRows, _iCols, _pusData16);
}

SciErr createNamedMatrixOfUnsignedInteger32(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned int* _puiData32)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT32, _iRows, _iCols, _puiData32);
}

#ifdef __SCILAB_INT64__
SciErr createNamedMatrixOfUnsignedInteger64(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned long long* _pullData64)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT64, _iRows, _iCols, _pullData64);
}
#endif

SciErr createNamedMatrixOfInteger8(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const char* _pcData8)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT8, _iRows, _iCols, _pcData8);
}

SciErr createNamedMatrixOfInteger16(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const short* _psData16)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT16, _iRows, _iCols, _psData16);
}

SciErr createNamedMatrixOfInteger32(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const int* _piData32)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT32, _iRows, _iCols, _piData32);
}

#ifdef __SCILAB_INT64__
SciErr createNamedMatrixOfInteger64(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const long long* _pllData64)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT64, _iRows, _iCols, _pllData64);
}
#endif

SciErr createCommonNamedMatrixOfInteger(void* _pvCtx, const char* _pstName, int _iPrecision, int _iRows, int _iCols, const void* _pvData)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    //int iVarID[nsiz];
    //int iSaveRhs			= api_Rhs((int*)_pvCtx);
    //int iSaveTop			= api_Top((int*)_pvCtx);
    //int *piAddr				= NULL;
    //void *pvData			= NULL;

    //int iRate				= (sizeof(double) / (_iPrecision % 10));
    //int iSize				= _iRows * _iCols;
    //int iDouble			= iSize / iRate;
    //int iMod				= (iSize % iRate) == 0 ? 0 : 1;
    //int iTotalSize	= iDouble + iMod;

    //C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
    //Top = Top + Nbvars + 1;

    //int iMemSize = iTotalSize + 2;
    //int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(Top));
    //if (iMemSize > iFreeSpace)
    //{
    //    addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
    //    return sciErr;
    //}

    //getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

    ////write matrix information
    //fillCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _iRows, _iCols, &pvData);
    ////copy data in stack
    //memcpy(pvData, _pvData, (_iPrecision % 10) * iSize);

    ////update "variable index"
    //updateLstk(Top, *Lstk(Top) + 4, iTotalSize);

    ////Rhs = 0;
    ////Add name in stack reference list
    //createNamedVariable(iVarID);

    ////Top = iSaveTop;
    ////Rhs = iSaveRhs;

    return sciErr;
}

SciErr getNamedMatrixOfIntegerPrecision(void* _pvCtx, const char* _pstName, int* _piPrecision)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_INT_PRECISION, _("%s: Unable to get precision of variable \"%s\""), "getNamedMatrixOfIntegerPrecision", _pstName);
		return sciErr;
	}

	//check variable type
	if(piAddr[0] != sci_ints)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getNamedMatrixOfIntegerPrecision", _("int matrix"));
		return sciErr;
	}

	*_piPrecision = piAddr[3];
	return sciErr;
}

SciErr readNamedMatrixOfUnsignedInteger8(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned char* _pucData8)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT8, _piRows, _piCols, _pucData8);
}

SciErr readNamedMatrixOfUnsignedInteger16(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned short* _pusData16)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT16, _piRows, _piCols, _pusData16);
}

SciErr readNamedMatrixOfUnsignedInteger32(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned int* _puiData32)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT32, _piRows, _piCols, _puiData32);
}

#ifdef __SCILAB_INT64__
SciErr readNamedMatrixOfUnsignedInteger64(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned long long* _pullData64)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT64, _piRows, _piCols, _pullData64);
}
#endif

SciErr readNamedMatrixOfInteger8(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, char* _pcData8)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT8, _piRows, _piCols, _pcData8);
}

SciErr readNamedMatrixOfInteger16(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, short* _psData16)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT16, _piRows, _piCols, _psData16);
}

SciErr readNamedMatrixOfInteger32(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piData32)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT32, _piRows, _piCols, _piData32);
}

#ifdef __SCILAB_INT64__
SciErr readNamedMatrixOfInteger64(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, long long* _pllData64)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT64, _piRows, _piCols, _pllData64);
}
#endif

SciErr readCommonNamedMatrixOfInteger(void* _pvCtx, const char* _pstName, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int iSize					= 0;
	void* pvData			= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_INT, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfInteger", _pstName);
		return sciErr;
	}

	sciErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, &pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_INT, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfInteger", _pstName);
		return sciErr;
	}
	iSize = *_piRows * *_piCols;

	if(pvData == NULL || _pvData == NULL)
	{
		return sciErr;
	}

	memcpy(_pvData, pvData, (_iPrecision % 10) * iSize);
	return sciErr;
}

/* shortcut functions */

int isIntegerType(void* _pvCtx, int* _piAddress)
{
	return checkVarType(_pvCtx, _piAddress, sci_ints);
}
/*--------------------------------------------------------------------------*/
int isNamedIntegerType(void* _pvCtx, const char* _pstName)
{
	return checkNamedVarType(_pvCtx, _pstName, sci_ints);
}
/*--------------------------------------------------------------------------*/
int getScalarInteger8(void* _pvCtx, int* _piAddress, char* _pcData)
{
	char* pcData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT8, (void**)&pcData);
	if(iRet)
	{
		return iRet;
	}

	*_pcData = pcData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getScalarInteger16(void* _pvCtx, int* _piAddress, short* _psData)
{
	short* psData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT16, (void**)&psData);
	if(iRet)
	{
		return iRet;
	}

	*_psData = psData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getScalarInteger32(void* _pvCtx, int* _piAddress, int* _piData)
{
	int* piData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT32, (void**)&piData);
	if(iRet)
	{
		return iRet;
	}

	*_piData = piData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int getScalarInteger64(void* _pvCtx, int* _piAddress, long long* _pllData)
{
	long long* pllData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_INT64, (void**)&pllData);
	if(iRet)
	{
		return iRet;
	}

	*_pllData = pllData[0];
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int getScalarUnsignedInteger8(void* _pvCtx, int* _piAddress, unsigned char* _pucData)
{
	unsigned char* pucData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT8, (void**)&pucData);
	if(iRet)
	{
		return iRet;
	}

	*_pucData = pucData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getScalarUnsignedInteger16(void* _pvCtx, int* _piAddress, unsigned short* _pusData)
{
	unsigned short* pusData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT16, (void**)&pusData);
	if(iRet)
	{
		return iRet;
	}

	*_pusData = pusData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getScalarUnsignedInteger32(void* _pvCtx, int* _piAddress, unsigned int* _puiData)
{
	unsigned int* puiData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT32, (void**)&puiData);
	if(iRet)
	{
		return iRet;
	}

	*_puiData = puiData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int getScalarUnsignedInteger64(void* _pvCtx, int* _piAddress, unsigned long long* _pullData)
{
	unsigned long long* pullData = NULL;

	int iRet = getCommonScalarInteger(_pvCtx, _piAddress, SCI_UINT64, (void**)&pullData);
	if(iRet)
	{
		return iRet;
	}

	*_pullData = pullData[0];
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
static int getCommonScalarInteger(void* _pvCtx, int* _piAddress, int _iPrec, void** _pvData)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	if(isScalar(_pvCtx, _piAddress) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_INTEGER, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "getScalarInteger", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = getCommonMatrixOfInteger(_pvCtx, _piAddress, _iPrec, &iRows, &iCols, _pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_INTEGER, _("%s: Unable to get argument #%d"), "getScalarInteger", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarInteger8(void* _pvCtx, const char* _pstName, char* _pcData)
{
	char* pcData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_INT8, (void**)&pcData);
	if(iRet)
	{
		return iRet;
	}

	*_pcData = pcData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarInteger16(void* _pvCtx, const char* _pstName, short* _psData)
{
	short* psData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_INT16, (void**)&psData);
	if(iRet)
	{
		return iRet;
	}

	*_psData = psData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarInteger32(void* _pvCtx, const char* _pstName, int* _piData)
{
	int* piData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_INT32, (void**)&piData);
	if(iRet)
	{
		return iRet;
	}

	*_piData = piData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int getNamedScalarInteger64(void* _pvCtx, const char* _pstName, long long* _pllData)
{
	long long* pllData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_INT64, (void**)&pllData);
	if(iRet)
	{
		return iRet;
	}

	*_pllData = pllData[0];
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int getNamedScalarUnsignedInteger8(void* _pvCtx, const char* _pstName, unsigned char* _pucData)
{
	unsigned char* pucData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_UINT8, (void**)&pucData);
	if(iRet)
	{
		return iRet;
	}

	*_pucData = pucData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarUnsignedInteger16(void* _pvCtx, const char* _pstName, unsigned short* _pusData)
{
	unsigned short* pusData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_UINT16, (void**)&pusData);
	if(iRet)
	{
		return iRet;
	}

	*_pusData = pusData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarUnsignedInteger32(void* _pvCtx, const char* _pstName, unsigned int* _puiData)
{
	unsigned int* puiData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_UINT32, (void**)&puiData);
	if(iRet)
	{
		return iRet;
	}

	*_puiData = puiData[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int getNamedScalarUnsignedInteger64(void* _pvCtx, const char* _pstName, unsigned long long* _pullData)
{
	unsigned long long* pullData = NULL;

	int iRet = getCommandNamedScalarInteger(_pvCtx, _pstName, SCI_UINT64, (void**)&pullData);
	if(iRet)
	{
		return iRet;
	}

	*_pullData = pullData[0];
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
static int getCommandNamedScalarInteger(void* _pvCtx, const char* _pstName, int _iPrec, void** _pvData)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	if(isNamedScalar(_pvCtx, _pstName) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_INTEGER, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), "getNamedScalarInteger", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = readCommonNamedMatrixOfInteger(_pvCtx, _pstName, _iPrec, &iRows, &iCols, _pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_INTEGER, _("%s: Unable to get argument \"%s\""), "getNamedScalarInteger", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarInteger8(void* _pvCtx, int _iVar, char _cData)
{
	SciErr sciErr;

	sciErr = createMatrixOfInteger8(_pvCtx, _iVar, 1, 1, &_cData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger8");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarInteger16(void* _pvCtx, int _iVar, short _sData)
{
	SciErr sciErr;

	sciErr = createMatrixOfInteger16(_pvCtx, _iVar, 1, 1, &_sData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger16");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarInteger32(void* _pvCtx, int _iVar, int _iData)
{
	SciErr sciErr;

	sciErr = createMatrixOfInteger32(_pvCtx, _iVar, 1, 1, &_iData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger32");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int createScalarInteger64(void* _pvCtx, int _iVar, long long _llData)
{
	SciErr sciErr;

	sciErr = createMatrixOfInteger64(_pvCtx, _iVar, 1, 1, &_llData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarInteger64");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int createScalarUnsignedInteger8(void* _pvCtx, int _iVar, unsigned char _ucData)
{
	SciErr sciErr;

	sciErr = createMatrixOfUnsignedInteger8(_pvCtx, _iVar, 1, 1, &_ucData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger8");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarUnsignedInteger16(void* _pvCtx, int _iVar, unsigned short _usData)
{
	SciErr sciErr;

	sciErr = createMatrixOfUnsignedInteger16(_pvCtx, _iVar, 1, 1, &_usData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger16");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarUnsignedInteger32(void* _pvCtx, int _iVar, unsigned int _uiData)
{
	SciErr sciErr;
	unsigned int* puiData = NULL;

	sciErr = createMatrixOfUnsignedInteger32(_pvCtx, _iVar, 1, 1, &_uiData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger32");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int createScalarUnsignedInteger64(void* _pvCtx, int _iVar, unsigned long long _ullData)
{
	SciErr sciErr;

	sciErr = createMatrixOfUnsignedInteger64(_pvCtx, _iVar, 1, 1, &_ullData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createScalarUnsignedInteger64");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int createNamedScalarInteger8(void* _pvCtx, const char* _pstName, char _cData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfInteger8(_pvCtx, _pstName, 1, 1, &_cData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarInteger8");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarInteger16(void* _pvCtx, const char* _pstName, short _sData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfInteger16(_pvCtx, _pstName, 1, 1, &_sData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarInteger16");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarInteger32(void* _pvCtx, const char* _pstName, int _iData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfInteger32(_pvCtx, _pstName, 1, 1, &_iData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarInteger32");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int createNamedScalarInteger64(void* _pvCtx, const char* _pstName, long long _llData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfInteger64(_pvCtx, _pstName, 1, 1, &_llData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarInteger64");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
int createNamedScalarUnsignedInteger8(void* _pvCtx, const char* _pstName, unsigned char _ucData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfUnsignedInteger8(_pvCtx, _pstName, 1, 1, &_ucData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarUnsignedInteger8");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarUnsignedInteger16(void* _pvCtx, const char* _pstName, unsigned short _usData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfUnsignedInteger16(_pvCtx, _pstName, 1, 1, &_usData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarUnsignedInteger16");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarUnsignedInteger32(void* _pvCtx, const char* _pstName, unsigned int _uiData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfUnsignedInteger32(_pvCtx, _pstName, 1, 1, &_uiData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarUnsignedInteger32");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef __SCILAB_INT64__
int createNamedScalarUnsignedInteger64(void* _pvCtx, const char* _pstName, unsigned long long _ullData)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfUnsignedInteger64(_pvCtx, _pstName, 1, 1, &_ullData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_INT, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarUnsignedInteger64");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
