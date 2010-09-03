/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "noparallel.h"
#include "concurrency.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int gw_parallel(void)
{
	Scierror(999,_("Scilab '%s' module not installed.\n"), "parallel");
	return 0;
}

int concurrency(void)
{
    return 0;
}
int forbidden(char const* fname)
{ /* Should not be called. */
    Scierror(999,_("%s: This function is forbidden is a concurrent execution context.\n"), fname);
    return 0;
}
/*--------------------------------------------------------------------------*/
