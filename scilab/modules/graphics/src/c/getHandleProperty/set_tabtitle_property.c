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
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "BOOL.h"
#include "TitlePositionType.h"
#include "stricmp.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_title_scroll_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;
    BOOL status = FALSE;

    b =  tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "title_scroll");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_TITLE_SCROLL__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "title_scroll");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
int set_title_position_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    enum TitlePositionType pos = TITLE_TOP;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "title_position");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "top") == 0)
    {
        pos = TITLE_TOP;
    }
    else if (stricmp((char*)_pvData, "left") == 0)
    {
        pos = TITLE_LEFT;
    }
    else if (stricmp((char*)_pvData, "bottom") == 0)
    {
        pos = TITLE_BOTTOM;
    }
    else if (stricmp((char*)_pvData, "right") == 0)
    {
        pos = TITLE_RIGHT;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s, %s or %s expected.\n"), "title_position", "'top'", "'left'", "'bottom'", "'right'");
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_UI_TITLE_POSITION__, &pos, jni_int, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "title_position");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
