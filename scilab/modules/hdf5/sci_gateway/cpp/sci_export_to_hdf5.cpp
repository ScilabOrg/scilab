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
*/

#include <hdf5.h>
extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_hdf5.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "deleteafile.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"
#include "freeArrayOfString.h"
#include "os_strdup.h"
#include "sci_mode.h"
#include "splitpath.h"
#include "scicurdir.h"
}

//#define PRINT_DEBUG
int iLevel = 0;

static bool isVarExist(int _iFile, char* _pstVarName);

static bool export_data(int _iH5File, int *_piVar, char* _pstName);
static bool export_list(int _iH5File, int *_piVar, char* _pstName, int _iVarType);
static bool export_double(int _iH5File, int *_piVar, char* _pstName);
static bool export_poly(int _iH5File, int *_piVar, char* _pstName);
static bool export_boolean(int _iH5File, int *_piVar, char* _pstName);
static bool export_sparse(int _iH5File, int *_piVar, char* _pstName);
static bool export_boolean_sparse(int _iH5File, int *_piVar, char* _pstName);
static bool export_matlab_sparse(int *_piVar, char* _pstName);
static bool export_ints(int _iH5File, int *_piVar, char* _pstName);
static bool export_handles(int *_piVar, char* _pstName);
static bool export_strings(int _iH5File, int *_piVar, char* _pstName);
static bool export_u_function(int *_piVar, char* _pstName);
static bool export_c_function(int *_piVar, char* _pstName);
static bool export_lib(int *_piVar, char* _pstName);
static bool export_lufact_pointer(int *_piVar, char* _pstName);
static bool export_void(int _iH5File, int *_piVar, char* _pstName);

void print_type(char* _pstType);
int extractVarNameList(int* pvCtx, int _iStart, int _iEnd, char** _pstNameList);


/*--------------------------------------------------------------------------*/
int sci_export_to_hdf5(char *fname, int* pvApiCtx)
{
    int iNbVar          = 0;
    int** piAddrList    = NULL;
    char** pstNameList  = NULL;
    char *pstFileName   = NULL;
    bool bExport        = false;
    bool bAppendMode    = false;

    SciErr sciErr;

    int iRhs = nbInputArgument(pvApiCtx);
    CheckInputArgumentAtLeast(pvApiCtx, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    pstNameList = (char**)MALLOC(sizeof(char*) * iRhs);
    iNbVar = extractVarNameList(1, iRhs, pstNameList);
    if (iNbVar == 0)
    {
        FREE(pstNameList);
        return 1;
    }

    piAddrList = (int**)MALLOC(sizeof(int*) * (iNbVar));
    for (int i = 1 ; i < iRhs ; i++)
    {
        if (strcmp(pstNameList[i], "-append") == 0)
        {
            bAppendMode = true;
        }
        else
        {
            sciErr = getVarAddressFromName(pvApiCtx, pstNameList[i], &piAddrList[i]);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Defined variable expected.\n"), fname, i + 1);
                printError(&sciErr, 0);
                return 1;
            }
        }
    }

    iLevel = 0;
    // open hdf5 file
    pstFileName = expandPathVariable(pstNameList[0]);
    int iH5File = 0;
    if (bAppendMode)
    {
        iH5File = openHDF5File(pstFileName, bAppendMode);
        if (iH5File < 0)
        {
            iH5File = createHDF5File(pstFileName);
        }
    }
    else
    {
        iH5File = createHDF5File(pstFileName);
    }


    if (iH5File < 0)
    {
        FREE(pstFileName);
        if (iH5File == -2)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" is a directory"), fname, 1, pstNameList[0]);
        }
        else
        {
            Scierror(999, _("%s: Cannot open file %s.\n"), fname, pstNameList[0]);
        }

        return 1;
    }

    if (bAppendMode)
    {
        int iVersion = getSODFormatAttribute(iH5File);
        if (iVersion != -1 && iVersion != SOD_FILE_VERSION)
        {
            //to update version must be the same
            closeHDF5File(iH5File);
            Scierror(999, _("%s: Wrong SOD file format version. Expected: %d Found: %d\n"), fname, SOD_FILE_VERSION, iVersion);
            return 1;
        }

    }

    // export data
    for (int i = 1 ; i < iRhs ; i++)
    {
        if (strcmp(pstNameList[i], "-append") == 0)
        {
            continue;
        }

        if (isVarExist(iH5File, pstNameList[i]))
        {
            if (bAppendMode)
            {
                if (deleteHDF5Var(iH5File, pstNameList[i]))
                {
                    closeHDF5File(iH5File);
                    Scierror(999, _("%s: Unable to delete existing variable \"%s\"."), fname, pstNameList[i]);
                    return 1;
                }
            }
            else
            {
                closeHDF5File(iH5File);
                Scierror(999, _("%s: Variable \'%s\' already exists in file \'%s\'\nUse -append option to replace existing variable\n."), fname, pstNameList[i], pstNameList[0]);
                return 1;
            }
        }

        bExport = export_data(iH5File, piAddrList[i], pstNameList[i]);
        if (bExport == false)
        {
            break;
        }
    }

    if (bExport && iRhs != 1)
    {
        //add or update scilab version and file version in hdf5 file
        if (updateScilabVersion(iH5File) < 0)
        {
            closeHDF5File(iH5File);
            Scierror(999, _("%s: Unable to update Scilab version in \"%s\"."), fname, pstNameList[0]);
            return 1;
        }

        if (updateFileVersion(iH5File) < 0)
        {
            closeHDF5File(iH5File);
            Scierror(999, _("%s: Unable to update HDF5 format version in \"%s\"."), fname, pstNameList[0]);
            return 1;
        }
    }

    //close hdf5 file
    closeHDF5File(iH5File);

    //delete file in case of error but nor in append mode
    if (bExport == false && bAppendMode == false && iRhs != 1)
    {
        //remove file
        deleteafile(pstFileName);
    }
    FREE(pstFileName);

    //create boolean return value
    int *piReturn = NULL;
    sciErr = allocMatrixOfBoolean(pvApiCtx, iRhs + 1, 1, 1, &piReturn);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (bExport == true || iRhs == 1)
    {
        piReturn[0] = 1;
    }
    else
    {
        piReturn[0] = 0;
    }


    //free memory
    for (int i = 0 ; i < iRhs ; i++)
    {
        FREE(pstNameList[i]);
    }
    FREE(pstNameList);

    FREE(piAddrList);


    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

