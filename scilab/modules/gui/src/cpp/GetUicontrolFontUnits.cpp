/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font units of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolFontUnits.hxx"

void* GetUicontrolFontUnits(void* _pvCtx, char *sciObjUID)
{
    char* fontUnits = NULL;
    void* status = NULL;

    getGraphicObjectProperty(sciObjUID, __GO_UI_FONTUNITS__, jni_string, (void**) &fontUnits);

    if (fontUnits == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontUnits");
        return NULL;
    }
    else
    {
        status = sciReturnString( fontUnits);
        delete[] fontUnits;
        return status;
    }

}
