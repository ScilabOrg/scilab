/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_tics_direction_property.c                                    */
/* desc : function to modify in Scilab the tics_direction field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "stricmp.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_tics_direction_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int ticksDirection = 0;
    int yNumberTicks = 0;
    int *piYNumberTicks = &yNumberTicks;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "tics_direction");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_Y_NUMBER_TICKS__, jni_int, (void **) &piYNumberTicks);

    if (piYNumberTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_direction");
        return SET_PROPERTY_ERROR;
    }

    if (yNumberTicks == 1)
    {
        if (stricmp((char*)_pvData, "top") == 0)
        {
            ticksDirection = 0;
        }
        else if (stricmp((char*)_pvData, "bottom") == 0)
        {
            ticksDirection = 1;
        }
        else
        {
            Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "tics_direction", "'top'", "'bottom'");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        if (stricmp((char*)_pvData, "left") == 0)
        {
            ticksDirection = 2;
        }
        else if (stricmp((char*)_pvData, "right") == 0)
        {
            ticksDirection = 3;
        }
        else
        {
            Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "tics_direction", "'left'", "'right'");
            return SET_PROPERTY_ERROR;
        }
    }

    status = setGraphicObjectProperty(iObjUID, __GO_TICKS_DIRECTION__, &ticksDirection, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_direction");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
