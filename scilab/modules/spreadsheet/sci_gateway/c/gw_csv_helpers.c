/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#include <string.h>
#include "api_scilab.h"
#include "sci_types.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "gw_csv_helpers.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
// =============================================================================
char *csv_getArgumentAsStringWithEmptyManagement(void* _pvCtx, int _iVar, const char *fname,
        const char *defaultValue,
        int *iErr)
{
    SciErr sciErr;
    char *returnedValue = NULL;
    int *piAddressVar = NULL;
    int iType = 0;
    int m = 0, n = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        *iErr = sciErr.iErr;
        return NULL;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        *iErr = sciErr.iErr;
        return NULL;
    }

    if (iType != sci_strings)
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVar))
        {
            /* [] equals default value */
            if (defaultValue)
            {
                *iErr = 0;
                returnedValue = strdup(defaultValue);
            }
            else
            {
                *iErr = 0;
                returnedValue = NULL;
            }
        }
        else
        {
            *iErr = API_ERROR_INVALID_TYPE;
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, _iVar);
            return NULL;
        }
    }
    else
    {
        *iErr = checkVarDimension(pvApiCtx, piAddressVar, 1, 1);

        if (*iErr == 0 )
        {
            *iErr = API_ERROR_CHECK_VAR_DIMENSION;
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, _iVar);
            return NULL;
        }

        *iErr = getAllocatedSingleString(pvApiCtx, piAddressVar, &returnedValue);
        if (*iErr)
        {
            if (returnedValue)
            {
                FREE(returnedValue);
            }

            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return NULL;
        }
        if (strlen(returnedValue) == 0)
        {
            returnedValue = strdup(defaultValue);
        }
    }
    return returnedValue;
}
// =============================================================================

