/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipDrag.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "CallJxgetmouse.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipmove(char *fname, unsigned long fname_len)
{
    int mouseButtonNumber = -1;
    int pixelCoords[2];
    int iType = 0;
    int iErr = 0;
    int* piAddr = NULL;
    int *piType = &iType;
    char* datatipUID = NULL;
    long long llHandle = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        iErr = getScalarHandle(pvApiCtx, piAddr, &llHandle);
        if (iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 1;
        }

        datatipUID = (char *)getObjectFromHandle((unsigned long) llHandle);
        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            getGraphicObjectProperty(datatipUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_DATATIP__)
            {
                while (mouseButtonNumber == -1)
                {
                    CallJxgetmouseWithOptions(1, 1);
                    mouseButtonNumber = getJxgetmouseMouseButtonNumber();
                    pixelCoords[0] = (int) getJxgetmouseXCoordinate();
                    pixelCoords[1] = (int) getJxgetmouseYCoordinate();
                    DatatipDrag::dragDatatip(getScilabJavaVM(), (char*)datatipUID, pixelCoords[0], pixelCoords[1]);
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 1);
        return 1;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
