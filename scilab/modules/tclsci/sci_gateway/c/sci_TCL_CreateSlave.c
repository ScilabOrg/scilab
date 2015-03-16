/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 20015 - Scilab-Enterprises - Anais AUBERT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "InitTclTk.h"
#include "Scierror.h"
#include "localization.h"
#include "InitializeTclTk.h"
#include "GlobalTclInterp.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/



int sci_TCL_CreateSlave(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    int* piAddrl2 = NULL;
    char* slaveName = NULL;
    int *piBool = FALSE;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (!existsGlobalInterp())
    {
        Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
        return 0;
    }

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        static int n2, m2;
        Tcl_Interp *TCLinterpreter = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &slaveName))
        {
            Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }

        if (nbInputArgument(pvApiCtx) == 2)
        {

            if (checkInputArgumentType(pvApiCtx, 2, sci_boolean))
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 1.
                if (getScalarBoolean(pvApiCtx, piAddrl2, piBool))
                {
                    Scierror(999, _("%s: Wrong type for argument #%d: A boolean expected.\n"), fname, 2);
                    return 1;
                }
            }

        }
        if (existsSlaveInterp(slaveName))
        {
            Scierror(999, _("%s: Slave interpreter already exists.\n"), fname);
        }
        if (!existsGlobalInterp())
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            return 0;
        }

        if (piBool)
        {
            Tcl_CreateSlave(getTclInterp(), slaveName, *piBool);
            releaseTclInterp();
            Tcl_Preserve(TCLinterpreter) ;
        }
        else
        {
            Tcl_CreateSlave(getTclInterp(), slaveName, FALSE);
            releaseTclInterp();
            Tcl_Preserve(TCLinterpreter) ;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
