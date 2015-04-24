/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <hdf5.h>
#include "gatewaystruct.hxx"
#include "internal.hxx"
#include "types.hxx"
#include "struct.hxx"
#include "cell.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "context.hxx"
#include "serializervisitor.hxx"


extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_hdf5.h"
#include "sci_malloc.h"
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
#include "os_string.h"
#include "configvariable_interface.h"
#include "splitpath.h"
#include "scicurdir.h"
}

//#define PRINT_DEBUG
int iLevel = 0;
static bool isVarExist(int* pvCtx, int _iFile, char* _pstVarName);

static bool export_data(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_list(int* pvCtx, int _iH5File, int *_piVar, char* _pstName, int _iVarType);
static bool export_struct(int* pvCtx, int _iH5File, int *_piVar, char* _pstName, int _iVarType);
static bool export_cell(int* pvCtx, int _iH5File, int *_piVar, char* _pstName, int _iVarType);
static bool export_hypermat(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_double(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_poly(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_boolean(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_sparse(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_boolean_sparse(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_matlab_sparse(int* pvCtx, int *_piVar, char* _pstName);
static bool export_ints(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_handles(int* pvCtx, int *_piVar, char* _pstName);
static bool export_strings(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_u_function(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_c_function(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);
static bool export_lib(int* pvCtx, int *_piVar, char* _pstName);
static bool export_lufact_pointer(int* pvCtx, int *_piVar, char* _pstName);
static bool export_void(int* pvCtx, int _iH5File, int *_piVar, char* _pstName);

void print_type(char* _pstType);
int extractVarNameList(int* pvCtx, int _iStart, int _iEnd, char** _pstNameList);


/*--------------------------------------------------------------------------*/
int sci_export_to_hdf5(char *fname, int* pvApiCtx)
{
    int iNbVar          = 0;
    int** piAddrList    = NULL;
    char** pstNameList  = NULL;
    char *pstFileName   = NULL;
    bool bExport        = true;
    bool bAppendMode    = false;
    int iRhs            = nbInputArgument(pvApiCtx);

    SciErr sciErr;

    CheckInputArgumentAtLeast(pvApiCtx, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    pstNameList = (char**)MALLOC(sizeof(char*) * nbInputArgument(pvApiCtx));
    iNbVar = extractVarNameList(pvApiCtx, 1, nbInputArgument(pvApiCtx), pstNameList);
    if (iNbVar == 0)
    {
        FREE(pstNameList);
        return 1;
    }

    piAddrList = (int**)MALLOC(sizeof(int*) * (iNbVar));
    for (int i = 1 ; i < nbInputArgument(pvApiCtx) ; i++)
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
                freeArrayOfString(pstNameList, iRhs);
                FREE(piAddrList);
                Scierror(999, _("%s: Wrong value for input argument #%d: Defined variable expected.\n"), fname, i + 1);
                printError(&sciErr, 0);
                return 1;
            }
        }
    }

    //check append option

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
        if (iH5File == -2)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" is a directory"), fname, 1, pstNameList[0]);
        }
        else
        {
            Scierror(999, _("%s: Cannot open file %s.\n"), fname, pstNameList[0]);
        }

        FREE(piAddrList);
        freeArrayOfString(pstNameList, iRhs);
        FREE(pstFileName);
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
            FREE(piAddrList);
            freeArrayOfString(pstNameList, iRhs);
            FREE(pstFileName);
            return 1;
        }
    }

    // export data
    for (int i = 1 ; i < nbInputArgument(pvApiCtx) ; i++)
    {
        if (strcmp(pstNameList[i], "-append") == 0)
        {
            continue;
        }

        if (isVarExist(pvApiCtx, iH5File, pstNameList[i]))
        {
            if (bAppendMode)
            {
                if (deleteHDF5Var(iH5File, pstNameList[i]))
                {
                    closeHDF5File(iH5File);
                    Scierror(999, _("%s: Unable to delete existing variable \"%s\"."), fname, pstNameList[i]);
                    FREE(piAddrList);
                    freeArrayOfString(pstNameList, iRhs);
                    FREE(pstFileName);
                    return 1;
                }
            }
            else
            {
                closeHDF5File(iH5File);
                Scierror(999, _("%s: Variable \'%s\' already exists in file \'%s\'\nUse -append option to replace existing variable\n."), fname, pstNameList[i], pstNameList[0]);
                FREE(piAddrList);
                freeArrayOfString(pstNameList, iRhs);
                FREE(pstFileName);
                return 1;
            }
        }

        bExport = export_data(pvApiCtx, iH5File, piAddrList[i], pstNameList[i]);
        if (bExport == false)
        {
            break;
        }
    }

    if (bExport && nbInputArgument(pvApiCtx) != 1)
    {
        //add or update scilab version and file version in hdf5 file
        if (updateScilabVersion(iH5File) < 0)
        {
            closeHDF5File(iH5File);
            Scierror(999, _("%s: Unable to update Scilab version in \"%s\"."), fname, pstNameList[0]);
            FREE(piAddrList);
            freeArrayOfString(pstNameList, iRhs);
            FREE(pstFileName);
            return 1;
        }

        if (updateFileVersion(iH5File) < 0)
        {
            closeHDF5File(iH5File);
            Scierror(999, _("%s: Unable to update HDF5 format version in \"%s\"."), fname, pstNameList[0]);
            FREE(piAddrList);
            freeArrayOfString(pstNameList, iRhs);
            FREE(pstFileName);
            return 1;
        }
    }

    //close hdf5 file
    closeHDF5File(iH5File);
    if (bExport == false && nbInputArgument(pvApiCtx) != 1)
    {
        //remove file
        deleteafile(pstFileName);
    }

    FREE(pstFileName);
    freeArrayOfString(pstNameList, iRhs);
    FREE(piAddrList);

    //create boolean return value
    int *piReturn = NULL;
    sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &piReturn);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (bExport == true || nbInputArgument(pvApiCtx) == 1)
    {
        piReturn[0] = 1;
    }
    else
    {
        piReturn[0] = 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

static bool export_data(int* pvCtx, int _iH5File, int* _piVar, char* _pstName)
{
    bool bReturn = false;
    int iType = 0;
    int iDims = 0;
    types::InternalType* pIT = (types::InternalType*)_piVar;

    SciErr sciErr = getVarType(pvCtx, _piVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    if (pIT->isGenericType())
    {
        types::GenericType* pGT = pIT->getAs<types::GenericType>();
        iDims = pGT->getDims();
    }

    if (iDims > 2)
    {
        bReturn = export_hypermat(pvCtx, _iH5File, _piVar, _pstName);
    }
    else
    {
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
                bReturn = export_u_function(pvCtx, _iH5File, _piVar, _pstName);
                break;
            }
            case sci_c_function :
            {
                bReturn = export_c_function(pvCtx, _iH5File, _piVar, _pstName);
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
                if (pIT->isStruct())
                {
                    return export_struct(pvCtx, _iH5File, _piVar, _pstName, iType);
                }
                if (pIT->isCell())
                {
                    return export_cell(pvCtx, _iH5File, _piVar, _pstName, iType);
                }
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

    //char pstMsg[] = "void";
    //print_type(pstMsg);
    return true;
}

static bool export_undefined(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = writeUndefined(_iH5File, _pstName);
    if (iRet)
    {
        return false;
    }

    //char pstMsg[] = "void";
    //print_type(pstMsg);
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

    //char pstMsg[256];
    //sprintf(pstMsg, "list (%d)", iItemNumber);
    //print_type(pstMsg);

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

static bool export_struct(int* pvCtx, int _iH5File, int *_piVar, char* _pstName, int _iVarType)
{
    char STR[] = "st";
    char DIMS[] = "dims";

    int iItemNumber = 0;

    types::Struct* pStruct = (types::Struct*)_piVar;
    types::SingleStruct** ppSStruct = pStruct->get();

    types::String* pStr = pStruct->getFieldNames();
    // for empty struct, getFieldNames return NULL
    if (pStr)
    {
        iItemNumber = pStr->getSize();
    }

    char** strStruct = new char*[iItemNumber + 2];
    strStruct[0] = STR;
    strStruct[1] = DIMS;

    for (int i = 0; i < iItemNumber; ++i)
    {
        strStruct[i + 2] = wide_string_to_UTF8(pStr->get(i));
    }

    //int iVarType = sci_mlist;
    int iRet = 0;
    bool bReturn = false;

    //create groupe name
    char* pstGroupName = createGroupName(_pstName);
    iLevel++;

    //open list
    void *pvList = openList(_iH5File, pstGroupName, 2 + iItemNumber);

    //export string ["st" "dims" "field name"]

    int piStrDims[2] = { 1, iItemNumber + 2 };

    char* pstPathName = createPathName(pstGroupName, 0);
    iRet = writeStringMatrix(_iH5File, pstPathName, 2, piStrDims, strStruct);
    if (iRet)
    {
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 0, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    // export size
    int piSizeDims[2] = { 1, 0 };
    piSizeDims[1] = pStruct->getDims();
    pstPathName = createPathName(pstGroupName, 1);
    iRet = writeInteger32Matrix(_iH5File, pstPathName, 2, piSizeDims, pStruct->getDimsArray());
    if (iRet)
    {
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 1, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    int *piNewVar = NULL;
    if (pStruct->getSize() == 1)
    {
        for (int i = 0; i < iItemNumber; ++i)
        {

            char* pstPathName = createPathName(pstGroupName, i + 2);

            piNewVar = (int*)ppSStruct[0]->get(pStr->get(i));


            if (piNewVar == NULL)
            {
                //undefined item
                bReturn = export_undefined(pvCtx, _iH5File, piNewVar, pstPathName);
            }
            else
            {
                bReturn = export_data(pvCtx, _iH5File, piNewVar, pstPathName);
            }

            iRet = addItemInList(_iH5File, pvList, i + 2, pstPathName);
            FREE(pstPathName);
            if (bReturn == false || iRet)
            {
                return false;
            }
        }
    }
    else if (pStruct->getSize() > 1) //
    {
        types::List* pListe = new types::List();

        for (int i = 0; i < iItemNumber; ++i)
        {

            char* pstPathName = createPathName(pstGroupName, i + 2);

            for (int iReadData = 0; iReadData < pStruct->getSize(); ++iReadData)
            {
                pListe->set(iReadData, ppSStruct[iReadData]->get(pStr->get(i)));
            }
            piNewVar = (int*)pListe;


            if (piNewVar == NULL)
            {
                //undefined item
                bReturn = export_undefined(pvCtx, _iH5File, piNewVar, pstPathName);
            }
            else
            {
                bReturn = export_data(pvCtx, _iH5File, piNewVar, pstPathName);
            }

            iRet = addItemInList(_iH5File, pvList, i + 2, pstPathName);
            FREE(pstPathName);
            if (bReturn == false || iRet)
            {
                return false;
            }
        }

        delete pListe;
    }

    iLevel--;
    closeList(_iH5File, pvList, _pstName, 2 + iItemNumber, _iVarType);
    FREE(pstGroupName);
    //close list
    return true;
}

static bool export_cell(int* pvCtx, int _iH5File, int *_piVar, char* _pstName, int _iVarType)
{
    char CELL[] = "ce";
    char DIMS[] = "dims";
    char ENTRIES[] = "entries";

    char* strCell[] = { CELL, DIMS, ENTRIES };

    types::Cell* pCell = (types::Cell*)_piVar;

    //int iVarType = sci_mlist;
    int iRet = 0;
    bool bReturn = false;

    //create groupe name
    char* pstGroupName = createGroupName(_pstName);
    iLevel++;

    //open list
    void *pvList = openList(_iH5File, pstGroupName, 3);

    //export string ["cell" "dims" "entries"]

    int piStrDims[2] = { 1, 3 };

    char* pstPathName = createPathName(pstGroupName, 0);
    iRet = writeStringMatrix(_iH5File, pstPathName, 2, piStrDims, strCell);
    if (iRet)
    {
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 0, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    // export size
    int piSizeDims[2] = { 1, 0 };
    piSizeDims[1] = pCell->getDims();
    pstPathName = createPathName(pstGroupName, 1);
    iRet = writeInteger32Matrix(_iH5File, pstPathName, 2, piSizeDims, pCell->getDimsArray());
    if (iRet)
    {
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 1, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    int *piNewVar = NULL;

    types::List* pListe = new types::List();

    pstPathName = createPathName(pstGroupName, 2);

    for (int iReadData = 0; iReadData < pCell->getSize(); ++iReadData)
    {
        pListe->set(iReadData, pCell->get(iReadData));
    }

    piNewVar = (int*)pListe;

    if (piNewVar == NULL)
    {
        //undefined item
        bReturn = export_undefined(pvCtx, _iH5File, piNewVar, pstPathName);
    }
    else
    {
        bReturn = export_data(pvCtx, _iH5File, piNewVar, pstPathName);
    }

    iRet = addItemInList(_iH5File, pvList, 2, pstPathName);
    FREE(pstPathName);
    if (bReturn == false || iRet)
    {
        return false;
    }

    delete pListe;

    iLevel--;
    closeList(_iH5File, pvList, _pstName, 3, _iVarType);
    FREE(pstGroupName);
    //close list
    return true;
}


static bool export_hypermat(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    char HM [] = "hm";
    char DIMS [] = "dims";
    char ENTRIES [] = "entries";

    char* strHypermat [] = {HM, DIMS, ENTRIES};

    int iVarType    = sci_mlist;
    int iRet        = 0;
    bool bReturn    = false;
    types::GenericType* pGT = (types::GenericType*)_piVar;

    //create groupe name
    char* pstGroupName = createGroupName(_pstName);
    iLevel++;

    //open list
    void *pvList = openList(_iH5File, pstGroupName, 3);

    //export string ["hm" "dims" "entries"]
    int piStrDims[2] = {1, 3};
    char* pstPathName = createPathName(pstGroupName, 0);
    iRet = writeStringMatrix(_iH5File, pstPathName, 2, piStrDims, strHypermat);
    if (iRet)
    {
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 0, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    // export size
    int piSizeDims[2] = {1, 0};
    piSizeDims[1] = pGT->getDims();
    pstPathName = createPathName(pstGroupName, 1);
    iRet = writeInteger32Matrix(_iH5File, pstPathName, 2, piSizeDims, pGT->getDimsArray());
    if (iRet)
    {
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 1, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    // export data
    int iDims = pGT->getDims();
    int* piDimsArray = new int[iDims];
    memcpy(piDimsArray, pGT->getDimsArray(), iDims * sizeof(int));

    pstPathName = createPathName(pstGroupName, 2);
    // reshape to prevent infinite call of export_hypermat
    // we have to export a colon vector
    pGT->reshape(pGT->getSize(), 1);
    bReturn = export_data(pvCtx, _iH5File, _piVar, pstPathName);
    // after export reshape again to keep variable unchainged.
    pGT->reshape(piDimsArray, iDims);
    delete[] piDimsArray;

    iRet = addItemInList(_iH5File, pvList, 2, pstPathName);
    FREE(pstPathName);
    if (bReturn == false || iRet)
    {
        return false;
    }

    //close list
    iLevel--;
    closeList(_iH5File, pvList, _pstName, 3, iVarType);
    FREE(pstGroupName);

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

    //char pstMsg[512];
    //sprintf(pstMsg, "double (%d x %d)", piDims[0], piDims[1]);
    //print_type(pstMsg);
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
        if (getAllocatedMatrixOfComplexPoly(pvCtx, _piVar, &piDims[0], &piDims[1], &piNbCoef, &pdblReal, &pdblImg))
        {
            freeAllocatedMatrixOfComplexPoly(piDims[0], piDims[1], piNbCoef, pdblReal, pdblImg);
            return false;
        }

        iRet = writePolyComplexMatrix(_iH5File, _pstName, pstVarName, 2, piDims, piNbCoef, pdblReal, pdblImg);
        freeAllocatedMatrixOfComplexPoly(piDims[0], piDims[1], piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        if (getAllocatedMatrixOfPoly(pvCtx, _piVar, &piDims[0], &piDims[1], &piNbCoef, &pdblReal))
        {
            freeAllocatedMatrixOfPoly(piDims[0], piDims[1], piNbCoef, pdblReal);
            return false;
        }

        iRet = writePolyMatrix(_iH5File, _pstName, pstVarName, 2, piDims, piNbCoef, pdblReal);
        freeAllocatedMatrixOfPoly(piDims[0], piDims[1], piNbCoef, pdblReal);
    }

    if (iRet)
    {
        return false;
    }

    //char pstMsg[512];
    //sprintf(pstMsg, "poly (%d x %d)", piDims[0], piDims[1]);
    //print_type(pstMsg);
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

    //char pstMsg[512];
    //sprintf(pstMsg, "bool (%d x %d)", piDims[0], piDims[1]);
    //print_type(pstMsg);
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

    //char pstMsg[512];
    //sprintf(pstMsg, "boolean sparse (%d x %d)", piDims[0], piDims[1]);
    //print_type(pstMsg);
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

    //char pstMsg[512];
    //sprintf(pstMsg, "sparse (%d x %d)", piDims[0], piDims[1]);
    //print_type(pstMsg);
    return true;
}

static bool export_matlab_sparse(int* pvCtx, int *_piVar, char* _pstName)
{
    //print_type(_pstName);
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

    //char pstMsg[512];
    //sprintf(pstMsg, "int%d (%d x %d)", 8 * iPrec, piDims[0], piDims[1]);
    //print_type(pstMsg);
    return true;
}

static bool export_handles(int* pvCtx, int *_piVar, char* _pstName)
{
    //print_type(_pstName);
    return false;
}

static bool export_strings(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    int iRet = 0;
    char** pstData = NULL;
    int piDims[2];

    SciErr sciErr = getMatrixOfString(pvCtx, _piVar, &piDims[0], &piDims[1], NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    if (getAllocatedMatrixOfString(pvCtx, _piVar, &piDims[0], &piDims[1], &pstData))
    {
        if (pstData)
        {
            freeAllocatedMatrixOfString(piDims[0], piDims[1], pstData);
        }

        return false;
    }

    iRet = writeStringMatrix(_iH5File, _pstName, 2, piDims, pstData);
    freeAllocatedMatrixOfString(piDims[0], piDims[1], pstData);
    if (iRet)
    {
        return false;
    }

    //char pstMsg[512];
    //sprintf(pstMsg, "string (%d x %d)", piDims[0], piDims[1]);
    //print_type(pstMsg);

    return true;
}

static bool export_u_function(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{
    return export_c_function(pvCtx, _iH5File, _piVar, _pstName);
}

static bool export_c_function(int* pvCtx, int _iH5File, int *_piVar, char* _pstName)
{

    bool iRet = false;
    int iVarType = sci_c_function;

    wchar_t* pwcString = to_wide_string(_pstName);
    types::InternalType* pITIn =  symbol::Context::getInstance()->get(symbol::Symbol(pwcString));
    FREE(pwcString);

    if (pITIn == NULL)
    {
        Scierror(999, _("%s: Unable to get address of variable \"%s\""), "export_c_function", _pstName);
        return false;
    }

    if ((pITIn->isMacro() || pITIn->isMacroFile()) == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Macro expected.\n"), "export_c_function", 2);
        return false;
    }

    Macro* pM = NULL;
    switch (pITIn->getType())
    {
        case InternalType::ScilabMacro:
        {
            pM = pITIn->getAs<Macro>();
            break;
        }
        case InternalType::ScilabMacroFile:
        {
            MacroFile* pMF = pITIn->getAs<MacroFile>();
            pM = pMF->getMacro();
            break;
        }
        default:
            break;
    }

    //create groupe name
    char* pstGroupName = createGroupName(_pstName);
    iLevel++;

    //open list
    void *pvList = openList(_iH5File, pstGroupName, 3);


    //export in argument ["inputs" "a" "b" "x"]
    std::list<symbol::Variable*>* pIn = pM->inputs_get();

    char** strInArg = (char**) MALLOC(sizeof(char*) * (pIn->size() + 1));
    strInArg[0] = "Inputs";

    if (pIn->size() != 0)
    {
        list<symbol::Variable*>::iterator itIn = pIn->begin();
        for (int i = 0; i < pIn->size(); i++, itIn++)
        {
            strInArg[i + 1] = wide_string_to_UTF8((*itIn)->getSymbol().getName().c_str());
        }
    }

    int piStrDims[2] = { 1, pM->getNbInputArgument() + 1 };
    char* pstPathName = createPathName(pstGroupName, 0);
    iRet = writeStringMatrix(_iH5File, pstPathName, 2, piStrDims, strInArg);
    if (iRet)
    {
        FREE(pstPathName);
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 0, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    for (int i = 1; i <= pIn->size(); ++i)
    {
        FREE(strInArg[i]);
    }
    FREE(strInArg);


    //export out argument["outputs" "a" "b" "x"]
    std::list<symbol::Variable*>* pOut = pM->outputs_get();

    char** strOutArg = (char**)MALLOC(sizeof(char*) * (pOut->size() + 1));
    strOutArg[0] = "Outputs";

    if (pOut->size() != 0)
    {
        list<symbol::Variable*>::iterator itOut = pOut->begin();
        for (int i = 0; i < pOut->size(); i++, itOut++)
        {
            strOutArg[i + 1] = wide_string_to_UTF8((*itOut)->getSymbol().getName().c_str());
        }
    }

    piStrDims[1] = ( pOut->size() + 1 );
    pstPathName = createPathName(pstGroupName, 1);
    iRet = writeStringMatrix(_iH5File, pstPathName, 2, piStrDims, strOutArg);
    if (iRet)
    {
        FREE(pstPathName);
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 1, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    for (int i = 1; i <= pOut->size(); ++i)
    {
        FREE(strOutArg[i]);
    }
    FREE(strOutArg);


    //export body
    int piSizeDims[2] = { 1, 0 };
    ast::Exp* pExp = pM->getBody();
    ast::SerializeVisitor serialMacro(pExp);

    //the size of body is in four firsts uchar
    unsigned int size = (unsigned int)(serialMacro.serialize(false, true)[0]) + (serialMacro.serialize(false, true)[1] << 8) + (serialMacro.serialize(false, true)[2] << 16) + (serialMacro.serialize(false, true)[3] << 32);

    piSizeDims[1] = size;
    pstPathName = createPathName(pstGroupName, 2);
    iRet = writeUnsignedInteger8Matrix(_iH5File, pstPathName, 2, piSizeDims, serialMacro.serialize(false, true));
    if (iRet)
    {
        FREE(pstPathName);
        return false;
    }

    iRet = addItemInList(_iH5File, pvList, 2, pstPathName);
    FREE(pstPathName);
    if (iRet)
    {
        return false;
    }

    iLevel--;
    //close list
    closeList(_iH5File, pvList, _pstName, 3, iVarType);
    FREE(pstGroupName);

    return true;
}

static bool export_lib(int* pvCtx, int *_piVar, char* _pstName)
{
    //print_type(_pstName);
    return false;
}

static bool export_lufact_pointer(int* pvCtx, int *_piVar, char* _pstName)
{
    /*print_type*/(_pstName);
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

/*--------------------------------------------------------------------------*/
static bool isVarExist(int* pvCtx, int _iFile, char* _pstVarName)
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
                freeArrayOfString(pstVarNameList, iNbItem);
                return true;
            }
        }

        freeArrayOfString(pstVarNameList, iNbItem);
    }

    return false;
}
/*--------------------------------------------------------------------------*/