char *csv_getArgumentAsString(void* _pvCtx, int _iVar,
                              const char *fname, int *iErr)
{
    SciErr sciErr;

    int *piAddressVar = NULL;
    int m = 0, n = 0;
    int iType = 0;

    char *returnedValue = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    if (iType != sci_strings)
    {
        *iErr = API_ERROR_INVALID_TYPE;
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return NULL;
    }

    *iErr = checkVarDimension(pvApiCtx, piAddressVar, 1, 1);

    if (*iErr == 0 )
    {
        *iErr = API_ERROR_CHECK_VAR_DIMENSION;
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, _iVar);
        return NULL;
    }

    *iErr = getAllocatedSingleString(pvApiCtx, piAddressVar, &returnedValue);
    if (*iErr)
    {
        if (returnedValue)
        {
            FREE(returnedValue);
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return NULL;
    }
    return returnedValue;
}
// =============================================================================
double csv_getArgumentAsScalarDouble(void* _pvCtx, int _iVar,
                                     const char *fname, int *iErr)
{
    SciErr sciErr;
    double dValue = 0.;
    int *piAddressVar = NULL;
    int m = 0, n = 0;
    int iType = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        *iErr = sciErr.iErr;
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        *iErr = sciErr.iErr;
        return 0;
    }

    if (iType != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), fname, _iVar);
        *iErr =  API_ERROR_INVALID_TYPE;
        return 0;
    }

    *iErr = checkVarDimension(pvApiCtx, piAddressVar, 1, 1);

    if (*iErr == 0 )
    {
        *iErr = API_ERROR_CHECK_VAR_DIMENSION;
        Scierror(999, _("%s: Wrong size for input argument #%d: A double expected.\n"), fname, _iVar);
        return 0;
    }

    *iErr = getScalarDouble(pvApiCtx, piAddressVar, &dValue);
    return dValue;
}
// =============================================================================
int csv_getArgumentAsScalarBoolean(void* _pvCtx, int _iVar,
                                   const char *fname, int *iErr)
{
    SciErr sciErr;
    int bValue = 0;
    int *piAddressVar = NULL;
    int m = 0, n = 0;
    int iType = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        *iErr = sciErr.iErr;
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        *iErr = sciErr.iErr;
        return 0;
    }

    if (iType != sci_boolean)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, _iVar);
        *iErr =  API_ERROR_INVALID_TYPE;
        return 0;
    }

    *iErr = checkVarDimension(pvApiCtx, piAddressVar, 1, 1);

    if (*iErr == 0 )
    {
        *iErr = API_ERROR_CHECK_VAR_DIMENSION;
        Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, _iVar);
        return 0;
    }

    *iErr = getScalarBoolean(pvApiCtx, piAddressVar, &bValue);
    return bValue;
}
// =============================================================================
char **csv_getArgumentAsMatrixOfString(void* _pvCtx, int _iVar,
                                       const char *fname,
                                       int *m, int *n, int *iErr)
{
    SciErr sciErr;
    char **pStringValues = NULL;
    int *piAddressVar = NULL;
    int m_ = 0, n_ = 0;
    int iType = 0;
    int *lengthStringValues = NULL;
    int i = 0;

    *m = 0;
    *n = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    if (iType != sci_strings)
    {
        *iErr =  API_ERROR_INVALID_TYPE;
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, _iVar);
        return NULL;
    }

    *iErr = getAllocatedMatrixOfString(pvApiCtx, piAddressVar, &m_, &n_, &pStringValues);
    if (*iErr != 0)
    {
        if (pStringValues)
        {
            FREE(pStringValues);
        }
        return NULL;
    }

    *iErr = 0;
    *m = m_;
    *n = n_;

    return pStringValues;
}
// =============================================================================
int csv_isRowVector(void* _pvCtx, int _iVar)
{
    SciErr sciErr;
    int *piAddressVar = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        return 0;
    }
    return isRowVector(pvApiCtx, piAddressVar);
}
// =============================================================================
int csv_isColumnVector(void* _pvCtx, int _iVar)
{
    SciErr sciErr;
    int *piAddressVar = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        return 0;
    }
    return isColumnVector(pvApiCtx, piAddressVar);
}
// =============================================================================
int csv_isScalar(void* _pvCtx, int _iVar)
{
    SciErr sciErr;
    int *piAddressVar = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        return 0;
    }
    return isScalar(pvApiCtx, piAddressVar);
}
// =============================================================================
int csv_isDoubleScalar(void* _pvCtx, int _iVar)
{
    SciErr sciErr;
    int *piAddressVar = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (csv_isScalar(_pvCtx, _iVar))
    {
        int iType = 0;
        sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
        if (sciErr.iErr)
        {
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVar) == 0)
        {
            return (iType == sci_matrix);
        }
    }
    return 0;
}
// =============================================================================
int csv_isEmpty(void* _pvCtx, int _iVar)
{
    SciErr sciErr;
    int *piAddressVar = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        if (sciErr.iErr)
        {
            return 0;
        }
    }

    return isEmptyMatrix(pvApiCtx, piAddressVar);
}
// =============================================================================
int *csv_getArgumentAsMatrixofIntFromDouble(void* _pvCtx, int _iVar,
        const char *fname,
        int *m, int *n, int *iErr)
{
    int *iReturnedArray = NULL;
    double *dArray = NULL;
    SciErr sciErr;
    int *piAddressVar = NULL;
    int iType = 0;
    int m_ = 0;
    int n_ = 0;
    int i = 0;
    int j = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    if (iType != sci_matrix)
    {
        *iErr =  API_ERROR_INVALID_TYPE;
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of double expected.\n"), fname, _iVar);
        return NULL;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar, &m_, &n_, &dArray);
    if (sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        return NULL;
    }

    for (j = 0; j < m_ * n_; j++)
    {
        int iVal = (int)dArray[j];
        if ((double)iVal != dArray[j])
        {
            *iErr = API_ERROR_GET_DOUBLE;
            Scierror(999, _("%s: Wrong value for input argument #%d: A matrix of double, with integer values, expected.\n"), fname, _iVar);
            return NULL;
        }
    }

    iReturnedArray = (int*)MALLOC(sizeof(int) * (m_ * n_));
    if (iReturnedArray == NULL)
    {
        *iErr = API_ERROR_ALLOC_DOUBLE;
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return NULL;
    }

    for (j = 0; j < m_ * n_; j++)
    {
        iReturnedArray[j] = (int)dArray[j];
    }

    *iErr = 0;
    *m = m_;
    *n = n_;

    return iReturnedArray;
}
// =============================================================================
