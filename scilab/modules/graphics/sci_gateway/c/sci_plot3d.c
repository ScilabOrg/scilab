/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU (nicer default plot3d)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_plot3d.c                                                     */
/* desc : interface for plot3d (and plot3d1) routine                      */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "GetCommandArg.h"
#include "MALLOC.h"
#include "sciCall.h"
#include "localization.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_plot3d(char * fname, void *pvApiCtx)
{
    SciErr sciErr;
    static double  ebox_def [6] = { 0, 1, 0, 1, 0, 1};
    double *ebox = ebox_def;
    static int iflag_def[3] = {2, 2, 4};
    int *iflag = iflag_def;
    double  alpha_def = 35.0 , theta_def = 45.0;
    double *alpha = &alpha_def, *theta = &theta_def;
    int m1 = 0, n1 = 0,  m2 = 0, n2 = 0, m3 = 0, n3 = 0;
    int m3n = 0, n3n = 0, m3l = 0;

    int izcol = 0,  isfac = 0;
    double *zcol = NULL;

    static rhs_opts opts[] =
    {
        { -1, "alpha", -1, 0, 0, NULL},
        { -1, "ebox", -1, 0, 0, NULL},
        { -1, "flag", -1, 0, 0, NULL},
        { -1, "leg", -1, 0, 0, NULL},
        { -1, "theta", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    char * legend = NULL;

    int* piAddr1  = NULL;
    int* piAddr2  = NULL;
    int* piAddr3  = NULL;
    int* piAddr31 = NULL;
    int* piAddr32 = NULL;

    double* l1  = NULL;
    double* l2  = NULL;
    double* l3  = NULL;
    double* l3n = NULL;

    /*
    ** This overload the function to call demo script
    ** the demo script is called %_<fname>
    */
    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, pvApiCtx);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 3, 8);

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (FirstOpt() < 4)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"), fname, 1, 4);
        return -1;
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    if (m1 * n1 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) >= 3)
    {
        /*     third argument can be a matrix z or a list list(z,zcol) */
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        switch (getInputArgumentType(pvApiCtx, 3))
        {
            case sci_matrix :
                //get variable address
                sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 3.
                sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &m3, &n3, &l3);
                if (sciErr.iErr)
                {
                    Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
                    printError(&sciErr, 0);
                    return 1;
                }

                izcol = 0;
                break;
            case sci_list :
                izcol = 1;
                /* z = list(z,colors) */
                sciErr = getListItemNumber(pvApiCtx, piAddr3, &m3l);
                if (sciErr.iErr)
                {
                    Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
                    printError(&sciErr, 0);
                    return 1;
                }

                if (m3l != 2)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: List of size %d expected.\n"),
                             fname, 2, m3l, 2);
                    return 1;
                }

                sciErr = getListItemAddress(pvApiCtx, piAddr3, 1, &piAddr31);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                sciErr = getMatrixOfDouble(pvApiCtx, piAddr31, &m3, &n3, &l3); /* z */
                if (sciErr.iErr)
                {
                    Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
                    printError(&sciErr, 0);
                    return 1;
                }

                sciErr = getListItemAddress(pvApiCtx, piAddr3, 2, &piAddr32);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                sciErr = getMatrixOfDouble(pvApiCtx, piAddr32, &m3n, &n3n, &l3n); /* z */
                if (sciErr.iErr)
                {
                    Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
                    printError(&sciErr, 0);
                    return 1;
                }

                zcol  = (l3n);
                if (m3n * n3n != n3 &&  m3n * n3n != m3 * n3)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: %d or %d expected.\n"), fname, 3, n3, m3 * n3);
                    return 1;
                }
                /*
                *   Added by E Segre 4/5/2000. In the case where zcol is a
                *   matrix of the same size as z, we set izcol to 2. This
                *   value is later transmitted to the C2F(fac3dg) routine,
                *   which has been modified to do the interpolated shading
                *    (see the file SCI/modules/graphics/src/c/Plo3d.c
                */
                if (m3n * n3n == m3 * n3)
                {
                    izcol = 2 ;
                }
                break;
            default :
                OverLoad(3);
                return 0;
        }
    }
    iflag_def[1] = 8;

    GetOptionalDoubleArg(pvApiCtx, fname, 4, "theta", &theta, 1, opts);
    GetOptionalDoubleArg(pvApiCtx, fname, 5, "alpha", &alpha, 1, opts);
    GetLabels(pvApiCtx, fname, 6, opts, &legend);
    GetOptionalIntArg(pvApiCtx, fname, 7, "flag", &iflag, 3, opts);
    GetOptionalDoubleArg(pvApiCtx, fname, 8, "ebox", &ebox, 6, opts);

    if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1)
    {
        if (! (m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3))
        {
            Scierror(999, _("%s: Wrong value for input arguments #%d, #%d and #%d: Incompatible length.\n"), fname, 1, 2, 3);
            return 1;
        }
    }
    else
    {
        if (m2 * n2 != n3)
        {
            Scierror(999, _("%s: Wrong value for input arguments #%d and #%d: Incompatible length.\n"), fname, 2, 3);
            return 1;
        }

        if (m1 * n1 != m3)
        {
            Scierror(999, _("%s: Wrong value for input arguments #%d and #%d: Incompatible length.\n"), fname, 1, 3);
            return 1;
        }

        if (m1 * n1 <= 1 || m2 * n2 <= 1)
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: %s expected.\n"), fname, 2, 3, ">= 2");
            return 1;
        }
    }

    if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    getOrCreateDefaultSubwin();

    /******************** 24/05/2002 ********************/
    if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
    {
        isfac = 1;
    }
    else
    {
        isfac = 0;
    }


    Objplot3d (fname, &isfac, &izcol, (l1), (l2), (l3), zcol, &m3, &n3, theta, alpha, legend, iflag, ebox, &m1, &n1, &m2, &n2, &m3, &n3, &m3n, &n3n); /*Adding F.Leray 12.03.04 and 19.03.04*/

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;

}
/*--------------------------------------------------------------------------*/
