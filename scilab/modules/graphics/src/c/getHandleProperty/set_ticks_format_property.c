/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_ticks_format_property.c                                      */
/* desc : function to modify in Scilab the ticks_format field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_ticks_format_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    char ** values = (char**)_pvData;
    int propr[3] = {__GO_X_AXIS_FORMAT__, __GO_Y_AXIS_FORMAT__, __GO_Z_AXIS_FORMAT__};
    int nb = nbRow * nbCol;
    int i = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "ticks_format");
        return SET_PROPERTY_ERROR;
    }

    if (nb > 3)
    {
        Scierror(999, _("Wrong size for '%s' property: At most %d elements expected.\n"), "ticks_format", 3);
        return SET_PROPERTY_ERROR;
    }

    for (i = 0; i < nb; i++)
    {
        status = setGraphicObjectProperty(iObjUID, propr[i], values[i], jni_string, 1);
        if (status == FALSE)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "ticks_format");
            return SET_PROPERTY_ERROR;
        }
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
