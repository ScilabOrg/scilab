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
int get_ytics_coord_property( sciPointObj * pobj )
{
    int* yNumberTicks;
    double* yTicksCoords;

#if 0
    if ( sciGetEntityType(pobj) != SCI_AXES )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"ytics_coord");
        return -1;
    }
#endif

    yTicksCoords = (double*) getGraphicObjectProperty(pobj->UID, __GO_Y_TICKS_COORDS__, jni_double_vector);

    if (yTicksCoords == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"ytics_coord");
        return -1;
    }

    yNumberTicks = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_NUMBER_TICKS__, jni_int);

    return sciReturnRowVector(yTicksCoords, *yNumberTicks);
}
/*------------------------------------------------------------------------*/
