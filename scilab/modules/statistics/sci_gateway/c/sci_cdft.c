/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "gw_statistics.h"
#include "stack-c.h"
#include "CdfBase.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cdft)(int *, double *, double *, double *, double *, int *, double *);
/*--------------------------------------------------------------------------*/
static void cdftErr(int status, double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdft
*  T distribution
*/
/*--------------------------------------------------------------------------*/
int cdftI(char* fname, unsigned long l)
{
    int m1, n1, l1, mDf, nDf, lDf, i;
    double *Df;
    Nbvars = 0;
    CheckRhs(3, 4);
    CheckLhs(1, 2);
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1), "PQ") == 0)
    {
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using decimal numbers for argument #%d may lead to incorrect results.\n"), fname, 3);
            }
        static int callpos[4] = {2, 3, 0, 1};
        CdfBase(fname, 2, 2, callpos, "PQ", _("T and Df"), 1, C2F(cdft),
                cdftErr);
    }
    else if ( strcmp(cstk(l1), "T") == 0)
    {
        GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using decimal numbers for argument #%d may lead to incorrect results.\n"), fname, 2);
            }
        static int callpos[4] = {1, 2, 3, 0};
        CdfBase(fname, 3, 1, callpos, "T", _("Df, P and Q"), 2, C2F(cdft),
                cdftErr);
    }
    else if ( strcmp(cstk(l1), "Df") == 0)
    {
        static int callpos[4] = {0, 1, 2, 3};
        CdfBase(fname, 3, 1, callpos, "Df", _("P,Q and T"), 3, C2F(cdft),
                cdftErr);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 1, "PQ", "T", "Df");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdftErr(int status, double bound)
{
    static char *param[7] = {"X", "P", "Q", "T", "Df"};
    switch ( status )
    {
        case 1 :
        {
            cdfLowestSearchError(bound);
        }
        break;
        case 2 :
        {
            cdfGreatestSearchError(bound);
        }
        break;
        case 3 :
        {
            Scierror(999, " P + Q .ne. 1 \n");
        }
        break;
        default :
        {
            CdfDefaultError(param, status, bound);
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
