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

#ifndef __STATISTICS_GW_HXX__
#define __STATISTICS_GW_HXX__

extern "C"
{
#include "dynlib_statistics_gw.h"
}

class StatisticsModule
{
private :
    StatisticsModule() {};
    ~StatisticsModule() {};
public :
    EXTERN_STATISTICS_GW static int Load();
    EXTERN_STATISTICS_GW static int Unload()
    {
        return 1;
    }
};

#endif /* !__STATISTICS_GW_HXX__ */
