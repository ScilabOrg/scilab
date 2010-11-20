/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_isoview_property.c                                           */
/* desc : function to modify in Scilab the isoview field of               */
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
int set_isoview_property(sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;

    if (sciGetEntityType(pobj) != SCI_SUBWIN)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "isoview");
        return SET_PROPERTY_ERROR;
    }

    b = tryGetBooleanValueFromStack(stackPointer, valueType, nbRow, nbCol, "isoview");
    if (b == NOT_A_BOOLEAN_VALUE)
        return SET_PROPERTY_ERROR;

    pSUBWIN_FEATURE(pobj)->isoview = b;
    return SET_PROPERTY_SUCCEED;
}

/*------------------------------------------------------------------------*/
