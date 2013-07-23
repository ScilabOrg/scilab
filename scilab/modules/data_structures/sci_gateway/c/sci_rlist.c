/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_data_structures1.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
extern int C2F(rattyp)(); /* fortran */
extern int C2F(mtlist)(); /* fortran */
/*--------------------------------------------------------------------------*/
int sci_rlist(char *fname, void* pvApiCtx)
{
    C2F(mtlist)(C2F(rattyp));
    return 0;
}
/*--------------------------------------------------------------------------*/
