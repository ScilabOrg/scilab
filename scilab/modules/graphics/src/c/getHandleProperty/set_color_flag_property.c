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
/* file: set_color_flag_property.c                                        */
/* desc : function to modify in Scilab the color_flag field of            */
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
#include "MALLOC.h"
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_color_flag_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int type = -1;
    int *piType = &type;
    int flagcolor = (int) ((double*)_pvData)[0];

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "color_flag");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: Scalar expected.\n"), "color_flag");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_PLOT3D__)
    {
        if (flagcolor < 0 || flagcolor > 1)
        {
            Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "color_flag", "0", "1");
            return SET_PROPERTY_ERROR;
        }

        setGraphicObjectProperty(iObjUID, __GO_COLOR_FLAG__, &flagcolor, jni_int, 1);

        return SET_PROPERTY_SUCCEED;
    }
    else if (type == __GO_FAC3D__)
    {
        if (flagcolor < 0 || flagcolor > 4)
        {
            Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "color_flag", "0, 1, 2, 3, 4");
            return SET_PROPERTY_ERROR;
        }

        setGraphicObjectProperty(iObjUID, __GO_COLOR_FLAG__, &flagcolor, jni_int, 1);

        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "color_flag");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;

}
/*------------------------------------------------------------------------*/
