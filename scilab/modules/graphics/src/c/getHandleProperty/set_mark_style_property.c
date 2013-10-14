/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_mark_style_property.c                                        */
/* desc : function to modify in Scilab the mark_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#define MAX_MARK_STYLE 14

/*------------------------------------------------------------------------*/
int set_mark_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int status1 = 0;

    BOOL status = FALSE;
    BOOL status2 = FALSE;

    int markMode = 1;
    int markStyle = 0;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "mark_style");
        return SET_PROPERTY_ERROR;
    }

    markStyle = (int)((double*)_pvData)[0];

    if (markStyle < 0 || markStyle > MAX_MARK_STYLE)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be between %d and %d.\n"), "mark_style", 0, MAX_MARK_STYLE);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
    status2 = setGraphicObjectProperty(iObjUID, __GO_MARK_STYLE__, &markStyle, jni_int, 1);

    if (status == TRUE && status2 == TRUE)
    {
        status1 = SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_mode");
        status1 = SET_PROPERTY_ERROR;
    }

    return sciSetFinalStatus((SetPropertyStatus)status1, (SetPropertyStatus)status2);
}
/*------------------------------------------------------------------------*/
