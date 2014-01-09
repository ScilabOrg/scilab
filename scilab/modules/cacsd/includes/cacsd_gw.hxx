/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CACSD_GW_HXX__
#define __CACSD_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_cacsd_gw.h"

class CacsdModule
{
private :
    CacsdModule() {};
    ~CacsdModule() {};
public :
    EXTERN_CACSD_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_arl2_ius);

#endif /* __CACSD_GW_HXX__ */
