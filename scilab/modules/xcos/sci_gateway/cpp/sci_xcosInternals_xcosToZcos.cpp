/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "XcosInternals.hxx"
#include "GiwsException.hxx"

extern "C" {
#include "gw_xcos.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos;


/*--------------------------------------------------------------------------*/
int sci_xcosInternals_xcosToZcos(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 0);
    CheckLhs(0, 1);

    XcosInternals::xcosToZcos(getScilabJavaVM());

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
