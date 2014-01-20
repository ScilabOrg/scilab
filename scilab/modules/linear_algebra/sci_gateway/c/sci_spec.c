
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "issymmetric.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int *num);
/*--------------------------------------------------------------------------*/
int C2F(intspec)(char *fname, unsigned long fname_len)
{
    int *header1, *header2;
    int CmplxA, CmplxB;
    int ret;
    int Symmetric;
    int X;

    switch (Rhs)
    {
        case 1:   /* spec(A)   */
            if (GetType(1) != sci_matrix)
            {
                OverLoad(1);
                return 0;
            }
            header1 = (int *) GetData(1);
            CmplxA = header1[3];
            X = 1;
            Symmetric = C2F(issymmetric)(&X);
            switch (CmplxA)
            {
                case REAL:
                    switch (Symmetric)
                    {
                        case NO :
                            ret = sci_dgeev("spec", 4L);
                            break;
                        case YES :
                            ret = sci_dsyev("spec", 4L);
                            break;
                    }
                    break;

                case COMPLEX:
                    switch (Symmetric)
                    {
                        case NO :
                            ret = sci_zgeev("spec", 4L);
                            break;
                        case YES:
                            ret = sci_zheev("spec", 4L);
                            break;
                    }
                    break;

                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                             fname, 1);
                    break;
            } /* end switch  (CmplxA) */
            break; /* end case 1 */

        case 2: /* gspec(A,B) */
            if (GetType(1) != sci_matrix)
            {
                OverLoad(1);
                return 0;
            }
            if (GetType(2) != sci_matrix)
            {
                OverLoad(2);
                return 0;
            }
            header1 = (int *) GetData(1);
            header2 = (int *) GetData(2);
            CmplxA = header1[3];
            CmplxB = header2[3];
            switch (CmplxA)
            {
                case REAL:
                    switch (CmplxB)
                    {
                        case REAL :
                            /* A real, Breal */
                            ret = sci_dggev("gspec", 5L);
                            break;
                        case COMPLEX :
                            /* A real, B complex : complexify A */
                            X = 1;
                            C2F(complexify)(&X);
                            ret = sci_zggev("gspec", 5L);
                            break;
                        default:
                            Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                                     fname, 2);
                            break;
                    }
                    break;
                case COMPLEX :
                    switch (CmplxB)
                    {
                        case REAL :
                            /* A complex, B real : complexify B */
                            X = 2;
                            C2F(complexify)(&X);
                            ret = sci_zggev("gspec", 5L);
                            break;
                        case COMPLEX :
                            /* A complex, B complex */
                            ret = sci_zggev("gspec", 5L);
                            break;
                        default:
                            Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                                     fname, 2);
                            break;
                    }
                    break;
                default :
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                             fname, 1);
                    break;
            } /*end  switch (CmplxA) */
            break;/* end case 2 */

    }/* end switch (Rhs) */
    return 0;
}
/*--------------------------------------------------------------------------*/
