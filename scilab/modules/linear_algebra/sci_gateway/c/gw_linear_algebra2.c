
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
#include "gw_linear_algebra.h"
#include "gw_linear_algebra2.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {C2F(intqr), "qr"},
    {C2F(intsvd), "svd"},
    {C2F(intlsq), "lsq"},
    {C2F(intspec), "spec"},
    {C2F(intinv), "inv"},
    {C2F(intrcond), "rcond"},
    {C2F(intchol), "chol"},
    {C2F(intlu), "lu"},
    {C2F(intslash), "slash"},
    {C2F(intbackslash), "backslash"},
    {C2F(intschur), "schur"},
    {C2F(inthess), "hess"},
    {C2F(intdet), "det"},
    {C2F(intbalanc), "balanc"},
	{C2F(intnorm),"norm"}
};
/*--------------------------------------------------------------------------*/
int gw_linear_algebra2(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    if (Err <= 0 && C2F(errgst).err1 <= 0)
    {
        PutLhsVar();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
