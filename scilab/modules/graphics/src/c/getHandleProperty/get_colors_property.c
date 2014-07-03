/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_colors_property.c                                            */
/* desc : function to get in Scilab the polyline colors                */
/*------------------------------------------------------------------------*/

#include "SetPropertyStatus.h"
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_colors_property(void* _pvCtx, char* pobjUID)
{
    int * colors = NULL;
    int numColors = 0;
    int * piNumColors = &numColors;
    int colorSet = 0;
    int *piColorSet = &colorSet;

    getGraphicObjectProperty(pobjUID, __GO_COLOR_SET__, jni_bool, (void **)&piColorSet);
    if (piColorSet == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "colors");
        return -1;
    }

    getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COLORS__, jni_int_vector, (void **)&colors);
    getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_COLORS__, jni_int, &piNumColors);

    if (colors == NULL || numColors == 0)
    {
        sciReturnEmptyMatrix(_pvCtx);
        return SET_PROPERTY_SUCCEED;
    }

    return sciReturnRowVectorFromInt(_pvCtx, colors, numColors);
}
/*------------------------------------------------------------------------*/
