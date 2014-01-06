/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include <mpi.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "serialization.h"

#define TAG 0


int sci_mpi_isend(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piAddr2 = NULL;
    int *piAddr3 = NULL;

    int *piBuffer = NULL;
    int iBufferSize = 0;
    double NodeID = 0;
    int iRequestID = 0;
    double dblRequestID = 0;

    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr2, &NodeID))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr3, &dblRequestID))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 3);
        return 0;
    }

    iRequestID = (int)dblRequestID;
    if (iRequestID < 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive value expected.\n"), fname, 3);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    //convert data from Scilab to MPI
    iRet = serialize_to_mpi(pvApiCtx, piAddr, &piBuffer, &iBufferSize);
    if (iRet)
    {
        Scierror(999, _("Unable to serialize data\n"));
        return 0;
    }

    iRet = MPI_Isend(piBuffer, iBufferSize, MPI_INT, (int)NodeID, TAG, MPI_COMM_WORLD, &request[iRequestID]);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: Could not send the variable to the node %d: %s\n"), fname, NodeID, error_string);
        return 0;
    }

    if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, !iRet))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
