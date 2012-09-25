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

#include <string>
#include <stdlib.h>
#include "stackapi_scilab.h"
#include "stackapi_internal_common.h"
#include "stackinfo.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

/*Global structure for scilab 5.x*/
extern "C"
{
    StrCtx *stackapi_pvApiCtx = NULL;
}

/*--------------------------------------------------------------------------*/
#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz)
/*--------------------------------------------------------------------------*/
/* Replaces Rhs */
int* stackapi_getNbInputArgument(void* _pvCtx)
{
    return &C2F(com).rhs;
}

/* Replaces Top */
int* stackapi_getNbArgumentOnStack(void* _pvCtx)
{
    return &C2F(vstk).top;
}

/* Replaces Lhs */
int* stackapi_getNbOutputArgument(void* _pvCtx)
{
    return &C2F(com).lhs;
}

int* stackapi_assignOutputVariable(void* _pvCtx, int _iVal)
{
    return &(C2F(intersci).lhsvar[_iVal - 1]);
}

int stackapi_returnArguments(void* _pvCtx)
{
    return C2F(putlhsvar)();
}

int stackapi_checkInputArgument(void* _pvCtx, int _iMin, int _iMax)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_iMin <= stackapi_nbInputArgument(_pvCtx) && _iMax >= stackapi_nbInputArgument(_pvCtx))
    {
        return 1;
    }

    if (_iMax == _iMin)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMax);
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMin, _iMax);
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_checkInputArgumentAtLeast(void* _pvCtx, int _iMin)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_iMin <= stackapi_nbInputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(77, _("%s: Wrong number of input argument(s): at least %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMin);
    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_checkInputArgumentAtMost(void* _pvCtx, int _iMax)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_iMax >= stackapi_nbInputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(77, _("%s: Wrong number of input argument(s): at most %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMax);
    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_checkOutputArgument(void* _pvCtx, int _iMin, int _iMax)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;


    if (_iMin <= stackapi_nbOutputArgument(_pvCtx) && _iMax >= stackapi_nbOutputArgument(_pvCtx))
    {
        return 1;
    }

    if (_iMax == _iMin)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMax);
    }
    else
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMin, _iMax);
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_checkOutputArgumentAtLeast(void* _pvCtx, int _iMin)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_iMin <= stackapi_nbOutputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(78, _("%s: Wrong number of output argument(s): at least %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMin);
    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_checkOutputArgumentAtMost(void* _pvCtx, int _iMax)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_iMax >= stackapi_nbOutputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(78, _("%s: Wrong number of output argument(s): at most %d expected.\n"), ((StrCtx *) _pvCtx)->pstName, _iMax);
    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_callOverloadFunction(void* _pvCtx, int _iVar, char* _pstName, unsigned int _iNameLen)
{
    int iVar = 0;
    if(_iVar != 0)
    {
        iVar = _iVar + Top - Rhs;
    }

    return C2F(overload)(&iVar, _pstName, _iNameLen);
}

/*--------------------------------------------------------------------------*/
SciErr stackapi_getVarDimension(void *_pvCtx, int *_piAddress, int *_piRows, int *_piCols)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_piAddress != NULL && stackapi_isVarMatrixType(_pvCtx, _piAddress))
    {
        *_piRows = _piAddress[1];
        *_piCols = _piAddress[2];
    }
    else
    {
        *_piRows = 0;
        *_piCols = 0;
        if (_piAddress == NULL)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getVarDimension");
        }
        else
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_NOT_MATRIX_TYPE, _("%s: matrix argument excepted"), "stackapi_getVarDimension");
        }
    }
    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr stackapi_getVarAddressFromPosition(void *_pvCtx, int _iVar, int **_piAddress)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iAddr = 0;
    int iValType = 0;

    /* we accept a call to stackapi_getVarAddressFromPosition after a create... call */
    if (_iVar > Top && _iVar > Nbvars)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POSITION, _("%s: bad call to %s! (1rst argument).\n"), ((StrCtx *) _pvCtx)->pstName,
                        "stackapi_getVarAddressFromPosition");
        return sciErr;
    }

    iAddr = iadr(*Lstk(Top - Rhs + _iVar));
    iValType = *istk(iAddr);
    if (iValType < 0)
    {
        iAddr = iadr(*istk(iAddr + 1));
    }

    *_piAddress = istk(iAddr);
    intersci_.ntypes[_iVar - 1] = '$';
    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr stackapi_getVarNameFromPosition(void *_pvCtx, int _iVar, char *_pstName)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    return sciErr;
}

