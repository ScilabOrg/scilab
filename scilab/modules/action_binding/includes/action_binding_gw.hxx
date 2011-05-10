/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __ACTION_BINDING_GW_HXX__
#define __ACTION_BINDING_GW_HXX__

#include "function.hxx"

#ifdef _MSC_VER
	#if ACTION_BINDING_GW_EXPORTS
		#define EXTERN_ACTION_BINDING_GW __declspec (dllexport)
	#else
		#define EXTERN_ACTION_BINDING_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_ACTION_BINDING_GW
#endif

class ActionBindingModule
{
private :
    ActionBindingModule() {};
    ~ActionBindingModule() {};

public :
	EXTERN_ACTION_BINDING_GW static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_notify);

#endif /* !__ACTION_BINDING_GW_HXX__ */
