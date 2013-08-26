/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

static int iTab = 0;
void insert_indent(void)
{
    int i = 0;
    for (i = 0 ; i < iTab ; i++)
    {
        sciprint("\t");
    }
}

int get_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_list_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_double_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_poly_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_boolean_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_sparse_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_bsparse_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_integer_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_string_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_pointer_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);

int common_read(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iItem       = 0;
    int iRet        = 0;
    int *piAddr     = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    get_info(pvApiCtx, 1, NULL, piAddr, 0);
    AssignOutputVariable(pvApiCtx, 1) = 0;
    return 0;
}

int get_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int iRet    = 0;
    int iType   = 0;

    sciErr = getVarType(_pvCtx, _piAddr, &iType);
    switch (iType)
    {
        case sci_matrix :
            iRet = get_double_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_poly :
            iRet = get_poly_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_boolean :
            iRet = get_boolean_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_sparse :
            iRet = get_sparse_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_boolean_sparse :
            iRet = get_bsparse_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_ints :
            iRet = get_integer_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_strings :
            iRet = get_string_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_list :
            insert_indent();
            sciprint("List ");
            iRet = get_list_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_tlist :
            insert_indent();
            sciprint("TList ");
            iRet = get_list_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_mlist :
            insert_indent();
            sciprint("MList ");
            iRet = get_list_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        case sci_pointer :
            iRet = get_pointer_info(_pvCtx, _iRhs, _piParent, _piAddr, _iItemPos);
            break;
        default :
            insert_indent();
            sciprint("Unknown type\n");
            return 1;
    }
    return iRet;
}

int get_list_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int i;
    int iRet        = 0;
    int iItem       = 0;
    int* piChild    = NULL;

    sciErr = getListItemNumber(_pvCtx, _piAddr, &iItem);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciprint("(%d)\n", iItem);
    for (i = 0 ; i < iItem ; i++)
    {
        sciErr = getListItemAddress(_pvCtx, _piAddr, i + 1, &piChild);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        iTab++;
        iRet = get_info(_pvCtx, _iRhs, _piAddr, piChild, i + 1);
        iTab--;
    }
    return 0;;
}

int get_double_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int iRows           = 0;
    int iCols           = 0;
    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    if (_iItemPos == 0)
    {
        //not in list
        if (isVarComplex(_pvCtx, _piAddr))
        {
            sciErr = getComplexMatrixOfDouble(_pvCtx, _piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
        }
        else
        {
            sciErr = getMatrixOfDouble(_pvCtx, _piAddr, &iRows, &iCols, &pdblReal);
        }
    }
    else
    {
        if (isVarComplex(_pvCtx, _piAddr))
        {
            sciErr = getComplexMatrixOfDoubleInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &pdblReal, &pdblImg);
        }
        else
        {
            sciErr = getMatrixOfDoubleInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &pdblReal);
        }
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();
    sciprint("Double (%d x %d)\n", iRows, iCols);
    return 0;;
}

int get_poly_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int i;
    int iLen            = 0;
    int iRows           = 0;
    int iCols           = 0;
    char pstVar[16];
    int* piCoeff        = NULL;
    double** pdblReal   = NULL;
    double** pdblImg    = NULL;

    sciErr = getPolyVariableName(_pvCtx, _piAddr, pstVar, &iLen);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (_iItemPos == 0)
    {
        //not in list
        sciErr = getMatrixOfPoly(_pvCtx, _piAddr, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piCoeff     = (int*)malloc(sizeof(int) * iRows * iCols);
        sciErr = getMatrixOfPoly(_pvCtx, _piAddr, &iRows, &iCols, piCoeff, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        pdblReal    = (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblImg     = (double**)malloc(sizeof(double*) * iRows * iCols);

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblImg[i]  = (double*)malloc(sizeof(double) * piCoeff[i]);
        }

        if (isVarComplex(_pvCtx, _piAddr))
        {
            sciErr = getComplexMatrixOfPoly(_pvCtx, _piAddr, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            sciErr = getMatrixOfPoly(_pvCtx, _piAddr, &iRows, &iCols, piCoeff, pdblReal);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
    }
    else
    {
        sciErr = getMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piCoeff = (int*)malloc(sizeof(int) * iRows * iCols);

        sciErr = getMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, piCoeff, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        pdblReal    = (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblImg     = (double**)malloc(sizeof(double*) * iRows * iCols);

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblImg[i]  = (double*)malloc(sizeof(double) * piCoeff[i]);
        }

        if (isVarComplex(_pvCtx, _piAddr))
        {
            sciErr = getComplexMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
        }
        else
        {
            sciErr = getMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, piCoeff, pdblReal);
        }
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();
    sciprint("Poly  (%d x %d), varname : \'%s\'\n", iRows, iCols, pstVar);

    for (i = 0 ; i < iRows * iCols ; i++)
    {
        free(pdblReal[i]);
        free(pdblImg[i]);
    }

    free(pdblReal);
    free(pdblImg);
    free(piCoeff);
    return 0;;
}
int get_boolean_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int iRows       = 0;
    int iCols       = 0;
    int* piBool     = NULL;

    if (_iItemPos == 0)
    {
        sciErr = getMatrixOfBoolean(_pvCtx, _piAddr, &iRows, &iCols, &piBool);
    }
    else
    {
        sciErr = getMatrixOfBooleanInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &piBool);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();
    sciprint("Boolean (%d x %d)\n", iRows, iCols);
    return 0;
}
int get_sparse_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int iRows           = 0;
    int iCols           = 0;
    int iItem           = 0;
    int* piNbRow        = NULL;
    int* piColPos       = NULL;
    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    if (_iItemPos == 0)
    {
        //Not in list
        if (isVarComplex(_pvCtx, _piAddr))
        {
            sciErr = getComplexSparseMatrix(_pvCtx, _piAddr, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal, &pdblImg);
        }
        else
        {
            sciErr = getSparseMatrix(_pvCtx, _piAddr, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal);
        }
    }
    else
    {
        if (isVarComplex(_pvCtx, _piAddr))
        {
            sciErr = getComplexSparseMatrixInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal, &pdblImg);
        }
        else
        {
            sciErr = getSparseMatrixInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal);
        }
    }

    insert_indent();
    sciprint("Sparse (%d x %d), Item(s) : %d \n", iRows, iCols, iItem);
    return 0;;
}

