/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "splitpath.h"
/*--------------------------------------------------------------------------*/
#define FILEPARTS_PATH_SELECTOR L"path"
#define FILEPARTS_FNAME_SELECTOR L"fname"
#define FILEPARTS_EXTENSION_SELECTOR L"extension"
/*--------------------------------------------------------------------------*/
int sci_fileparts(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    int *lenStVarOne = NULL;
    wchar_t *pStVarTwo = NULL;
    int *piAddressVarTwo = NULL;

    wchar_t **drv = NULL;
    wchar_t **dir = NULL;
    wchar_t **name = NULL;
    wchar_t **ext = NULL;
    wchar_t **path_out = NULL;
    wchar_t **output_value = NULL;
    int iRows = 0;
    int iCols = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);
    int i = 0; // loop index

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 3);

    if ((iRhs == 2) && (iLhs != 1))
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarOne, &iRows, &iCols, &pStVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
        return 0;
    }

    

    if (iRhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }
    }

    drv = (wchar_t **) MALLOC(sizeof(wchar_t*) * (iRows*iCols + 1));
    dir = (wchar_t **) MALLOC(sizeof(wchar_t*) * (iRows*iCols + 1));
    name = (wchar_t *) MALLOC(sizeof(wchar_t*) * (iRows*iCols + 1));
    ext = (wchar_t **) MALLOC(sizeof(wchar_t*) * (iRows*iCols + 1));
    path_out = (wchar_t **) MALLOC(sizeof(wchar_t*) * (iRows*iCols + 1));
    lenStVarOne = (int *)MALLOC(sizeof(int) * (iRows * iCols));
    
    if ((drv == NULL) || (dir == NULL) || (name == NULL) || (ext == NULL) || (path_out == NULL) || lenStVarOne == NULL)
    {
        if (pStVarOne)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            pStVarOne = NULL;
        }
        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }
        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
        if (path_out)
        {
            FREE(path_out);
            path_out = NULL;
        }
        if (lenStVarOne)
        {
            FREE(lenStVarOne);
            lenStVarOne = NULL;
        }
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }
    

    if (pStVarTwo)              /* Rhs == 2 */
    {
    
        output_value = (wchar_t **) MALLOC(sizeof(wchar_t*) * (iRows*iCols + 1));
        if (output_value == NULL)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            freeAllocatedSingleWideString(pStVarTwo);
            FREE(drv);
            FREE(dir);
            FREE(name);
            FREE(ext);
            FREE(path_out);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        for (i = 0; i < iRows*iCols; i++)
        {

            lenStVarOne[i] = (int)wcslen(pStVarOne[i]);
            drv[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            dir[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            name[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            ext[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            path_out[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            output_value[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));

            if ((drv[i] == NULL) || (dir[i] == NULL) || (name[i] == NULL) || (ext[i] == NULL) || (path_out[i] == NULL) || (output_value[i] == NULL))
            {
                if (pStVarOne[i])
                {
                    freeArrayOfWideString(pStVarOne, i);
                    pStVarOne[i] = NULL;
                }
                if (pStVarTwo)
                {
                    freeAllocatedSingleWideString(pStVarTwo);
                    pStVarTwo = NULL;
                }
                if (drv[i])
                {
                    freeArrayOfWideString(drv, i);
                    drv[i] = NULL;
                }
                if (dir[i])
                {
                    freeArrayOfWideString(dir, i);
                    dir[i] = NULL;
                }
                if (name[i])
                {
                    freeArrayOfWideString(name, i);
                    name[i] = NULL;
                }
                if (ext[i])
                {
                    freeArrayOfWideString(ext, i);
                    ext[i] = NULL;
                }
                if (path_out[i])
                {
                    freeArrayOfWideString(path_out, i);
                    path_out[i] = NULL;
                }
                if (output_value[i])
                {
                    freeArrayOfWideString(output_value, i);
                    output_value[i] = NULL;
                }

                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            splitpathW(pStVarOne[i], FALSE, drv[i], dir[i], name[i], ext[i]);

            if (wcscmp(pStVarTwo, FILEPARTS_PATH_SELECTOR) == 0)
            {
                output_value = path_out;
                wcscpy(output_value[i], drv[i]);
                wcscat(output_value[i], dir[i]);
            }
            else if (wcscmp(pStVarTwo, FILEPARTS_FNAME_SELECTOR) == 0)
            {
                output_value[i] = name[i];
            }
            else if (wcscmp(pStVarTwo, FILEPARTS_EXTENSION_SELECTOR) == 0)
            {
                output_value[i] = ext[i];
            }
            else
            {
                if (pStVarTwo[i])
                {
                    freeAllocatedSingleWideString(pStVarTwo);
                    pStVarTwo = NULL;
                }
                if (drv[i])
                {
                    freeArrayOfWideString(drv, i);
                    drv[i] = NULL;
                }
                if (dir[i])
                {
                    freeArrayOfWideString(dir, i);
                    dir[i] = NULL;
                }
                if (name[i])
                {
                    freeArrayOfWideString(name, i);
                    name[i] = NULL;
                }
                if (ext[i])
                {
                    freeArrayOfWideString(ext, i);
                    ext[i] = NULL;
                }
                if (path_out[i])
                {
                    freeArrayOfWideString(path_out, i);
                    path_out[i] = NULL;
                }
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }

        }
        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows, iCols, output_value);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
        if (path_out)
        {
            FREE(path_out);
            path_out = NULL;
        }
        if (pStVarOne)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            pStVarOne = NULL;
        }
        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }
        FREE(lenStVarOne);
        ReturnArguments(pvApiCtx);
    }
    else
    {
        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }
        
        for (i = 0; i < iRows*iCols; i++)
        {
        
            lenStVarOne[i] = (int)wcslen(pStVarOne[i]);
            drv[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            dir[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            name[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            ext[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            path_out[i] = (wchar_t*) MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
            if (drv[i] == NULL)
            {
                freeArrayOfWideString(drv, i);
                drv = NULL;
            }
            if (dir[i] == NULL)
            {
                freeArrayOfWideString(dir, i);
                dir = NULL;
            }
            if (name[i] == NULL)
            {
                freeArrayOfWideString(name, i);
                name = NULL;
            }
            if (ext[i] == NULL)
            {
                freeArrayOfWideString(ext, i);
                ext = NULL;
            }
            if (path_out[i] == NULL)
            {
                freeArrayOfWideString(path_out, i);
                path_out = NULL;
            }
            splitpathW(pStVarOne[i], FALSE, drv[i], dir[i], name[i], ext[i]);
            wcscpy(path_out[i], drv[i]);
            wcscat(path_out[i], dir[i]);
        }

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows, iCols, path_out);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
        
        sciErr = createMatrixOfWideString(pvApiCtx, iRhs +2, iRows, iCols, name);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs +3, iRows, iCols, ext);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }


        AssignOutputVariable(pvApiCtx, 3) = iRhs + 3;
        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
        if (path_out)
        {
            FREE(path_out);
            path_out = NULL;
        }
        if (pStVarOne)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            pStVarOne = NULL;
        }
        ReturnArguments(pvApiCtx);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
