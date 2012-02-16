/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/*--------------------------------------------------------
 * An example of an hand written interface
 * passing a Scilab function as input of function ex8c
 *
 * call in Scilab:-->ex8c(x1,x2,a_function)
 *     x1<->1    (double array)
 *     x2<->2    (double array)
 *     a_function <-> 3    (a scilab function).
 *     a_function is the function "myfunction" defined
 *     in ex8c.sce. It has mlhs=2 inputs and mrhs=3 outputs.
 *
 *--------------------------------------------------------*/
/* ************ This code is deprecated. See api_scilab ************* */

int intex8c(char *fname)
{
    int ibegin;
    static int l1, m1, n1, m2, n2, l2, mlhs, mrhs, lf, l4, l5, i;
    static int minlhs = 1, minrhs = 3, maxlhs = 3, maxrhs = 3;

    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

#pragma message("Deprecated code. See help('api_scilab')")

    /* Getting x1 and x2 at l1 and l2 . x1 is m1 x n1 and x2 is m2 x n2 */
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    /* Getting the Scilab function:
       It is the 3rd input parameter of ex8c and it is a function ("f") */
    GetRhsVar(3, EXTERNAL_DATATYPE, &mlhs, &mrhs, &lf);
    /*   It is at lf and has mlhs outputs and mrhs inputs */

    if ( mrhs != 2 )
    {
        Scierror(999, "%s: Third argument has an invalid rhs\n", fname);
        return 0;
    }

    /*
     *  To call a_function it is required that its input arguments are
     *  stored in the last positions of the variables stack. NOTE that when
     *  called, the function destroys its input variables and replaces them by
     *  the output variables. so in this  case we need to make a copy of
     *  them.
     *  Remark: if the calling sequence of ex8c had been ex8c(a_function,x1,x2)
     *  the following two copies would be un-necessary.
     */

    CreateVar(3 + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l4);
    for (i = 0 ; i < m1 * n1 ; i++) *stk(l4 + i) = *stk(l1 + i);

    CreateVar(3 + mrhs, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l5);
    for (i = 0 ; i < m1 * n1 ; i++) *stk(l5 + i) = *stk(l2 + i);

    /*
     *  Here a_function  takes  variables 4 and 5 as inputs and generates output
     *  variables at positions 4 to 4-1+mlhs
     *  ibegin must be the index of the first input variable of a_function
     */

    ibegin = 3 + 1;

    /* execute the function */

    SciFunction(&ibegin, &lf, &mlhs, &mrhs);
    /* check if an error has occurred while running a_function */
    if (Err > 0 ) return 0;

    /* output variables: 4 and 5 (created by a_function) and possibly 6
     * if a_function has 3 output parameters
     */

    LhsVar(1) = 4;
    LhsVar(2) = 5;
    if ( mlhs == 3) LhsVar(3) = 6;
    return 0;
}




