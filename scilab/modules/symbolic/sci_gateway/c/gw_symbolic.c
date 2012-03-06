/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_symbolic.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
{sci_addf,"addf"},
{sci_subf,"subf"},
{sci_mulf,"mulf"},
{sci_ldivf,"ldivf"},
{sci_rdivf,"rdivf"}
};
/*--------------------------------------------------------------------------*/
int gw_symbolic(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
