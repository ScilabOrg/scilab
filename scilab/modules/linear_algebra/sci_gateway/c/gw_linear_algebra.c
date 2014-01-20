
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include "api_scilab.h"
#include "MALLOC.h"
#include "gw_linear_algebra.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static int C2F(intvoid)(char *fname, unsigned long fname_len)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {C2F(inthess), "hess"},
    {C2F(intschur), "schur"},
    {C2F(intspec), "spec"},
    {C2F(intbdiagr), "bdiag"},
    {C2F(intvoid), "xxxx"},
    {C2F(intbalanc), "balanc"}
};
/*--------------------------------------------------------------------------*/
int gw_linear_algebra(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
