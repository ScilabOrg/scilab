/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
/* file: get_box_property.c                                               */
/* desc : function to retrieve in Scilab the box field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_box_property(char *pobjUID)
{
    char* type;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_string, (void **) &type);

    /*
     * Required since the Box property is implemented differently for the Axes and Text
     * objects (respectively as an Integer and a Boolean).
     * To be corrected
     */
    if (strcmp(type, __GO_AXES__) == 0)
    {
        int iBoxType = 0;
        int* piBoxType = &iBoxType;

        getGraphicObjectProperty(pobjUID, __GO_BOX_TYPE__, jni_int, (void **) &piBoxType);

        if (piBoxType == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
            return -1;
        }

        if (iBoxType == 0)
        {
            return sciReturnString("off");
        }
        else if (iBoxType == 1)
        {
            return sciReturnString("on");
        }
        else if (iBoxType == 2)
        {
            return sciReturnString("hidden_axes");
        }
        else if (iBoxType == 3)
        {
            return sciReturnString("back_half");
        }

    }
    else if (strcmp(type, __GO_TEXT__) == 0)
    {
        int iBox = 0;
        int* piBox = &iBox;

        getGraphicObjectProperty(pobjUID, __GO_BOX__, jni_bool, (void **) &piBox);

        if (piBox == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
            return -1;
        }

        if (iBox)
        {
            return sciReturnString( "on" );
        }
        else
        {
            return sciReturnString( "off" );
        }

    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
        return -1;
    }

}
/*------------------------------------------------------------------------*/
