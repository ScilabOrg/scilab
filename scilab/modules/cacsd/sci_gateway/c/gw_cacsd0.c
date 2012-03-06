
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include "gw_cacsd0.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
//    {sci_gschur, "gschur"}, obsolete (use schur in linear algebra)
//    {sci_gspec, "gspec"}, obsolete (use spec in linear algebra)
    {sci_ereduc, "ereduc"},
    {sci_fstair, "fstair"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd0(void)
{  
    return 0;
}
/*--------------------------------------------------------------------------*/
