/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the max property of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolMax.hxx"

int GetUicontrolMax(sciPointObj* sciObj)
{
    int maxValue = 0;
    int* piMaxValue = &maxValue;

    getGraphicObjectProperty(sciObj->UID, const_cast<char*>(__GO_UI_MAX__), jni_int, (void**) &piMaxValue);

    if (piMaxValue == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Max");
        return FALSE; 
    }
    else
    {
        return sciReturnDouble((double) maxValue);
    }
}
