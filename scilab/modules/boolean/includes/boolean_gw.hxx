/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __BOOLEAN_GW_HXX__
#define __BOOLEAN_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_boolean_gw.h"
}

class BooleanModule
{
private :
    BooleanModule() {};
    ~BooleanModule() {};
public :
    BOOLEAN_GW_IMPEXP static int Load();
    BOOLEAN_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_bool2s);
CPP_GATEWAY_PROTOTYPE(sci_find);
CPP_GATEWAY_PROTOTYPE(sci_and);
CPP_GATEWAY_PROTOTYPE(sci_or);

#endif /* __BOOLEAN_GW_HXX__ */
