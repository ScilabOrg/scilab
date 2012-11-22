/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __USERDATAHANDLER_HXX__
#define __USERDATAHANDLER_HXX__

#include <map>
#include <vector>

extern "C"
{
#define __USE_DEPRECATED_STACK_FUNCTIONS__

#include "api_scilab.h"
#include "stack2.h"
#include "returnProperty.h"

    void removeUIWidgetUserData(const int uid);
}

namespace org_scilab_modules_uiwidget
{

class UserDataHandler
{

    typedef std::vector<int> UserData;
    typedef std::map<int, const UserData *> UserDataMap;
    typedef UserDataMap::iterator UserDataMapIt;

    static UserDataMap map;

public:

    static void put(const int uid, const UserData * data);

    static void put(const int uid, const int * data, const int size);

    static void put(const int uid, const int stackPos);

    static int get(const int uid, void * pvApiCtx, const int stackPos);

    static void remove(const int uid);
};
}

#endif // __USERDATAHANDLER_HXX__
