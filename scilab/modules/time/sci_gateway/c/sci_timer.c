/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_time.h"
#include "timer.h"
#include "localization.h"
#include "Scierror.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_timer(char *fname, unsigned long fname_len)
{
    double timerval = 0;

    Rhs = Max(0, Rhs);
    CheckLhs(0, 1);
    CheckRhs(0, 0);

    timerval = scilab_timer();

    if (timerval >= 0.)
    {
        int l1 = 0, n1 = 1;

        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &n1, &n1, &l1);
        *stk(l1) = (double)timerval;

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: An error occurred.\n"), fname);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
