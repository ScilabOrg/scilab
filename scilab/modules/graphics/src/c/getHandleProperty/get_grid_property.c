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
/* file: get_grid_property.c                                              */
/* desc : function to retrieve in Scilab the grid field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_grid_property( sciPointObj * pobj )
{
    double grid[3];
    int iGridColor = 0;
    int* piGridColor = &iGridColor;
    int iView = 0;
    int* piView = &iView;

#if 0
    if (sciGetEntityType (pobj) != SCI_SUBWIN)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"grid");
        return -1;
    }
#endif

    /* need conversion for display in double */
    getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_GRID_COLOR__, jni_int, &piGridColor);

    if (piGridColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"grid");
        return -1;
    }

    grid[0] = (double) iGridColor;

    getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_GRID_COLOR__, jni_int, &piGridColor);
    grid[1] = (double) iGridColor;

    getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_GRID_COLOR__, jni_int, &piGridColor);
    grid[2] = (double) iGridColor;

    getGraphicObjectProperty(pobj->UID, __GO_VIEW__, jni_int, &piView);

    if (iView)
    {
        return sciReturnRowVector( grid, 3 );
    }
    else
    {
        return sciReturnRowVector( grid, 2 );
    }

}
/*------------------------------------------------------------------------*/
