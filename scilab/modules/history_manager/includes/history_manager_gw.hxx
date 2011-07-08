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

#ifndef __HISTORY_MANAGER_GW_HXX__
#define __HISTORY_MANAGER_GW_HXX__

#ifdef _MSC_VER
	#if HISTORY_MANAGER_GW_EXPORTS
		#define EXTERN_HISTORY_MANAGER_GW __declspec (dllexport)
	#else
		#define EXTERN_HISTORY_MANAGER_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_HISTORY_MANAGER_GW
#endif

#include "cpp_gateway_prototype.hxx"

class HistoryManagerModule
{
private :
   HistoryManagerModule(){};
   ~HistoryManagerModule(){};
public :
   EXTERN_HISTORY_MANAGER_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_loadhistory);
CPP_GATEWAY_PROTOTYPE(sci_addhistory);
CPP_GATEWAY_PROTOTYPE(sci_displayhistory);
CPP_GATEWAY_PROTOTYPE(sci_gethistoryfile);
CPP_GATEWAY_PROTOTYPE(sci_gethistory);
CPP_GATEWAY_PROTOTYPE(sci_historymanager);
CPP_GATEWAY_PROTOTYPE(sci_historysize);
CPP_GATEWAY_PROTOTYPE(sci_removelinehistory);
CPP_GATEWAY_PROTOTYPE(sci_resethistory);
CPP_GATEWAY_PROTOTYPE(sci_saveafterncommands);
CPP_GATEWAY_PROTOTYPE(sci_saveconsecutivecommands);
CPP_GATEWAY_PROTOTYPE(sci_savehistory);
CPP_GATEWAY_PROTOTYPE(sci_sethistoryfile);

#endif /* !__HISTORY_MANAGER_GW_HXX__ */
