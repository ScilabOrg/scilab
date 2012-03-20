/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA 
* Copyright (C) DIGITEO - 2012 - Allan CORNET
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
#include "elementary_functions.h"
/*--------------------------------------------------------------------------*/
/**
dset assigns a scalar to all elements of a vector
@param[in]n: number of elements of vector dy
@param[in]dx: scalar value to assign
@param[out]dy: vector
@param[in]incy: increment value between two consecutive elements of the vector y in the table dy
@return 0
*/
/*--------------------------------------------------------------------------*/
int C2F(dset)(int *n, double *dx, double *dy, int *incy)
{
    int iy = 0;
    int i = 0;

    if (*n <= 0) 
    {
	    return 0;
    }

    if (*incy < 0) 
    {
	    iy = (-(*n) + 1) * *incy;
    }

    for (i = 0; i < *n; ++i) 
    {
        dy[iy] = *dx;
        iy += *incy;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
