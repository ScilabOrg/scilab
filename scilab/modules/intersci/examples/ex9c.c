/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "../../routines/machine.h"
#include <stdlib.h>

/*************************************************************
  Example of an int array created by C code
  converted to output Scilab variable by cintf
*************************************************************/

C2F(ccalc3) (int **a, int *m, int *n)
{
    int i ;
    *n = 5 ;
    *m = 3 ;
    *a = ( int *) malloc( (unsigned) (*m) * (*n) * sizeof(int));
    for ( i = 0 ; i < (*m) * (*n) ; i++)
    {
        (*a)[i] = i ;
    }
}


