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

#include "SetUicontrolSelected.hxx"

int SetUicontrolSelected(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int value = 0;
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "selected");
        return SET_PROPERTY_ERROR;
    }

    value = (int)((double*)_pvData)[0];

    status = setGraphicObjectProperty(iObjUID, __GO_UI_SELECTED__, &value, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "selected");
        return SET_PROPERTY_ERROR;
    }

}
