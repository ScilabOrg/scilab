/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_core.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "machine.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_debug),"debug"},
{NULL, ""}, //who
{NULL, ""}, // warning
{NULL, ""}, //argn
{C2F(sci_getvariablesonstack),"getvariablesonstack"},
{C2F(sci_comp),"comp"},
{NULL, ""}, //getscilabmode
{C2F(sci_mode),"mode"},
{NULL ,""}, //type
{NULL, ""}, //error
{NULL, ""}, //resune
{NULL,""}, //return
{NULL,""}, //format
{NULL, ""}, // isdef
{NULL, ""}, //exists
{C2F(sci_errcatch),"errcatch"},
{NULL, ""}, // "errclear"
{C2F(sci_iserror),"iserror"},
{NULL,""}, //predef
{NULL,""}, //newfun
{C2F(sci_clearfun),"clearfun"},
{C2F(sci_funptr),"funptr"},
{C2F(sci_macr2lst),"macr2lst"},
{C2F(sci_setbpt),"setbpt"},
{C2F(sci_delbpt),"delbpt"},
{C2F(sci_dispbpt),"dispbpt"},
{NULL,""},//funcprot
{NULL,""}, //where
{C2F(sci_havewindow),"havewindow"},
{NULL, ""},//stacksize
{C2F(sci_mtlb_mode),"mtlb_mode"},
{NULL,""}, // clear
{C2F(sci_what),"what"},
{NULL, ""}, //sciargs
{C2F(sci_ieee),"ieee"},
{NULL,""},//typename
{NULL, ""}, //global
{NULL, ""}, //clearglobal
{NULL, ""}, //isglobal
{C2F(sci_gstacksize),"gstacksize"},
{C2F(sci_intppty),"intppty"},
{NULL, ""}, //lasterror
{NULL, ""}, //getversion
{C2F(sci_macr2tree),"macr2tree"},
{NULL, ""}, //getos
{NULL, ""}, //banner
{C2F(sci_getmemory),"getmemory"},
{NULL, ""}, //getmd5
{NULL,""}, //getmodules
{NULL, ""}, //with_module
{NULL, ""}, //getdebuginfo
{C2F(sci_readgateway),"readgateway"},
{NULL, ""} //exit
};
/*--------------------------------------------------------------------------*/
int gw_core(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
