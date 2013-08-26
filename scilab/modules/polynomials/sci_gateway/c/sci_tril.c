/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_polynomials.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intptriu)(int *job, int *id);
/*--------------------------------------------------------------------------*/
int sci_ptril(char *fname, unsigned long fname_len)
{
    static int id[6];
    int job = 0;
    C2F(intptriu)(&job, id);
    return 0;
}
/*--------------------------------------------------------------------------*/
