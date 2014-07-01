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
/* file: set_colors_property.c                                            */
/* desc : function to modify in Scilab he polylines colors                */
/*------------------------------------------------------------------------*/

#include "MALLOC.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_colors_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int iNumElements = 0;
    int* piNumElements = &iNumElements;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "colors");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void **) &piNumElements);

    /*
     * A way to display a more explicit message would be to first get the
     * interpolation vector set flag and test it for NULL.
     */
    if (piNumElements == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data");
        return SET_PROPERTY_ERROR;
    }

    if (nbCol == 0)
    {
        int colorSet = 0;
        status = setGraphicObjectProperty(pobjUID, __GO_COLOR_SET__, &colorSet, jni_bool, 1);
        if (status == FALSE)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "colors");
            return SET_PROPERTY_ERROR;
        }
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COLORS__, NULL, jni_int_vector, 0);

        return SET_PROPERTY_SUCCEED;
    }

    if (nbCol == iNumElements)
    {
        int * tmp = MALLOC(nbCol * sizeof(int));
        stk(stackPointer);

        copyDoubleVectorToIntFromStack(stackPointer, tmp, nbCol);

        status = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COLORS__, tmp, jni_int_vector, nbCol);
        if (status == TRUE)
        {
            int colorSet = 1;
            setGraphicObjectProperty(pobjUID, __GO_COLOR_SET__, &colorSet, jni_bool, 1);
            FREE(tmp);
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            FREE(tmp);
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "colors");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        Scierror(999, _("The number of column of the color vector must match the number of points defining the line.\n"));
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
