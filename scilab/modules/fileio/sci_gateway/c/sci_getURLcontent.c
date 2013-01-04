/*
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "sciprint.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "dlManager.h"
#include "localization.h"
/* ==================================================================== */
void freeAllocatedStrings(char *url, char *username, char *password)
{
    if (url != NULL)
    {
        freeAllocatedSingleString(url);
    }
    if (username != NULL)
    {
        freeAllocatedSingleString(username);
    }
    if (password != NULL)
    {
        freeAllocatedSingleString(password);
    }
}
/* ==================================================================== */
int sci_getURLcontent(char *fname, int fname_len)
{
    SciErr sciErr;
    int length = 0;

    int *piAddr = NULL;
    char *url = NULL;
    char *username = NULL;
    char *password = NULL;

    int iRows = 0, iCols = 0;
    int iType = 0;
    int *piAddressVarOne = NULL;
    int ret = 0;
    char *content = NULL;

    CheckLhs(0, 1);
    CheckRhs(1, 3);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for argument %d: A single string expected.\n"), fname, 1);
        return 0;
    }

    ret = getAllocatedSingleString(pvApiCtx, piAddressVarOne, &url);

    if (ret)
    {
        Scierror(999, _("%s: Wrong type for argument %d: A single string expected.\n"), fname, 1);
        freeAllocatedStrings(url, username, password);
        return 0;
    }

    if (Rhs > 1)
    {
        /* Specify the username */
        int *piAddressVarTwo = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedStrings(url, username, password);
            return 0;
        }

        if (!isStringType(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong type for argument %d: A single string expected.\n"), fname, 2);
            freeAllocatedStrings(url, username, password);
            return 0;
        }

        ret = getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &username);

        if (ret)
        {
            Scierror(999, _("%s: Wrong type for argument %d: A single string expected.\n"), fname, 2);
            freeAllocatedStrings(url, username, password);
            return 0;
        }


    }

    if (Rhs > 2)
    {
        /* Specify the password */
        int *piAddressVarThree = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedStrings(url, username, password);
            return 0;
        }

        if (!isStringType(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong type for argument %d: A single string expected.\n"), fname, 3);
            freeAllocatedStrings(url, username, password);
            return 0;
        }

        ret = getAllocatedSingleString(pvApiCtx, piAddressVarThree, &password);

        if (ret)
        {
            Scierror(999, _("%s: Wrong type for argument %d: A single string expected.\n"), fname, 3);
            freeAllocatedStrings(url, username, password);
            return 0;
        }

    }

    /* Set to NULL if 0 length strings */
    if (url != NULL)
        if (strlen(url) == 0)
        {
            url = NULL;
        }
    if (username != NULL)
        if (strlen(username) == 0)
        {
            username = NULL;
        }
    if (password != NULL)
        if (strlen(password) == 0)
        {
            password = NULL;
        }


    // call function
    content = downloadContent(url, username, password);

    if (content != NULL)
    {
        //create new variable
        int res = createSingleString(pvApiCtx, Rhs + 1, content);
        FREE(content);
        content = NULL;
        if (res)
        {
            Scierror(999, _("%s: Could not create the output argument.\n"));
            freeAllocatedStrings(url, username, password);
            return 0;
        }
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
}
/* ==================================================================== */
