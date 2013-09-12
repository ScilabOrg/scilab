/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "gw_data_structures2.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intgetfield)(); /* fortran */
/*--------------------------------------------------------------------------*/
int sci_getfield(char *fname, unsigned long fname_len)
{
    C2F(intgetfield)();
    return 0;
}
/*--------------------------------------------------------------------------*/
