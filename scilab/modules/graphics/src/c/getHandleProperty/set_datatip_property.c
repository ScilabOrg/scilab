/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/**
 * Sets the datatip data.
 */
int set_tip_data_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    double *tip_data;

    if (!( valueType == sci_matrix ))
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "tip_data");
        return SET_PROPERTY_ERROR ;
    }

    /* We must have 4 elements */
    if ( nbRow * nbCol != 3 )
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "tip_data", 3);
        return SET_PROPERTY_ERROR;
    }

    tip_data = getDoubleMatrixFromStack(stackPointer);


    status = setGraphicObjectProperty(pobjUID, __GO_DATATIP_DATA__, tip_data, jni_double_vector, 3);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_data");
        return SET_PROPERTY_ERROR;
    }
}



/**
 * Sets the datatip oriantation.
 */
int set_tip_orientation_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int tip_orientation;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "tip_orientation");
        return SET_PROPERTY_ERROR;
    }

    tip_orientation = (int)getDoubleFromStack(stackPointer);

    status = setGraphicObjectProperty(pobjUID, __GO_DATATIP_ORIENTATION__, &tip_orientation, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_orientation");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Enable/disable the datatip Z component to be displayed.
 */
int set_tip_3component_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int use_z = tryGetBooleanValueFromStack(stackPointer, valueType, nbRow, nbCol, "tip_3component");
    if(use_z == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(pobjUID, __GO_DATATIP_3COMPONENT__, &use_z, jni_bool, 1); 

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_3component");
        return SET_PROPERTY_ERROR;
    }
}

