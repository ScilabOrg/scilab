/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
}

#include "HDF5Scilab.hxx"
#include "H5Options.hxx"

using namespace org_modules_hdf5;

/*
  Set global options in HDF5
*/

int sci_h5option(char *fname, unsigned long fname_len)
{
    SciErr err;
    int * addr = 0;
    int row;
    int col;
    char ** strs = 0;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 0, 1);

    if (nbIn == 0)
    {
        std::vector<std::string> v = H5Options::getOptions();
        const char ** opts = new const char *[v.size()];
        for (int i = 0; i < v.size(); i++)
        {
            opts[i] = v[i].c_str();
        }

        err = createMatrixOfString(pvApiCtx, 1, (int)(v.size() / 2), 2, opts);
        delete[] opts;
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not write output argument.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = 1;
        ReturnArguments(pvApiCtx);

        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &strs) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    if (col != 2)
    {
        freeAllocatedMatrixOfString(row, col, strs);
        Scierror(999, _("%s: Wrong size for input argument #%d: two columns expected.\n"), fname, 1);
        return 0;
    }

    try
    {
        for (int i = 0; i < row; i++)
        {
            H5Options::setOption(strs[i], strs[row + i]);
        }
    }
    catch (const std::exception & e)
    {
        freeAllocatedMatrixOfString(row, col, strs);
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    freeAllocatedMatrixOfString(row, col, strs);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
