/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

extern "C"
{
#include <hdf5.h>
#include <string.h>
#include "gw_hdf5.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "h5_readDataFromFile.h"
#include "h5_readDataFromFile_v1.h"
}
#include "import_from_hdf5_v1.hxx"

bool import_double_v1(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    double *pdblReal = NULL;
    double *pdblImg = NULL;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;
    SciErr sciErr;

#ifdef TIME_DEBUG
    LARGE_INTEGER iStart, iEnd, iFreq;

    QueryPerformanceFrequency(&iFreq);
    QueryPerformanceCounter(&iStart);
#endif

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    iComplex = isComplexData(_iDatasetId);
    if (iRet)
    {
        return false;
    }

    if (iRows * iCols != 0)
    {
        if (iComplex)
        {
            pdblReal = (double *)MALLOC(iRows * iCols * sizeof(double));
            pdblImg = (double *)MALLOC(iRows * iCols * sizeof(double));
            iRet = readDoubleComplexMatrix_v1(_iDatasetId, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            pdblReal = (double *)MALLOC(iRows * iCols * sizeof(double));
            iRet = readDoubleMatrix_v1(_iDatasetId, iRows, iCols, pdblReal);
        }

        if (iRet)
        {
            return false;
        }
    }
    else
    {
        /*bug 7224 : to close dataset */
        iRet = readEmptyMatrix(_iDatasetId);
        if (iRet)
        {
            return false;
        }
    }

    if (_piAddress == NULL)
    {
        if (iComplex)
        {
            sciErr = createNamedComplexMatrixOfDouble(pvApiCtx, _pstVarname, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfDouble(pvApiCtx, _pstVarname, iRows, iCols, pdblReal);
        }
    }
    else                        //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr = createComplexMatrixOfDoubleInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createMatrixOfDoubleInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pdblReal);
        }
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "double_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif

    if (pdblReal)
    {
        FREE(pdblReal);
    }

    if (pdblImg)
    {
        FREE(pdblImg);
    }

    if (iRet)
    {
        return false;
    }

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iEnd);
    double dblTime = ((iEnd.QuadPart - iStart.QuadPart) * 1000.0) / iFreq.QuadPart;

    printf("Total Double : %0.3f ms\n\n", dblTime);
#endif

    return true;
}
