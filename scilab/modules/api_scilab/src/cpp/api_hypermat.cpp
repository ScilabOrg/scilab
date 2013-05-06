/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#include <stdio.h>
#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_double.h"
#include "localization.h"

#include "call_scilab.h"

int isHypermatType(void* _pvCtx, int* _piAddress)
{
    if (_piAddress[0] != sci_mlist || _piAddress[1] != 3)
    {
        // not a mlist or not containing 3 fields
        return 0;
    }

    if (_piAddress[6] != sci_strings || _piAddress[7] != 1 || _piAddress[8] != 3)
    {
        // first field is not a matrix 1x3 of strings
        return 0;
    }

    if (_piAddress[11] - 1 != 2 || _piAddress[14] != 17 || _piAddress[15] != 22)
    {
        // mlist type is not of length 2 or type is not "hm" ("hm" == [17 22])
        return 0;
    }

    return 1;
}

SciErr getHypermatType(void *_pvCtx, int *_piAddress, int *_piType)
{
    int * _piItemAddress = NULL;
    SciErr sciErr;

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &_piItemAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getVarType(_pvCtx, _piItemAddress, _piType);
}

SciErr getHypermatOfIntegerPrecision(void *_pvCtx, int *_piAddress, int *_piPrecision)
{
    int * _piItemAddress = NULL;
    SciErr sciErr;

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &_piItemAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfIntegerPrecision(_pvCtx, _piItemAddress, _piPrecision);
}

SciErr getHypermatDimensions(void *_pvCtx, int *_piAddress, int **_dims, int *_ndims)
{
    int * _piItemAddress = NULL;
    SciErr sciErr;
    int _rows = 0;

    sciErr = getListItemAddress(_pvCtx, _piAddress, 2, &_piItemAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfInteger32(_pvCtx, _piItemAddress, &_rows, _ndims, _dims);
}

SciErr getHypermatEntries(void* _pvCtx, int* _piAddress, int ** _piEntriesAddress)
{
    return getListItemAddress(_pvCtx, _piAddress, 3, _piEntriesAddress);
}

SciErr getHypermatOfDouble(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, double** _pdblReal)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfDouble(_pvCtx, entries, &_rows, &_cols, _pdblReal);
}

SciErr getComplexHypermatOfDouble(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getComplexMatrixOfDouble(_pvCtx, entries, &_rows, &_cols, _pdblReal, _pdblImg);
}

SciErr getHypermatOfInteger8(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, char** _pcData8)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfInteger8(_pvCtx, entries, &_rows, &_cols, _pcData8);
}

SciErr getHypermatOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned char** _pucData8)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfUnsignedInteger8(_pvCtx, entries, &_rows, &_cols, _pucData8);
}

SciErr getHypermatOfInteger16(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, short** _psData16)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfInteger16(_pvCtx, entries, &_rows, &_cols, _psData16);
}

SciErr getHypermatOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned short** _pusData16)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfUnsignedInteger16(_pvCtx, entries, &_rows, &_cols, _pusData16);
}

SciErr getHypermatOfInteger32(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int** _piData32)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfInteger32(_pvCtx, entries, &_rows, &_cols, _piData32);
}

SciErr getHypermatOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned int** _puiData32)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfUnsignedInteger32(_pvCtx, entries, &_rows, &_cols, _puiData32);
}

SciErr getHypermatOfBoolean(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int** _piBool)
{
    SciErr sciErr;
    int * entries = NULL;
    int _rows = 0;
    int _cols = 0;

    sciErr = getHypermatDimensions(_pvCtx, _piAddress, _dims, _ndims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return getMatrixOfBoolean(_pvCtx, entries, &_rows, &_cols, _piBool);
}

SciErr createEmptyHypermat(void *_pvCtx, int _iVar, const int * _dims, int _ndims, int ** _piAddress)
{
    static const char * fields[3] = {"hypermat", "dims", "entries"};
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createMList(_pvCtx, _iVar, 3, _piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, *_piAddress, 1, 1, 3, fields);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = createMatrixOfInteger32InList(_pvCtx, _iVar, *_piAddress, 2, 1, _ndims, _dims);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfDouble(void *_pvCtx, int _iVar, int * _dims, int _ndims, const double * _pdblReal)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _pdblReal);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfBoolean(void *_pvCtx, int _iVar, int * _dims, int _ndims, const int * _piBool)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfBooleanInList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _piBool);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfInteger8(void *_pvCtx, int _iVar, int * _dims, int _ndims, const char * _pcData8)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfInteger8InList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _pcData8);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfUnsignedInteger8(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned char * _pucData8)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfUnsignedInteger8InList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _pucData8);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfInteger16(void *_pvCtx, int _iVar, int * _dims, int _ndims, const short * _psData16)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfInteger16InList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _psData16);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfUnsignedInteger16(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned short * _pusData16)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfUnsignedInteger16InList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _pusData16);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfInteger32(void *_pvCtx, int _iVar, int * _dims, int _ndims, const int * _piData32)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfInteger32InList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _piData32);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}

SciErr createHypermatOfUnsignedInteger32(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned int * _puiData32)
{
    int * _piAddress = NULL;
    int iNbElements = 1;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = createEmptyHypermat(_pvCtx, _iVar, _dims, _ndims, &_piAddress);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    for (unsigned int i = 0; i < _ndims; i++)
    {
        iNbElements *= _dims[i];
    }

    sciErr = createMatrixOfUnsignedInteger32InList(_pvCtx, _iVar, _piAddress, 3, 1, iNbElements, _puiData32);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    return sciErr;
}
