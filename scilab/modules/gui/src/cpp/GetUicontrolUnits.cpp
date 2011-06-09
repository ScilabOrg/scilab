/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the units of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolUnits.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolUnits(sciPointObj* sciObj)
{
    char* units = NULL;
    char* type = NULL;

    /* Handle must be a uicontrol */
    getGraphicObjectProperty(sciObj->UID, __GO_TYPE__, jni_string, (void**) &type);
    if (strcmp(type, __GO_UICONTROL__) != 0)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Units");
        return FALSE;
    }

    getGraphicObjectProperty(sciObj->UID, __GO_UI_UNITS__, jni_string, (void**) &units);
    return sciReturnString(units);
}
