/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: set_auto_dimensionning_property.c                                */
/* desc : function to modify in Scilab the auto_dimensionning field of    */
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
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_auto_dimensionning_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    static int oldTextBoxMode = -1;

    BOOL status = FALSE;
    int b =  (int)FALSE;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "auto_dimensionning");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObjUID, __GO_AUTO_DIMENSIONING__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        if (b && oldTextBoxMode != -1)
        {
            setGraphicObjectProperty(iObjUID, __GO_TEXT_BOX_MODE__, &oldTextBoxMode, jni_int, 1);
        }
        else
        {
            int textBoxMode = 2;
            int * piOldTextBoxMode = &oldTextBoxMode;
            getGraphicObjectProperty(iObjUID, __GO_TEXT_BOX_MODE__, jni_int, (void**)&piOldTextBoxMode);
            setGraphicObjectProperty(iObjUID, __GO_TEXT_BOX_MODE__, &textBoxMode, jni_int, 1);
        }

        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_dimensionning");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/

