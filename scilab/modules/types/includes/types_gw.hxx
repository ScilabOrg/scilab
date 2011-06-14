/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_GW_HXX__
#define __TYPES_GW_HXX__


#include "dynlib_types_gw.h"

#include "cpp_gateway_prototype.hxx"

class TypesModule
{
private :
  TypesModule() {};
  ~TypesModule() {};

public :
  TYPES_GW_IMPEXP static bool Load();
};
CPP_GATEWAY_PROTOTYPE(sci_list);
CPP_GATEWAY_PROTOTYPE(sci_struct);
CPP_GATEWAY_PROTOTYPE(sci_null);
CPP_GATEWAY_PROTOTYPE(sci_insert);
CPP_GATEWAY_PROTOTYPE(sci_cell);
CPP_GATEWAY_PROTOTYPE(sci_makecell);
CPP_GATEWAY_PROTOTYPE(sci_usertype);
CPP_GATEWAY_PROTOTYPE(sci_typeof);
CPP_GATEWAY_PROTOTYPE(sci_tlist);
CPP_GATEWAY_PROTOTYPE(sci_mlist);
CPP_GATEWAY_PROTOTYPE(sci_isfield);
CPP_GATEWAY_PROTOTYPE(sci_getfield);
CPP_GATEWAY_PROTOTYPE(sci_fieldnames);

#endif /* !__TYPES_GW_HXX__ */
