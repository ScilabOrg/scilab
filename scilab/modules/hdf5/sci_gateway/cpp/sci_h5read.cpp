/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "sciprint.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5read(char *fname, unsigned long fname_len)
{
    SciErr err;
    int * addr = 0;
    char * path = 0;
    char * name = 0;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        freeAllocatedSingleString(path);
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        freeAllocatedSingleString(path);
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &name) != 0)
    {
        freeAllocatedSingleString(path);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    try
    {
        HDF5Scilab::readData(path, name, Rhs + 1, pvApiCtx);
        freeAllocatedSingleString(name);
        freeAllocatedSingleString(path);
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        freeAllocatedSingleString(name);
        freeAllocatedSingleString(path);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
