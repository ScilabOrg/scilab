/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "sciprint.h"

/*************************************************************
 *  Example of array created by C code
 *  converted to Scilab output variable
 *************************************************************/

/* ************ This code is deprecated. See api_scilab ************* */

#pragma message("Deprecated code. See help('api_scilab')")

/*     double array    */
int dblearrayc(double **a, int *m, int *n, int *ierr)
{
    int i ;
    *n = 5 ;
    *m = 3 ;
    *a = ( double *) malloc( (unsigned) (*m) * (*n) * sizeof(double));
    if ( *a != (double *) 0)
    {
        *ierr = 0;
        for ( i = 0 ; i < (*m) * (*n) ; i++) (*a)[i] = i + 1;
    }
    else
        *ierr = 1;
    return(0);
}


/*     int array    */
int intarrayc(int **a, int *m, int *n, int *ierr)
{
    int i ;
    *n = 5 ;
    *m = 3 ;
    *a = ( int *) malloc( (unsigned) (*m) * (*n) * sizeof(int));
    if ( *a != (int *) 0)
    {
        *ierr = 0;
        for ( i = 0 ; i < (*m) * (*n) ; i++) (*a)[i] = i + 1;
    }
    else
        *ierr = 1;
    return(0);
}

/*************************************************************
 * Example of character string created here.
 * Its length is sent back as an output.
 * It is converted into Scilab variable in the interface program.
 * The allocated array is freed in the interface program.
 *************************************************************/

#define MYSTR "Scilab is ..."

int crestrc(char **a, int *m, int *ierr)
{
    *m = strlen(MYSTR);
    *a = (char *) malloc((unsigned) (*m + 1) * sizeof(char));
    if ( *a != (char *) 0)
    {
        *ierr = 0;
        sprintf((*a), MYSTR);
    }
    else
    {
        *ierr = 1;
        sciprint("malloc : No more space\n");
    }
    return(0);
}
