/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <stdlib.h>
#endif

#include "FindIconHelper.hxx"

extern "C" {
#include "SetUicontrol.h"
#include "splitpath.h"
#include "expandPathVariable.h"
#include "getScilabJavaVM.h"
#include "scicurdir.h"
#include "MALLOC.h"
}
/*------------------------------------------------------------------------*/
int SetUicontrolIcon(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    char* expandedpath = NULL;
    char absolutepath[4096];

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "icon");
        return SET_PROPERTY_ERROR;
    }

    //get file path
    expandedpath = expandPathVariable((char*)_pvData);
#ifdef _MSC_VER
    {
        GetFullPathName((char*)_pvData, 4096, absolutepath, NULL);
    }
#else
    {
        realpath((char*)_pvData, absolutepath);
    }
#endif

    //it is a absolute path, put it only in model
    if (strcmp(expandedpath, absolutepath))
    {
        int iErr = 0;
        char* pwd = scigetcwd(&iErr);

        //add it to ScilabSwingUtilities java class
        org_scilab_modules_commons_gui::FindIconHelper::addThemePath(getScilabJavaVM(), pwd);
    }

    FREE(expandedpath);

    if (setGraphicObjectProperty(iObjUID, __GO_UI_ICON__, (char*)_pvData, jni_string, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "icon");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
