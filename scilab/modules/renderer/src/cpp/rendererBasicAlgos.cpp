/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Several C++ functions that parform some basic operations
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>
#include <iostream>

#include "rendererBasicAlgos.h"
extern "C"
{
#include "sciprint.h"
#include "localization.h"
#include "DestroyObjects.h"
}

/*--------------------------------------------------------------------------*/
void destroyRendererString(char * string)
{
  delete[] string;
}
/*--------------------------------------------------------------------------*/
void callFunctionFromGatewayWithExceptions(gw_generic_table * tab)
{
	try
	{
		// Exception might be generated in the renderer module.
		callFunctionFromGateway(tab);
	}
	catch (std::exception & e)
	{
		// Tell the user that somthing wrong occured
		sciprint(_("Warning !!!\nScilab has found a critical error (%s).\nSave your data and restart Scilab.\n"), "Unknow exception");
		// print the exception in the error output
		std::cerr << e.what() << std::endl;
	}
}
/*--------------------------------------------------------------------------*/
