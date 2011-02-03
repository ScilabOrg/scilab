/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
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
/* file: get_grid_position_property.c                                     */
/* desc : function to retrieve in Scilab the grid_position field of a     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_grid_position_property( sciPointObj * pobj )
{
    int iGridPosition;
    int* piGridPosition = &iGridPosition;

#if 0
    if ( sciGetEntityType(pobj) != SCI_SUBWIN )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_position");
        return -1;
    }
#endif

    getGraphicObjectProperty(pobj->UID, __GO_GRID_POSITION__, jni_int, &piGridPosition);

    if (piGridPosition == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_position");
        return -1;
    }

    if (iGridPosition)
    {
        return sciReturnString("foreground");
    }
    else
    {
        return sciReturnString("background");
    }
}
/*------------------------------------------------------------------------*/
