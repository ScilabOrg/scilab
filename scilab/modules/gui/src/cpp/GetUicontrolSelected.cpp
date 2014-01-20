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

#include "GetUicontrolSelected.hxx"

int GetUicontrolSelected(void* _pvCtx, int iObjUID)
{
    int selected = 0;
    int *piSelected = &selected;

    getGraphicObjectProperty(iObjUID, __GO_UI_SELECTED__, jni_int, (void **)&piSelected);

    if (piSelected == NULL)
    {
        Scierror(999, const_cast < char *>(_("'%s' property does not exist for this handle.\n")), "Selected");

        return FALSE;
    }

    return sciReturnInt(_pvCtx, selected);
}
