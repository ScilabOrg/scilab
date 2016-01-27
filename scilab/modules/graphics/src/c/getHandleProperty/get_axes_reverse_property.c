/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_axes_reverse_property.c                                      */
/* desc : function to retrieve in Scilab the axes_reverse field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "os_string.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_axes_reverse_property(void* _pvCtx, int iObjUID)
{
    int const axesReversePropertiesNames[3] = {__GO_X_AXIS_REVERSE__, __GO_Y_AXIS_REVERSE__, __GO_Z_AXIS_REVERSE__};
    char * axes_reverse[3]  = { NULL, NULL, NULL };
    int iAxesReverse = 0;
    int* piAxesReverse = &iAxesReverse;

    int i = 0;
    int j = 0;
    void* status = NULL;

    for (i = 0 ; i < 3 ; i++)
    {
        getGraphicObjectProperty(iObjUID, axesReversePropertiesNames[i], jni_bool, (void **)&piAxesReverse);

        if (piAxesReverse == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "axes_reverse");
            return NULL;
        }

        if (iAxesReverse)
        {
            axes_reverse[i] = os_strdup("on");
        }
        else
        {
            axes_reverse[i] = os_strdup("off");
        }

        if (axes_reverse[i] == NULL)
        {
            for (j = 0 ; j < i ; j++)
            {
                FREE(axes_reverse[j]);
            }

            Scierror(999, _("%s: No more memory.\n"), "get_axes_reverse_property");
            return NULL;
        }

    }

    status = sciReturnRowStringVector(axes_reverse, 3);

    for (i = 0 ; i < 3 ; i++)
    {
        FREE(axes_reverse[i]);
    }

    return status;
}
/*------------------------------------------------------------------------*/
