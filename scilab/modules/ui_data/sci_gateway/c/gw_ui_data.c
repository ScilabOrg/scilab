/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_ui_data.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
/*static BOOL loadedDep = FALSE;*/
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
    {NULL, ""}, //"editvar"
    {NULL, ""}, //"browsevar"
    {NULL, ""}, //filebrowser
    {NULL, ""} //updatebrowsevar
};
/*--------------------------------------------------------------------------*/
int gw_ui_data(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
