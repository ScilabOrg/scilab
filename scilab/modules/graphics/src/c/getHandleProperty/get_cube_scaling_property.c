/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: get_cube_scaling_property.c                                      */
/* desc : function to retrieve in Scilab the cube_scaling field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_cube_scaling_property(void* _pvCtx, int iObjUID)
{
    int iCubeScaling = 0;
    int* piCubeScaling = &iCubeScaling;

    getGraphicObjectProperty(iObjUID, __GO_CUBE_SCALING__, jni_bool, (void **)&piCubeScaling);

    if (piCubeScaling == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "cube_scaling");
        return -1;
    }

    if (iCubeScaling)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
/*------------------------------------------------------------------------*/
