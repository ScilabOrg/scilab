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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_clip_box_property.c                                          */
/* desc : function to retrieve in Scilab the clip_box field of            */
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
int get_clip_box_property(char *pobjUID)
{
    int iClipState = 0;
    int* piClipState = &iClipState;
    double* clipBox;

    getGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, jni_int, &piClipState);

    if (piClipState == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"clip_box");
        return -1;
    }

    if (iClipState > 1)
    {
        /* clip state on */

        getGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, jni_double_vector, &clipBox);

        if (clipBox == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"),"clip_box");
            return -1;
        }

        return sciReturnRowVector(clipBox, 4);
    }
    else if (iClipState == 0 || iClipState == 1)
    {
        /* clip state off or clipgrf */
        return sciReturnEmptyMatrix();
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"),"clip_state");
        return -1;
    }
}
/*------------------------------------------------------------------------*/
