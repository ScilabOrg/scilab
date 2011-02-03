/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_event_handler_enable_property.c                              */
/* desc : function to retrieve in Scilab the event_handler_enable field   */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "ObjectStructure.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_event_handler_enable_property( sciPointObj * pobj )
{
    int iEventHandlerEnable = 0;
    int *piEventHandlerEnable = &iEventHandlerEnable;

    getGraphicObjectProperty(pobj->UID, __GO_EVENTHANDLER_ENABLE__, jni_bool, &piEventHandlerEnable);

    if ( piEventHandlerEnable == NULL )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"event_handler_enable") ;
        return -1 ;
    }

    if (iEventHandlerEnable)
    {
        return sciReturnString( "on" ) ;
    }
    return sciReturnString( "off" ) ;

}
/*------------------------------------------------------------------------*/
