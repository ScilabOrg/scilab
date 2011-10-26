/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_scinotes.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
#include "sci_mode.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
    {NULL, ""}, //"editor"
    {NULL, ""} // "closeEditor"
};
/*--------------------------------------------------------------------------*/
int gw_scinotes(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
