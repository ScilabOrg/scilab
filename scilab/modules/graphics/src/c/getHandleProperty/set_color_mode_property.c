/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_color_mode_property.c                                        */
/* desc : function to modify in Scilab the color_mode field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "BasicAlgos.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_color_mode_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int colorMode = 0;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "color_mode");
        return SET_PROPERTY_ERROR;
    }

    colorMode = (int)((double*)_pvData)[0];

    status = setGraphicObjectProperty(iObjUID, __GO_COLOR_MODE__, &colorMode, jni_int, 1);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "color_mode");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
