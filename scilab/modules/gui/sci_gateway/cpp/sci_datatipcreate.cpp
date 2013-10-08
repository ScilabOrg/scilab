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

#include "DatatipCreate.hxx"
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

int sci_datatipcreate(char *fname, unsigned long fname_len)
{
    int iErr            = 0;
    int nbRow           = 0;
    int nbCol           = 0;
    int indexPoint      = 0;
    long long llHandle  = 0;

    int* piAddressVarOne    = NULL;
    int* piAddressVarTwo    = NULL;
    char* datatip_handler   = NULL;
    char* polylineUID       = NULL;
    double* pdblReal        = NULL;

    int iType = 0;
    int *piType = &iType;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 1;
        }

        iErr = getScalarHandle(pvApiCtx, piAddressVarOne, &llHandle);
        if (iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 1;
        }

        polylineUID = (char *)getObjectFromHandle((unsigned long) llHandle);

        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            getGraphicObjectProperty(polylineUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_POLYLINE__)
            {
                if (checkInputArgumentType(pvApiCtx, 2, sci_matrix))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &nbRow, &nbCol, &pdblReal);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if (nbRow * nbCol == 1)
                    {
                        indexPoint = (int) pdblReal[0];
                        datatip_handler = DatatipCreate::createDatatipProgramIndex(getScilabJavaVM(), (char*)polylineUID, indexPoint);
                    }
                    else if (nbRow * nbCol == 2)
                    {
                        datatip_handler = DatatipCreate::createDatatipProgramCoord(getScilabJavaVM(), (char*)polylineUID, pdblReal, 2);
                    }
                    else if (nbRow * nbCol == 3)
                    {
                        datatip_handler = DatatipCreate::createDatatipProgramCoord(getScilabJavaVM(), (char*)polylineUID, pdblReal, 3);
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: 1-by-%d or %d-by-1 vector expected.\n"), fname, 2, 2, 3);
                        return 1;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar or a vector expected.\n"), fname, 2);
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
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Polyline");
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return 1;
    }

    iErr = createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, llHandle);
    if (iErr)
    {
        Scierror(999, _("%s: Can not create output argument #%d.\n"), fname, 1);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

