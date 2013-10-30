/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C" {
#include "Scierror.h"
#include "gw_external_objects_java.h"
#include "MALLOC.h"
}

#include "ScilabJavaEnvironment.hxx"

using namespace org_scilab_modules_external_objects_java;
using namespace org_modules_external_objects;

int sci_jcreatejar(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int res = 1;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    // Input argument 1: jar file path
    int* addr1 = NULL;
    getVarAddressFromPosition(pvApiCtx, 1, &addr1);

    char *jarFilePath = NULL;
    if (getAllocatedSingleString(pvApiCtx, addr1, &jarFilePath))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    // Input argument 2: paths of files to be jar-red
    int* addr2 = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &addr2);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(jarFilePath);
        printError(&sciErr, 0);
        return 1;
    }

    if (!isStringType(pvApiCtx, addr2))
    {
        freeAllocatedSingleString(jarFilePath);
        Scierror(202, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 2);
        return 1;
    }

    int nbRow = 0;
    int nbCol = 0;
    sciErr = getVarDimension(pvApiCtx, addr2, &nbRow, &nbCol);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(jarFilePath);
        printError(&sciErr, 0);
        return 1;
    }

    if ((nbRow < 1 || nbCol != 1) && (nbCol < 1 || nbRow != 1))
    {
        freeAllocatedSingleString(jarFilePath);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimension for argument #%d: A row or a column expected."), 2);
    }
    int nbFilePaths = nbRow * nbCol;

    char **filePaths = NULL;
    if (getAllocatedMatrixOfString(pvApiCtx, addr2, &nbRow, &nbCol, &filePaths))
    {
        freeAllocatedSingleString(jarFilePath);
        printError(&sciErr, 0);
        return 1;
    }

    try
    {
        ScilabJavaEnvironment::start();
        ScilabJavaEnvironment *javaEnvironment = ScilabJavaEnvironment::getInstance();
        if (javaEnvironment)
        {
            res = javaEnvironment->createJarArchive(jarFilePath, filePaths, nbFilePaths);
        }
        else
        {
            Scierror(999, "%s: No Java environment available (instance is null).", fname);
            res = 1;
        }
    }
    catch (std::exception &e)
    {
        Scierror(999, "%s: An error occured: %s", fname, e.what());
        res = 1;
    }

    freeAllocatedSingleString(jarFilePath);
    freeAllocatedMatrixOfString(nbRow, nbCol, filePaths);

    // Create boolean return value
    int *ret = NULL;
    sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &ret);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    ret[0] = (res == 0) ? 1 : 0;

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
