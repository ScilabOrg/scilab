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
#include "BOOL.h"
#include "gw_gui.h"
#include "CurrentFigure.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "CurrentObject.h"
#include "HandleManagement.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipcreate(char *fname, unsigned long fname_len)
{
    int n = 0, numrow = 0, numcol = 0, l1 = 0, i = 0;
    long handlesvalue = 0;
    char *pobjUID = NULL;
    int *piAddr	= NULL;
    int index_pointer;
    int out_index = 0;
    char * datatip_handler = NULL;

    CheckRhs(2, 2);
    CheckLhs(0, 1);

    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &l1);
    n = numrow * numcol;

    for (i = 0; i < n; i++)
    {
        handlesvalue = (unsigned long) (hstk(l1))[n - 1 - i];
        pobjUID = (char*)getObjectFromHandle(handlesvalue);
    }

    GetMatrixdims(2, &numrow, &numcol);

    if (numrow == 1 && numcol == 1) {
      
        GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &numrow, &numcol, &index_pointer);

        int index = *istk(index_pointer);

        datatip_handler = DatatipCreate::createDatatipProgramIndex(getScilabJavaVM(), (char*)pobjUID, index);

    } else if (numrow == 1 && numcol == 2) {

        getVarAddressFromPosition(pvApiCtx, 2, &piAddr);

        double* pdblReal = NULL;

        getMatrixOfDouble(pvApiCtx, piAddr, &numrow, &numcol, &pdblReal);

        datatip_handler = DatatipCreate::createDatatipProgramCoord(getScilabJavaVM(), (char*)pobjUID, pdblReal, 2);
    }

    CreateVar(Rhs + 1, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &out_index);
    hstk(out_index)[0] = getHandle(datatip_handler);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return TRUE;
}

