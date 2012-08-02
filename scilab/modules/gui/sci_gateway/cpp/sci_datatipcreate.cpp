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

#include "DatatipCreate.hxx"
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

int sci_datatipcreate(char *fname, unsigned long fname_len)
{
    char const * figureUid;
    char const * datatip_handle;
    SciErr sciErr;
    int *piAddr			= NULL;
    int iFigureId = 0; // id of the figure to export
    int stkAdr = 0;
    int nbRow = 0;
    int nbCol = 0;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);
    char *pstFigureUID = NULL;
    char *pstHandleType = NULL;
    int *piFigureId;// = &iFigureId;
    size_t stackPointer = 0;

    /* Use this code because api_scilab does not implement handles capabilities */
    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
        return FALSE;
    }


    pstFigureUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));
    printf("pstFigureUID %d \n", pstFigureUID);
    getGraphicObjectProperty(pstFigureUID, __GO_ID__, jni_int, (void **)&piFigureId);
    printf("piFigureUID %d \n", piFigureId);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    double* pdblReal	= NULL;
    /* todo: check type + size */
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &pdblReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    //  try
    //    {

    //      int fig_num = *istk(endereco_fig_num);
    //      int posx = *istk(endereco_posx);
    //      int posy = *istk(endereco_posy);
    figureUid = getFigureFromIndex(0); /* Todo update that */

    if (Rhs == 2)
    {

        datatip_handle = DatatipCreate::createDatatip(getScilabJavaVM(), (char*)figureUid, pdblReal[0], pdblReal[1]);
    }
    //    } catch (


    //  int *datatip_handle_ptr = new int[1];

    //  (*datatip_handle_ptr) = datatip_handle;


    int m = 1, n = 1;

    //  CreateVarFromPtr(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &m, &n, &datatip_handle_ptr);


    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    //delete datatip_handle_ptr;

    return TRUE;

}
