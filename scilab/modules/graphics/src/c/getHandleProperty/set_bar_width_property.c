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
/* file: set_bar_width_property.c                                         */
/* desc : function to modify in Scilab the bar_width field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_bar_width_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status;
    double barWidth;

    if ( !isParameterDoubleMatrix( valueType ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "bar_width");
        return SET_PROPERTY_ERROR;
    }

#if 0
    if ( sciGetEntityType (pobj) != SCI_POLYLINE )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"bar_width");
        return SET_PROPERTY_ERROR;
    }
#endif

    barWidth = getDoubleFromStack( stackPointer );

    status = setGraphicObjectProperty(pobj->UID, __GO_BAR_WIDTH__, &barWidth, jni_double, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"bar_width");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/

