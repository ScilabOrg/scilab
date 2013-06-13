/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab-Enterprises - Cedric Delamarre
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
#include "localization.h"
#include "Scierror.h"
#include "xerbla.hxx"
}
/*--------------------------------------------------------------------------*/
extern "C" {
    void C2F(xerbla)(char* pFuncName, int* iNumArg)
    {
        Scierror(998, _("xerbla: On entry to %s parameter number %d had an illegal value (lapack library probem)\n"), pFuncName, *iNumArg);
    }
}
/*--------------------------------------------------------------------------*/
