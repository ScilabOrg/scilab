/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipFunctions.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include "MALLOC.h"
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

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);
    int nbRow = 0, nbCol = 0;
    char* pFigureUID = NULL;
    char* datatipUID = NULL;
    char* polylineUID = NULL;
    int stkAdr = 0;
    int* piAddr	= NULL;
    double* pdblReal = NULL;
    int indexPos = 0;

    if (Rhs == 1) {

        pFigureUID = (char*)getCurrentFigure();

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }

        datatipUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

        DatatipDelete::datatipRemoveProgramHandler(getScilabJavaVM(), (char*)datatipUID, (char*)pFigureUID);

    } else if (Rhs == 2) {

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }

        polylineUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (nbRow * nbCol == 1) {

            indexPos = (int) pdblReal[0];

            DatatipDelete::datatipRemoveProgramIndex(getScilabJavaVM(), (char*)polylineUID, indexPos);

        } else {

            Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), fname, 2);
            return FALSE;

        }

    } else {

        Scierror(999, _("%s: Wrong number for input argument: %d or %d expected.\n"), fname, 1, 2);
        return FALSE;
    }

}
