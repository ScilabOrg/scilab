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

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_history_manager_gw.h"
}
class HistoryManagerModule
{
private :
   HistoryManagerModule(){};
   ~HistoryManagerModule(){};
public :
   HISTORY_MANAGER_GW_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_loadhistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_addhistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_displayhistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_gethistoryfile, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_gethistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_historymanager, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_historysize, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_removelinehistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_resethistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_saveafterncommands, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_saveconsecutivecommands, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_savehistory, HISTORY_MANAGER_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_sethistoryfile, HISTORY_MANAGER_GW_IMPEXP);

#endif /* !__HISTORY_MANAGER_GW_HXX__ */
