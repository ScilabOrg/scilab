/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_drawing_method_property.c                                    */
/* desc : function to modify in Scilab the drawing_method field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "stricmp.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_arc_drawing_method_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int arcDrawingMethod = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "arc_drawing_method");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "nurbs") == 0)
    {
        arcDrawingMethod = 0;
    }
    else if (stricmp((char*)_pvData, "lines") == 0)
    {
        arcDrawingMethod = 1;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "drawing_method", "nurbs", "lines");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_ARC_DRAWING_METHOD__, &arcDrawingMethod, jni_int, 1);
    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "arc_drawing_method");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
