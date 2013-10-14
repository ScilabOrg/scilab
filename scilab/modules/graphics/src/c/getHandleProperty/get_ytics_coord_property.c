/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_ytics_coord_property.c                                       */
/* desc : function to retrieve in Scilab the ytics_coord field of         */
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
int get_ytics_coord_property(void* _pvCtx, int iObjUID)
{
    int iYNumberTicks = 0;
    int* piYNumberTicks = &iYNumberTicks;
    double* yTicksCoords = NULL;

    getGraphicObjectProperty(iObjUID, __GO_Y_TICKS_COORDS__, jni_double_vector, (void **)&yTicksCoords);

    if (yTicksCoords == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ytics_coord");
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_Y_NUMBER_TICKS__, jni_int, (void**)&piYNumberTicks);
    if (piYNumberTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ytics_coord");
        return -1;
    }

    return sciReturnRowVector(_pvCtx, yTicksCoords, iYNumberTicks);
}
/*------------------------------------------------------------------------*/