int get_bsparse_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int iRows       = 0;
    int iCols       = 0;
    int iItem       = 0;
    int* piNbRow    = NULL;
    int* piColPos   = NULL;

    if (_iItemPos == 0)
    {
        //Not in list
        sciErr = getBooleanSparseMatrix(_pvCtx, _piAddr, &iRows, &iCols, &iItem, &piNbRow, &piColPos);
    }
    else
    {
        sciErr = getBooleanSparseMatrixInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &iItem, &piNbRow, &piColPos);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();
    sciprint("Boolean Sparse (%d x %d), Item(s) : %d \n", iRows, iCols, iItem);
    return 0;;
}
int get_integer_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int iPrec               = 0;
    int iRows               = 0;
    int iCols               = 0;
    char* pcData            = NULL;
    short* psData           = NULL;
    int* piData             = NULL;
    unsigned char* pucData  = NULL;
    unsigned short* pusData = NULL;
    unsigned int* puiData   = NULL;

    if (_iItemPos == 0)
    {
        //Not in list
        sciErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddr, &iPrec);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        switch (iPrec)
        {
            case SCI_INT8 :
                sciErr = getMatrixOfInteger8(_pvCtx, _piAddr, &iRows, &iCols, &pcData);
                break;
            case SCI_INT16 :
                sciErr = getMatrixOfInteger16(_pvCtx, _piAddr, &iRows, &iCols, &psData);
                break;
            case SCI_INT32 :
                sciErr = getMatrixOfInteger32(_pvCtx, _piAddr, &iRows, &iCols, &piData);
                break;
            case SCI_UINT8 :
                sciErr = getMatrixOfUnsignedInteger8(_pvCtx, _piAddr, &iRows, &iCols, &pucData);
                break;
            case SCI_UINT16 :
                sciErr = getMatrixOfUnsignedInteger16(_pvCtx, _piAddr, &iRows, &iCols, &pusData);
                break;
            case SCI_UINT32 :
                sciErr = getMatrixOfUnsignedInteger32(_pvCtx, _piAddr, &iRows, &iCols, &puiData);
                break;
            default :
                return 1;
        }
    }
    else
    {
        sciErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddr, &iPrec);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        switch (iPrec)
        {
            case SCI_INT8 :
                sciErr = getMatrixOfInteger8InList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &pcData);
                break;
            case SCI_INT16 :
                sciErr = getMatrixOfInteger16InList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &psData);
                break;
            case SCI_INT32 :
                sciErr = getMatrixOfInteger32InList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &piData);
                break;
            case SCI_UINT8 :
                sciErr = getMatrixOfUnsignedInteger8InList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &pucData);
                break;
            case SCI_UINT16 :
                sciErr = getMatrixOfUnsignedInteger16InList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &pusData);
                break;
            case SCI_UINT32 :
                sciErr = getMatrixOfUnsignedInteger32InList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, &puiData);
                break;
            default :
                return 1;
        }
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();

    if (iPrec > 10)
    {
        sciprint("Unsigned ");
    }

    sciprint("Integer %d bits (%d x %d)\n", (iPrec % 10) * 8, iRows, iCols);
    return 0;;
}
int get_string_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    int i;
    int iRows       = 0;
    int iCols       = 0;
    int* piLen      = NULL;
    char **pstData  = NULL;

    if (_iItemPos == 0)
    {
        //Not in list
        sciErr = getMatrixOfString(_pvCtx, _piAddr, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        sciErr = getMatrixOfString(_pvCtx, _piAddr, &iRows, &iCols, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        sciErr = getMatrixOfString(_pvCtx, _piAddr, &iRows, &iCols, piLen, pstData);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        sciErr = getMatrixOfStringInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);

        sciErr = getMatrixOfStringInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        sciErr = getMatrixOfStringInList(_pvCtx, _piParent, _iItemPos, &iRows, &iCols, piLen, pstData);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();
    sciprint("Strings (%d x %d)\n", iRows, iCols);
    return 0;;
}
int get_pointer_info(void* _pvCtx, int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    SciErr sciErr;
    void* pvPtr     = NULL;

    if (_iItemPos == 0)
    {
        sciErr = getPointer(_pvCtx, _piAddr, &pvPtr);
    }
    else
    {
        sciErr = getPointerInList(_pvCtx, _piParent, _iItemPos, &pvPtr);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    insert_indent();
    sciprint("Pointer : 0x%08X\n", pvPtr);
    return 0;
}
