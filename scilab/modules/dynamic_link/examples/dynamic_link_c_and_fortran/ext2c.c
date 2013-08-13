/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */


/************************************
 *     simple example 2 (using sin and cos)
 *     -->link('ext2c.o','ext2c','C');
 *     -->a=[1,2,3];b=[4,5,6];n=3;
 *     -->c=call('ext2c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
 *     -->c=sin(a)+cos(b)
 ************************************/

#include <math.h>

int ext2c(int *n, double *a, double *b, double *c)
{
    static int k;
    for (k = 0; k < *n; ++k)
    {
        c[k] = sin(a[k]) + cos(b[k]);
    }
    return(0);
}
