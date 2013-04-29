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
#include "localization.h"

#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)

/*-Lapack routines----------------------------------------------------------*/
extern double C2F(dlange) (char *Norm, int *m, int *n, double *A, int *lda, double *work);
extern int C2F(dgesdd) (char *job, int *m, int *n, double *A, int *lda,
                          double *s, double *u, int *ldu, double *vt, int *ldvt,
                          double *work, int *lwork, int *iwork, int *info);
/*--------------------------------------------------------------------------*/

double normString (double *A, int iRows, int iCols, char *flag)
{
    double ret = 0;
    double *work = NULL;

    if (strcmp(flag, "inf") == 0)
    {
        work = (double *) calloc(iRows, sizeof(double));

        // Call Lapack routine for computation of the infinite norm.
        ret = C2F(dlange)("i", &iRows, &iCols, A, &iRows, work);

        free(work);
        return ret;
    }

    if (strcmp(flag, "fro") == 0)
    {
        // Call Lapack routine for computation of the Frobenius norm.
        ret = C2F(dlange)("f", &iRows, &iCols, A, &iRows, NULL);

        return ret;
    }

    return 0;
}

double normP (double *A, int iRows, int iCols, int p)
{
    double ret = 0;
    double *S, *work;
    int maxRC, minRC, lwork, info, one = 1, *iwork;

    maxRC = max(iRows, iCols);
    minRC = min(iRows, iCols);
    lwork = 3*minRC + max(maxRC, 7*minRC);

    switch (p)
    {
        case 1:
        {
            // Call Lapack routine for computation of norm 1.
            ret = C2F(dlange)("1", &iRows, &iCols, A, &iRows, NULL);

            return ret;
        }
        default: // p = 2.
        {
            // Allocating workspaces.
            S     = (double *) malloc(minRC * sizeof(double));
            work  = (double *) malloc(max(1, lwork) * sizeof(double));
            iwork = (int *) malloc(8*minRC * sizeof(int));

            // Call Lapack routine for computation of norm 2.
            // Not computing singular vectors, so arguments 7, 8, 9 and 10 are dummies. 
            C2F(dgesdd)("N", &iRows, &iCols, A, &iRows, S, NULL, &one, NULL, &one, work, &lwork, iwork, &info);

            if (info < 0)
            {
                // Lapack provides its own error messages. Return.
                free(S);
                free(work);
                return 0;
            }
            else
            {
                if (info > 0)
                {
                    // Lapack provides its own error messages. Return.
                    free(S);
                    free(work);
                    return 0;
                }
                else
                {
                    // info = 0: successful termination.
                    // The largest singular value of A is stored in the first element of S.
                    ret = S[0];

                    free(S);
                    free(work);
                    return ret;
                }
            }
        }
    }
}
