/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include "gw_jvm.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table  Tab[]=
{
	{NULL, ""}, //with_embedded_jre
	{NULL, ""}, //system_setproperty
	{NULL, ""}, //system_getproperty
	{NULL, ""}, //javaclasspath
	{NULL, ""} //javalibrarypath
};
/*--------------------------------------------------------------------------*/
int gw_jvm(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
