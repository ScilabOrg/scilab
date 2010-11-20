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
 * SCILAB function : pvm_halt 
 ******************************************/
int sci_pvm_halt(char *fname, unsigned long fname_len)
{
    int res, un = 1;

    CheckRhs(0, 0);
    CheckLhs(1, 1);
    CreateVar(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &un, &un, &res);
    C2F(scipvmhalt) (istk(res));
    LhsVar(1) = Rhs + 1;

    pvm_error_check(fname, *istk(res), fname_len);
    C2F(putlhsvar) ();
    return 0;
}

/*--------------------------------------------------------------------------*/
