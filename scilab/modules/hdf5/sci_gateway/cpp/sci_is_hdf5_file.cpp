/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#include <hdf5.h>
#include "gw_hdf5.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "h5_fileManagement.h"
#include "sciprint.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
}


int sci_is_hdf5_file(char *fname, int* pvApiCtx)
{
    SciErr sciErr;
    int *piAddr     = NULL;
    char* pstFile   = NULL;


    CheckRhs(1, 1);
    CheckLhs(1, 4);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if(getAllocatedSingleString(pvApiCtx, piAddr, &pstFile))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    char* pstFileName = expandPathVariable(pstFile);
    if(isHDF5File(pstFileName))
    {
        createScalarBoolean(pvApiCtx, Rhs + 1, 1);
    }
    else
    {
        createScalarBoolean(pvApiCtx, Rhs + 1, 0);
    }

    FREE(pstFileName);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
