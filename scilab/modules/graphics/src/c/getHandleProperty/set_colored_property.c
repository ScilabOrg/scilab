/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: set_colored_property.c                                           */
/* desc : function to modify in Scilab the colored field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_colored_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int b =  (int)FALSE;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "colored");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_COLORED__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "colored");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
