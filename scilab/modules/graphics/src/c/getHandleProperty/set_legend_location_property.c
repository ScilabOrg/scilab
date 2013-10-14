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
/* file: set_legend_location_property.c                                   */
/* desc : function to modify in Scilab the legend_location (place) field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Axes.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_legend_location_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    char* legendLocationsNames[11] = {"in_upper_right", "in_upper_left", "in_lower_right", "in_lower_left",
                                      "out_upper_right", "out_upper_left", "out_lower_right", "out_lower_left", "upper_caption", "lower_caption",
                                      "by_coordinates"
                                     };
    int i = 0;
    int index = -1;
    char* legendLocation = NULL;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "legend_location");
        return SET_PROPERTY_ERROR;
    }

    legendLocation = (char*)_pvData;

    for (i = 0; i < 11; i++)
    {
        if (strcmp(legendLocation, legendLocationsNames[i]) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "legend_location", "in_upper_right, in_upper_left, in_lower_right, in_lower_left, out_upper_right, out_upper_left, out_lower_right, out_lower_left, upper_caption, lower_caption, by_coordinates");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_LEGEND_LOCATION__, &index, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "legend_location");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
