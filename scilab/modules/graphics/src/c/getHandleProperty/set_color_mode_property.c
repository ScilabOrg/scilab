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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_color_mode_property.c                                        */
/* desc : function to modify in Scilab the color_mode field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_color_mode_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int colorMode = 0;

    if ( !isParameterDoubleMatrix( valueType ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "color_mode");
        return SET_PROPERTY_ERROR;
    }

#if 0
    if ( sciGetEntityType(pobj) != SCI_SURFACE )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"color_mode") ;
        return SET_PROPERTY_ERROR;
    }
#endif

    colorMode = (int) getDoubleFromStack(stackPointer);

    status = setGraphicObjectProperty(pobjUID, __GO_COLOR_MODE__, &colorMode, jni_int, 1);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"color_mode");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
