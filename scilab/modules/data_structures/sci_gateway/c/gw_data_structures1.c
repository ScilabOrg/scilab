/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#include "gw_data_structures1.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{NULL, ""}, // list => Moved to type module
	{NULL, ""}, // tlist => Moved to type module
	{sci_rlist, "rlist"},
	{sci_lsslist, "lsslist"},
	{sci_glist, "glist"},
	{sci_lstcat, "lstcat"},
	{NULL, ""}, // mlist => Moved to type module
	{sci_definedfields, "definedfields"},
	{sci_lstsize, "lstsize"}
};
/*--------------------------------------------------------------------------*/
int gw_data_structures1(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(gwdatastructures1)(void)
{
	return gw_data_structures1();
}
/*--------------------------------------------------------------------------*/
