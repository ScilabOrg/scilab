/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_matplot.h                                                    */
/* desc : interface for matplot routine                                   */
/*------------------------------------------------------------------------*/

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "graphichandle.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include <string.h>
#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "Matplot.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_matplot(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    int m1 = 0, n1 = 0;
    int *dims = NULL;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    int *frameflag = NULL;
    int *axesflag  = NULL;

    char   * strf    = NULL ;
    char strfl[4];
    double* rect    = NULL ;
    int    * nax     = NULL ;
    BOOL     flagNax = FALSE;

    unsigned char * l1 = NULL;
    int plottype = -1;

    if (in.size() < 1)
    {
        return Overload::call(L"%_Matplot", in, _iRetCount, out, new ast::ExecVisitor());

    }
    else if (in.size() > 5)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "Matplot", 1, 5);
        return types::Function::Error;
    }

    if (opt.size() > 4)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "Matplot", 1, 5);
        return types::Function::Error;
    }
    else if (opt.size() > 0)
    {
        for (int i = 0; i < opt.size(); i++)
        {
            if (opt[i].first == L"rect")
            {
                rect = opt[i].second->getAs<types::Double>()->get();
            }
            else if (opt[i].first == L"nax")
            {
                nax = (int*)opt[i].second->getAs<types::Double>()->get();
            }
            else if (opt[i].first == L"frameflag")
            {
                frameflag = (int*)opt[i].second->getAs<types::Double>()->get();
            }
            else if (opt[i].first == L"axesflag")
            {
                axesflag = (int*)opt[i].second->getAs<types::Double>()->get();
            }
        }
    }

    if (in[0]->isDouble())
    {
        types::Double *pDbl = in[0]->getAs<types::Double>();
        l1 = (unsigned char *) pDbl->get();
        if (pDbl->getDims() > 2)
        {

            dims = pDbl->getDimsArray();
            if (pDbl->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
                return types::Function::Error;
            }

            m1 = dims[0];
            n1 = dims[1];
            if (dims[2] == 1)
            {
                plottype = buildMatplotType(MATPLOT_HM1_Double, MATPLOT_FORTRAN, MATPLOT_GRAY);
            }
            else if (dims[2] == 3)
            {
                plottype = buildMatplotType(MATPLOT_HM3_Double, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM4_Double, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pDbl->getRows();
            n1 = pDbl->getCols();
            plottype = buildMatplotType(MATPLOT_Double, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isInt8())
    {
        types::Int8 *pDbl = in[0]->getAs<types::Int8>();
        l1 = (unsigned char *) pDbl->get();
        if (pDbl->getDims() > 2)
        {

            dims = pDbl->getDimsArray();
            if (pDbl->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
                return types::Function::Error;
            }

            m1 = dims[0];
            n1 = dims[1];
            if (dims[2] == 1)
            {
                plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_GRAY);
            }
            else if (dims[2] == 3)
            {
                plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pDbl->getRows();
            n1 = pDbl->getCols();
            plottype = buildMatplotType(MATPLOT_Char, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isUInt8())
    {
        types::UInt8 *pDbl = in[0]->getAs<types::UInt8>();
        l1 = (unsigned char *) pDbl->get();
        if (pDbl->getDims() > 2)
        {

            dims = pDbl->getDimsArray();
            if (pDbl->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
                return types::Function::Error;
            }

            m1 = dims[0];
            n1 = dims[1];
            if (dims[2] == 1)
            {
                plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_GRAY);
            }
            else if (dims[2] == 3)
            {
                plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pDbl->getRows();
            n1 = pDbl->getCols();
            plottype = buildMatplotType(MATPLOT_UChar, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isInt16())
    {
        types::Int16 *pDbl = in[0]->getAs<types::Int16>();
        l1 = (unsigned char *) pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_Short, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16 *pDbl = in[0]->getAs<types::UInt16>();
        l1 = (unsigned char *) pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_UShort, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }
    else if ((in[0]->isInt32()) || (in[0]->isInt64()))
    {
        types::Int32 *pDbl = in[0]->getAs<types::Int32>();
        l1 = (unsigned char *) pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_Int, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }
    else if ((in[0]->isUInt32()) || (in[0]->isUInt64()))
    {
        types::UInt32 *pDbl = in[0]->getAs<types::UInt32>();
        l1 = (unsigned char *) pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_UInt, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }

    if (in.size() > 1)
    {
        if (!(in[1]->isString()))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "Matplot1", 2);
            return types::Function::Error;

        }
        strf =  wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
        if (in.size() > 2)
        {
            if (!(in[2]->isDouble()))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "Matplot1", 3);
                return types::Function::Error;

            }
            rect =  in[2]->getAs<types::Double>()->get();
            if (in.size() > 3)
            {
                if (!(in[3]->isDouble()))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "Matplot1", 4);
                    return types::Function::Error;

                }
                nax =  (int*)in[3]->getAs<types::Double>()->get();
                flagNax = TRUE;
            }
        }
    }

    getOrCreateDefaultSubwin();

    if (isDefStrf(strf))
    {
        strcpy(strfl, DEFSTRFN);

        strf = strfl;
        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }

        if (frameflag != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }

        if (axesflag != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }
    else
    {
        strf = strfl;
        strfl[0] = '0';
        strfl[1] = '8';
        strfl[2] = '1';

    }
    ObjmatplotImage(l1, &m1, &n1, strf, rect, nax, flagNax, plottype);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