static bool export_data(int* pvCtx, int _iH5File, int* _piVar, char* _pstName)
{
    bool bReturn = false;
    int iType = 0;

    SciErr sciErr = getVarType(pvCtx, _piVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    switch (iType)
    {
        case sci_matrix :
            {
                bReturn = export_double(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_poly :
            {
                bReturn = export_poly(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_boolean :
            {
                bReturn = export_boolean(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_sparse :
            {
                bReturn = export_sparse(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_boolean_sparse :
            {
                bReturn = export_boolean_sparse(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_matlab_sparse :
            {
                bReturn = export_matlab_sparse(pvCtx, _piVar, _pstName);
                break;
            }
        case sci_ints :
            {
                bReturn = export_ints(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_handles :
            {
                bReturn = export_handles(pvCtx, _piVar, _pstName);
                break;
            }
        case sci_strings :
            {
                bReturn = export_strings(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        case sci_u_function :
            {
                bReturn = export_u_function(pvCtx, _piVar, _pstName);
                break;
            }
        case sci_c_function :
            {
                bReturn = export_c_function(pvCtx, _piVar, _pstName);
                break;
            }
        case sci_lib :
            {
                bReturn = export_lib(pvCtx, _piVar, _pstName);
                break;
            }
        case sci_list :
        case sci_tlist :
        case sci_mlist :
            {
                bReturn = export_list(pvCtx, _iH5File, _piVar, _pstName, iType);
                break;
            }
        case sci_lufact_pointer :
            {
                bReturn = export_lufact_pointer(pvCtx, _piVar, _pstName);
                break;
            }
        case 0 : //void case to "null" items in list
            {
                bReturn = export_void(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
        default :
        {
            bReturn = false;
            break;
        }
    }
    return bReturn;
}

static bool export_void(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = writeVoid(_iH5File, _pstName);
    if (iRet)
    {
        return false;
    }

    char pstMsg[] = "void";
    print_type(pstMsg);
    return true;
}

static bool export_undefined(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = writeUndefined(_iH5File, _pstName);
    if (iRet)
    {
        return false;
    }

    char pstMsg[] = "void";
    print_type(pstMsg);
    return true;
}

static bool export_list(int* pvCtx, int _iH5File, int *_piVar, char* _pstName, int _iVarType)
{
    int iRet        = 0;
    bool bReturn    = false;
    int iItemNumber = 0;
    SciErr sciErr   = getListItemNumber(pvCtx, _piVar, &iItemNumber);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }


    //create groupe name
    char* pstGroupName	= createGroupName(_pstName);

    char pstMsg[256];
    sprintf(pstMsg, "list (%d)", iItemNumber);
    print_type(pstMsg);

    iLevel++;
    //open list
    void *pvList = openList(_iH5File, pstGroupName, iItemNumber);
    for (int i = 0 ; i < iItemNumber ; i++)
    {
        int *piNewVar = NULL;
        getListItemAddress(pvCtx, _piVar, i + 1, &piNewVar);//1 indexed
        char* pstPathName   = createPathName(pstGroupName, i);

        if (piNewVar == NULL)
        {
            //undefined item
            bReturn = export_undefined(pvCtx, _iH5File, piNewVar, pstPathName);
        }
        else
        {
            bReturn = export_data(pvCtx, _iH5File, piNewVar, pstPathName);
        }

        iRet = addItemInList(_iH5File, pvList, i, pstPathName);
        FREE(pstPathName);
        if (bReturn == false || iRet)
        {
            return false;
        }
    }
    iLevel--;
    closeList(_iH5File, pvList, _pstName, iItemNumber, _iVarType);
    FREE(pstGroupName);
    //close list
    return true;
}

static bool export_double(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet            = 0;
    int iComplex        = isVarComplex(pvCtx, _piVar);
    int piDims[2];
    int iType = 0;
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    SciErr sciErr = getVarType(pvCtx, _piVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    if (iType != sci_matrix)
    {
        return false;
    }

    if (iComplex)
    {
        sciErr = getComplexMatrixOfDouble(pvCtx, _piVar, &piDims[0], &piDims[1], &pdblReal, &pdblImg);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeDoubleComplexMatrix(_iH5File, _pstName, 2, piDims, pdblReal, pdblImg);
    }
    else
    {
        sciErr = getMatrixOfDouble(pvCtx, _piVar, &piDims[0], &piDims[1], &pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeDoubleMatrix(_iH5File, _pstName, 2, piDims, pdblReal);
    }

    if (iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "double (%d x %d)", piDims[0], piDims[1]);
    print_type(pstMsg);
    return true;
}

static bool export_poly(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = 0;
    int* piNbCoef = NULL;
    double** pdblReal = NULL;
    double** pdblImg = NULL;
    char pstVarName[64]	= {0};
    int iVarNameLen = 0;
    int piDims[2];



    SciErr sciErr = getPolyVariableName(pvCtx, _piVar, pstVarName, &iVarNameLen);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    if (isVarComplex(pvCtx, _piVar))
    {
        sciErr = getComplexMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], NULL, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        piNbCoef = (int*)MALLOC(piDims[0] * piDims[1] * sizeof(int));
        sciErr = getComplexMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], piNbCoef, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        pdblReal = (double**)MALLOC(sizeof(double*) * piDims[0] * piDims[1]);
        pdblImg = (double**)MALLOC(sizeof(double*) * piDims[0] * piDims[1]);
        for (int i = 0 ; i < piDims[0] * piDims[1] ; i++)
        {
            pdblReal[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
            pdblImg[i]	= (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
        }
        sciErr = getComplexMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], piNbCoef, pdblReal, pdblImg);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writePolyComplexMatrix(_iH5File, _pstName, pstVarName, 2, piDims, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        sciErr = getMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        piNbCoef = (int*)MALLOC(piDims[0] * piDims[1] * sizeof(int));
        sciErr = getMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], piNbCoef, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        pdblReal = (double**)MALLOC(sizeof(double*) * piDims[0] * piDims[1]);
        for (int i = 0 ; i < piDims[0] * piDims[1] ; i++)
        {
            pdblReal[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
        }
        sciErr = getMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], piNbCoef, pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writePolyMatrix(_iH5File, _pstName, pstVarName, 2, piDims, piNbCoef, pdblReal);
    }

    if (iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "poly (%d x %d)", piDims[0], piDims[1]);
    print_type(pstMsg);

    if (pdblReal)
    {
        for (int i = 0 ; i < piDims[0] * piDims[1] ; i++)
        {
            FREE(pdblReal[i]);
        }
        FREE(pdblReal);
    }

    if (pdblImg)
    {
        for (int i = 0 ; i < piDims[0] * piDims[1] ; i++)
        {
            FREE(pdblImg[i]);
        }
        FREE(pdblImg);
    }

    FREE(piNbCoef);
    return true;
}

static bool export_boolean(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int *piData = NULL;
    int piDims[2];

    //for error management
    SciErr sciErr = getMatrixOfBoolean(pvCtx, _piVar, &piDims[0], &piDims[1], &piData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int iRet = writeBooleanMatrix(_iH5File, _pstName, 2, piDims, piData);
    if (iRet)
    {
        //Msg ??
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "bool (%d x %d)", piDims[0], piDims[1]);
    print_type(pstMsg);
    return true;
}

static bool export_boolean_sparse(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = 0;
    int iNbItem = 0;
    int* piNbItemRow = NULL;
    int* piColPos = NULL;
    int piDims[2];

    SciErr sciErr = getBooleanSparseMatrix(pvCtx, _piVar, &piDims[0], &piDims[1], &iNbItem, &piNbItemRow, &piColPos);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    iRet = writeBooleanSparseMatrix(_iH5File, _pstName, piDims[0], piDims[1], iNbItem, piNbItemRow, piColPos);
    if (iRet)
    {
        //Msg ??
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "boolean sparse (%d x %d)", piDims[0], piDims[1]);
    print_type(pstMsg);
    return true;
}

static bool export_sparse(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = 0;
    int iNbItem = 0;
    int* piNbItemRow = NULL;
    int* piColPos = NULL;
    double* pdblReal = NULL;
    double* pdblImg = NULL;
    int piDims[2];
    SciErr sciErr;

    if (isVarComplex(pvCtx, _piVar))
    {
        sciErr = getComplexSparseMatrix(pvCtx, _piVar, &piDims[0], &piDims[1], &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeSparseComplexMatrix(_iH5File, _pstName, piDims[0], piDims[1], iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
    }
    else
    {
        sciErr = getSparseMatrix(pvCtx, _piVar, &piDims[0], &piDims[1], &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeSparseMatrix(_iH5File, _pstName, piDims[0], piDims[1], iNbItem, piNbItemRow, piColPos, pdblReal);
    }

    if (iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "sparse (%d x %d)", piDims[0], piDims[1]);
    print_type(pstMsg);
    return true;
}

static bool export_matlab_sparse(int* pvCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}

static bool export_ints(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = 0;
    void *piData = NULL;
    int iPrec = 0;
    int piDims[2];

    SciErr sciErr = getMatrixOfIntegerPrecision(pvCtx, _piVar, &iPrec);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    switch (iPrec)
    {
        case SCI_INT8 :
            sciErr = getMatrixOfInteger8(pvCtx, _piVar, &piDims[0], &piDims[1], (char**)&piData);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return false;
            }
            iRet = writeInteger8Matrix(_iH5File, _pstName, 2, piDims, (char*)piData);
            break;
        case SCI_UINT8 :
            sciErr = getMatrixOfUnsignedInteger8(pvCtx, _piVar, &piDims[0], &piDims[1], (unsigned char**)&piData);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return false;
            }
            iRet = writeUnsignedInteger8Matrix(_iH5File, _pstName, 2, piDims, (unsigned char*)piData);
            break;
        case SCI_INT16 :
            sciErr = getMatrixOfInteger16(pvCtx, _piVar, &piDims[0], &piDims[1], (short**)&piData);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return false;
            }
            iRet = writeInteger16Matrix(_iH5File, _pstName, 2, piDims, (short*)piData);
            break;
        case SCI_UINT16 :
            sciErr = getMatrixOfUnsignedInteger16(pvCtx, _piVar, &piDims[0], &piDims[1], (unsigned short**)&piData);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return false;
            }
            iRet = writeUnsignedInteger16Matrix(_iH5File, _pstName, 2, piDims, (unsigned short*)piData);
            break;
        case SCI_INT32 :
            sciErr = getMatrixOfInteger32(pvCtx, _piVar, &piDims[0], &piDims[1], (int**)&piData);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return false;
            }
            iRet = writeInteger32Matrix(_iH5File, _pstName, 2, piDims, (int*)piData);
            break;
        case SCI_UINT32 :
            sciErr = getMatrixOfUnsignedInteger32(pvCtx, _piVar, &piDims[0], &piDims[1], (unsigned int**)&piData);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return false;
            }
            iRet = writeUnsignedInteger32Matrix(_iH5File, _pstName, 2, piDims, (unsigned int*)piData);
            break;
        case SCI_INT64 :
            //sciErr = getMatrixOfInteger64(_piVar, &piDims[0], &piDims[1], (long long**)&piData);
            //if(sciErr.iErr)
            //{
            //	printError(&sciErr, 0);
            //	return false;
            //}
            //iRet = writeInteger64Matrix(_iH5File, _pstName, 2, piDims, (long long*)piData);
            //break;
        case SCI_UINT64 :
            //sciErr = getMatrixOfUnsignedInteger64(_piVar, &piDims[0], &piDims[1], (unsigned long long**)&piData);
            //if(sciErr.iErr)
            //{
            //	printError(&sciErr, 0);
            //	return false;
            //}
            //iRet = writeUnsignedInteger64Matrix(_iH5File, _pstName, 2, piDims, (unsigned long long*)piData);
            //break;
        default :
            return 1;
            break;
    }

    if (iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "int%d (%d x %d)", 8 * iPrec, piDims[0], piDims[1]);
    print_type(pstMsg);
    return true;
}

static bool export_handles(int* pvCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}

static bool export_strings(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = 0;
    int* piLen = NULL;
    char** pstData = NULL;
    int piDims[2];

    SciErr sciErr = getMatrixOfString(pvCtx, _piVar, &piDims[0], &piDims[1], NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    piLen = (int*)MALLOC(piDims[0] * piDims[1] * sizeof(int));
    sciErr = getMatrixOfString(pvCtx, _piVar, &piDims[0], &piDims[1], piLen, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    pstData = (char**)MALLOC(sizeof(char*) * piDims[0] * piDims[1]);
    for (int i = 0 ; i < piDims[0] * piDims[1] ; i++)
    {
        pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));// for null termination
    }

    sciErr = getMatrixOfString(pvCtx, _piVar, &piDims[0], &piDims[1], piLen, pstData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    iRet = writeStringMatrix(_iH5File, _pstName, 2, piDims, pstData);

    if (iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "string (%d x %d)", piDims[0], piDims[1]);
    print_type(pstMsg);

    freeArrayOfString(pstData, piDims[0] * piDims[1]);
    return true;
}

static bool export_u_function(int* pvCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}

static bool export_c_function(int* pvCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}

static bool export_lib(int* pvCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}

static bool export_lufact_pointer(int* pvCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}

void print_type(char* _pstType)
{
#ifdef PRINT_DEBUG
    for (int i = 0 ; i < iLevel ; i++)
    {
        sciprint("\t");
    }
    sciprint("%s\n", _pstType);
#endif
}

int extractVarNameList(int* pvCtx, int _iStart, int _iEnd, char** _pstNameList)
{
    int iCount = 0;

    for (int i = _iStart ; i <= _iEnd ; i++)
    {
        int* piAddr = NULL;
        int iType   = 0;

        SciErr sciErr = getVarAddressFromPosition(pvCtx, i, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (getAllocatedSingleString(pvCtx, piAddr, &_pstNameList[iCount]))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "export_to_hdf5", i);
            return 0;
        }

        iCount++;
    }
    return iCount;
}

static bool isVarExist(int _iFile, char* _pstVarName)
{
    //check if variable already exists
    int iNbItem = getVariableNames(_iFile, NULL);
    if (iNbItem)
    {
        char **pstVarNameList = (char **)MALLOC(sizeof(char *) * iNbItem);

        iNbItem = getVariableNames(_iFile, pstVarNameList);

        //import all data
        for (int i = 0 ; i < iNbItem ; i++)
        {
            if (strcmp(pstVarNameList[i], _pstVarName) == 0)
            {
                return true;
            }

            FREE(pstVarNameList[i]);
        }

        FREE(pstVarNameList);
    }

    return false;
}
/*--------------------------------------------------------------------------*/
