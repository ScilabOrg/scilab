
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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
#include "stack-c.h"
#include "sci_rankqr.h"
#include "gw_slicot.h"
/*--------------------------------------------------------------------------*/
int sci_rankqr(char* fname, unsigned long fname_len)
{
    int *header1;
    int Cmplx;
    int ret;

    header1 = (int*)GetData(1);
    Cmplx = header1[3];
    if (Cmplx == 0)
    {
        ret = sci_rrankqr("rankqr", 6L);
        return ret;
    }
    else
    {
        ret = sci_zrankqr("rankqr", 6L);
        return ret;
    }
}
/*--------------------------------------------------------------------------*/
