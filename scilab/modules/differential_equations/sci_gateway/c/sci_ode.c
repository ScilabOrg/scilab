/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_differential_equations1.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
extern int C2F(sciode)();
/*--------------------------------------------------------------------------*/
int sci_ode(char *fname, unsigned long fname_len)
{
    C2F(sciode)();
    return 0;
}
/*--------------------------------------------------------------------------*/
