/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

int ex1c(char *ch, int *a, int *ia, float *b, int *ib, double *c, int *mc, int *nc, double *d, double *w, int *err);

/**************************************************
 * An example of an hand written interface 
 * the interface program is intex1c, the
 * associated scilab function is ex1c
 **************************************************/

int intex1c(char *fname)
{
    int i1, i2;
    static int ierr;
    static int l1, m1, n1, m2, n2, l2, m3, n3, l3, m4, n4, l4, l5, l6;
    static int minlhs = 1, minrhs = 4, maxlhs = 5, maxrhs = 4;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);
    /*  1-Get the 4 input variables of ex1f:
     *
     * Variable #1 is a chain ("c") , GetRhsVar returns its
     * dimensions: m1 = # of rows = length of the chain, 
     * n1 = # of columns = 1 default value for chains.
     * The chain starts at cstk(l1)  
     */
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);

    /*  a=variable #2 is an int ('i') matrix, getrhsvar returns its
     *   dimensions: m2 = # of rows , n2 = # of columns
     * values in matrix a are istk(l2), istk(l2+1), ... (columnwise) 
     */
    GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);

    /*  b=variable #3 is an real ('r') matrix, getrhsvar returns its
     * dimensions: m3 = # of rows , n3 = # of columns
     * values in matrix a are sstk(l2), sstk(l2+1), ... (columnwise) 
     */

    GetRhsVar(3, MATRIX_OF_RATIONAL_DATATYPE, &m3, &n3, &l3);

    /*  c=variable #4 is an double ('d') matrix, getrhsvar returns its
     * dimensions: m4 = # of rows , n4 = # of columns
     *  values in matrix a are stk(l2), stk(l2+1), ... (columnwise)  
     */

    GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &m4, &n4, &l4);

    /*  2-Create  Scilab variable #5  as double matrix of
     *  dimensions  m4 x n4. The first fourth parameters of
     *  Createvar are input parameters. Createvar returns the adress l5
     *  as output. 
     *  The values of stk(l5), stk(l5+1), ... should be set
     *  to desired values.  
     */

    CreateVar(5, MATRIX_OF_DOUBLE_DATATYPE, &m4, &n4, &l5);
    CreateVar(6, MATRIX_OF_DOUBLE_DATATYPE, &m4, &n4, &l6);

    i1 = n2 * m2;
    i2 = n3 * m3;

    ex1c(cstk(l1), istk(l2), &i1, sstk(l3), &i2, stk(l4), &m4, &n4, stk(l5), stk(l6), &ierr);

    if (ierr > 0)
    {
        Scierror(999, "%s: Internal error\n", fname);
        return 0;
    }

    /*   Variables #5, #4, #3, #2, #1 are returned in that
     *   order. For instance, w=ex1f('chain',a,b,c) puts
     *   in w the variable #5 and [w,r]=ex1f('chain',a,b,c)
     *   puts in w the variable #5 and in r the variable #4  
     */

    LhsVar(1) = 5;
    LhsVar(2) = 4;
    LhsVar(3) = 3;
    LhsVar(4) = 2;
    LhsVar(5) = 1;
    return 0;
}

/**************************************************
 *     inputs:  ch, a,b and c; ia,ib and mc,nc 
 *     ch=character, a=integer, b=float and c=double 
 *     ia,ib and [mc,nc] are the dimensions of a,b and c resp. 
 *     outputs: a,b,c,d 
 *     if ch='mul'   a,b and c = 2 * (a,b and c) 
 *     and d of same dimensions as c with 
 *     d(i,j)=(i+j)*c(i,j) 
 *     if ch='add' a,b and c = 2 + (a,b and c) 
 *     d(i,j)=(i+j)+c(i,j) 
 *     w is a working array of size [mc,nc] 
 *********************************************/

int ex1c(char *ch, int *a, int *ia, float *b, int *ib, double *c, int *mc, int *nc, double *d, double *w, int *err)
{
    static int i, j, k;

    *err = 0;
    if (strcmp(ch, "mul") == 0)
    {
        for (k = 0; k < *ib; ++k)
            a[k] <<= 1;
        for (k = 0; k < *ib; ++k)
            b[k] *= (float)2.;
        for (i = 0; i < *mc; ++i)
            for (j = 0; j < *nc; ++j)
                c[i + j * (*mc)] *= 2.;
        for (i = 0; i < *mc; ++i)
            for (j = 0; j < *nc; ++j)
            {
                w[i + j * (*mc)] = (double)(i + j);
                d[i + j * (*mc)] = w[i + j * (*mc)] * c[i + j * (*mc)];
            }
    }
    else if (strcmp(ch, "add") == 0)
    {
        for (k = 0; k < *ia; ++k)
            a[k] += 2;
        for (k = 0; k < *ib; ++k)
            b[k] += (float)2.;
        for (i = 0; i < *mc; ++i)
            for (j = 0; j < *nc; ++j)
                c[i + j * (*mc)] += 2.;
        for (i = 0; i < *mc; ++i)
            for (j = 0; j < *nc; ++j)
            {
                w[i + j * (*mc)] = (double)(i + j);
                d[i + j * (*mc)] = w[i + j * (*mc)] + c[i + j * (*mc)];
            }
    }
    else
    {
        *err = 1;
    }
    return (0);
}
