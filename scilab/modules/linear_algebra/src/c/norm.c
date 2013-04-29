/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include "MALLOC.h"
#include "machine.h"

extern double C2F(dlange) (char *Norm, int *m, int *n, double *A, int *lda, double *work);
//extern int C2F(dlassq) (double *Norm, int *m, int *n, double *A, int *LDA, double *work);
extern int C2F(dnrm2)  (int *n, double *A, int *incx);
extern int C2F(dset)   (int *n, double *dx, double *dy, int *incy);
extern int C2F(dcopy)  (int *, double *, int *, double *, int *);

double normString (double *A, int iRows, int iCols, char *flag)
{
    double ret = 0;
    double *work = NULL;

    if (strcmp(flag, "inf") == 0)
    {
        work = calloc (iRows, sizeof(double));

        ret = C2F(dlange)("i", &iRows, &iCols, A, &iCols, work);

        free(work);
        return ret;
    }

    if (strcmp(flag, "fro") == 0)
    {
        ret = C2F(dlange)("f", &iRows, &iCols, A, &iRows, NULL);

        return ret;
    }

    return 0;
}

double normP (double *A, int iRows, int iCols, int p)
{
    int i;
    double ret = 0;

    switch (p)
    {
        case 1:
        {
            ret = C2F(dlange)("1", &iRows, &iCols, A, &iRows, NULL);

            return ret;
        }
        case 2:
        {
            //C2F(dnrm2)();
        }
    }

    return ret;
}
