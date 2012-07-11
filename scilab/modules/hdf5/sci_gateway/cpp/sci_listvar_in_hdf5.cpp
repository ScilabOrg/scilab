/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

extern "C"
{
#include <hdf5.h>
#include <string.h>
#include "gw_hdf5.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile.h"
#include "expandPathVariable.h"
}

#include <vector>
#include "forceJHDF5load.hxx"

typedef struct __VAR_INFO__
{
    char pstInfo[128];
    char varName[128];
    int iType;
    int iSize;
    int iDims;
    int piDims[2];

    __VAR_INFO__() : iType(0), iSize(0), iDims(0) {}
} VarInfo;

static bool read_data(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_double(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_string(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_boolean(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_integer(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_boolean_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_poly(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_list(int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_void(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_undefined(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);

static void generateInfo(VarInfo* _pInfo, const char* _pstType);

int sci_listvar_in_hdf5(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int *piAddr     = NULL;
    char* pstFile   = NULL;
    int iFile       = 0;
    int iNbItem     = 0;
    VarInfo* pInfo  = NULL;
    bool bRet       = true;

    CheckRhs(1, 1);
    CheckLhs(1, 4);

#ifndef _MSC_VER
    forceJHDF5load();
#endif

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFile))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    char* pstFileName = expandPathVariable(pstFile);
    iFile = openHDF5File(pstFileName);
    if (iFile < 0)
    {
        FREE(pstFileName);
        FREE(pstFile);
        Scierror(999, _("%s: Unable to open file: %s\n"), fname, pstFile);
        return 1;
    }
    FREE(pstFileName);
    FREE(pstFile);

    iNbItem = getVariableNames(iFile, NULL);
    if (iNbItem != 0)
    {
        char** pstVarNameList = (char**)MALLOC(sizeof(char*) * iNbItem);
        pInfo = (VarInfo*)MALLOC(iNbItem * sizeof(VarInfo));

        if (Lhs == 1)
        {
            sciprint("Name                     Type           Size            Bytes\n");
            sciprint("---------------------------------------------------------------\n");
        }

        iNbItem = getVariableNames(iFile, pstVarNameList);
        for (int i = 0; i < iNbItem; i++)
        {
            int iDataSetId = getDataSetIdFromName(iFile, pstVarNameList[i]);
            if (iDataSetId == 0)
            {
                bRet = false;
                break;
            }

            strcpy(pInfo[i].varName, pstVarNameList[i]);
            FREE(pstVarNameList[i]);
            if (read_data(iDataSetId, 0, NULL, &pInfo[i]) == false)
            {
                bRet = false;
                break;
            }

            if (Lhs == 1)
            {
                sciprint("%s\n", pInfo[i].pstInfo);
            }
        }
    }
    else
    {
        //no variable returms [] for each Lhs
        for (int i = 0 ; i < Lhs ; i++)
        {
            createEmptyMatrix(pvApiCtx, Rhs + i + 1);
            LhsVar(i + 1) = Rhs + i + 1;
        }

        PutLhsVar();
        return 0;
    }

    //1st Lhs
    char** pstVarName = (char**)MALLOC(sizeof(char*) * iNbItem);
    for (int i = 0 ; i < iNbItem ; i++)
    {
        pstVarName[i] = pInfo[i].varName;
    }

    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iNbItem, 1, pstVarName);
    FREE(pstVarName);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    LhsVar(1) = Rhs + 1;

    if (Lhs > 1)
    {
        //2nd Lhs
        double* pdblType;
        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 2, iNbItem, 1, &pdblType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        for (int i = 0 ; i < iNbItem ; i++)
        {
            pdblType[i] = pInfo[i].iType;
        }

        LhsVar(2) = Rhs + 2;

        if (Lhs > 2)
        {
            //3rd Lhs
            int* pList = NULL;
            sciErr = createList(pvApiCtx, Rhs + 3, iNbItem, &pList);
            for (int i = 0 ; i < iNbItem ; i++)
            {
                double* pdblDims = NULL;
                allocMatrixOfDoubleInList(pvApiCtx, Rhs + 3, pList, i + 1, 1, pInfo[i].iDims, &pdblDims);
                for (int j = 0 ; j < pInfo[i].iDims ; j++)
                {
                    pdblDims[j] = pInfo[i].piDims[j];
                }
            }

            LhsVar(3) = Rhs + 3;
        }

        if (Lhs > 3)
        {
            //4th Lhs
            double* pdblSize;
            sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 4, iNbItem, 1, &pdblSize);
            for (int i = 0 ; i < iNbItem ; i++)
            {
                pdblSize[i] = pInfo[i].iSize;
            }

            LhsVar(4) = Rhs + 4;
        }

    }

    FREE(pInfo);
    PutLhsVar();
    return 0;
}

static bool read_data(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    bool bRet = false;

    _pInfo->iType = getScilabTypeFromDataSet(_iDatasetId);
    switch (_pInfo->iType)
    {
        case sci_matrix:
        {
            bRet = read_double(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_strings:
        {
            bRet = read_string(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_list:
        case sci_tlist:
        case sci_mlist:
        {
            bRet = read_list(_iDatasetId, _pInfo->iType, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_boolean:
        {
            bRet = read_boolean(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_poly:
        {
            bRet = read_poly(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_ints:
        {
            bRet = read_integer(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_sparse:
        {
            bRet = read_sparse(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_boolean_sparse:
        {
            bRet = read_boolean_sparse(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_void:             //void item only on list variable
        {
            bRet = read_void(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_undefined:        //undefined item only on list variable
        {
            bRet = read_undefined(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        default:
        {
            Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), "listvar_in_hdf5");
            break;
        }
    }

    return bRet;
}

static bool read_double(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    iComplex = isComplexData(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = (2 + (iRows * iCols * (iComplex + 1))) * 8;

    generateInfo(_pInfo, "constant");
    return true;
}

static bool read_string(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    char** pstData = NULL;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;

    pstData = (char **)MALLOC(iRows * iCols * sizeof(char *));
    iRet = readStringMatrix(_iDatasetId, iRows, iCols, pstData);


    for (int i = 0 ; i < iRows * iCols ; i++)
    {
        _pInfo->iSize += (int)strlen(pstData[i]) * 4;
        FREE(pstData[i]);
    }

    FREE(pstData);
    //always full double size
    _pInfo->iSize += (8 - (_pInfo->iSize % 8));
    //header + offset
    _pInfo->iSize += 16 + (1 + iRows * iCols) * 4;

    generateInfo(_pInfo, "string");
    return true;
}

static bool read_boolean(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = (3 + iRows * iCols) * 4;

    generateInfo(_pInfo, "boolean");
    return true;
}

static bool read_integer(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iPrec = 0;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    iRet = getDatasetPrecision(_iDatasetId, &iPrec);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 16 + iRows * iCols * (iPrec % 10);

    generateInfo(_pInfo, "integer");
    return true;
}

static bool read_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int iComplex = 0;

    iRet = getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 20 + iRows * 4 + iNbItem * 4 + (iNbItem * (iComplex + 1) * 8);

    generateInfo(_pInfo, "sparse");
    return true;
}

static bool read_boolean_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int iComplex = 0;

    iRet = getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 20 + iRows * 4 + iNbItem * 4;

    generateInfo(_pInfo, "boolean sparse");
    return true;
}

static bool read_poly(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;
    char pstVarName[64] = { 0 };
    double **pdblReal = NULL;
    double **pdblImg = NULL;
    int *piNbCoef = NULL;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 8 * 4 + (iRows * iCols + 1) * 4;

    if (iComplex)
    {
        piNbCoef = (int *)MALLOC(iRows * iCols * sizeof(int));
        pdblReal = (double **)MALLOC(iRows * iCols * sizeof(double *));
        pdblImg = (double **)MALLOC(iRows * iCols * sizeof(double *));
        iRet = readPolyComplexMatrix(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        piNbCoef = (int *)MALLOC(iRows * iCols * sizeof(int));
        pdblReal = (double **)MALLOC(iRows * iCols * sizeof(double *));
        iRet = readPolyMatrix(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal);
    }

    for (int i = 0 ; i < iRows * iCols ; i++)
    {
        _pInfo->iSize += piNbCoef[i] * 8 * (iComplex + 1);
        FREE(pdblReal[i]);
        if (iComplex)
        {
            FREE(pdblImg[i]);
        }
    }

    FREE(piNbCoef);
    FREE(pdblReal);
    if (iComplex)
    {
        FREE(pdblImg);
    }

    generateInfo(_pInfo, "polynomial");
    return true;
}

static bool read_list(int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iItems = 0;
    int *piListAddr = NULL;
    hobj_ref_t *piItemRef = NULL;

    iRet = getListDims(_iDatasetId, &iItems);
    if (iRet)
    {
        return false;
    }

    if (iItems == 0)
    {
        //special case for empty list
    }
    else
    {
        iRet = getListItemReferences(_iDatasetId, &piItemRef);
        if (iRet)
        {
            return false;
        }
    }
    //_pInfo = (VarInfo*)MALLOC(sizeof(VarInfo));
    _pInfo->iDims = 1;
    _pInfo->piDims[0] = iItems;
    _pInfo->iSize = (2 + iItems + 1) * 4;

    for (int i = 0; i < iItems; i++)
    {
        int iItemDataset = 0;

        iRet = getListItemDataset(_iDatasetId, piItemRef, i, &iItemDataset);
        if (iRet || iItemDataset == 0)
        {
            return false;
        }
        VarInfo info;
        bool bRet = read_data(iItemDataset, i + 1, piListAddr, &info);
        if (bRet == false)
        {
            return false;
        }

        _pInfo->iSize += info.iSize;
    }

    if (_iVarType == sci_list)
    {
        generateInfo(_pInfo, "list");
    }
    else if (_iVarType == sci_tlist)
    {
        generateInfo(_pInfo, "tlist");
    }
    else if (_iVarType == sci_mlist)
    {
        generateInfo(_pInfo, "mlist");
    }

    return true;
}

static bool read_void(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    _pInfo->iSize = 1;
    return true;
}

static bool read_undefined(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    _pInfo->iSize = 1;
    return true;
}

static void generateInfo(VarInfo* _pInfo, const char* _pstType)
{
    char pstSize[17];

    if (_pInfo->iDims == 2)
    {
        sprintf(pstSize, "%d by %d", _pInfo->piDims[0], _pInfo->piDims[1]);
    }
    else
    {
        sprintf(pstSize, "%d", _pInfo->piDims[0]);
    }
    sprintf(_pInfo->pstInfo, "%-*s%-*s%-*s%-*d", 25, _pInfo->varName, 15, _pstType, 16, pstSize, 10, _pInfo->iSize);
}
