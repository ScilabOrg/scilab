/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "stack-c.h"
#include "gw_fileio.h"
#include "mseek.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_mseek(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0;
    int m2 = 0, n2 = 0, l2 = 0;
    int m3 = 0, n3 = 0, l3 = 0;
    int err = 0;
    int fd = ALL_FILES_DESCRIPTOR;
    char *flag = NULL;

    Nbvars = 0;
    CheckRhs(1, 3);
    CheckLhs(1, 1);

    if (GetType(1) == sci_matrix)
    {
        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
        if (m1*n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 1);
        return 0;
    }

    if ( Rhs >= 2)
    {
        if (GetType(2) == sci_matrix)
        {
            GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
            if (m2*n2 == 1)
            {
                fd = *istk(l2);
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), fname, 2);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 2);
            return 0;
        }
    }

    if ( Rhs >= 3)
    {
        GetRhsVar(3, STRING_DATATYPE, &m3, &n3, &l3);
        flag = cstk(l3);
    }
    else
    {
        flag = "set";
    }

    C2F(mseek)(&fd, stk(l1), flag, &err);

    if (err >  0)
    {
        SciError(10000);
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
