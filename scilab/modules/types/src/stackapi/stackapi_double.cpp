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
#include "stackapi_internal_double.h"
#include "localization.h"

/*******************************/
/*   double matrix functions   */
/*******************************/

static int stackapi_getCommonScalarDouble(void* _pvCtx, int* _piAddress, int _iComplex, double* _pdblReal, double* _pdblImg);
static int stackapi_createCommonScalarDouble(void* _pvCtx, int _iVar, int _iComplex, double _dblReal, double _dblImg);

SciErr stackapi_getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
{
    return stackapi_getCommonMatrixOfDouble(_pvCtx, _piAddress, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr stackapi_getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    return stackapi_getCommonMatrixOfDouble(_pvCtx, _piAddress, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

SciErr stackapi_getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal = NULL;
    double *pdblImg	 = NULL;

    sciErr = stackapi_getCommonMatrixOfDouble(_pvCtx, _piAddress, stackapi_isVarComplex(_pvCtx, _piAddress), _piRows, _piCols, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ZDOUBLE, _("%s: Unable to get argument #%d"), "getComplexZMatrixOfDouble", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    //*_pdblZ	= oGetDoubleComplexFromPointer(pdblReal, pdblImg, *_piRows * *_piCols);
    return sciErr;
}

SciErr stackapi_getCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iType = 0;
    if(	_piAddress == NULL)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfDouble" : "stackapi_getMatrixOfDouble");
        return sciErr;
    }

    sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
    if(sciErr.iErr || iType != sci_matrix)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "getComplexMatrixOfDouble" : "stackapi_getMatrixOfDouble", _("double matrix"));
        return sciErr;
    }

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfDouble" : "stackapi_getMatrixOfDouble", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if(_pdblReal != NULL)
    {
        *_pdblReal	= (double*)(_piAddress + 4);
    }
    if(stackapi_isVarComplex(_pvCtx, _piAddress) && _pdblImg != NULL)
    {
        *_pdblImg	= (double*)(_piAddress + 4) + *_piRows * *_piCols;
    }
    return sciErr;
}

SciErr stackapi_allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal	= NULL;

    sciErr = stackapi_allocCommonMatrixOfDouble(_pvCtx, _iVar, 0, _iRows, _iCols, &pdblReal, NULL);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfDouble");
        return sciErr;
    }

    *_pdblReal	= pdblReal;

    return sciErr;
}

SciErr stackapi_allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    sciErr = stackapi_allocCommonMatrixOfDouble(_pvCtx, _iVar, 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "stackapi_allocComplexMatrixOfDouble");
        return sciErr;
    }

    *_pdblReal	= pdblReal;
    *_pdblImg		= pdblImg;
    return sciErr;
}

SciErr stackapi_allocCommonMatrixOfDouble(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNewPos			= Top - Rhs + _iVar;
    int iAddr				= *Lstk(iNewPos);
    int* piAddr			= NULL;

    int iMemSize = _iRows * _iCols * (_iComplex + 1) + 2;
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
    if (iMemSize > iFreeSpace)
    {
        stackapi_addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
    stackapi_fillCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
    stackapi_updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
    stackapi_updateLstk(iNewPos, sadr(iadr(iAddr) + 4), _iRows * _iCols * (_iComplex + 1));
    return sciErr;
}

SciErr stackapi_fillCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    _piAddress[0]		= sci_matrix;
    _piAddress[1]		= Min(_iRows, _iRows * _iCols);
    _piAddress[2]		= Min(_iCols, _iRows * _iCols);
    _piAddress[3]		= _iComplex;


    if(_pdblReal != NULL)
    {
        *_pdblReal		= (double*)(_piAddress + 4);
    }

    if(_iComplex != 0 && _pdblImg != NULL)
    {
        *_pdblImg	= *_pdblReal + _iRows * _iCols;
    }

    return sciErr;
}

SciErr stackapi_createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal	= NULL;

    int iOne					= 1;
    int iSize					= _iRows * _iCols;

    sciErr = stackapi_allocMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "stackapi_allocComplexMatrixOfDouble");
        return sciErr;
    }

    C2F(dcopy)(&iSize, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
    return sciErr;
}

SciErr stackapi_createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    int iOne					= 1;
    int iSize					= _iRows * _iCols;

    sciErr = stackapi_allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "stackapi_allocComplexMatrixOfDouble");
        return sciErr;
    }

    C2F(dcopy)(&iSize, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
    if(_pdblImg)
    {
        C2F(dcopy)(&iSize, const_cast<double*>(_pdblImg), &iOne, pdblImg, &iOne);
    }
    else
    {//fill imag part with 0
        memset(pdblImg, 0x00, iSize * sizeof(double));
    }
    return sciErr;
}

SciErr stackapi_createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex* _pdblData)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal		= NULL;
    double *pdblImg			= NULL;


    sciErr = stackapi_allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_ZDOUBLE, _("%s: Unable to create variable in Scilab memory"), "stackapi_allocComplexMatrixOfDouble");
        return sciErr;
    }

    //vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);
    return sciErr;
}

/*shortcut functions*/

int stackapi_isDoubleType(void* _pvCtx, int* _piAddress)
{
    return stackapi_checkVarType(_pvCtx, _piAddress, sci_matrix);
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarDouble(void* _pvCtx, int* _piAddress, double* _pdblReal)
{
    return stackapi_getCommonScalarDouble(_pvCtx, _piAddress, 0, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int stackapi_getScalarComplexDouble(void* _pvCtx, int* _piAddress, double* _pdblReal, double* _pdblImg)
{
    return stackapi_getCommonScalarDouble(_pvCtx, _piAddress, 1, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int stackapi_getCommonScalarDouble(void* _pvCtx, int* _piAddress, int _iComplex, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iRows	= 0;
    int iCols	= 0;

    double* pdblReal = NULL;
    double* pdblImg	 = NULL;

    sciErr = stackapi_getCommonMatrixOfDouble(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SCALAR_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getScalarComplexDouble" : "getScalarDouble", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        stackapi_printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if(stackapi_isScalar(_pvCtx, _piAddress) == 0)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SCALAR_DOUBLE, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _iComplex ? "getScalarComplexDouble" : "getScalarDouble", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        stackapi_printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if(_pdblReal != NULL)
    {
        *_pdblReal	= pdblReal[0];
    }

    if(_pdblImg != NULL)
    {
        *_pdblImg		= pdblImg[0];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarDouble(void* _pvCtx, int _iVar, double _dblReal)
{
    return stackapi_createCommonScalarDouble(_pvCtx, _iVar, 0, _dblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int stackapi_createScalarComplexDouble(void* _pvCtx, int _iVar, double _dblReal, double _dblImg)
{
    return stackapi_createCommonScalarDouble(_pvCtx, _iVar, 1, _dblReal, _dblImg);
}
/*--------------------------------------------------------------------------*/
static int stackapi_createCommonScalarDouble(void* _pvCtx, int _iVar, int _iComplex, double _dblReal, double _dblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    sciErr = stackapi_allocCommonMatrixOfDouble(_pvCtx, _iVar, _iComplex, 1, 1, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SCALAR_DOUBLE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createScalarComplexDouble" : "createScalarDouble");
        stackapi_printError(&sciErr, 0);
        return sciErr.iErr;
    }

    pdblReal[0] = _dblReal;
    if(_iComplex)
    {
        pdblImg[0]	= _dblImg;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
