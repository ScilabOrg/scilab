/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_preferences.h"
#include "callFunctionFromGateway.h"
#include "loadOnUseClassPath.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
    {NULL, "addModulePreferences"}, //addModulePreferences
    {NULL, ""} //removeModulePreferences
};
/*--------------------------------------------------------------------------*/
int gw_preferences(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
