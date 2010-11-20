/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_special_functions.h"
#include "stack-c.h"
#include "Scierror.h"
#include "msgs.h"
/*--------------------------------------------------------------------------*/
extern void C2F(dset) (int *n, double *a, double *x, int *ix);
extern void C2F(dbesiv) (double *x, int *nx, double *alpha, int *na, int *kode, double *r, double *w, int *ierr);
extern void C2F(zbesiv) (double *xr, double *xi, int *nx, double *alpha, int *na, int *kode, double *rr, double *ri, double *wr, double *wi,
                         int *ierr);
/*--------------------------------------------------------------------------*/
int sci_besseli(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, it2 = 0, l2 = 0, l2r = 0, l2i = 0;
    int mr = 0, nr = 0, itr = 0, lr = 0, li = 0, lwi = 0, lwr = 0;
    int r1 = 0, r2 = 0, na = 0, nx = 0, kode = 0, lpos = 0;
    int isint = 0, ispos = 0, i = 0, t = 0;
    int un = 1, nl2 = 0, ierr = 0;
    double zero = 0.0;

    int *Lstk = C2F(vstk).lstk - 1;

    CheckRhs(2, 3);

    kode = 1;
    if (Rhs == 3)
    {
        /* normalized bessel required */
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
        CheckScalar(1, m1, n1);
        kode = (int)*stk(l1) + 1;
    }

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1); /* get alpha */
    r1 = l1 > Lstk[Bot];        /* true if the variable has been passed by reference */

    GetRhsCVar(2, MATRIX_OF_DOUBLE_DATATYPE, &it2, &m2, &n2, &l2, &l2i);    /* get x */
    r2 = l2 > Lstk[Bot];        /* true if the variable has been passed by reference */

    if (m1 * n1 == 0)
    {
        /*besseli([],x) */
        LhsVar(1) = 1;
        C2F(putlhsvar) ();
        return 0;
    }
    if (m2 * n2 == 0)
    {
        /*besseli(alpha,[]) */
        LhsVar(1) = 2;
        C2F(putlhsvar) ();
        return 0;
    }
    /* determine if the result is real or complex */
    itr = it2;
    if (itr == 0)
    {
        isint = 1;
        ispos = 1;
        for (i = 0; i < m1 * n1; i++)
        {
            t = (int)*stk(l1 + i);
            if (t != *stk(l1 + i))
            {
                isint = 0;
                break;
            }
        }
        if (isint == 0)
        {
            for (i = 0; i < m2 * n2; i++)
            {
                if (*stk(l2 + i) < 0.0)
                {
                    ispos = 0;
                    break;
                }
            }
        }
        if (ispos == 0)
            itr = 1;
    }

    if (itr == 1 && it2 == 0)
    {
        /* transform to complex */
        nl2 = m2 * n2;
        CreateCVar(3, MATRIX_OF_DOUBLE_DATATYPE, &itr, &m2, &n2, &l2r, &l2i);
        C2F(dcopy) (&nl2, stk(l2), &un, stk(l2r), &un);
        l2 = l2r;
        C2F(dset) (&nl2, &zero, stk(l2i), &un);
        lpos = 3;
        r2 = 0;
    }
    else
    {
        l2i = l2 + m2 * n2;
        lpos = 2;
    }

    if (m1 * n1 == 1)
    {
        /*besseli(scalar,matrix) */
        double wr[2], wi[2];

        mr = m2;
        nr = n2;
        if (r2)
        {
            CreateCVar(lpos + 1, MATRIX_OF_DOUBLE_DATATYPE, &itr, &mr, &nr, &lr, &li);
            LhsVar(1) = lpos + 1;
        }
        else
        {
            lr = l2;
            li = l2i;
            LhsVar(1) = lpos;
        }
        nx = m2 * n2;
        na = 1;
        if (itr == 0)
        {
            C2F(dbesiv) (stk(l2), &nx, stk(l1), &na, &kode, stk(lr), wr, &ierr);
        }
        else
        {
            C2F(zbesiv) (stk(l2), stk(l2i), &nx, stk(l1), &na, &kode, stk(lr), stk(li), wr, wi, &ierr);
        }
    }
    else if (m2 * n2 == 1)
    {
        /* besseli(matrix,scalar) */
        int llwr = 0, llwi = 0, nw = 0;

        mr = m1;
        nr = n1;
        CreateCVar(lpos + 1, MATRIX_OF_DOUBLE_DATATYPE, &itr, &mr, &nr, &lr, &li);
        nx = 1;
        na = m1 * n1;
        nw = 2 * na;
        CreateCVar(lpos + 2, MATRIX_OF_DOUBLE_DATATYPE, &itr, &nx, &nw, &llwr, &llwi);
        if (itr == 0)
        {
            C2F(dbesiv) (stk(l2), &nx, stk(l1), &na, &kode, stk(lr), stk(llwr), &ierr);
        }
        else
        {
            C2F(zbesiv) (stk(l2), stk(l2i), &nx, stk(l1), &na, &kode, stk(lr), stk(li), stk(llwr), stk(llwi), &ierr);
        }

        LhsVar(1) = lpos + 1;
    }
    else if ((m1 == 1 && n2 == 1) || (n1 == 1 && m2 == 1))
    {
        /* besseli(row,col) or besseli(col,row) */
        int lun = 1, nw = 0;

        mr = m2 * n2;
        nr = m1 * n1;
        CreateCVar(lpos + 1, MATRIX_OF_DOUBLE_DATATYPE, &itr, &mr, &nr, &lr, &li);
        nx = m2 * n2;
        na = m1 * n1;
        nw = 2 * na;
        CreateCVar(lpos + 2, MATRIX_OF_DOUBLE_DATATYPE, &itr, &lun, &nw, &lwr, &lwi);
        if (itr == 0)
        {
            C2F(dbesiv) (stk(l2), &nx, stk(l1), &na, &kode, stk(lr), stk(lwr), &ierr);
        }
        else
        {
            C2F(zbesiv) (stk(l2), stk(l2i), &nx, stk(l1), &na, &kode, stk(lr), stk(li), stk(lwr), stk(lwi), &ierr);
        }

        LhsVar(1) = lpos + 1;
    }
    else
    {
        /* element wise case */
        double wr[2], wi[2];

        CheckDimProp(1, 2, m1 * n1 != m2 * n2) mr = m2;
        nr = n2;
        if (r2)
        {
            CreateCVar(lpos + 1, MATRIX_OF_DOUBLE_DATATYPE, &itr, &mr, &nr, &lr, &li);
            LhsVar(1) = lpos + 1;
        }
        else
        {
            lr = l2;
            li = l2i;
            LhsVar(1) = lpos;
        }
        nx = mr * nr;
        na = -1;
        if (itr == 0)
        {
            C2F(dbesiv) (stk(l2), &nx, stk(l1), &na, &kode, stk(lr), wr, &ierr);
        }
        else
        {
            C2F(zbesiv) (stk(l2), stk(l2i), &nx, stk(l1), &na, &kode, stk(lr), stk(li), wr, wi, &ierr);
        }
    }
    if (ierr == 2)
    {
        if (C2F(errgst).ieee == 0)
        {
            ierr = 69;
            SciError(ierr);
        }
        else if (C2F(errgst).ieee == 1)
        {
            ierr = 63;
            C2F(msgs) (&ierr, &un);

        }
    }
    else if (ierr == 3)
    {
        /* inacurate result */
        ierr = 4;
        C2F(msgs) (&ierr, &un);
    }
    else if (ierr == 4 || ierr == 5)
    {
        if (C2F(errgst).ieee == 0)
        {
            ierr = 69;
            SciError(ierr);
        }
        else if (C2F(errgst).ieee == 1)
        {
            ierr = 107;
            C2F(msgs) (&ierr, &un);
        }
    }

    C2F(putlhsvar) ();
    return 0;
}

/*--------------------------------------------------------------------------*/
