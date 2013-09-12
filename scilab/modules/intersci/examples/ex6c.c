/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
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
 * Example of a dynamically allocated array of character created here.
 * Its size is sent back as an output.
 * It is converted into Scilab variable in the interface program.
 * The allocated array is freed.
 * the Scilab variable is of type stringmat and is not in the argument list of the scilab
 * function. The  Fortran type is Cstringv
 *************************************************************/

C2F(ccalc6b)(a, m, n, err)
char ***a;
int *m, *n, *err;
{
    int i, nstring;
    *m = 3;
    *n = 2;
    nstring = (*m) * (*n);
    *a = (char **) malloc((unsigned) (nstring * sizeof(char *)));
    if ( *a == 0)
    {
        sciprint("No more space\n");
        *err = 1;
        return;
    }
    for ( i = 0  ; i < nstring ; i++)
    {
        (*a)[i] = (char *) malloc ((8) * sizeof(char));
        sprintf((*a)[i], "char %d", i);
        *err = 0;
    }
}


/*************************************************************
 * This example shows how a Scilab argument of type stringmat
 * (which is in the calling list of the Scilab function )
 * is transmited to a C program.
 * a is a stringmat Scilab argument and a Cstringv Fortran argument
 * a is allocated in the interface
 *************************************************************/

C2F(ccalc6a)(a, m, n)
char ***a;
int *m, *n;
{
    int i, j;
    for ( i = 0 ; i < *m * (*n) ; i++)
    {
        char *loc = (*a)[i];
        for ( j = 0 ; j < strlen(loc); j++)
            if ( loc[j] == 'a' )
            {
                loc[j] = 'o';
            }
    }
}









