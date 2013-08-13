/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */


#include "stack-c.h"

/****************************************
 *     exemple with a call to ReadMatrix
 *     -->param=[0.04,10000,3d+7];
 *     -->link('ext11c.o','ext11c','C')
 *     -->y=ode([1;0;0],0,[0.4,4],'ext11c')
 *     param must be defined as a scilab variable
 *********************************/

int ext11c(int *neq, double *t, double *y, double *ydot)
{
    static int m, n;
    static double param[3];
    ReadMatrix("param", &m, &n, param);
    ydot[0] = -param[0] * y[0] + param[1] * y[1] * y[2];
    ydot[2] = param[2] * y[1] * y[1];
    ydot[1] = -ydot[0] - ydot[2];
    return(0);
}
