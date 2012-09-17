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

#include <string>

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5link(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    H5Object * target = 0;
    SciErr err;
    int * addr = 0;
    char * name = 0;
    char * targetName = 0;
    std::string _name;
    std::string _targetName;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        hobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!hobj)
        {
            Scierror(999, _("%s: Can not print H5Object: invalid object.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A H5Object expected.\n"), fname, 1);
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &name) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    _name = std::string(name);
    freeAllocatedSingleString(name);

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        target = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!target)
        {
            Scierror(999, _("%s: Can not use H5Object: invalid object.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &targetName) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        _targetName = std::string(targetName);
        freeAllocatedSingleString(targetName);
    }

    try
    {
        if (target)
        {
            HDF5Scilab::createSoftLink(*hobj, _name, *target);
        }
        else
        {
            HDF5Scilab::createSoftLink(*hobj, _name, _targetName);
        }
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
