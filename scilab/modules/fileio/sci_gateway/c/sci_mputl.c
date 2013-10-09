/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "filesmanagement.h"
#include "MALLOC.h"
#include "BOOL.h"
#include "mputl.h"
/*--------------------------------------------------------------------------*/
int sci_mputl(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    char **pStVarOne = NULL;
    int *lenStVarOne = NULL;
    int mOne = 0, nOne = 0;
    int mnOne = 0;

    char *filename = NULL;
    int fileDescriptor = 0;
    BOOL bCloseFile = FALSE;

    int i = 0;
    int mputlErr = MPUTL_ERROR;

    if (Rhs != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return 0;
    }

    if (Lhs != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        double dValue = 0.;

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Integer expected.\n"), fname, 2);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        fileDescriptor = (int)dValue;
    }
    else if (isStringType(pvApiCtx, piAddressVarTwo))
    {
#define WRITE_ONLY_TEXT_MODE "wt"
        int f_swap = 0;
        double res = 0.0;
        int ierr = 0;
        char *expandedFileName = NULL;

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &filename))
        {
            if (filename)
            {
                freeAllocatedSingleString(filename);
            }

            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        expandedFileName = expandPathVariable(filename);

        C2F(mopen)(&fileDescriptor, expandedFileName, WRITE_ONLY_TEXT_MODE, &f_swap, &res, &ierr);
        FREE(expandedFileName);

        switch (ierr)
        {
            case MOPEN_NO_ERROR:
                break;
            case MOPEN_NO_MORE_LOGICAL_UNIT:
            {
                freeAllocatedSingleString(filename);
                Scierror(66, _("%s: Too many files opened!\n"), fname);
                return 0;
            }
            break;
            case MOPEN_CAN_NOT_OPEN_FILE:
            {
                Scierror(999, _("%s: Cannot open file %s.\n"), fname, filename);
                freeAllocatedSingleString(filename);
                return 0;
            }
            break;
            case MOPEN_NO_MORE_MEMORY:
            {
                freeAllocatedSingleString(filename);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            break;
            case MOPEN_INVALID_FILENAME:
            {
                if (filename)
                {
                    Scierror(999, _("%s: invalid filename %s.\n"), fname, filename);
                }
                else
                {
                    freeAllocatedSingleString(filename);
                    Scierror(999, _("%s: invalid filename.\n"), fname);
                }
                return 0;
            }
            break;
            case MOPEN_INVALID_STATUS:
            default:
            {
                freeAllocatedSingleString(filename);
                Scierror(999, _("%s: invalid status.\n"), fname);
                return 0;
            }
            break;
        }
        bCloseFile = TRUE;
        freeAllocatedSingleString(filename);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), fname, 2);
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
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    if (isScalar(pvApiCtx, piAddressVarOne) == FALSE && isVector(pvApiCtx, piAddressVarOne) == FALSE)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, &pStVarOne))
    {
        if (pStVarOne)
        {
            freeAllocatedMatrixOfString(mOne, nOne, pStVarOne);
        }

        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    mnOne = mOne * nOne;

    mputlErr = mputl(fileDescriptor, pStVarOne, mnOne);
    freeArrayOfString(pStVarOne, mnOne);

    if (bCloseFile)
    {
        double dErrClose = 0.;
        C2F(mclose)(&fileDescriptor, &dErrClose);
    }

    switch (mputlErr)
    {
        case MPUTL_NO_ERROR:
            createScalarBoolean(pvApiCtx, Rhs + 1, TRUE);
            LhsVar(1) = Rhs + 1;
            PutLhsVar();
            break;

        case MPUTL_INVALID_FILE_DESCRIPTOR:
            // commented for compatiblity
            // Scierror(999, _("%s: invalid file descriptor.\n"), fname);
            // break;
        case MPUTL_ERROR:
        case MPUTL_NO_WRITE_RIGHT:
        default:
            createScalarBoolean(pvApiCtx, Rhs + 1, FALSE);
            LhsVar(1) = Rhs + 1;
            PutLhsVar();
            break;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