/*--------------------------------------------------------------------------*/
int stackapi_getNewVarAddressFromPosition(void *_pvCtx, int _iVar, int **_piAddress)
{
    int iAddr = iadr(*Lstk(_iVar));
    *_piAddress = istk(iAddr);
    return 0;
}
/*--------------------------------------------------------------------------*/
SciErr stackapi_getVarType(void *_pvCtx, int *_piAddress, int *_piType)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    if (_piAddress == NULL)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "stackapi_getVarType");
        return sciErr;
    }

    *_piType = _piAddress[0];
    return sciErr;
}

/*--------------------------------------------------------------------------*/
int stackapi_isVarComplex(void *_pvCtx, int *_piAddress)
{
    int iType = 0;
    int iComplex = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    stackapi_getVarType(_pvCtx, _piAddress, &iType);
    switch (iType)
    {
        case sci_matrix:
        case sci_poly:
        case sci_sparse:
            iComplex = _piAddress[3];
            break;
        default:
            iComplex = 0;
    }
    return iComplex;
}
/*--------------------------------------------------------------------------*/
int stackapi_updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress)
{
    intersci_.ntypes[_iVar - 1] = _cType;
    intersci_.iwhere[_iVar - 1] = _iSCIAddress;
    intersci_.lad[_iVar - 1] = _iSCIDataAddress;
    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_updateLstk(int _iNewpos, int _iSCIDataAddress, int _iVarSize)
{
    *Lstk(_iNewpos + 1) = _iSCIDataAddress + _iVarSize;
    return 0;
}

/*--------------------------------------------------------------------------*/
int stackapi_isVarMatrixType(void *_pvCtx, int *_piAddress)
{
    if (_piAddress != NULL)
    {
        int iType = 0;

        stackapi_getVarType(_pvCtx, _piAddress, &iType);

        switch (iType)
        {
            case sci_matrix:
            case sci_poly:
            case sci_boolean:
            case sci_sparse:
            case sci_boolean_sparse:
            case sci_matlab_sparse:
            case sci_ints:
            case sci_handles:
            case sci_strings:
                return 1;
            default:
                return 0;
        }
    }
    else
    {
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
SciErr stackapi_getProcessMode(void *_pvCtx, int _iPos, int *_piAddRef, int *_piMode)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iRows1 = 0;
    int iCols1 = 0;
    int iRows2 = 0;
    int iCols2 = 0;
    int iType2 = 0;
    int iMode = 0;
    int *piAddr2 = NULL;

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddRef, &iRows1, &iCols1);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
        return sciErr;
    }

    sciErr = stackapi_getVarAddressFromPosition(_pvCtx, _iPos, &piAddr2);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get variable address"), "getProcessMode");
        return sciErr;
    }

    sciErr = stackapi_getVarType(_pvCtx, piAddr2, &iType2);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument type"), "getProcessMode");
        return sciErr;
    }

    if (iType2 == sci_matrix && !stackapi_isVarComplex(_pvCtx, piAddr2))
    {
        double *pdblReal2 = NULL;

        sciErr = stackapi_getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblReal2);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
            return sciErr;
        }

        if (iRows2 != 1 || iCols2 != 1)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1,
                            1);
            return sciErr;
        }

        iMode = (int)pdblReal2[0];
    }
    else if (iType2 == sci_strings)
    {
        int iLen = 0;
        char *pstMode[1] = { "" };

        sciErr = stackapi_getVarDimension(_pvCtx, piAddr2, &iRows2, &iCols2);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
            return sciErr;
        }

        if (iRows2 != 1 || iCols2 != 1)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1,
                            1);
            return sciErr;
        }

        sciErr = stackapi_getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, NULL);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
            return sciErr;
        }

        pstMode[0] = (char *)MALLOC(sizeof(char) * (iLen + 1)); //+1 for null termination
        sciErr = stackapi_getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, pstMode);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
            return sciErr;
        }

        iMode = (int)pstMode[0][0];
        FREE(pstMode[0]);
    }
    else
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"),
            "getProcessMode", _iPos);
        return sciErr;
    }

    if (iMode == ROW_LETTER || iMode == BY_ROWS)
        *_piMode = BY_ROWS;
    else if (iMode == COL_LETTER || iMode == BY_COLS)
        *_piMode = BY_COLS;
    else if (iMode == STAR_LETTER || iMode == BY_ALL)
        *_piMode = BY_ALL;
    else if (iMode == MTLB_LETTER || iMode == BY_MTLB)
    {
        *_piMode = 0;
        if (iRows1 > 1)
            *_piMode = 1;
        else if (iCols1 > 1)
            *_piMode = 2;
    }
    else
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_PROCESSMODE, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "getProcessMode",
            _iPos, "'*', 'r', 'c', 'm', '0', '1', '2'", "-1");
        return sciErr;
    }
    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr stackapi_getDimFromVar(void *_pvCtx, int *_piAddress, int *_piVal)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iType = 0;
    int iRows = 0;
    int iCols = 0;
    double *pdblReal = NULL;

    sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument type"), "stackapi_getDimFromVar");
        return sciErr;
    }

    if (iType == sci_matrix)
    {
        if (stackapi_isVarComplex(_pvCtx, _piAddress))
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "stackapi_getDimFromVar",
                            stackapi_getRhsFromAddress(_pvCtx, _piAddress));
            return sciErr;
        }

        sciErr = stackapi_getMatrixOfDouble(_pvCtx, _piAddress, &iRows, &iCols, &pdblReal);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
            return sciErr;
        }

        *_piVal = (int)Max(pdblReal[0], 0);
    }
    else if (iType == sci_ints)
    {
        int iPrec = 0;

        sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument dimension"), "stackapi_getDimFromVar");
            return sciErr;
        }

        if (iRows != 1 || iCols != 1)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode",
                            stackapi_getRhsFromAddress(_pvCtx, _piAddress), 1, 1);
            return sciErr;
        }

        sciErr = stackapi_getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
        if (sciErr.iErr)
        {
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument precision"), "stackapi_getDimFromVar");
            return sciErr;
        }

        switch (iPrec)
        {
            case SCI_INT8:
            {
                char *pcData = NULL;

                sciErr = stackapi_getMatrixOfInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pcData);
                if (sciErr.iErr)
                {
                    stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
                    return sciErr;
                }
                *_piVal = pcData[0];
            }
            break;
            case SCI_UINT8:
            {
                unsigned char *pucData = NULL;

                sciErr = stackapi_getMatrixOfUnsignedInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pucData);
                if (sciErr.iErr)
                {
                    stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
                    return sciErr;
                }
                *_piVal = pucData[0];
            }
            break;
            case SCI_INT16:
            {
                short *psData = NULL;

                sciErr = stackapi_getMatrixOfInteger16(_pvCtx, _piAddress, &iRows, &iCols, &psData);
                if (sciErr.iErr)
                {
                    stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
                    return sciErr;
                }
                *_piVal = psData[0];
            }
            break;
            case SCI_UINT16:
            {
                unsigned short *pusData = NULL;

                sciErr = stackapi_getMatrixOfUnsignedInteger16(_pvCtx, _piAddress, &iRows, &iCols, &pusData);
                if (sciErr.iErr)
                {
                    stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
                    return sciErr;
                }
                *_piVal = pusData[0];
            }
            break;
            case SCI_INT32:
            {
                int *piData = NULL;

                sciErr = stackapi_getMatrixOfInteger32(_pvCtx, _piAddress, &iRows, &iCols, &piData);
                if (sciErr.iErr)
                {
                    stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
                    return sciErr;
                }
                *_piVal = piData[0];
            }
            break;
            case SCI_UINT32:
            {
                unsigned int *puiData = NULL;

                sciErr = stackapi_getMatrixOfUnsignedInteger32(_pvCtx, _piAddress, &iRows, &iCols, &puiData);
                if (sciErr.iErr)
                {
                    stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "stackapi_getDimFromVar");
                    return sciErr;
                }
                *_piVal = puiData[0];
            }
            break;
        }
    }
    else
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for input argument #%d: A real scalar or an integer scalar expected.\n"),
                        "stackapi_getDimFromVar", stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }
    return sciErr;
}
/*--------------------------------------------------------------------------*/
int stackapi_getRhsFromAddress(void *_pvCtx, int *_piAddress)
{
    int i = 0;
    int *piAddr = NULL;

    for (i = 0; i < Rhs; i++)
    {
        stackapi_getVarAddressFromPosition(_pvCtx, i + 1, &piAddr);
        if (_piAddress == piAddr)
        {
            return i + 1;
        }
    }
    return 0;
}

