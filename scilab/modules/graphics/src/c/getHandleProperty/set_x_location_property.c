/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_x_location_property.c                                        */
/* desc : function to modify in Scilab the x_location field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_x_location_property(sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{

    if (!isParameterStringMatrix(valueType))
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "x_location");
        return SET_PROPERTY_ERROR;
    }

    if (sciGetEntityType(pobj) != SCI_SUBWIN)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "x_location");
        return SET_PROPERTY_ERROR;
    }

    if (isStringParamEqual(stackPointer, "top"))
    {
        pSUBWIN_FEATURE(pobj)->axes.xdir = 'u';
    }
    else if (isStringParamEqual(stackPointer, "bottom"))
    {
        pSUBWIN_FEATURE(pobj)->axes.xdir = 'd';
    }
    else if (isStringParamEqual(stackPointer, "middle"))
    {
        pSUBWIN_FEATURE(pobj)->axes.xdir = 'c';
    }
    else if (isStringParamEqual(stackPointer, "origin"))
    {
        pSUBWIN_FEATURE(pobj)->axes.xdir = 'o';
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "x_location", "top, bottom, middle, origin");
        return SET_PROPERTY_ERROR;
    }
    return SET_PROPERTY_SUCCEED;
}

/*------------------------------------------------------------------------*/
