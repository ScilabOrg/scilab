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
  TYPES_GW_IMPEXP static int Load();
};
CPP_GATEWAY_PROTOTYPE(sci_list, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_struct, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_null, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_insert, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_cell, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_makecell, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_usertype, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_typeof, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_tlist, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_mlist, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_isfield, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_getfield, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_fieldnames, TYPES_GW_IMPEXP);
#ifndef NDEBUG
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetItemCount, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inspectorShowItem, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetItem, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetUnreferencedItem, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inspectorDeleteUnreferencedItems, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inspectorShowUnreferencedItem, TYPES_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetFunctionList, TYPES_GW_IMPEXP);
#endif

#endif /* !__TYPES_GW_HXX__ */
