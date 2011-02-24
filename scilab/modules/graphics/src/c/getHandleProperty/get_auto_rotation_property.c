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
/* file: get_auto_rotation_property.c                                     */
/* desc : function to retrieve in Scilab the auto_rotation field of       */
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
int get_auto_rotation_property( sciPointObj * pobj )
{
    int iAutoRotation = 0;
    int* piAutoRotation = &iAutoRotation;

#if 0
    if( sciGetEntityType(pobj) != SCI_LABEL )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_rotation");
        return -1;
    }
#endif

    getGraphicObjectProperty(pobj->UID, __GO_AUTO_ROTATION__, jni_bool, &piAutoRotation);

    if (piAutoRotation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_rotation");
        return -1;
    }

    if (iAutoRotation)
    {
        return sciReturnString( "on" );
    }
    else
    {
        return sciReturnString( "off" );
    }
}
/*------------------------------------------------------------------------*/
