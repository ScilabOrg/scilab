/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_rect_property.c                                         */
/* desc : function to modify in Scilab the rect field of             */
/*        a matplot handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "getGraphicObjectProperty.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_rect_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double * rect = (double*)_pvData;
    double pdblScale[2];
    int numX;
    int * piNumX = &numX;
    int numY;
    int * piNumY = &numY;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "rect");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 4)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "rect", 4);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_BOUNDS__, _pvData, jni_double_vector, 4);
    if (status == TRUE)
    {
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void **)&piNumX);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void **)&piNumY);
        setGraphicObjectProperty(iObjUID, __GO_MATPLOT_TRANSLATE__, _pvData, jni_double_vector, 2);
        pdblScale[0] = (rect[2] - rect[0]) / (numX - 1.0);
        pdblScale[1] = (rect[3] - rect[1]) / (numY - 1.0);
        setGraphicObjectProperty(iObjUID, __GO_MATPLOT_SCALE__, pdblScale, jni_double_vector, 2);

        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "margins");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
