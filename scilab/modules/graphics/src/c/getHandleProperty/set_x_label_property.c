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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_x_label_property.c                                           */
/* desc : function to modify in Scilab the x_label field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_x_label_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    char* type;

#if 0
    if ( sciGetEntityType(pobj) != SCI_SUBWIN )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"x_label");
        return SET_PROPERTY_ERROR;
    }
#endif

    getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string, &type);

    if (strcmp(type, __GO_AXES__) != 0)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"x_label");
        return SET_PROPERTY_ERROR;
    }

    Scierror(999, _("Can not set directly a label object.\n"));
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
