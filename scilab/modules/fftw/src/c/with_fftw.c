/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "with_fftw.h"
/*--------------------------------------------------------------------------*/
int C2F(withfftw)(int *rep)
{
    *rep = 1;
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL withfftw(void)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
