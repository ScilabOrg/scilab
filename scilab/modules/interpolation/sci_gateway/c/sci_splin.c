/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/
#include <string.h>
#include "gw_interpolation.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(derivd) (double *x, double *y, double *d, int *n, int *inc, int *type);
extern int C2F(dpchim) (int *n, double *x, double *y, double *d, int *inc);
extern int C2F(splinecub) (double *x, double *y, double *d, int *n, int *type,
                           double *wk1, double *wk2, double *wk3, double *wk4);
/*--------------------------------------------------------------------------*/
#define NB_SPLINE_TYPE 7
static TableType SplineTable[NB_SPLINE_TYPE] =
{
    { "not_a_knot"   , NOT_A_KNOT    },
    { "natural"      , NATURAL       },
    { "clamped"      , CLAMPED       },
    { "periodic"     , PERIODIC      },
    { "monotone"     , MONOTONE      },
    { "fast"         , FAST          },
    { "fast_periodic", FAST_PERIODIC }
};
/*--------------------------------------------------------------------------*/
int intsplin(char *fname, unsigned long fname_len)
{
    int minrhs = 2, maxrhs = 4, minlhs = 1, maxlhs = 1;

    int mx = 0, nx = 0, lx = 0, my = 0, ny = 0, ly = 0, mc = 0, nc = 0, lc = 0, n = 0, spline_type = 0;
    int *str_spline_type = NULL, ns = 0;
    int ld = 0, i = 0;
    int mwk1 = 0, nwk1 = 0, lwk1 = 0, mwk2 = 0, nwk2 = 0, lwk2 = 0, mwk3 = 0;
    int nwk3 = 0, lwk3 = 0, mwk4 = 0, nwk4 = 0, lwk4 = 0;
    double *x = NULL, *y = NULL, *d = NULL, *c = NULL;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);

    for (i = 1; i <= minrhs; i++)
    {
        SciErr sciErr;
        int *piAddressVar = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddressVar);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVar))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), fname, i);
            return 0;
        }
    }

    if ( mx != my  ||  nx != ny  ||  (mx != 1  &&  nx != 1) )
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Vector of same size expected.\n"), fname, 1, 2);
        return 0;
    }

    n = mx * nx;    /* number of interpolation points */
    if ( n < 2 )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Must be %s.\n"), fname, 1, ">= 2");
        return 0;
    }

    x = stk(lx);
    y = stk(ly);
    if (! good_order(x, n))  /* verify strict increasing abscissae */
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), fname, 1);
        return 0;
    }

    if ( Rhs >= 3 )   /* get the spline type */
    {
        GetRhsScalarString(3, &ns, &str_spline_type);
        spline_type =  get_type(SplineTable, NB_SPLINE_TYPE, str_spline_type, ns);
        if ( spline_type == UNDEFINED )
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Unknown '%s' type.\n"), fname, 3, "spline");
            return 0;
        };
    }
    else
    {
        spline_type = NOT_A_KNOT;
    }

    if ( spline_type == CLAMPED ) /* get arg 4 which contains the end point slopes */
    {
        if ( Rhs != 4 )
        {
            Scierror(999, _("%s: For a clamped spline, you must give the endpoint slopes.\n"), fname);
            return 0;
        }
        GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &mc, &nc, &lc);
        if ( mc*nc != 2 )
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Endpoint slopes.\n"), fname, 4);
            return 0;
        }
        c = stk(lc);
    }
    else if ( Rhs == 4 )
    {
        Scierror(999, _("%s: Wrong number of input argument(s).\n"), fname);
        return 0;
    }

    /*  verify y(1) = y(n) for periodic splines */
    if ( (spline_type == PERIODIC || spline_type == FAST_PERIODIC)  &&  y[0] != y[n - 1] )
    {
        Scierror(999, _("%s: Wrong value for periodic spline %s: Must be equal to %s.\n"), fname, "y(1)", "y(n)");
        return(0);
    };

    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &mx,  &nx,   &ld); /* memory for d (only argument returned) */
    d = stk(ld);

    switch (spline_type)
    {
        case(FAST) :
        case(FAST_PERIODIC) :
            nwk1 = 1;
            C2F(derivd) (x, y, d, &n, &nwk1, &spline_type);
            break;

        case(MONOTONE) :
            nwk1 = 1;
            C2F(dpchim) (&n, x, y, d, &nwk1);
            break;

        case(NOT_A_KNOT) :
        case(NATURAL) :
        case(CLAMPED) :
        case(PERIODIC) :
            /*  (the wk4 work array is used only in the periodic case) */
            mwk1 = n;
            nwk1 = 1;
            mwk2 = n - 1;
            nwk2 = 1;
            mwk3 = n - 1;
            nwk3 = 1;
            mwk4 = n - 1;
            nwk4 = 1;
            CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &mwk1,  &nwk1,   &lwk1);
            CreateVar(Rhs + 3, MATRIX_OF_DOUBLE_DATATYPE, &mwk2,  &nwk2,   &lwk2);
            CreateVar(Rhs + 4, MATRIX_OF_DOUBLE_DATATYPE, &mwk3,  &nwk3,   &lwk3);
            lwk4 = lwk1;
            if (spline_type == CLAMPED)
            {
                d[0] = c[0];
                d[n - 1] = c[1];
            };
            if (spline_type == PERIODIC)
            {
                CreateVar(Rhs + 5, MATRIX_OF_DOUBLE_DATATYPE, &mwk4,  &nwk4,   &lwk4);
            }
            C2F(splinecub) (x, y, d, &n, &spline_type, stk(lwk1), stk(lwk2), stk(lwk3), stk(lwk4));
            break;
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/