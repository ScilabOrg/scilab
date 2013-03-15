
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifdef _MSC_VER
#pragma warning(disable: 4113)
#endif
/*--------------------------------------------------------------------------*/
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_slicot.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_sident, "sident"},
    {sci_sorder, "sorder"},
    {sci_findbd, "findbd"},
    {sci_rankqr, "rankqr"},
    {intab01od, "contr"},
    {sci_mucomp, "mucomp"},
    {sci_ricc2, "pet_ricc"},
    {sci_hinf, "hinf"},
    {sci_dhinf, "dhinf"},
    {sci_linmeq, "linmeq"}
};
/*--------------------------------------------------------------------------*/
int gw_slicot(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/

