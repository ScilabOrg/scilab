/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 * desc : interface for xs2png routine
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_scilab.h"
#include "gw_gui.h"
#include "stack-c.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
#include "exportUserInterface.hxx"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_exportUI(char * fname, unsigned long fname_len)
{
    int iFigureId = 0; // id of the figure to export
    int iRows = 0;
    int iCols = 0;
    size_t stackPointer = 0;
    int *piAddr1;

    CheckOutputArgument(pvApiCtx, 0, 1);
    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 1);

    if (GetType(1) == sci_handles) // exportUI(figHandle)
    {
        char *pstFigureUID = NULL;
        char *pstHandleType = NULL;
        int *piFigureId = &iFigureId;

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &iRows, &iCols, &stackPointer);
        if (iRows * iCols != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
        }

        pstFigureUID = getObjectFromHandle((unsigned long) * (hstk(stackPointer)));

        getGraphicObjectProperty(pstFigureUID, __GO_TYPE__, jni_string, (void **)&pstHandleType);
        if (strcmp(pstHandleType, __GO_FIGURE__) != 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return FALSE;
        }

        getGraphicObjectProperty(pstFigureUID, __GO_ID__, jni_int, (void **)&piFigureId);
    }
    else
    {
        //get variable address of the input argument
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isDoubleType(pvApiCtx, piAddr1)) // exportUI(figId)
        {
            getScalarDouble(pvApiCtx, piAddr1, iFigureId);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return FALSE;
        }
    }
    // call the export function
    exportUserInterface(iFigureId);

    AssignOutputVariable(pvApiCtx, 1) = 0;

    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
