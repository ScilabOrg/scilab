/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_y_label_property.c                                           */
/* desc : function to modify in Scilab the y_label field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_y_label_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_AXES__)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "y_label");
        return SET_PROPERTY_ERROR;
    }

    Scierror(999, _("Can not set directly a label object.\n"));
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