/*short cut functions*/

/*--------------------------------------------------------------------------*/
int stackapi_isRowVector(void *_pvCtx, int *_piAddress)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (stackapi_isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_IS_ROW_VECTOR, _("%s: Unable to get argument dimension"), "isRowVector");
        stackapi_printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (iRows == 1 && iCols > 1)
    {
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_isColumnVector(void *_pvCtx, int *_piAddress)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (stackapi_isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_IS_COLUMN_VECTOR, _("%s: Unable to get argument dimension"), "isColumnVector");
        stackapi_printError(&sciErr, 0);
        return 0;
    }

    if (iCols == 1 && iRows > 1)
    {
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_isVector(void *_pvCtx, int *_piAddress)
{
    return stackapi_isRowVector(_pvCtx, _piAddress) || stackapi_isColumnVector(_pvCtx, _piAddress);
}
/*--------------------------------------------------------------------------*/
int stackapi_isScalar(void *_pvCtx, int *_piAddress)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (stackapi_isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_IS_SCALAR, _("%s: Unable to get argument dimension"), "stackapi_isScalar");
        stackapi_printError(&sciErr, 0);
        return 0;
    }

    if (iCols == 1 && iRows == 1)
    {
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_isSquareMatrix(void *_pvCtx, int *_piAddress)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (stackapi_isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_IS_SQUARE, _("%s: Unable to get argument dimension"), "isSquareMatrix");
        stackapi_printError(&sciErr, 0);
        return 0;
    }

    if (iRows > 1 && iCols == iRows)
    {
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_checkVarDimension(void *_pvCtx, int *_piAddress, int _iRows, int _iCols)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (stackapi_isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    sciErr = stackapi_getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CHECK_VAR_DIMENSION, _("%s: Unable to get argument dimension"), "checkVarDimension");
        stackapi_printError(&sciErr, 0);
        return 0;
    }

    if ((_iRows == iRows || _iRows == -1) && (_iCols == iCols || _iCols == -1))
    {
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_checkVarType(void *_pvCtx, int *_piAddress, int _iType)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    int iType = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (iType == _iType)
    {
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_isEmptyMatrix(void *_pvCtx, int *_piAddress)
{
    if (stackapi_checkVarType(_pvCtx, _piAddress, sci_matrix))
    {
        return stackapi_checkVarDimension(_pvCtx, _piAddress, 0, 0);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int stackapi_createEmptyMatrix(void *_pvCtx, int _iVar)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;
    double dblReal = 0;

    sciErr = stackapi_createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        stackapi_printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
