/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: set_tics_color_property.c                                        */
/* desc : function to modify in Scilab the tics_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_tics_color_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status;
    int ticksColor;

    if ( !isParameterDoubleMatrix( valueType ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "tics_color");
        return SET_PROPERTY_ERROR;
    }

    ticksColor = (int) getDoubleFromStack(stackPointer);

    status = setGraphicObjectProperty(pobj->UID, __GO_TICKS_COLOR__, &ticksColor, jni_int, 1);

/* Deactivated for now since it involves color range checks, to be implemented. */
#if 0
    sciSetForeground(pobj, (int) getDoubleFromStack( stackPointer ));
#endif

  /* To be implemented using the MVC framework */
#if 0
    if ( sciGetEntityType(pobj) == SCI_SUBWIN )
    {
        sciprint("Warning: tics_color use is deprecated and no more taken into account, use foreground property to edit Axes color\n");

        /* To be implemented using the MVC framework */
        pSUBWIN_FEATURE (pobj)->axes.ticscolor = (int)getDoubleFromStack( stackPointer ) ;
    }
#endif

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_color");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
