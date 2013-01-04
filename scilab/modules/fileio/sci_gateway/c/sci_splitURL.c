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
#include <libxml/uri.h>
#include "sciprint.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"

/* ==================================================================== */
int sci_splitURL(char *fname, int fname_len)
{
    SciErr sciErr;
    int length = 0;
    int *piAddr = NULL;
    char *url = NULL;
    char *dest = NULL;

    int iRows = 0, iCols = 0;
    int iType = 0;
    int *piAddressVarOne = NULL;
    char *emptyString = "";
    int ret = 0;
    xmlURIPtr c = NULL;
    int zero = 0;

    CheckLhs(0, 7);
    CheckRhs(1, 1);

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
        return 0;
    }

    c = xmlParseURI(url);

    if (c == NULL)
    {
        Scierror(999, "Could not parse the URL.\n");
        return 0;
    }

    // protocol (http, ftp...)
    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &c->scheme);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(1) = Rhs + 1;

    // server
    sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 1, 1, &c->server);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(2) = Rhs + 2;

    // path
    if (c->path != NULL)
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 3, 1, 1, &c->path);
    }
    else
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 3, 1, 1, &emptyString);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(3) = Rhs + 3;

    // query
    if (c->query != NULL)
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 4, 1, 1, &c->query);
    }
    else
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 4, 1, 1, &emptyString);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(4) = Rhs + 4;

    // user
    if (c->user != NULL)
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 5, 1, 1, &c->user);
    }
    else
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 5, 1, 1, &emptyString);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(5) = Rhs + 5;

    // port
    if (c->port != NULL)
    {
        sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 6, 1, 1, &c->port);
    }
    else
    {
        sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 6, 1, 1, &zero);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(6) = Rhs + 6;

    // Fragment
    if (c->fragment != NULL)
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 7, 1, 1, &c->fragment);
    }
    else
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 7, 1, 1, &emptyString);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(7) = Rhs + 7;

    PutLhsVar();

}

/*
probably useless or internal to libxml
// opaque
if (c->opaque != NULL)
{
sciErr = createMatrixOfString(pvApiCtx, Rhs + 5, 1, 1, &c->opaque);
}
else
{
sciErr = createMatrixOfString(pvApiCtx, Rhs + 5, 1, 1, &emptyString);
}
if (sciErr.iErr)
{
printError(&sciErr, 0);
return 0;
}
LhsVar(5) = Rhs + 5;

// authority
if (c->authority != NULL)
{
sciErr = createMatrixOfString(pvApiCtx, Rhs + 6, 1, 1, &c->authority);
}
else
{
sciErr = createMatrixOfString(pvApiCtx, Rhs + 6, 1, 1, &emptyString);
}
if (sciErr.iErr)
{
printError(&sciErr, 0);
return 0;
}
LhsVar(6) = Rhs + 6;

// Clean up
sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 10, 1, 1, &c->cleanup);
if (sciErr.iErr)
{
printError(&sciErr, 0);
return 0;
}
LhsVar(10) = Rhs + 10;

*/
