/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SIGNAL_GW_HXX__
#define __SIGNAL_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#ifdef _MSC_VER
	#if SIGNAL_GW_EXPORTS
		#define EXTERN_SIGNAL_GW __declspec (dllexport)
	#else
		#define EXTERN_SIGNAL_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_SIGNAL_GW
#endif

class SignalProcessingModule
{
private :
   SignalProcessingModule(){};
   ~SignalProcessingModule(){};
public :
   EXTERN_SIGNAL_GW static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_rpem);

#endif /* !__SIGNAL_GW_HXX__ */
