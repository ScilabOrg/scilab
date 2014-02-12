/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_elementary_functions.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfind)();
/*--------------------------------------------------------------------------*/
int C2F(sci_find)(char *fname, unsigned long fname_len)
{
    C2F(intfind)();
    return 0;
}
/*--------------------------------------------------------------------------*/
