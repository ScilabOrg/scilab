/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xfpolys.c                                                    */
/* desc : interface for xfpolys routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "localization.h"

#include "CurrentObject.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xfpolys(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0;
    int m2 = 0, n2 = 0, l2 = 0;
    int m3 = 0, n3 = 0, l3 = 0;
    int mn2 = 0;

    int v1 = 0;                 /* v1 is the flag used for flat (v1==1) or interpolated (v1==2) shading */
    int i = 0;
    int iClosed = 1;
    long hdl = 0;

    char *pstSubWinUID = NULL;
    int iSubWinForeground = 0;
    int *piSubWinForeground = &iSubWinForeground;

    CheckRhs(2, 3);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    CheckSameDims(1, 2, m1, n1, m2, n2);
    mn2 = m2 * n2;
    if (mn2 == 0)
    {
        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    if (Rhs == 3)
    {
        GetRhsVar(3, MATRIX_OF_INTEGER_DATATYPE, &m3, &n3, &l3);

        if (m3 * n3 == m1 * n1)
        {
            CheckSameDims(1, 3, m1, n1, m3, n3);
            v1 = 2;             /* interpolated shading */

            if (m3 != 3 && m3 != 4)
            {
                Scierror(999, _("%s: Interpolated shading only works for polygons of size %d or %d\n"), fname, 3, 4);
                return 0;
            }
        }
        else
        {
            CheckVector(3, m3, n3);
            CheckDimProp(2, 3, m3 * n3 != n2);
            v1 = 1;             /* flat shading */
        }
    }
    else
    {
        int un = 1, ix = 0;

        CreateVar(3, MATRIX_OF_INTEGER_DATATYPE, &un, &n2, &l3);
        for (ix = 0; ix < n2; ++ix)
        {
            *istk(l3 + ix) = 0;
        }
        m3 = n3 = 1;
    }

    pstSubWinUID = getOrCreateDefaultSubwin();

    for (i = 0; i < n1; ++i)
    {
        if (m3 == 1 || n3 == 1) /* color vector specified */
        {
            if (*istk(l3 + i) == 0)
            {
                getGraphicObjectProperty(pstSubWinUID, __GO_MARK_STYLE__, jni_int, (void **)&piSubWinForeground);
                Objfpoly(stk(l1 + (i * m1)), stk(l2 + (i * m1)), m1, piSubWinForeground, &iClosed, &hdl, v1);
            }
            else
            {
                Objfpoly(stk(l1 + (i * m1)), stk(l2 + (i * m1)), m1, istk(l3 + i), &iClosed, &hdl, v1);
            }
        }
        else                    /* we have a color matrix used for interpolated shading : one color per vertex */
        {
            Objfpoly(stk(l1 + (i * m1)), stk(l2 + (i * m1)), m1, istk(l3 + i * m3), &iClosed, &hdl, v1);
        }
    }

    /** Construct Compound and make it current object**/
    setCurrentObject(ConstructCompoundSeq(n1));

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
