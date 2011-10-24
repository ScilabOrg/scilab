/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CORE_GW_HXX__
#define __CORE_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_core_gw.h"

class CoreModule
{
private :
    CoreModule(){};
    ~CoreModule(){};
public :
    CORE_GW_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_getmodules, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_clear, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_banner, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_quit, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_exit, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_exists, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_isdef, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_argn, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_global, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_clearglobal, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_isglobal, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_getscilabmode, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_getos, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_with_module, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_lasterror, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_getversion, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_warning, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_getmd5, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_error, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_pause, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_resume, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_abort, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_who, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_stacksize, CORE_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_sciargs, CORE_GW_IMPEXP);

#endif /* __CORE_GW_HXX__ */
