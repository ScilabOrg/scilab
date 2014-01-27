/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_plot2d.c                                                     */
/* desc : interface for plot2d routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "api_scilab.h"
#include "BasicAlgos.h"
#include "sciCall.h"
#include "DefaultCommandArg.h"
#include "Scierror.h"
#include "localization.h"
#include <sciprint.h>

#include "BuildObjects.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int sci_plot2d(char* fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    double* lt = NULL;
    int iTypel1 = 0;
    int iTypel2 = 0;
    int lw = 0;

    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int test = 0, i = 0, j = 0, iskip = 0;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;

    /* F.Leray 18.05.04 : log. case test*/
    int size_x = 0, size_y = 0;
    char dataflag = 0;

    char* logFlags = NULL;
    int* style = NULL;
    double* rect = NULL;
    char* strf = NULL;
    char* legend = NULL;
    int* nax = NULL;
    BOOL flagNax = FALSE;
    char strfl[4];
    BOOL freeStrf = FALSE;

    static rhs_opts opts[] =
    {
        { -1, "axesflag", -1, 0, 0, NULL},
        { -1, "frameflag", -1, 0, 0, NULL},
        { -1, "leg", -1, 0, 0, NULL},
        { -1, "logflag", -1, 0, 0, NULL},
        { -1, "nax", -1, 0, 0, NULL},
        { -1, "rect", -1, 0, 0, NULL},
        { -1, "strf", -1, 0, 0, NULL},
        { -1, "style", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    if (nbInputArgument(pvApiCtx) == 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 9);

    iskip = 0;
    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        /* logflags */
        GetLogflags(pvApiCtx, fname, 1, opts, &logFlags);
        iskip = 1;
    }

    if (FirstOpt() == 2 + iskip)                                /** plot2d([loglags,] y, <opt_args>); **/
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1 + iskip, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        sciErr = getVarType(pvApiCtx, piAddrl2, &iTypel2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // the argument can be a matrix of doubles or other
        // If it is not a matrix of doubles, call overload
        if (iTypel2 == sci_matrix)
        {

            // Retrieve a matrix of double at position 1 + iskip.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1 + iskip);
                return 1;
            }
        }
        else
        {
            lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
            C2F(overload)(&lw, "plot2d", 6);
            return 0;
        }

        if (m2 == 1 && n2 > 1)
        {
            m2 = n2;
            n2 = 1;
        }

        m1 = m2;
        n1 = n2;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        for (i = 0; i < m2 ; ++i)
        {
            for (j = 0 ; j < n2 ;  ++j)
            {
                *(l1 + i + m2 * j) = (double) i + 1;
            }
        }
    }
    else if (FirstOpt() >= 3 + iskip)     /** plot2d([loglags,] x, y[, style [,...]]); **/
    {
        /* x */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1 + iskip, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        sciErr = getVarType(pvApiCtx, piAddrl1, &iTypel1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // x can be a matrix of doubles or other
        // If x is not a matrix of doubles, call overload
        if (iTypel1 == sci_matrix)
        {

            // Retrieve a matrix of double at position 1 + iskip.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1 + iskip);
                return 1;
            }
        }
        else
        {
            lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
            C2F(overload)(&lw, "plot2d", 6);
            return 0;
        }

        /* y */
        sciErr = getVarAddressFromPosition(pvApiCtx, 2 + iskip, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        sciErr = getVarType(pvApiCtx, piAddrl2, &iTypel2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // y can be a matrix of doubles or other
        // If y is not a matrix of doubles, call overload
        if (iTypel2 == sci_matrix)
        {

            // Retrieve a matrix of double at position 1 + iskip.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2 + iskip);
                return 1;
            }
        }
        else
        {
            lw = 2 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
            C2F(overload)(&lw, "plot2d", 6);
            return 0;
        }

        test = (m1 * n1 == 0) ||
               ((m1 == 1 || n1 == 1) && (m2 == 1 || n2 == 1) && (m1 * n1 == m2 * n2))  ||
               ((m1 == m2) && (n1 == n2)) ||
               ((m1 == 1 && n1 == m2) || (n1 == 1 && m1 == m2));
        //CheckDimProp
        if (!test)
        {
            Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
            return 1;
        }

        if (m1 * n1 == 0)
        {
            /* default x=1:n */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m2, n2, &lt);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            if (m2 == 1 && n2 > 1)
            {
                m2 = n2;
                n2 = 1;
            }
            for (i = 0; i < m2 ; ++i)
            {
                for (j = 0 ; j < n2 ;  ++j)
                {
                    *(lt + i + m2 * j) = (double) i + 1;
                }
            }
            m1 = m2;
            n1 = n2;
            l1 = lt;
        }
        else if ((m1 == 1 || n1 == 1) && (m2 != 1 && n2 != 1))
        {
            /* a single x vector for mutiple columns for y */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m2, n2, &lt);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            for (i = 0; i < m2 ; ++i)
            {
                for (j = 0 ; j < n2 ;  ++j)
                {
                    *(lt + i + m2 * j) = *(l1 + i);
                }
            }
            m1 = m2;
            n1 = n2;
            l1 = lt;
        }
        else if ((m1 == 1 && n1 == 1) && (n2 != 1))
        {
            /* a single y row vector  for a single x */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n2, &lt);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            for (j = 0 ; j < n2 ;  ++j)
            {
                lt[j] = *l1;
            }
            n1 = n2;
            l1 = lt;
        }
        else
        {
            if (m2 == 1 && n2 > 1)
            {
                m2 = n2;
                n2 = 1;
            }
            if (m1 == 1 && n1 > 1)
            {
                m1 = n1;
                n1 = 1;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of mandatory input arguments. At least %d expected.\n"), fname, 1);
        return 0;
    }

    if (n1 == -1 || n2 == -1 || m1 == -1 || m2 == -1)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d.\n"), fname, 1, 2); /* @TODO : detail error */
        return 0;
    }

    sciGetStyle(pvApiCtx, fname, 3 + iskip, n1, opts, &style);
    GetStrf(pvApiCtx, fname, 4 + iskip, opts, &strf);
    GetLegend(pvApiCtx, fname, 5 + iskip, opts, &legend);
    GetRect(pvApiCtx, fname, 6 + iskip, opts, &rect);
    GetNax(pvApiCtx, 7 + iskip, opts, &nax, &flagNax);

    if (iskip == 0)
    {
        GetLogflags(pvApiCtx, fname, 8, opts, &logFlags);
    }

    freeStrf = !isDefStrf(strf);

    // Check strf [0-1][0-8][0-5]
    if (!isDefStrf(strf) && (strlen(strf) != 3 || strf[0] < '0' || strf[0] > '1' || strf[1] < '0' || strf[1] > '8' || strf[2] < '0' || strf[2] > '5'))
    {
        Scierror(999, _("%s: Wrong value for strf option: %s.\n"), fname, strf);
        if (freeStrf)
        {
            freeAllocatedSingleString(strf);
        }
        return -1;
    }

    if (isDefStrf(strf))
    {
        strcpy(strfl, DEFSTRFN);

        strf = strfl;
        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }
        if (!isDefLegend(legend))
        {
            strfl[0] = '1';
        }

        GetOptionalIntArg(pvApiCtx, fname, 9, "frameflag", &frame, 1, opts);
        if (frame != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }

        GetOptionalIntArg(pvApiCtx, fname, 9, "axesflag", &axes, 1, opts);

        if (axes != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }

    /* Make a test on log. mode : available or not depending on the bounds set by Rect arg. or xmin/xmax :
       Rect case :
       - if the min bound is strictly posivite, we can use log. mode
       - if not, send error message
       x/y min/max case:
       - we find the first strictly positive min bound in Plo2dn.c ?? */

    switch (strf[1])
    {
        case '0':
            /* no computation, the plot use the previous (or default) scale */
            break;
        case '1' :
        case '3' :
        case '5' :
        case '7':
            /* based on Rect arg */
            if (rect[0] > rect[2] || rect[1] > rect[3])
            {
                if (freeStrf)
                {
                    freeAllocatedSingleString(strf);
                }
                Scierror(999, _("%s: Impossible status min > max in x or y rect data.\n"), fname);
                return -1;
            }

            if (rect[0] <= 0. && logFlags[1] == 'l') /* xmin */
            {
                if (freeStrf)
                {
                    freeAllocatedSingleString(strf);
                }
                Scierror(999, _("%s: Bounds on x axis must be strictly positive to use logarithmic mode.\n"), fname);
                return -1;
            }

            if (rect[1] <= 0. && logFlags[2] == 'l') /* ymin */
            {
                if (freeStrf)
                {
                    freeAllocatedSingleString(strf);
                }
                Scierror(999, _("%s: Bounds on y axis must be strictly positive to use logarithmic mode.\n"), fname);
                return -1;
            }

            break;
        case '2' :
        case '4' :
        case '6' :
        case '8':
        case '9':
            /* computed from the x/y min/max */
            if ((int)strlen(logFlags) < 1)
            {
                dataflag = 'g';
            }
            else
            {
                dataflag = logFlags[0];
            }

            switch (dataflag)
            {
                case 'e' :
                    size_x = (m1 != 0) ? 2 : 0;
                    break;
                case 'o' :
                    size_x = m1;
                    break;
                case 'g' :
                default  :
                    size_x = (n1 * m1);
                    break;
            }

            if (size_x != 0)
            {
                if (logFlags[1] == 'l' && sciFindStPosMin((l1), size_x) <= 0.0)
                {
                    if (freeStrf)
                    {
                        freeAllocatedSingleString(strf);
                    }
                    Scierror(999, _("%s: At least one x data must be strictly positive to compute the bounds and use logarithmic mode.\n"), fname);
                    return -1;
                }
            }

            size_y = (n1 * m1);

            if (size_y != 0)
            {
                if (logFlags[2] == 'l' && sciFindStPosMin((l2), size_y) <= 0.0)
                {
                    if (freeStrf)
                    {
                        freeAllocatedSingleString(strf);
                    }
                    Scierror(999, _("%s: At least one y data must be strictly positive to compute the bounds and use logarithmic mode\n"), fname);
                    return -1;
                }
            }

            break;
    }

    // open a figure if none already exists
    getOrCreateDefaultSubwin();

    Objplot2d (1, logFlags, (l1), (l2), &n1, &m1, style, strf, legend, rect, nax, flagNax);

    // Allocated by sciGetStyle (get_style_arg function in GetCommandArg.c)
    FREE(style);

    if (freeStrf)
    {
        freeAllocatedSingleString(strf);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*------------------------------------------------------------------------*/
