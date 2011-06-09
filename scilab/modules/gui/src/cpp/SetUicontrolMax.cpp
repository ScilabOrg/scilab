/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Sets the max property of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolMax.hxx"

int SetUicontrolMax(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    int maxValue = 0;
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real expected.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }
    if(nbCol != 1 || nbRow != 1)
    {
        /* Wrong value size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real expected.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }


    /* Store the value in Scilab */
    maxValue = (int) getDoubleFromStack(stackPointer);

    status = setGraphicObjectProperty(sciObj->UID, const_cast<char*>(__GO_UI_MAX__), &maxValue, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }
}
