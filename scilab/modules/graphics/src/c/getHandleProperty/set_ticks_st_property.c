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
/* file: set_ticks_st_property.c                                          */
/* desc : function to modify in Scilab the ticks_st field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_ticks_st_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int propr[3] = {__GO_X_AXIS_ST_FACTORS__, __GO_Y_AXIS_ST_FACTORS__, __GO_Z_AXIS_ST_FACTORS__};
    double* values = (double*)_pvData;
    int nb = nbRow * nbCol;
    int i = 0;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "ticks_st");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow != 2 && nbCol > 3)
    {
        Scierror(999, _("Wrong size for '%s' property: At most %d columns and %d rows expected.\n"), "ticks_st", 3, 2);
        return SET_PROPERTY_ERROR;
    }

    for (i = 0; i < nb / 2; i++)
    {
        status = setGraphicObjectProperty(iObjUID, propr[i], values + 2 * i, jni_double_vector, 2);
        if (status == FALSE)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "ticks_st");
            return SET_PROPERTY_ERROR;
        }
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
