/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_antialiased_font_property.c                                  */
/* desc : function to retrieve in Scilab the antialiased_font field of    */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_antialiased_font_property(char *pobjUID)
{
    int iAntialiasedFont = 0;
    int* antialiasedFont = &iAntialiasedFont;

#if 0
    if (   sciGetEntityType(pobj) != SCI_SUBWIN
            && sciGetEntityType(pobj) != SCI_TEXT
            && sciGetEntityType(pobj) != SCI_LABEL
            && sciGetEntityType(pobj) != SCI_AXES
            && sciGetEntityType(pobj) != SCI_LEGEND)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "antialiased_font");
        return -1;
    }
#endif

    getGraphicObjectProperty(pobjUID, __GO_FONT_ANTIALIASED__, jni_bool, &antialiasedFont);

    if (antialiasedFont == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "antialiased_font");
        return -1;
    }

    if (iAntialiasedFont)
    {
        return sciReturnString( "on" );
    }
    else
    {
        return sciReturnString( "off" );
    }
}
/*------------------------------------------------------------------------*/
