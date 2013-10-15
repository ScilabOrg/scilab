/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipDelete.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipremove(char *fname, unsigned long fname_len)
{
    int iFigureUID      = 0;
    int iDatatipUID     = 0;
    int iPolylineUID    = 0;
    double* pdblReal    = NULL;
    int* piAddr         = NULL;
    long long llHandle  = 0;
    int indexPos        = 0;
    int iErr            = 0;
    int nbRow           = 0;
    int nbCol           = 0;
    int iType           = 0;
    int *piType         = &iType;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    iErr = getScalarHandle(pvApiCtx, piAddr, &llHandle);
    if (iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if (nbInputArgument(pvApiCtx) == 1)
    {
        iFigureUID = getCurrentFigure();
        iDatatipUID = getObjectFromHandle((unsigned long) llHandle);

        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            getGraphicObjectProperty(iDatatipUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_DATATIP__)
            {
                DatatipDelete::datatipRemoveProgramHandler(getScilabJavaVM(), iDatatipUID, iFigureUID);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A handle expected.\n"), fname, 1);
            return 1;
        }
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        iPolylineUID = getObjectFromHandle((unsigned long) llHandle);

        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            getGraphicObjectProperty(iPolylineUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_POLYLINE__)
            {

                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &pdblReal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (checkInputArgumentType(pvApiCtx, 2, sci_matrix))
                {

                    if (nbRow * nbCol == 1)
                    {
                        indexPos = (int) pdblReal[0];
                        DatatipDelete::datatipRemoveProgramIndex(getScilabJavaVM(), iPolylineUID, indexPos);

                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
                        return 1;

                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
                    return 1;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Polyline");
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A handle expected.\n"), fname, 1);
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number for input argument: %d or %d expected.\n"), fname, 1, 2);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
