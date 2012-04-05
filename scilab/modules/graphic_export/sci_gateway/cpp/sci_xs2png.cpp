/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Koumar Sylvestre
 * desc : interface for xs2png routine
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
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"

#include "gw_graphic_export.h"
}

#include "ScilabView.hxx"
#include "FileExporter.hxx"

/*--------------------------------------------------------------------------*/
int sci_xs2png( char * fname, unsigned long fname_len )
{
    SciErr err;
    int *addr = 0;
    char *path = 0;

    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    org_scilab_modules_graphic_export::FileExporter::fileExport2(getScilabJavaVM(), ScilabView::getCurrentFigure(), path);
    freeAllocatedSingleString(path);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
