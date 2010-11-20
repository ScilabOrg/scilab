/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * Copyright (C) 2002 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* PVM functions interfaces */
/*--------------------------------------------------------------------------*/
#include "sci_pvm.h"
#include "gw_pvm.h"
/*--------------------------------------------------------------------------*/
/******************************************
 * SCILAB function : pvm_getinst, fin = 5
 ******************************************/
int sci_pvm_getinst(char *fname, unsigned long fname_len)
{
    int m1, n1, l1, m2, n2, l2, un = 1, l3;

    CheckRhs(2, 2);
    CheckLhs(1, 1);
    /*  checking variable group */
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    /*  checking variable tid */
    GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
    CheckScalar(2, m2, n2);
    /* cross variable size checking */
    CreateVar(Rhs + 3, MATRIX_OF_INTEGER_DATATYPE, &un, &un, &l3);  /* named: inum */
    *istk(l3) = pvm_getinst(cstk(l1), *istk(l2));
    LhsVar(1) = Rhs + 3;
    C2F(putlhsvar) ();
    return 0;
}

/*--------------------------------------------------------------------------*/
