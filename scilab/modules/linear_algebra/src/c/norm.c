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

#include "core_math.h"    // fabs, pow, Min and Max
#include "machine.h"      // C2F
#include "localization.h" // Translations

// Lapack routines, for complex and real input
extern double C2F(clange) (char *norm, int *m, int *n, double *A, int *lda, double *work);
extern int     C2F(cgesdd) (char *job, int *m, int *n, double *A, int *lda,
                            double *s, double *u, int *ldu, double *vt, int *ldvt,
                            double *work, int *lwork, int *iwork, int *info);
extern double C2F(dlange) (char *norm, int *m, int *n, double *A, int *lda, double *work);
extern int     C2F(dgesdd) (char *job, int *m, int *n, double *A, int *lda,
                            double *s, double *u, int *ldu, double *vt, int *ldvt,
                            double *work, int *lwork, int *iwork, int *info);

double normString (double *A, int iRows, int iCols, char *flag)
{
    double ret = 0;
    double *work = NULL;

    if (strcmp(flag, "inf") == 0 || strcmp(flag, "i") == 0)
    {
        work = (double *) calloc(Max(1, iRows), sizeof(double));

        // Call Lapack routine for computation of the infinite norm.
        ret = C2F(dlange)("I", &iRows, &iCols, A, &iRows, work);

        free(work);
        return ret;
    }

    if (strcmp(flag, "fro") == 0 || strncmp(flag, "f") == 0)
    {
        // Call Lapack routine for computation of the Frobenius norm.
        ret = C2F(dlange)("F", &iRows, &iCols, A, &iRows, NULL);

        return ret;
    }

    return 0;
}

double normP (double *A, int iRows, int iCols, double p)
{
    double ret = 0;
    double *S, *work;
    int *iwork;
    int i, maxRC, minRC, lwork, info, one = 1;

    maxRC = Max(iRows, iCols);
    minRC = Min(iRows, iCols);
    lwork = 3 * minRC + Max(maxRC, 7 * minRC);

    if (p != p) // p = %nan is a special case, return 0./0 = %nan.
    {
        ret = 0. / 0;
        return ret;
    }
    if (p == 0) // p = 0 is a special case, return 1./0 = %inf.
    {
        ret = 1. / 0;
        return ret;
    }
    if (p == 1) // Call the Lapack routine for computation of norm 1.
    {
        ret = C2F(dlange)("1", &iRows, &iCols, A, &iRows, NULL);
        return ret;
    }
    if (p == 2) // Call the Lapack routine for computation of norm 2.
    {
        // Allocating workspaces.
        S     = (double *) malloc(minRC * sizeof(double));
        work  = (double *) malloc(Max(1, lwork) * sizeof(double));
        iwork = (int *) malloc(8 * minRC * sizeof(int));

        // Not computing singular vectors, so arguments 7, 8, 9 and 10 are dummies.
        C2F(dgesdd)("N", &iRows, &iCols, A, &iRows, S, NULL, &one, NULL, &one, work, &lwork, iwork, &info);

        if (info < 0)
        {
            // Lapack provides its own error messages. Return.
            free(S);
            free(work);
            free(iwork);
            return 0;
        }
        else
        {
            if (info > 0)
            {
                // Lapack provides its own error messages. Return.
                free(S);
                free(work);
                free(iwork);
                return 0;
            }
            else
            {
                // info = 0: successful termination.
                // The largest singular value of A is stored in the first element of S, return it.
                ret = S[0];
                free(S);
                free(work);
                free(iwork);
                return ret;
            }
        }
    }
    // Here, A is a vector of length iRows, return sum(abs(A(i))^p))^(1/p).
    for (i = 0; i < iRows; ++i)
    {
        ret += pow(fabs(A[i]), p);
    }
    return pow(ret, 1. / p);
}
