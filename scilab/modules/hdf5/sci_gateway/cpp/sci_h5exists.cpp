/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <set>
#include <string>

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "expandPathVariable.h"
}

#include "HDF5Scilab.hxx"
#include "H5File.hxx"

using namespace org_modules_hdf5;

/*
  Test object existence
  Scilab prototype:
  - h5exists(obj, location)
  - h5exists(obj, location, attrName)
  - h5exists(filename, location)
  - h5exists(filename, location, attrName)
*/

/*--------------------------------------------------------------------------*/
int sci_h5exists(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    char * str = 0;
    char ** locations = 0;
    char ** attrNames = 0;
    char * expandedPath = 0;
    std::string filename;
    int rowl, coll;
    int rowa, cola;
    int size;
    int * ret = 0;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 2, 3);

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
            Scierror(999, _("%s: Invalid H5Object.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        expandedPath = expandPathVariable(str);
        filename = std::string(expandedPath);
        FREE(expandedPath);
        freeAllocatedSingleString(str);
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, addr, &rowl, &coll, &locations) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    if (nbIn == 3 && (rowl != 1 || coll != 1))
    {
        freeAllocatedMatrixOfString(rowl, coll, locations);
        Scierror(999, _("%s: Wrong size for argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    size = rowl * coll;

    if (nbIn == 3)
    {
        err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
        if (err.iErr)
        {
            freeAllocatedMatrixOfString(rowl, coll, locations);
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr))
        {
            freeAllocatedMatrixOfString(rowl, coll, locations);
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &rowa, &cola, &attrNames) != 0)
        {
            freeAllocatedMatrixOfString(rowl, coll, locations);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        size = rowa * cola;
    }

    try
    {
        if (hobj)
        {
            ret = HDF5Scilab::exists(*hobj, size, const_cast<const char **>(locations), const_cast<const char **>(attrNames));
        }
        else
        {
            ret = HDF5Scilab::exists(filename, size, const_cast<const char **>(locations), const_cast<const char **>(attrNames));
        }

        freeAllocatedMatrixOfString(rowl, coll, locations);
        if (attrNames)
        {
            freeAllocatedMatrixOfString(rowa, cola, attrNames);
        }
    }
    catch (const std::exception & e)
    {
        freeAllocatedMatrixOfString(rowl, coll, locations);
        if (attrNames)
        {
            freeAllocatedMatrixOfString(rowa, cola, attrNames);
        }
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    if (attrNames)
    {
        err = createMatrixOfBoolean(pvApiCtx, nbIn + 1, rowa, cola, ret);
    }
    else
    {
        err = createMatrixOfBoolean(pvApiCtx, nbIn + 1, rowl, coll, ret);
    }

    delete[] ret;

    if (err.iErr)
    {
        Scierror(999, _("%s: Can not create output argument.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
